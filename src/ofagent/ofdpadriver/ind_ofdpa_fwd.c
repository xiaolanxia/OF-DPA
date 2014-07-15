/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2013-2014
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*
**********************************************************************
*
* @filename   ind_ofdpa_fwd.c
*
* @purpose    OF-DPA Driver for Indigo
*
* @component  OF-DPA
*
* @comments   none
*
* @create     6 Nov 2013
*
* @end
*
**********************************************************************/
#include <ind_ofdpa_util.h>
#include <unistd.h>
#include <indigo/memory.h>
#include <indigo/forwarding.h>
#include <ind_ofdpa_log.h>
#include <indigo/of_state_manager.h>
#include <indigo/fi.h>
#include <OFStateManager/ofstatemanager.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <stdbool.h>
#include <pthread.h>
#include <errno.h>

ind_ofdpa_fields_t ind_ofdpa_match_fields_bitmask;

static indigo_error_t ind_ofdpa_packet_out_actions_get(of_list_action_t *of_list_actions, 
                                                       indPacketOutActions_t *packetOutActions);
static indigo_error_t ind_ofdpa_match_fields_masks_get(const of_match_t *match, ofdpaFlowEntry_t *flow);
static indigo_error_t ind_ofdpa_translate_openflow_actions(of_list_action_t *actions, ofdpaFlowEntry_t *flow);

extern int ofagent_of_version;

indTableNameList_t tableNameList[] =
{
  {OFDPA_FLOW_TABLE_ID_INGRESS_PORT,      "Ingress Port"},
  {OFDPA_FLOW_TABLE_ID_VLAN,              "VLAN"},
  {OFDPA_FLOW_TABLE_ID_TERMINATION_MAC,   "Termination MAC"},
  {OFDPA_FLOW_TABLE_ID_UNICAST_ROUTING,   "Unicast Routing"},
  {OFDPA_FLOW_TABLE_ID_MULTICAST_ROUTING, "Multicast Routing"},
  {OFDPA_FLOW_TABLE_ID_BRIDGING,          "Bridging"},
  {OFDPA_FLOW_TABLE_ID_ACL_POLICY,        "ACL Policy"}
};

#define TABLE_NAME_LIST_SIZE (sizeof(tableNameList)/sizeof(tableNameList[0]))

static indigo_error_t ind_ofdpa_match_fields_prerequisite_validate(const of_match_t *match, OFDPA_FLOW_TABLE_ID_t tableId)
{
  indigo_error_t err = INDIGO_ERROR_NONE;

  switch(tableId)
  {
    case OFDPA_FLOW_TABLE_ID_INGRESS_PORT:
    case OFDPA_FLOW_TABLE_ID_VLAN:
    case OFDPA_FLOW_TABLE_ID_TERMINATION_MAC:
    case OFDPA_FLOW_TABLE_ID_UNICAST_ROUTING:
    case OFDPA_FLOW_TABLE_ID_MULTICAST_ROUTING:
    case OFDPA_FLOW_TABLE_ID_BRIDGING:
      break;
    case OFDPA_FLOW_TABLE_ID_ACL_POLICY:

      /* Check if IPv4 ether type is missed/incorrect */
      if ((ind_ofdpa_match_fields_bitmask & (IND_OFDPA_IPV4_DST | IND_OFDPA_IPV4_SRC)) &&
           match->fields.eth_type != ETH_P_IP)
      {
        LOG_ERROR("Invalid ethertype for IPv4 match fields.");
        err = INDIGO_ERROR_COMPAT;
        break;
      }

      if ((ind_ofdpa_match_fields_bitmask & (IND_OFDPA_IPV6_DST | IND_OFDPA_IPV6_SRC | IND_OFDPA_IPV6_FLOW_LABEL)) &&
           match->fields.eth_type != ETH_P_IPV6)
      {
        LOG_ERROR("Invalid ethertype for IPv6 match fields.");
        err = INDIGO_ERROR_COMPAT;
        break;
      }

      if ((match->fields.eth_type != ETH_P_IP) && (match->fields.eth_type != ETH_P_IPV6))
      {
        if (ind_ofdpa_match_fields_bitmask & IND_OFDPA_IP_DSCP)
        {
          LOG_ERROR("Invalid ethertype (0x%x) for IP DSCP match field.", match->fields.eth_type);
          err = INDIGO_ERROR_COMPAT;
          break;
        }

        if (ind_ofdpa_match_fields_bitmask & IND_OFDPA_IP_ECN)
        {
          LOG_ERROR("Invalid ethertype (0x%x) for IP ECN match field.", match->fields.eth_type);
          err = INDIGO_ERROR_COMPAT;
          break;
        }

        if (ind_ofdpa_match_fields_bitmask & IND_OFDPA_IP_PROTO)
        {
          LOG_ERROR("Invalid ethertype (0x%x) for IP Protocol match field.", match->fields.eth_type);
          err = INDIGO_ERROR_COMPAT;
          break;
        }

      }

      if ((ind_ofdpa_match_fields_bitmask & IND_OFDPA_IPV6_FLOW_LABEL) && (match->fields.eth_type != ETH_P_IPV6))
      {
        LOG_ERROR("Invalid ethertype (0x%x) for IPv6 Flow Label match field.", match->fields.eth_type);
        err = INDIGO_ERROR_COMPAT;
        break;
      }

      /* Vlan PCP must be allowed only when preceded by Vlan ID */
      if ((ind_ofdpa_match_fields_bitmask & IND_OFDPA_VLAN_PCP) &&
          (!(match->fields.vlan_vid & OFDPA_VID_EXACT_MASK)))
      {   
        LOG_ERROR("Vlan PCP match field must be preceded by Vlan ID match field.");
        err = INDIGO_ERROR_COMPAT;
        break;
      }

      if ((ind_ofdpa_match_fields_bitmask & (IND_OFDPA_TCP_L4_SRC_PORT | IND_OFDPA_TCP_L4_DST_PORT)) &&
          (match->fields.ip_proto != IPPROTO_TCP))
      {
        LOG_ERROR("Invalid protocol ID %d for TCP L4 src/dst ports.", match->fields.ip_proto);
        err = INDIGO_ERROR_COMPAT;
        break;
      }

      if ((ind_ofdpa_match_fields_bitmask & (IND_OFDPA_UDP_L4_SRC_PORT | IND_OFDPA_UDP_L4_DST_PORT)) &&
          (match->fields.ip_proto != IPPROTO_UDP))
      {
        LOG_ERROR("Invalid protocol ID %d for UDP L4 src/dst ports.", match->fields.ip_proto);
        err = INDIGO_ERROR_COMPAT;
        break;
      }

      if ((ind_ofdpa_match_fields_bitmask & (IND_OFDPA_SCTP_L4_SRC_PORT | IND_OFDPA_SCTP_L4_DST_PORT)) &&
          (match->fields.ip_proto != IPPROTO_SCTP))
      {
        LOG_ERROR("Invalid protocol ID %d for SCTP L4 src/dst ports.", match->fields.ip_proto);
        err = INDIGO_ERROR_COMPAT;
        break;
      }

      if ((ind_ofdpa_match_fields_bitmask & (IND_OFDPA_ICMPV4_CODE | IND_OFDPA_ICMPV4_TYPE)) &&
           (match->fields.ip_proto != IPPROTO_ICMP))
      {
        LOG_ERROR("Invalid protocol ID %d for ICMPv4 type/code.", match->fields.ip_proto);
        err = INDIGO_ERROR_COMPAT;
        break;
      }

      if ((ind_ofdpa_match_fields_bitmask & (IND_OFDPA_ICMPV6_CODE | IND_OFDPA_ICMPV6_TYPE)) &&
           (match->fields.ip_proto != IPPROTO_ICMPV6))
      {
        LOG_ERROR("Invalid protocol ID %d for ICMPv6 type/code.", match->fields.ip_proto);
        err = INDIGO_ERROR_COMPAT;
        break;
      }
      break;
    default:
      break;
  }
 
  return err; 
}

/* Get the flow match criteria from of_match */

static indigo_error_t ind_ofdpa_match_fields_masks_get(const of_match_t *match, ofdpaFlowEntry_t *flow)
{
  indigo_error_t err = INDIGO_ERROR_NONE;

  switch(flow->tableId)
  {
    case OFDPA_FLOW_TABLE_ID_INGRESS_PORT:
      if ((ind_ofdpa_match_fields_bitmask | IND_OFDPA_ING_PORT_FLOW_MATCH_BITMAP) != IND_OFDPA_ING_PORT_FLOW_MATCH_BITMAP)
      {
        err = INDIGO_ERROR_COMPAT;
        break;
      }
      flow->flowData.ingressPortFlowEntry.match_criteria.inPort = match->fields.in_port;
      flow->flowData.ingressPortFlowEntry.match_criteria.inPortMask = OFDPA_INPORT_TYPE_MASK;
      break; 
      
      
    case OFDPA_FLOW_TABLE_ID_VLAN:
	
      if ((ind_ofdpa_match_fields_bitmask | IND_OFDPA_VLAN_FLOW_MATCH_BITMAP) != IND_OFDPA_VLAN_FLOW_MATCH_BITMAP)
      {
        err = INDIGO_ERROR_COMPAT;
        break;
      }    

      flow->flowData.vlanFlowEntry.match_criteria.inPort = match->fields.in_port;

      /* CFI bit indicating 'present' is included in the VID match field */
      flow->flowData.vlanFlowEntry.match_criteria.vlanId = match->fields.vlan_vid; 
      if (match->masks.vlan_vid != 0)
      {
        flow->flowData.vlanFlowEntry.match_criteria.vlanIdMask = (match->masks.vlan_vid) & 
                                                                 (OFDPA_VID_PRESENT | OFDPA_VID_EXACT_MASK);
      }
      else
      {
        /* When the mask passed is 0, assume that vlan is 0. 
           Hence just set the mask without OFDPA_VID_PRESENT */
        flow->flowData.vlanFlowEntry.match_criteria.vlanIdMask = OFDPA_VID_EXACT_MASK;
      }
      break;
   
    case OFDPA_FLOW_TABLE_ID_TERMINATION_MAC:
      if ((ind_ofdpa_match_fields_bitmask | IND_OFDPA_TERM_MAC_FLOW_MATCH_BITMAP) != IND_OFDPA_TERM_MAC_FLOW_MATCH_BITMAP)
      {
        err = INDIGO_ERROR_COMPAT;
        break;
      }
     
      if (ind_ofdpa_match_fields_bitmask & IND_OFDPA_PORT) 
      {
        flow->flowData.terminationMacFlowEntry.match_criteria.inPort = match->fields.in_port;
        if (match->fields.in_port == 0) /* For multicast flow of termination mac table in_port must be 0 */
        {
          flow->flowData.terminationMacFlowEntry.match_criteria.inPortMask = 0;
        }
        else
        {
          if (match->masks.in_port != 0)
          {
            flow->flowData.terminationMacFlowEntry.match_criteria.inPortMask = match->masks.in_port;
          }
          else
          {
            flow->flowData.terminationMacFlowEntry.match_criteria.inPortMask = OFDPA_INPORT_EXACT_MASK;
          }
        }
      }

      flow->flowData.terminationMacFlowEntry.match_criteria.etherType = match->fields.eth_type;

      memcpy(&flow->flowData.terminationMacFlowEntry.match_criteria.destMac, &match->fields.eth_dst, OF_MAC_ADDR_BYTES);
      memcpy(&flow->flowData.terminationMacFlowEntry.match_criteria.destMacMask, &match->masks.eth_dst, OF_MAC_ADDR_BYTES);

      flow->flowData.terminationMacFlowEntry.match_criteria.vlanId = match->fields.vlan_vid & OFDPA_VID_EXACT_MASK;
      if (!(match->fields.vlan_vid & OFDPA_VID_EXACT_MASK))
      {
        flow->flowData.terminationMacFlowEntry.match_criteria.vlanIdMask = 0;
      }
      else
      {
        if (match->masks.vlan_vid != 0)
        {
          flow->flowData.terminationMacFlowEntry.match_criteria.vlanIdMask = match->masks.vlan_vid & OFDPA_VID_EXACT_MASK;
        }
        else
        {
          flow->flowData.terminationMacFlowEntry.match_criteria.vlanIdMask = OFDPA_VID_EXACT_MASK;
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_UNICAST_ROUTING:
      if ((ind_ofdpa_match_fields_bitmask | IND_OFDPA_UCAST_ROUTING_FLOW_MATCH_BITMAP) != IND_OFDPA_UCAST_ROUTING_FLOW_MATCH_BITMAP)
      {
        err = INDIGO_ERROR_COMPAT;
        break;
      }

      if (((ind_ofdpa_match_fields_bitmask & IND_OFDPA_IPV6_DST) && (match->fields.eth_type != ETH_P_IPV6)) ||
          ((ind_ofdpa_match_fields_bitmask & IND_OFDPA_IPV4_DST) && (match->fields.eth_type != ETH_P_IP)))
      {
        LOG_ERROR("Invalid IP for 0x%x ethertype", match->fields.eth_type);
        err = INDIGO_ERROR_COMPAT;
        break;
      }

      flow->flowData.unicastRoutingFlowEntry.match_criteria.etherType = match->fields.eth_type;
      if (match->fields.eth_type == ETH_P_IP) 
      {
        flow->flowData.unicastRoutingFlowEntry.match_criteria.dstIp4 = match->fields.ipv4_dst;
        flow->flowData.unicastRoutingFlowEntry.match_criteria.dstIp4Mask = match->masks.ipv4_dst;
      }
      else if (match->fields.eth_type == ETH_P_IPV6) 
      {
        memcpy(&flow->flowData.unicastRoutingFlowEntry.match_criteria.dstIp6, &match->fields.ipv6_dst, OF_IPV6_BYTES);
        memcpy(&flow->flowData.unicastRoutingFlowEntry.match_criteria.dstIp6Mask, &match->masks.ipv6_dst, OF_IPV6_BYTES);
      }
      break;

    case OFDPA_FLOW_TABLE_ID_MULTICAST_ROUTING:
      if ((ind_ofdpa_match_fields_bitmask | IND_OFDPA_MCAST_ROUTING_FLOW_MATCH_BITMAP) != IND_OFDPA_MCAST_ROUTING_FLOW_MATCH_BITMAP)
      {
        err = INDIGO_ERROR_COMPAT;
        break;
      }

      if ((ind_ofdpa_match_fields_bitmask & (IND_OFDPA_IPV4_DST | IND_OFDPA_IPV4_SRC)) && 
          (match->fields.eth_type != ETH_P_IP))
      {
        LOG_ERROR("Invalid ether type for IPv4 match fields.");
        err = INDIGO_ERROR_COMPAT;
        break;
      }

      if ((ind_ofdpa_match_fields_bitmask & (IND_OFDPA_IPV6_DST | IND_OFDPA_IPV6_SRC)) && 
          (match->fields.eth_type != ETH_P_IPV6))
      {
        LOG_ERROR("Invalid ether type for IPv6 match fields.");
        err = INDIGO_ERROR_COMPAT;
        break;
      }

      flow->flowData.multicastRoutingFlowEntry.match_criteria.etherType = match->fields.eth_type;
      flow->flowData.multicastRoutingFlowEntry.match_criteria.vlanId = match->fields.vlan_vid & OFDPA_VID_EXACT_MASK;

      if (match->fields.eth_type == ETH_P_IP)
      {
        flow->flowData.multicastRoutingFlowEntry.match_criteria.srcIp4 = match->fields.ipv4_src;
        flow->flowData.multicastRoutingFlowEntry.match_criteria.srcIp4Mask = match->masks.ipv4_src;
        flow->flowData.multicastRoutingFlowEntry.match_criteria.dstIp4 = match->fields.ipv4_dst;
      }
      else if (match->fields.eth_type == ETH_P_IPV6)
      {
        memcpy(flow->flowData.multicastRoutingFlowEntry.match_criteria.srcIp6.s6_addr, match->fields.ipv6_src.addr, OF_IPV6_BYTES);
        memcpy(flow->flowData.multicastRoutingFlowEntry.match_criteria.srcIp6Mask.s6_addr, match->masks.ipv6_src.addr, OF_IPV6_BYTES);
        memcpy(flow->flowData.multicastRoutingFlowEntry.match_criteria.dstIp6.s6_addr, match->fields.ipv6_dst.addr, OF_IPV6_BYTES);
      }
      break;

    case OFDPA_FLOW_TABLE_ID_BRIDGING:
      if ((ind_ofdpa_match_fields_bitmask | IND_OFDPA_BRIDGING_FLOW_MATCH_BITMAP) != IND_OFDPA_BRIDGING_FLOW_MATCH_BITMAP)
      {
        err = INDIGO_ERROR_COMPAT;
        break;
      }

      if (ind_ofdpa_match_fields_bitmask & IND_OFDPA_TUNNEL_ID)
      {
        flow->flowData.bridgingFlowEntry.match_criteria.tunnelId = match->fields.tunnel_id; 
      }
      else if (ind_ofdpa_match_fields_bitmask & IND_OFDPA_VLANID)
      {
        flow->flowData.bridgingFlowEntry.match_criteria.vlanId = match->fields.vlan_vid & OFDPA_VID_EXACT_MASK;
      }
      memcpy(&flow->flowData.bridgingFlowEntry.match_criteria.destMac, &match->fields.eth_dst, OF_MAC_ADDR_BYTES);
      memcpy(&flow->flowData.bridgingFlowEntry.match_criteria.destMacMask, &match->masks.eth_dst, OF_MAC_ADDR_BYTES);
      break;

    case OFDPA_FLOW_TABLE_ID_ACL_POLICY:
      if ((ind_ofdpa_match_fields_bitmask | IND_OFDPA_ACL_POLICY_FLOW_MATCH_BITMAP) != IND_OFDPA_ACL_POLICY_FLOW_MATCH_BITMAP)
      {
        err = INDIGO_ERROR_COMPAT;
        break;
      }

      /* Validate the pre-requisites for match fields */
      err = ind_ofdpa_match_fields_prerequisite_validate(match, flow->tableId);
      if (err != INDIGO_ERROR_NONE)
      {
        break; 
      }

      /* In Port */      
      if (match->fields.in_port != 0) /* match on a port */
      {
        flow->flowData.policyAclFlowEntry.match_criteria.inPort = match->fields.in_port; 
        if (match->masks.in_port != 0)
        {
          flow->flowData.policyAclFlowEntry.match_criteria.inPortMask = match->masks.in_port;
        }
        else
        {
          flow->flowData.policyAclFlowEntry.match_criteria.inPortMask = OFDPA_INPORT_EXACT_MASK;
        }
      }
      else /* Match on all ports. Applicable to only physical ports */
      {
        ofdpaPortTypeSet(&flow->flowData.policyAclFlowEntry.match_criteria.inPort, OFDPA_PORT_TYPE_PHYSICAL);
        flow->flowData.policyAclFlowEntry.match_criteria.inPortMask = OFDPA_INPORT_TYPE_MASK;
      }

      /* Ethertype */
      if (ind_ofdpa_match_fields_bitmask & IND_OFDPA_ETHER_TYPE)
      {
        flow->flowData.policyAclFlowEntry.match_criteria.etherType = match->fields.eth_type; 
      }

      /* Src MAC */
      if (ind_ofdpa_match_fields_bitmask & IND_OFDPA_SRCMAC)
      {
        memcpy(&flow->flowData.policyAclFlowEntry.match_criteria.srcMac, &match->fields.eth_src, OF_MAC_ADDR_BYTES);
        if (memcmp(&match->masks.eth_src, &of_mac_addr_all_zeros, sizeof(match->masks.eth_src)) == 0)
        {
          memcpy(&flow->flowData.policyAclFlowEntry.match_criteria.srcMacMask, &of_mac_addr_all_ones, OF_MAC_ADDR_BYTES);
        }
        else
        {
          memcpy(&flow->flowData.policyAclFlowEntry.match_criteria.srcMacMask, &match->masks.eth_src, OF_MAC_ADDR_BYTES);
        }
      }

      /* Dst MAC */
      if (ind_ofdpa_match_fields_bitmask & IND_OFDPA_DSTMAC)
      {
        memcpy(&flow->flowData.policyAclFlowEntry.match_criteria.destMac, &match->fields.eth_dst, OF_MAC_ADDR_BYTES);
        if (memcmp(&match->masks.eth_dst, &of_mac_addr_all_zeros, sizeof(match->masks.eth_src)) == 0)
        {
          memcpy(&flow->flowData.policyAclFlowEntry.match_criteria.destMacMask, &of_mac_addr_all_ones, OF_MAC_ADDR_BYTES);
        }
        else
        {
          memcpy(&flow->flowData.policyAclFlowEntry.match_criteria.destMacMask, &match->masks.eth_dst, OF_MAC_ADDR_BYTES);
        }
      }

      /* Vlan ID */
      if (ind_ofdpa_match_fields_bitmask & IND_OFDPA_VLANID)
      {
        flow->flowData.policyAclFlowEntry.match_criteria.vlanId = match->fields.vlan_vid & OFDPA_VID_EXACT_MASK;
        if (match->masks.vlan_vid != 0)
        {
          flow->flowData.policyAclFlowEntry.match_criteria.vlanIdMask = match->masks.vlan_vid & OFDPA_VID_EXACT_MASK;
        }
        else
        {
          flow->flowData.policyAclFlowEntry.match_criteria.vlanIdMask = OFDPA_VID_EXACT_MASK;
        }
        /* To be removed once tunnel Id match condition is implemented */
        /* flow->flowData.policyAclFlowEntry.match_criteria.tunnelId = 0; */
      }

      /* Tunnel ID */
      if (ind_ofdpa_match_fields_bitmask & IND_OFDPA_TUNNEL_ID)
      {
        flow->flowData.policyAclFlowEntry.match_criteria.tunnelId = match->fields.tunnel_id;
      }

      /* Vlan PCP */
      if (ind_ofdpa_match_fields_bitmask & IND_OFDPA_VLAN_PCP)
      {
        flow->flowData.policyAclFlowEntry.match_criteria.vlanPcp = match->fields.vlan_pcp;
        if (match->masks.vlan_pcp != 0)
        {
          flow->flowData.policyAclFlowEntry.match_criteria.vlanPcpMask = match->masks.vlan_pcp;
        }
        else
        {
          flow->flowData.policyAclFlowEntry.match_criteria.vlanPcpMask = 0x7;
        }
      }

      if (match->fields.eth_type == ETH_P_IP) 
      {
        /* IPv4 SRC */
        if (ind_ofdpa_match_fields_bitmask & IND_OFDPA_IPV4_SRC)
        {
          flow->flowData.policyAclFlowEntry.match_criteria.sourceIp4 = match->fields.ipv4_src;
          if (match->masks.ipv4_src != 0)
          {
            flow->flowData.policyAclFlowEntry.match_criteria.sourceIp4Mask = match->masks.ipv4_src;
          }
          else
          {
            flow->flowData.policyAclFlowEntry.match_criteria.sourceIp4Mask = IND_OFDPA_DEFAULT_SOURCEIP4MASK;
          }
        }

        /* IPv4 DST */
        if (ind_ofdpa_match_fields_bitmask & IND_OFDPA_IPV4_DST)
        {
          flow->flowData.policyAclFlowEntry.match_criteria.destIp4 = match->fields.ipv4_dst;
          if (match->masks.ipv4_dst != 0)
          {
            flow->flowData.policyAclFlowEntry.match_criteria.destIp4Mask = match->masks.ipv4_dst;
          }
          else
          {
            flow->flowData.policyAclFlowEntry.match_criteria.destIp4Mask = IND_OFDPA_DEFAULT_DESTIP4MASK;
          }
        }
      }
      else if (match->fields.eth_type == ETH_P_IPV6)
      {
        /* IPv6 SRC */
        if (ind_ofdpa_match_fields_bitmask & IND_OFDPA_IPV6_SRC)
        {
          memcpy(flow->flowData.policyAclFlowEntry.match_criteria.sourceIp6.s6_addr, match->fields.ipv6_src.addr, OF_IPV6_BYTES);
          if (memcmp(&match->masks.ipv6_src.addr, &of_ipv6_all_zeros, OF_IPV6_BYTES) == 0)
          {
            int i;
            for (i = 0; i < 4; i++) /* Prefix length as 128*/
            {
              flow->flowData.policyAclFlowEntry.match_criteria.sourceIp6Mask.s6_addr32[i] = ~0;
            }
          }
          else
          {
            memcpy(flow->flowData.policyAclFlowEntry.match_criteria.sourceIp6Mask.s6_addr, match->masks.ipv6_src.addr, OF_IPV6_BYTES);
          }
        }

        /* IPv6 DST */
        if (ind_ofdpa_match_fields_bitmask & IND_OFDPA_IPV6_DST)
        {
          memcpy(flow->flowData.policyAclFlowEntry.match_criteria.destIp6.s6_addr, match->fields.ipv6_dst.addr, OF_IPV6_BYTES);
          if (memcmp(&(match->masks.ipv6_dst), &of_ipv6_all_zeros, OF_IPV6_BYTES) == 0)
          {
            int i;
            for (i = 0; i < 4; i++) /* Prefix length as 128*/
            {
              flow->flowData.policyAclFlowEntry.match_criteria.destIp6Mask.s6_addr32[i] = ~0;
            }
          }
          else
          {
            memcpy(flow->flowData.policyAclFlowEntry.match_criteria.destIp6Mask.s6_addr, match->masks.ipv6_dst.addr, OF_IPV6_BYTES);
          }
        }

        /* IPv6 flow label */
        if (ind_ofdpa_match_fields_bitmask & IND_OFDPA_IPV6_FLOW_LABEL)
        {
          flow->flowData.policyAclFlowEntry.match_criteria.ipv6FlowLabel = match->fields.ipv6_flabel;
          if (match->masks.ipv6_flabel != 0)
          {
            flow->flowData.policyAclFlowEntry.match_criteria.ipv6FlowLabelMask = match->masks.ipv6_flabel;
          }
          else
          {
            flow->flowData.policyAclFlowEntry.match_criteria.ipv6FlowLabelMask = ~0;
          }
        }
      }

      if (match->fields.eth_type == ETH_P_ARP)
      {
#if 0
        /* ARP Source IP Address */
        if (ind_ofdpa_match_fields_bitmask & IND_OFDPA_IPV4_ARP_SPA)
        {
          flow->flowData.policyAclFlowEntry.match_criteria.ipv4ArpSpa = match->fields.arp_spa;
          if (match->masks.arp_spa != 0)
          {
            flow->flowData.policyAclFlowEntry.match_criteria.ipv4ArpSpaMask = match->masks.arp_spa;
          }
          else
          {
            flow->flowData.policyAclFlowEntry.match_criteria.ipv4ArpSpaMask = IND_OFDPA_DEFAULT_SOURCEIP4MASK;
          }
        }

        /* ARP IP Protocol */
        if (ind_ofdpa_match_fields_bitmask & IND_OFDPA_IP_PROTO)
        {
          flow->flowData.policyAclFlowEntry.match_criteria.ipProto = match->fields.arp_op & 0xff;
          if ((match->masks.arp_op & 0xff))
          {
            flow->flowData.policyAclFlowEntry.match_criteria.ipProtoMask = match->masks.arp_op & 0xff;
          }
          else
          {
            flow->flowData.policyAclFlowEntry.match_criteria.ipProtoMask = 0xff;
          }
        }
#endif
        LOG_ERROR("ARP Source IP Address is unsupported.");
        return INDIGO_ERROR_COMPAT;
      }
      else
      {
        if (match->fields.eth_type == ETH_P_IP || match->fields.eth_type == ETH_P_IPV6)
        {
          /* IP Protocol */
          if (ind_ofdpa_match_fields_bitmask & IND_OFDPA_IP_PROTO)
          {
            flow->flowData.policyAclFlowEntry.match_criteria.ipProto = match->fields.ip_proto;
            if (match->masks.ip_proto != 0)
            {
              flow->flowData.policyAclFlowEntry.match_criteria.ipProtoMask = match->masks.ip_proto;
            }
            else
            {
              flow->flowData.policyAclFlowEntry.match_criteria.ipProtoMask = 0xff; 
            }
          }

          /* IP DSCP */
          if (ind_ofdpa_match_fields_bitmask & IND_OFDPA_IP_DSCP)
          {
            flow->flowData.policyAclFlowEntry.match_criteria.dscp = match->fields.ip_dscp;
            if (match->masks.ip_dscp != 0)
            {
              flow->flowData.policyAclFlowEntry.match_criteria.dscpMask = match->masks.ip_dscp;
            }
            else
            {
              flow->flowData.policyAclFlowEntry.match_criteria.dscpMask = 0xff;
            }
          }

          if (ind_ofdpa_match_fields_bitmask & IND_OFDPA_IP_ECN)
          {
#if 0
            flow->flowData.policyAclFlowEntry.match_criteria.ecn = match->fields.ip_ecn;
            if (match->masks.ip_ecn !=0)
            {
              flow->flowData.policyAclFlowEntry.match_criteria.ecnMask = match->masks.ip_ecn;
            }
            else
            {
              flow->flowData.policyAclFlowEntry.match_criteria.ecnMask = 0xff;
            }
#endif
            LOG_ERROR("ECN match field is unsupported.");
          }
        }
      }

      if (match->fields.ip_proto == IPPROTO_TCP) 
      {
        /* TCP L4 source port */
        if (ind_ofdpa_match_fields_bitmask & IND_OFDPA_TCP_L4_SRC_PORT)
        {
          flow->flowData.policyAclFlowEntry.match_criteria.srcL4Port = match->fields.tcp_src;
          if (match->masks.tcp_src != 0)
          {
            flow->flowData.policyAclFlowEntry.match_criteria.srcL4PortMask = match->masks.tcp_src;
          }
          else
          {
            flow->flowData.policyAclFlowEntry.match_criteria.srcL4PortMask = 0xff;
          }
        }

        /* TCP L4 destination port */
        if (ind_ofdpa_match_fields_bitmask & IND_OFDPA_TCP_L4_DST_PORT)
        {
          flow->flowData.policyAclFlowEntry.match_criteria.destL4Port = match->fields.tcp_dst;
          if (match->masks.tcp_dst != 0)
          {
            flow->flowData.policyAclFlowEntry.match_criteria.destL4PortMask = match->masks.tcp_dst;
          }
          else
          {
            flow->flowData.policyAclFlowEntry.match_criteria.destL4PortMask = 0xff;
          }
        }
      }
      else if (match->fields.ip_proto == IPPROTO_UDP) 
      {
        if (ind_ofdpa_match_fields_bitmask & IND_OFDPA_UDP_L4_SRC_PORT)
        {
          flow->flowData.policyAclFlowEntry.match_criteria.srcL4Port = match->fields.udp_src;
          if (match->masks.udp_src != 0)
          {
            flow->flowData.policyAclFlowEntry.match_criteria.srcL4PortMask = match->masks.udp_src;
          }
          else
          {
            flow->flowData.policyAclFlowEntry.match_criteria.srcL4PortMask = 0xff;
          }
        }

        if (ind_ofdpa_match_fields_bitmask & IND_OFDPA_UDP_L4_DST_PORT)
        {
          flow->flowData.policyAclFlowEntry.match_criteria.destL4Port = match->fields.udp_dst;
          if (match->masks.udp_dst != 0)
          {
            flow->flowData.policyAclFlowEntry.match_criteria.destL4PortMask = match->masks.udp_dst;
          }
          else
          {
            flow->flowData.policyAclFlowEntry.match_criteria.destL4PortMask = 0xff;
          }
        }
      }
      else if (match->fields.ip_proto == IPPROTO_SCTP)
      {
        if (ind_ofdpa_match_fields_bitmask & IND_OFDPA_SCTP_L4_SRC_PORT)
        {
          flow->flowData.policyAclFlowEntry.match_criteria.srcL4Port = match->fields.sctp_src;
          if (match->masks.sctp_src != 0)
          {
            flow->flowData.policyAclFlowEntry.match_criteria.srcL4PortMask = match->masks.sctp_src;
          }
          else
          {
            flow->flowData.policyAclFlowEntry.match_criteria.srcL4PortMask = 0xff;
          }
        }

        if (ind_ofdpa_match_fields_bitmask & IND_OFDPA_SCTP_L4_DST_PORT)
        {
          flow->flowData.policyAclFlowEntry.match_criteria.destL4Port = match->fields.sctp_dst;
          if (match->masks.sctp_dst != 0)
          {
            flow->flowData.policyAclFlowEntry.match_criteria.destL4PortMask = match->masks.sctp_dst;
          }
          else
          {
            flow->flowData.policyAclFlowEntry.match_criteria.destL4PortMask = 0xff;
          }
        }
      }
      else if (match->fields.ip_proto == IPPROTO_ICMP)
      {
        if (ind_ofdpa_match_fields_bitmask & IND_OFDPA_ICMPV4_TYPE)
        {
          flow->flowData.policyAclFlowEntry.match_criteria.icmpType = match->fields.icmpv4_type;
          if (match->masks.icmpv4_type != 0)
          {
          flow->flowData.policyAclFlowEntry.match_criteria.icmpTypeMask = match->masks.icmpv4_type;
          }
          else
          {
            flow->flowData.policyAclFlowEntry.match_criteria.icmpTypeMask = 0xff;
          }
        }

        if (ind_ofdpa_match_fields_bitmask & IND_OFDPA_ICMPV4_CODE)
        {
          flow->flowData.policyAclFlowEntry.match_criteria.icmpCode = match->fields.icmpv4_code;
          if (match->masks.icmpv4_code != 0)
          {
            flow->flowData.policyAclFlowEntry.match_criteria.icmpCodeMask = match->masks.icmpv4_code;
          }
          else
          {
            flow->flowData.policyAclFlowEntry.match_criteria.icmpCodeMask = 0xff;
          }
        }
      }
      else if (match->fields.ip_proto == IPPROTO_ICMPV6)
      {
        if (ind_ofdpa_match_fields_bitmask & IND_OFDPA_ICMPV6_TYPE)
        {
          flow->flowData.policyAclFlowEntry.match_criteria.icmpType = match->fields.icmpv6_type;
          if (match->masks.icmpv6_type != 0)
          {
            flow->flowData.policyAclFlowEntry.match_criteria.icmpTypeMask = match->masks.icmpv6_type;
          }
          else
          {
            flow->flowData.policyAclFlowEntry.match_criteria.icmpTypeMask = 0xff;
          }
        }

        if (ind_ofdpa_match_fields_bitmask & IND_OFDPA_ICMPV6_CODE)
        {
          flow->flowData.policyAclFlowEntry.match_criteria.icmpCode = match->fields.icmpv6_code;
          if (match->masks.icmpv6_code != 0)
          {
            flow->flowData.policyAclFlowEntry.match_criteria.icmpCodeMask = match->masks.icmpv6_code;
          }
          else
          {
            flow->flowData.policyAclFlowEntry.match_criteria.icmpCodeMask = 0xff;
          }
        }
      }
      break;
    default:
      LOG_ERROR("Invalid table id %d", flow->tableId);
      err = INDIGO_ERROR_PARAM; 
      break;
  }

  if (err == INDIGO_ERROR_COMPAT)
  {
    LOG_ERROR("Incompatible match field(s) for table %d.", flow->tableId);
  }

  return err;
}

static indigo_error_t ind_ofdpa_translate_openflow_actions(of_list_action_t *actions, ofdpaFlowEntry_t *flow)
{
  of_action_t act;
  of_port_no_t port_no;
  uint32_t invalidPort = 0; /* Flag to check if the output port is valid*/
  int rv;

  OF_LIST_ACTION_ITER(actions, &act, rv) 
  {
    switch (act.header.object_id) 
    {
      case OF_ACTION_OUTPUT: 
      {
        of_action_output_port_get(&act.output, &port_no);
        switch (port_no) 
        {
          case OF_PORT_DEST_CONTROLLER: 
          {
            switch(flow->tableId)
            {
              case OFDPA_FLOW_TABLE_ID_TERMINATION_MAC:
                flow->flowData.terminationMacFlowEntry.outputPort = OFDPA_PORT_CONTROLLER; 
                break;
              case OFDPA_FLOW_TABLE_ID_BRIDGING:
                flow->flowData.bridgingFlowEntry.outputPort = OFDPA_PORT_CONTROLLER;
                break;
              case OFDPA_FLOW_TABLE_ID_ACL_POLICY:
                flow->flowData.policyAclFlowEntry.outputPort = OFDPA_PORT_CONTROLLER;
                break;
              default:
                LOG_ERROR("Upsupported output port action (OFPP_CONTROLLER) for Table: %d", flow->tableId);
                return INDIGO_ERROR_COMPAT;
            }
            break;
          }
          case OF_PORT_DEST_FLOOD: 
          case OF_PORT_DEST_ALL:
          case OF_PORT_DEST_USE_TABLE:
          case OF_PORT_DEST_LOCAL:
          case OF_PORT_DEST_IN_PORT:
          case OF_PORT_DEST_NORMAL:
            LOG_INFO("Unsupported output port 0x%x", port_no);
            return INDIGO_ERROR_NOT_SUPPORTED;
          default:
            /* Physical or logical port as output port */ 
            /* If the port is tunnel logical port */
            if (ind_ofdpa_match_fields_bitmask & IND_OFDPA_TUNNEL_ID)
            {
              if (flow->tableId == OFDPA_FLOW_TABLE_ID_BRIDGING)
              {
                flow->flowData.bridgingFlowEntry.tunnelLogicalPort = port_no;
              }
              else if (flow->tableId == OFDPA_FLOW_TABLE_ID_ACL_POLICY)
              { 
                flow->flowData.policyAclFlowEntry.outputPort = port_no;
              }
              else
              {
                invalidPort = 1;
              }
            }
            else
            {
              invalidPort = 1;
            }

            if (invalidPort)
            {
              LOG_ERROR("Unsupported output port %d for Table %d", port_no, flow->tableId);
              return INDIGO_ERROR_NOT_SUPPORTED;
            }
            break;
        }
        break;
      }
      case OF_ACTION_SET_FIELD: 
      {
        /* HACK loci does not yet support the OXM field in the set-field action */
        of_oxm_t oxm;
        of_oxm_header_init(&oxm.header, act.header.version, 0, 1);
        oxm.header.wire_object = act.header.wire_object;
        oxm.header.wire_object.obj_offset += 4; /* skip action header */
        oxm.header.parent = &act.header;
        of_object_wire_init(&oxm.header, OF_OXM, 0);
        if (oxm.header.length == 0) 
        {
          LOG_ERROR("Failed to parse set-field action");
          return INDIGO_ERROR_COMPAT;
        }
        switch (oxm.header.object_id) 
        {
          case OF_OXM_VLAN_VID: 
          {
            uint16_t vlan_vid;
            of_oxm_vlan_vid_value_get(&oxm.vlan_vid, &vlan_vid);
            if (flow->tableId == OFDPA_FLOW_TABLE_ID_VLAN)
            {
              flow->flowData.vlanFlowEntry.newVlanId = vlan_vid;
              break;
            }
            else
            {
              LOG_ERROR("Unsupported set-field oxm %s for table %d", of_object_id_str[oxm.header.object_id], flow->tableId);
              return INDIGO_ERROR_COMPAT;
            }
          }
          case OF_OXM_VLAN_PCP: 
          {
            uint8_t vlan_pcp;
            of_oxm_vlan_pcp_value_get(&oxm.vlan_pcp, &vlan_pcp);
            if (flow->tableId == OFDPA_FLOW_TABLE_ID_ACL_POLICY)
            {
              flow->flowData.policyAclFlowEntry.vlanPcpAction = 1;
              flow->flowData.policyAclFlowEntry.vlanPcp = vlan_pcp;
              break;
            } 
            else
            {
              LOG_ERROR("Unsupported set-field oxm %s for table %d", of_object_id_str[oxm.header.object_id], flow->tableId);
              return INDIGO_ERROR_COMPAT;
            }
          }
          case OF_OXM_ETH_SRC: 
          {
            of_mac_addr_t mac;
            of_oxm_eth_src_value_get(&oxm.eth_src, &mac);
            break;
          }
          case OF_OXM_ETH_DST: 
          {
            of_mac_addr_t mac;
            of_oxm_eth_dst_value_get(&oxm.eth_dst, &mac);
            break;
          }
          case OF_OXM_IPV4_SRC: 
          {
            uint32_t ipv4;
            of_oxm_ipv4_src_value_get(&oxm.ipv4_src, &ipv4);
            break;
          }
          case OF_OXM_IPV4_DST: 
          {
            uint32_t ipv4;
            of_oxm_ipv4_dst_value_get(&oxm.ipv4_dst, &ipv4);
            break;
          }
          case OF_OXM_IP_DSCP: 
          {
            uint8_t ip_dscp;
            of_oxm_ip_dscp_value_get(&oxm.ip_dscp, &ip_dscp);
            if (ip_dscp > ((uint8_t)IND_OFDPA_IP_DSCP_MASK >> 2)) 
            {
              LOG_ERROR("invalid dscp %d in action %s", ip_dscp,
                        of_object_id_str[act.header.object_id]);
              return INDIGO_ERROR_COMPAT;
            }
            if (flow->tableId == OFDPA_FLOW_TABLE_ID_ACL_POLICY)
            {
              flow->flowData.policyAclFlowEntry.dscpAction = 1;
              flow->flowData.policyAclFlowEntry.dscp = ip_dscp;
            }
            else
            {
              LOG_ERROR("Unsupported set-field oxm %s for table %d", of_object_id_str[oxm.header.object_id], flow->tableId);
              return INDIGO_ERROR_COMPAT;
            }
            break;
          }
          case OF_OXM_IP_ECN: 
          {
            uint8_t ip_ecn;
            of_oxm_ip_ecn_value_get(&oxm.ip_ecn, &ip_ecn);

            if (ip_ecn > IND_OFDPA_IP_ECN_MASK) 
            {
              LOG_ERROR("invalid ecn %d in action %s", ip_ecn,
                        of_object_id_str[act.header.object_id]);
              return INDIGO_ERROR_COMPAT;
            }
            break;
          }
          case OF_OXM_IPV6_SRC: 
          {
            of_ipv6_t ipv6;
            of_oxm_ipv6_src_value_get(&oxm.ipv6_src, &ipv6);
            break;
          }
          case OF_OXM_IPV6_DST: 
          {
            of_ipv6_t ipv6;
            of_oxm_ipv6_dst_value_get(&oxm.ipv6_dst, &ipv6);
            break;
          }
          case OF_OXM_IPV6_FLABEL: 
          {
            uint32_t flabel;
            of_oxm_ipv6_flabel_value_get(&oxm.ipv6_flabel, &flabel);
            if (flabel > IND_OFDPA_IPV6_FLABEL_MASK) 
            {
              LOG_ERROR("invalid flabel 0x%04x in action %s", flabel,
                        of_object_id_str[act.header.object_id]);
              return INDIGO_ERROR_COMPAT;
            }
            break;
          }
          case OF_OXM_TCP_SRC: 
          {
            uint16_t port;
            of_oxm_tcp_src_value_get(&oxm.tcp_src, &port);
            break;
          }
          case OF_OXM_TCP_DST: 
          {
            uint16_t port;
            of_oxm_tcp_dst_value_get(&oxm.tcp_dst, &port);
            break;
          }
          case OF_OXM_UDP_SRC:
          {
            uint16_t port;
            of_oxm_udp_src_value_get(&oxm.udp_src, &port);
            break;
          }
          case OF_OXM_UDP_DST: 
          {
            uint16_t port;
            of_oxm_udp_dst_value_get(&oxm.udp_dst, &port);
            break;
          }
         
          default:
           LOG_ERROR("unsupported set-field oxm %s", of_object_id_str[oxm.header.object_id]);
           return INDIGO_ERROR_COMPAT;
        }
        break;
      }
      case OF_ACTION_SET_QUEUE:
      {
        uint32_t queue_id;
        of_action_set_queue_queue_id_get(&act.set_queue, &queue_id);
        if (flow->tableId == OFDPA_FLOW_TABLE_ID_ACL_POLICY)
        {
          flow->flowData.policyAclFlowEntry.queueIDAction = 1;
          flow->flowData.policyAclFlowEntry.queueID = queue_id;
        }
        else
        {
          LOG_ERROR("Unsupported action %s for table %d", of_object_id_str[act.header.object_id], flow->tableId);
          return INDIGO_ERROR_COMPAT;
        }
        break;
      } 
      case OF_ACTION_SET_DL_DST: 
      {
        of_mac_addr_t mac;
        of_action_set_dl_dst_dl_addr_get(&act.set_dl_dst, &mac);
        break;
      }
      case OF_ACTION_SET_DL_SRC: 
      {
        of_mac_addr_t mac;
        of_action_set_dl_src_dl_addr_get(&act.set_dl_src, &mac);
        break;
      }
      case OF_ACTION_SET_NW_DST: 
      {
        uint32_t ipv4;
        of_action_set_nw_dst_nw_addr_get(&act.set_nw_dst, &ipv4);
        break;
      }
      case OF_ACTION_SET_NW_SRC: 
      {
        uint32_t ipv4;
        of_action_set_nw_src_nw_addr_get(&act.set_nw_src, &ipv4);
        break;
      }
      case OF_ACTION_SET_NW_TOS: 
      {
        uint8_t tos;
        of_action_set_nw_tos_nw_tos_get(&act.set_nw_tos, &tos);
        if (flow->tableId == OFDPA_FLOW_TABLE_ID_ACL_POLICY)
        {
          flow->flowData.policyAclFlowEntry.dscpAction = 1; 
          flow->flowData.policyAclFlowEntry.dscp = tos;
        }
        else
        {
          LOG_ERROR("Unsupported action %s for table %d", of_object_id_str[act.header.object_id], flow->tableId);
          return INDIGO_ERROR_COMPAT;
        }
        break;
      }
      case OF_ACTION_SET_TP_DST: 
      {
        uint16_t port;
        of_action_set_tp_dst_tp_port_get(&act.set_tp_dst, &port);
        break;
      }
      case OF_ACTION_SET_TP_SRC: 
      {
        uint16_t port;
        of_action_set_tp_src_tp_port_get(&act.set_tp_src, &port);
        break;
      }
      case OF_ACTION_SET_VLAN_VID: 
      {
        uint16_t vlan_vid;
        of_action_set_vlan_vid_vlan_vid_get(&act.set_vlan_vid, &vlan_vid);
        if (flow->tableId == OFDPA_FLOW_TABLE_ID_VLAN)
        {   
          flow->flowData.vlanFlowEntry.newVlanId = vlan_vid;
        }
        else
        {
          LOG_ERROR("Unsupported action %s for table %d", of_object_id_str[act.header.object_id], flow->tableId);
          return INDIGO_ERROR_COMPAT;
        }
        break;
      }
      case OF_ACTION_SET_VLAN_PCP: 
      {
        uint8_t vlan_pcp;
        of_action_set_vlan_pcp_vlan_pcp_get(&act.set_vlan_pcp, &vlan_pcp);
        if (flow->tableId == OFDPA_FLOW_TABLE_ID_ACL_POLICY)
        {
          flow->flowData.policyAclFlowEntry.vlanPcpAction = 1;
          flow->flowData.policyAclFlowEntry.vlanPcp = vlan_pcp;
        }
        else
        {
          LOG_ERROR("Unsupported action %s for table %d", of_object_id_str[act.header.object_id], flow->tableId);
          return INDIGO_ERROR_COMPAT;
        } 
        break;
      }
      case OF_ACTION_POP_VLAN:
      case OF_ACTION_STRIP_VLAN: 
      {
        break;
      }
      case OF_ACTION_PUSH_VLAN: 
      {
        uint16_t eth_type;
        of_action_push_vlan_ethertype_get(&act.push_vlan, &eth_type);

        if (eth_type != ETH_P_8021Q) 
        {
          LOG_ERROR("unsupported eth_type 0x%04x in action %s", eth_type,
                    of_object_id_str[act.header.object_id]);
           return INDIGO_ERROR_COMPAT;
        }
        break;
      }
      case OF_ACTION_DEC_NW_TTL:
      case OF_ACTION_NICIRA_DEC_TTL: 
      {
        break;
      }
      case OF_ACTION_SET_NW_TTL: 
      {
        uint8_t ttl;
        of_action_set_nw_ttl_nw_ttl_get(&act.set_nw_ttl, &ttl);
        break;
      }
      case OF_ACTION_GROUP: 
      {
        uint32_t group_id;
        of_action_group_group_id_get(&act.group, &group_id);
        switch(flow->tableId)
        {
          case OFDPA_FLOW_TABLE_ID_BRIDGING:
            flow->flowData.bridgingFlowEntry.groupID = group_id;
            break;
        
          case OFDPA_FLOW_TABLE_ID_UNICAST_ROUTING:
            flow->flowData.unicastRoutingFlowEntry.groupID = group_id;
            break;

          case OFDPA_FLOW_TABLE_ID_MULTICAST_ROUTING:
            flow->flowData.multicastRoutingFlowEntry.groupID = group_id;
            break;

          case OFDPA_FLOW_TABLE_ID_ACL_POLICY:
            flow->flowData.policyAclFlowEntry.groupID = group_id;
            break;

          default:
            LOG_ERROR("Unsupported action %s for table %d", of_object_id_str[act.header.object_id], flow->tableId);
            break;
        }
        break;
      }
      default:
       LOG_ERROR("unsupported action %s", of_object_id_str[act.header.object_id]);
       return INDIGO_ERROR_COMPAT;
    }
  }

  return INDIGO_ERROR_NONE;
}

static indigo_error_t
ind_ofdpa_instructions_get(of_flow_modify_t *flow_mod, ofdpaFlowEntry_t *flow)
{
  of_list_action_t openflow_actions;
  indigo_error_t err;
  uint8_t next_table_id;
  uint32_t meter_id;
  int rv;
  of_list_instruction_t insts;
  of_instruction_t inst;
  uint8_t table_id;


  of_flow_modify_instructions_bind(flow_mod, &insts);

  of_flow_modify_table_id_get(flow_mod, &table_id);

  OF_LIST_INSTRUCTION_ITER(&insts, &inst, rv) 
  {
    switch (inst.header.object_id) 
    {
      case OF_INSTRUCTION_APPLY_ACTIONS:
        switch(flow->tableId)
        {
          case OFDPA_FLOW_TABLE_ID_INGRESS_PORT:
          case OFDPA_FLOW_TABLE_ID_UNICAST_ROUTING:
          case OFDPA_FLOW_TABLE_ID_MULTICAST_ROUTING:
            LOG_ERROR("Unsupported instruction %s for flow table %d.", of_object_id_str[inst.header.object_id], flow->tableId);
            return INDIGO_ERROR_COMPAT;
          case OFDPA_FLOW_TABLE_ID_VLAN:
          case OFDPA_FLOW_TABLE_ID_TERMINATION_MAC:
          case OFDPA_FLOW_TABLE_ID_BRIDGING:
          case OFDPA_FLOW_TABLE_ID_ACL_POLICY:
          default:
            break;
        }

        of_instruction_apply_actions_actions_bind(&inst.apply_actions, 
                                                  &openflow_actions);
        if ((err = ind_ofdpa_translate_openflow_actions(&openflow_actions,
                                                        flow)) < 0) 
        {
          return err;
        }
        break;
        case OF_INSTRUCTION_WRITE_ACTIONS:
          switch(flow->tableId)
          {
            case OFDPA_FLOW_TABLE_ID_INGRESS_PORT:
            case OFDPA_FLOW_TABLE_ID_VLAN:
            case OFDPA_FLOW_TABLE_ID_TERMINATION_MAC:
              LOG_ERROR("Unsupported instruction %s for flow table %d.", of_object_id_str[inst.header.object_id], flow->tableId);
              return INDIGO_ERROR_COMPAT;
            case OFDPA_FLOW_TABLE_ID_UNICAST_ROUTING:
            case OFDPA_FLOW_TABLE_ID_MULTICAST_ROUTING:
            case OFDPA_FLOW_TABLE_ID_BRIDGING:
            case OFDPA_FLOW_TABLE_ID_ACL_POLICY:
            default:
              break;
          }
          of_instruction_write_actions_actions_bind(&inst.write_actions,
                                                    &openflow_actions);
          if ((err = ind_ofdpa_translate_openflow_actions(&openflow_actions,
                                                          flow)) < 0) 
          {
            return err;
          }
          break;
        case OF_INSTRUCTION_CLEAR_ACTIONS:
          if (flow->tableId == OFDPA_FLOW_TABLE_ID_ACL_POLICY)
          {
              flow->flowData.policyAclFlowEntry.clearActions = 1;
          }
          else
          {
            LOG_ERROR("Unsupported instruction %s for flow table %d.", of_object_id_str[inst.header.object_id], flow->tableId);
            return INDIGO_ERROR_COMPAT;
          }
          break;
        case OF_INSTRUCTION_GOTO_TABLE:
          of_instruction_goto_table_table_id_get(&inst.goto_table, &next_table_id);

          switch(flow->tableId)
          {
            case OFDPA_FLOW_TABLE_ID_INGRESS_PORT:
              flow->flowData.ingressPortFlowEntry.gotoTableId = next_table_id;
              break;
            case OFDPA_FLOW_TABLE_ID_VLAN:     
              flow->flowData.vlanFlowEntry.gotoTableId = next_table_id;
              break;
            case OFDPA_FLOW_TABLE_ID_TERMINATION_MAC:
              flow->flowData.terminationMacFlowEntry.gotoTableId = next_table_id;
              break;
            case OFDPA_FLOW_TABLE_ID_UNICAST_ROUTING:
              flow->flowData.unicastRoutingFlowEntry.gotoTableId = next_table_id;
              break;
            case OFDPA_FLOW_TABLE_ID_MULTICAST_ROUTING:
              flow->flowData.multicastRoutingFlowEntry.gotoTableId = next_table_id;
              break;
            case OFDPA_FLOW_TABLE_ID_BRIDGING:
              flow->flowData.bridgingFlowEntry.gotoTableId = next_table_id;
              break;
            case OFDPA_FLOW_TABLE_ID_ACL_POLICY:
              LOG_ERROR("Unsupported instruction %s for flow table %d.", of_object_id_str[inst.header.object_id], flow->tableId);
              return INDIGO_ERROR_COMPAT;
            default:
              LOG_INFO("Invalid Table id: %d.", flow->tableId);
              break;
          }
          break;
        case OF_INSTRUCTION_METER:
          of_instruction_meter_meter_id_get(&inst.meter, &meter_id);
          LOG_INFO("Unsupported instruction: meter_id.");
          break;
      default:
        LOG_INFO("Invalid instruction.");
        return INDIGO_ERROR_COMPAT;
    }
  }
  return INDIGO_ERROR_NONE;
}

static indigo_error_t ind_ofdpa_packet_out_actions_get(of_list_action_t *of_list_actions, 
                                                       indPacketOutActions_t *packetOutActions)
{
  of_action_t act;
  indigo_error_t err = INDIGO_ERROR_NONE;
  int rv;


  OF_LIST_ACTION_ITER(of_list_actions, &act, rv)
  {
    switch (act.header.object_id)
    {
      case OF_ACTION_OUTPUT:
      {
        of_port_no_t port_no;
        of_action_output_port_get(&act.output, &port_no);
        switch (port_no)
        {
          case OF_PORT_DEST_CONTROLLER:
          case OF_PORT_DEST_FLOOD:
          case OF_PORT_DEST_ALL:
          case OF_PORT_DEST_LOCAL:
          case OF_PORT_DEST_IN_PORT:
          case OF_PORT_DEST_NORMAL:
            LOG_INFO("Unsupported output port 0x%x", port_no);
            err = INDIGO_ERROR_NOT_SUPPORTED;
            break;
          case OF_PORT_DEST_USE_TABLE:
            packetOutActions->pipeline = 1;
            break;
          default:
            packetOutActions->outputPort = port_no;
            break;
        }
        break;
      }
      default:
        LOG_ERROR("Unsupported action for packet out: %s", of_object_id_str[act.header.object_id]);
        err = INDIGO_ERROR_NOT_SUPPORTED;
        break;
    }
  } 

  return err; 
}


indigo_error_t indigo_fwd_forwarding_features_get(of_features_reply_t *features_reply)
{
  uint32_t capabilities = 0;

  LOG_TRACE("%s() called", __FUNCTION__);

  if (features_reply->version < OF_VERSION_1_3)
  {
    LOG_INFO("Unsupported OpenFlow version 0x%x.", features_reply->version);
    return INDIGO_ERROR_VERSION;
  }

  /* Number of tables supported by datapath. */
  of_features_reply_n_tables_set(features_reply, TABLE_NAME_LIST_SIZE);

  OF_CAPABILITIES_FLAG_FLOW_STATS_SET(capabilities, features_reply->version);
  OF_CAPABILITIES_FLAG_TABLE_STATS_SET(capabilities, features_reply->version);
  OF_CAPABILITIES_FLAG_PORT_STATS_SET(capabilities, features_reply->version);
  OF_CAPABILITIES_FLAG_QUEUE_STATS_SET(capabilities, features_reply->version);
  of_features_reply_capabilities_set(features_reply, capabilities);

  return INDIGO_ERROR_NONE;
}


indigo_error_t indigo_fwd_flow_create(indigo_cookie_t flow_id,
                                      of_flow_add_t *flow_add,
                                      uint8_t *table_id)
{
  indigo_error_t err = INDIGO_ERROR_NONE;
  OFDPA_ERROR_t ofdpa_rv = OFDPA_E_NONE;
  ofdpaFlowEntry_t flow;
  ofdpaFlowEntryStats_t  flowStats;
  uint16_t priority;
  uint16_t idle_timeout, hard_timeout; 
  of_match_t of_match;

  LOG_TRACE("Flow create called");

  if (flow_add->version < OF_VERSION_1_3) 
  {
    LOG_INFO("OpenFlow version 0x%x unsupported", flow_add->version);
    return INDIGO_ERROR_VERSION;
  }

  memset(&flowStats, 0, sizeof(flowStats));
  memset(&flow, 0, sizeof(flow));
    
  flow.cookie = flow_id;

  /* Get the Flow Table ID */
  of_flow_add_table_id_get(flow_add, table_id);
  flow.tableId = (uint32_t)*table_id;

  /* ofdpa Flow priority */
  of_flow_add_priority_get(flow_add, &priority);
  flow.priority = (uint32_t)priority;

  /* Get the idle time and hard time */
  (void)of_flow_modify_idle_timeout_get((of_flow_modify_t *)flow_add, &idle_timeout);
  (void)of_flow_modify_hard_timeout_get((of_flow_modify_t *)flow_add, &hard_timeout);
  flow.idle_time = (uint32_t)idle_timeout;
  flow.hard_time = (uint32_t)hard_timeout;

  memset(&of_match, 0, sizeof(of_match));
  ind_ofdpa_match_fields_bitmask = 0; /* Set the bit mask to 0 before being set in of_flow_add_match_get() */
  if (of_flow_add_match_get(flow_add, &of_match) < 0) 
  {
    LOG_ERROR("Error getting openflow match criteria.");
    return INDIGO_ERROR_UNKNOWN;
  }

  /* Get the match fields and masks from LOCI match structure */
  err = ind_ofdpa_match_fields_masks_get(&of_match, &flow);
  if (err != INDIGO_ERROR_NONE)
  {
    LOG_INFO("Error getting match fields and masks. (err = %d)", err);
    return err;
  }
  
  /* Get the instructions set from the LOCI flow add object */
  err = ind_ofdpa_instructions_get(flow_add, &flow); 
  if (err != INDIGO_ERROR_NONE)
  {
    LOG_ERROR("Failed to get flow instructions. (err = %d)", err);
    return err; 
  }

  /* Submit the changes to ofdpa */
  ofdpa_rv = ofdpaFlowAdd(&flow);
  if (ofdpa_rv != OFDPA_E_NONE)
  {
    LOG_ERROR("Failed to add flow. (ofdpa_rv = %d)", ofdpa_rv);
  }
  else
  {
    LOG_INFO("Flow added successfully. (ofdpa_rv = %d)", ofdpa_rv);
  }
  

  return (indigoConvertOfdpaRv(ofdpa_rv));
}

indigo_error_t indigo_fwd_flow_modify(indigo_cookie_t flow_id,
                                      of_flow_modify_t *flow_modify)
{
  indigo_error_t err = INDIGO_ERROR_NONE;
  ofdpaFlowEntry_t flow;
  ofdpaFlowEntryStats_t flowStats;
  OFDPA_ERROR_t ofdpa_rv = OFDPA_E_NONE;  
  of_match_t of_match;

  LOG_TRACE("Flow modify called");	

  if (flow_modify->version < OF_VERSION_1_3)
  {
    LOG_ERROR("OpenFlow version 0x%x unsupported", flow_modify->version);
    return INDIGO_ERROR_VERSION;
  }

  memset(&flow, 0, sizeof(flow));
  memset(&flowStats, 0, sizeof(flowStats));

  /* Get the flow entries and flow stats from the indigo cookie */
  ofdpa_rv = ofdpaFlowByCookieGet(flow_id, &flow, &flowStats);
  if (ofdpa_rv != OFDPA_E_NONE)
  {
    if (ofdpa_rv == OFDPA_E_NOT_FOUND)
    {
      LOG_ERROR("Request to modify non-existent flow. (ofdpa_rv = %d)", ofdpa_rv);
    }
    else
    {
      LOG_ERROR("Invalid flow. (ofdpa_rv = %d)", ofdpa_rv);
    }
    return (indigoConvertOfdpaRv(ofdpa_rv));   
  }

  memset(&of_match, 0, sizeof(of_match));
  if (of_flow_add_match_get(flow_modify, &of_match) < 0)
  {
    LOG_ERROR("Error getting openflow match criteria.");
    return INDIGO_ERROR_UNKNOWN;
  }
  
  memset(&flow.flowData, 0, sizeof(flow.flowData));

  /* Get the match fields and masks from LOCI match structure */
  err = ind_ofdpa_match_fields_masks_get(&of_match, &flow);
  if (err != INDIGO_ERROR_NONE)
  {
    LOG_ERROR("Error getting match fields and masks. (err = %d)", err);
    return err;
  }

  /* Get the modified instructions set from the LOCI flow add object */
  err = ind_ofdpa_instructions_get(flow_modify, &flow);
  if (err != INDIGO_ERROR_NONE)  
  {
    LOG_ERROR("Failed to get flow instructions. (err = %d)", err);
    return err;
  } 

  /* Submit the changes to ofdpa */
  ofdpa_rv = ofdpaFlowModify(&flow);
  if (ofdpa_rv!= OFDPA_E_NONE)
  {
    LOG_ERROR("Failed to modify flow. (ofdpa_rv = %d)", ofdpa_rv);
  }
  else
  {
    LOG_TRACE("Flow modified successfully. (ofdpa_rv = %d)", ofdpa_rv);
  }

  return (indigoConvertOfdpaRv(ofdpa_rv));
}

indigo_error_t indigo_fwd_flow_delete(indigo_cookie_t flow_id,
                                      indigo_fi_flow_stats_t *flow_stats)
{
  ofdpaFlowEntry_t flow;
  ofdpaFlowEntryStats_t flowStats;
  OFDPA_ERROR_t ofdpa_rv = OFDPA_E_NONE;


  LOG_TRACE("Flow delete called");

  memset(&flow, 0, sizeof(flow));
  memset(&flowStats, 0, sizeof(flowStats));
	
  ofdpa_rv = ofdpaFlowByCookieGet(flow_id, &flow, &flowStats);
  if (ofdpa_rv != OFDPA_E_NONE)
  {
    if (ofdpa_rv == OFDPA_E_NOT_FOUND)
    {
      LOG_INFO("Request to delete non-existent flow. (ofdpa_rv = %d)", ofdpa_rv);
    }
    else
    {
      LOG_ERROR("Invalid flow. (ofdpa_rv = %d)", ofdpa_rv);
    }

    return (indigoConvertOfdpaRv(ofdpa_rv));
  }

  flow_stats->flow_id = flow_id;
  flow_stats->packets = flowStats.receivedPackets;
  flow_stats->bytes = flowStats.receivedBytes;
  flow_stats->duration_ns = (flowStats.durationSec)*(IND_OFDPA_NANO_SEC); /* Convert to nano seconds*/

  /* Delete the flow entry */
  ofdpa_rv = ofdpaFlowByCookieDelete(flow_id);
  if (ofdpa_rv != OFDPA_E_NONE)
  {
    LOG_INFO("Failed to delete flow. (ofdpa_rv = %d)", ofdpa_rv);
  }
  else
  {
    LOG_TRACE("Flow deleted successfully. (ofdpa_rv = %d)", ofdpa_rv);
  }

  return (indigoConvertOfdpaRv(ofdpa_rv));;
}

indigo_error_t indigo_fwd_flow_stats_get(indigo_cookie_t flow_id,
                                         indigo_fi_flow_stats_t *flow_stats)
{
  OFDPA_ERROR_t ofdpa_rv = OFDPA_E_NONE;
  ofdpaFlowEntry_t flow;
  ofdpaFlowEntryStats_t flowStats;

  memset(&flow, 0, sizeof(flow));
  memset(&flowStats, 0, sizeof(flowStats));

  /* Get the flow and flow stats from flow id */	
  ofdpa_rv = ofdpaFlowByCookieGet(flow_id, &flow, &flowStats);
  if (ofdpa_rv == OFDPA_E_NONE)
  {
    flow_stats->flow_id = flow_id;
    flow_stats->duration_ns = (flowStats.durationSec) * (IND_OFDPA_NANO_SEC); /* Convert to nsecs */
    flow_stats->packets = flowStats.receivedPackets;
    flow_stats->bytes = flowStats.receivedBytes;

    LOG_INFO("Flow stats get successful. (ofdpa_rv = %d)", ofdpa_rv);
  }
  else if (ofdpa_rv == OFDPA_E_NOT_FOUND)
  {
    LOG_ERROR("Request to get stats of a non-existent flow. (ofdpa_rv = %d)", ofdpa_rv);
  }
  else
  {
    LOG_ERROR("Failed to get flow stats. (ofdpa_rv = %d)", ofdpa_rv);
  }

  return (indigoConvertOfdpaRv(ofdpa_rv));
}

void indigo_fwd_table_mod(of_table_mod_t *of_table_mod,
                          indigo_cookie_t callback_cookie)
{
  LOG_INFO("indigo_fwd_table_mod() unsupported.");
  return;
}

indigo_error_t indigo_fwd_table_stats_get(of_table_stats_request_t *table_stats_request,
                                          of_table_stats_reply_t **table_stats_reply)
{
  of_version_t version = table_stats_request->version;
  OFDPA_ERROR_t ofdpa_rv = OFDPA_E_NONE;
  uint32_t xid;
  uint32_t i;
  ofdpaFlowTableInfo_t tableInfo;
  of_table_stats_entry_t entry[1];
  of_table_stats_reply_t *reply;
  

  if (version < OF_VERSION_1_3)
  {
    LOG_INFO("Unsupported OpenFlow version 0x%x.", version);
    return INDIGO_ERROR_VERSION;
  }

  reply = of_table_stats_reply_new(version);
  if (reply == NULL) 
  {
    LOG_ERROR("Error allocating memory");  
    return INDIGO_ERROR_RESOURCE; 
  }

  *table_stats_reply = reply;

  of_table_stats_request_xid_get(table_stats_request, &xid);
  of_table_stats_reply_xid_set(*table_stats_reply, xid);

  of_list_table_stats_entry_t list[1];
  of_table_stats_reply_entries_bind(*table_stats_reply, list);

  
  for (i = 0; i < TABLE_NAME_LIST_SIZE; i++)
  {
    of_table_stats_entry_init(entry, version, -1, 1);
    (void) of_list_table_stats_entry_append_bind(list, entry);

    ofdpa_rv = ofdpaFlowTableInfoGet(tableNameList[i].type, &tableInfo);
    if (ofdpa_rv != OFDPA_E_NONE)
    {
      LOG_INFO("Error getting flow table info. (ofdpa_rv = %d)", ofdpa_rv);
      return (indigoConvertOfdpaRv(ofdpa_rv));
    }

    /* Table Id */
    of_table_stats_entry_table_id_set(entry, tableNameList[i].type);

    /* Number of entries in the table */
    of_table_stats_entry_active_count_set(entry, tableInfo.numEntries);

    /* Number of packets looked up in table. */
    of_table_stats_entry_lookup_count_set(entry, 0);

    /* Number of packets that hit table. */
    of_table_stats_entry_matched_count_set(entry, 0);
  }

  return (indigoConvertOfdpaRv(ofdpa_rv));
}

indigo_error_t indigo_fwd_packet_out(of_packet_out_t *packet_out)
{
  OFDPA_ERROR_t  ofdpa_rv = OFDPA_E_NONE;
  indigo_error_t err = INDIGO_ERROR_NONE;
  indPacketOutActions_t packetOutActions;
  ofdpa_buffdesc pkt;

  of_port_no_t   of_port_num;
  of_list_action_t of_list_action[1];
  of_octets_t    of_octets[1];

  of_packet_out_in_port_get(packet_out, &of_port_num);
  of_packet_out_data_get(packet_out, of_octets);
  of_packet_out_actions_bind(packet_out, of_list_action);

	
  pkt.pstart = (char *)of_octets->data;
  pkt.size = of_octets->bytes; 

  memset(&packetOutActions, 0, sizeof(packetOutActions)); 
  err = ind_ofdpa_packet_out_actions_get(of_list_action, &packetOutActions);
  if (err != INDIGO_ERROR_NONE)
  {
    LOG_ERROR("Failed to get packet out actions. (err = %d)", err);
    return err;
  }


  if (packetOutActions.pipeline)
  {
    ofdpa_rv = ofdpaPktSend(&pkt, OFDPA_PKT_LOOKUP, packetOutActions.outputPort, of_port_num);
  }
  else
  {
    ofdpa_rv = ofdpaPktSend(&pkt, 0, packetOutActions.outputPort, 0);
  }

  if (ofdpa_rv != OFDPA_E_NONE)
  {
    LOG_ERROR("Packet send failed. (ofdpa_rv = %d)", ofdpa_rv);
  }
  else
  {
    LOG_INFO("Packet sent out of output port (%d) successfully. (ofdpa_rv = %d)", packetOutActions.outputPort, ofdpa_rv);
  }

  return (indigoConvertOfdpaRv(ofdpa_rv));
}

indigo_error_t indigo_fwd_experimenter(of_experimenter_t *experimenter,
                                       indigo_cxn_id_t cxn_id)
{
  LOG_INFO("indigo_fwd_experimenter() unsupported.");
  return INDIGO_ERROR_NOT_SUPPORTED;
}

indigo_error_t indigo_fwd_expiration_enable_set(int is_enabled)
{
  LOG_INFO("indigo_fwd_expiration_enable_set() unsupported.");
  return INDIGO_ERROR_NOT_SUPPORTED;
}

indigo_error_t indigo_fwd_expiration_enable_get(int *is_enabled)
{
  LOG_INFO("indigo_fwd_expiration_enable_get() unsupported.");
  return INDIGO_ERROR_NOT_SUPPORTED;
}

void ind_ofdpa_flow_event_receive(void)
{
  ofdpaFlowEvent_t flowEventData;

  LOG_TRACE("Reading Flow Events");

  memset(&flowEventData, 0, sizeof(flowEventData));
  flowEventData.flowMatch.tableId = OFDPA_FLOW_TABLE_ID_VLAN;

  while (ofdpaFlowEventNextGet(&flowEventData) == OFDPA_E_NONE)
  {
    if (flowEventData.eventMask & OFDPA_FLOW_EVENT_HARD_TIMEOUT)
    {
      LOG_INFO("Received flow event on hard timeout.");
      ind_core_flow_expiry_handler(flowEventData.flowMatch.cookie,
                                   INDIGO_FLOW_REMOVED_HARD_TIMEOUT);
    }
    else
    {
      LOG_INFO("Received flow event on idle timeout.");
      ind_core_flow_expiry_handler(flowEventData.flowMatch.cookie,
                                   INDIGO_FLOW_REMOVED_IDLE_TIMEOUT);
    }
  }
  return;
}

static void ind_ofdpa_key_to_match(uint32_t portNum, of_match_t *match)
{
  memset(match, 0, sizeof(*match));

  /* We only populate the masks for this OF version */
  match->version = ofagent_of_version;

  of_match_fields_t *fields = &match->fields;

  fields->in_port = portNum;
  OF_MATCH_MASK_IN_PORT_EXACT_SET(match);
}

static indigo_error_t
ind_ofdpa_fwd_pkt_in(of_port_no_t in_port,
               uint8_t *data, unsigned int len, unsigned reason,
               of_match_t *match, OFDPA_FLOW_TABLE_ID_t tableId)
{
  of_octets_t of_octets = { .data = data, .bytes = len };
  of_packet_in_t *of_packet_in;

  LOG_TRACE("Sending packet-in");

  of_packet_in = of_packet_in_new(ofagent_of_version);
  if (of_packet_in == NULL) 
  {
    return INDIGO_ERROR_RESOURCE;
  }

  of_packet_in_total_len_set(of_packet_in, len);
  of_packet_in_reason_set(of_packet_in, reason);
  of_packet_in_table_id_set(of_packet_in, tableId);
  of_packet_in_cookie_set(of_packet_in, 0xffffffffffffffff);

  if (of_packet_in_match_set(of_packet_in, match) != OF_ERROR_NONE) 
  {
    LOG_ERROR("Failed to write match to packet-in message");
    of_packet_in_delete(of_packet_in);
    return INDIGO_ERROR_UNKNOWN;
  }

  if (of_packet_in_data_set(of_packet_in, &of_octets) != OF_ERROR_NONE) 
  {
    LOG_ERROR("Failed to write packet data to packet-in message");
    of_packet_in_delete(of_packet_in);
    return INDIGO_ERROR_UNKNOWN;
  }

  return indigo_core_packet_in(of_packet_in);
}

void ind_ofdpa_pkt_receive(void)
{
  indigo_error_t rc;
  uint32_t i;
  uint32_t maxPktSize;
  ofdpaPacket_t rxPkt;
  of_match_t match;
  struct timeval timeout;

  /* Determine how large receive buffer must be */
  if (ofdpaMaxPktSizeGet(&maxPktSize) != OFDPA_E_NONE)
  {
    LOG_ERROR("\nFailed to determine maximum receive packet size.\r\n");
    return;
  }

  memset(&rxPkt, 0, sizeof(ofdpaPacket_t));
  rxPkt.pktData.pstart = (char*) malloc(maxPktSize);
  if (rxPkt.pktData.pstart == NULL)
  {
    LOG_ERROR("\nFailed to allocate receive packet buffer\r\n");
    return;
  }
  rxPkt.pktData.size = maxPktSize;

  timeout.tv_sec = 0;
  timeout.tv_usec = 0;

  while (ofdpaPktReceive(&timeout, &rxPkt) == OFDPA_E_NONE)
  {
    LOG_TRACE("Client received packet");
    LOG_TRACE("Reason:  %d", rxPkt.reason);
    LOG_TRACE("Table ID:  %d", rxPkt.tableId);
    LOG_TRACE("Ingress port:  %u", rxPkt.inPortNum);
    LOG_TRACE("Size:  %u\r\n", rxPkt.pktData.size);
    for (i = 0; i < rxPkt.pktData.size; i++)
    {
      if (i && ((i % 16) == 0))
        LOG_TRACE("\r\n");
      LOG_TRACE("%02x ", (unsigned int) *(rxPkt.pktData.pstart + i));
    }
    LOG_TRACE("\r\n");

    ind_ofdpa_key_to_match(rxPkt.inPortNum, &match);

    rc = ind_ofdpa_fwd_pkt_in(rxPkt.inPortNum, rxPkt.pktData.pstart, 
                         (rxPkt.pktData.size - 4), rxPkt.reason, 
                         &match, rxPkt.tableId);

    if (rc != INDIGO_ERROR_NONE)
    {
      LOG_ERROR("Could not send Packet-in message, rc = 0x%x", rc);
    }
  }
  free(rxPkt.pktData.pstart);
  return;
}


