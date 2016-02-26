/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2013-2016
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
#include <unistd.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <stdbool.h>
#include <pthread.h>
#include <errno.h>
#include "ind_ofdpa_util.h"
#include "indigo/memory.h"
#include "indigo/forwarding.h"
#include "ind_ofdpa_log.h"
#include "indigo/of_state_manager.h"
#include "indigo/fi.h"
#include "OFStateManager/ofstatemanager.h"


static indigo_error_t ind_ofdpa_packet_out_actions_get(of_list_action_t *of_list_actions,
                                                       indPacketOutActions_t *packetOutActions);
static indigo_error_t ind_ofdpa_match_fields_masks_get(const of_match_t *match, ofdpaFlowEntry_t *flow);
static indigo_error_t ind_ofdpa_translate_openflow_actions(of_object_id_t type, of_list_action_t *actions, ofdpaFlowEntry_t *flow);
static indigo_error_t indigo_set_mpls_qos(ofdpa_mpls_set_qos_action_mod_msg_t *mpls_set_qos_action);
static indigo_error_t indigo_oam_dataplane(ofdpa_oam_dataplane_ctr_mod_msg_t *oam_dataplane_ctr);
static indigo_error_t indigo_drop_status(ofdpa_oam_drop_status_mod_msg_t *oam_drop_status);
static indigo_error_t indigo_remark_action(ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *mpls_tunnel_label_remark);
extern int ofagent_of_version;

/*
 * Build up a record of all the match fields included in the flow_mod message. This is used to detect when the message
 * contains a match field that is not supported by the flow table. The agent is required to reject flows that request
 * a match that the switch cannot support.
 */
static void ind_ofdpa_populate_flow_bitmask(const of_match_t *match, ind_ofdpa_fields_t *ind_ofdpa_match_fields_bitmask)
{
  of_mac_addr_t macAddr;
  of_ipv6_t     ipAddr;

  *ind_ofdpa_match_fields_bitmask = 0;

  if (match->masks.vlan_vid != 0)
  {
    (*ind_ofdpa_match_fields_bitmask) |= IND_OFDPA_VLANID;
  }

  memset(&macAddr, 0, sizeof(macAddr));

  if ((memcmp(&match->masks.eth_src, &macAddr, sizeof(match->masks.eth_src))) != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_SRCMAC;
  }

  if ((memcmp(&match->masks.eth_dst, &macAddr, sizeof(match->masks.eth_dst))) != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_DSTMAC;
  }

  if ((match->masks.in_port != 0) ||(match->masks.in_phy_port != 0))
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_PORT;
  }

  if (match->masks.eth_type != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_ETHER_TYPE;
  }

  if (match->masks.ipv4_dst != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_IPV4_DST;
  }

  if (match->masks.ipv4_src != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_IPV4_SRC;
  }

  memset(&ipAddr, 0, sizeof(ipAddr));

  if ((memcmp(&match->masks.ipv6_dst, &ipAddr, sizeof(match->masks.ipv6_dst))) != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_IPV6_DST;
  }

  if ((memcmp(&match->masks.ipv6_src, &ipAddr, sizeof(match->masks.ipv6_src))) != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_IPV6_SRC;
  }

  if (match->masks.tunnel_id != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_TUNNEL_ID;
  }

  if (match->masks.vlan_pcp != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_VLAN_PCP;
  }

  if (match->masks.ofdpa_dei != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_VLAN_DEI;
  }

  if (match->masks.arp_spa != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_IPV4_ARP_SPA;
  }

  if (match->masks.ip_dscp != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_IP_DSCP;
  }

  if (match->masks.ip_ecn != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_IP_ECN;
  }

  if (match->masks.ip_proto != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_IP_PROTO;
  }

  if (match->masks.tcp_src != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_TCP_L4_SRC_PORT;
  }

  if (match->masks.tcp_dst != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_TCP_L4_DST_PORT;
  }

  if (match->masks.udp_src != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_UDP_L4_SRC_PORT;
  }

  if (match->masks.udp_dst != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_UDP_L4_DST_PORT;
  }

  if (match->masks.sctp_src != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_SCTP_L4_SRC_PORT;
  }

  if (match->masks.sctp_dst != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_SCTP_L4_DST_PORT;
  }

  if (match->masks.icmpv4_type != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_ICMPV4_TYPE;
  }

  if (match->masks.icmpv4_code != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_ICMPV4_CODE;
  }

  if (match->masks.ipv6_flabel != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_IPV6_FLOW_LABEL;
  }

  if (match->masks.icmpv6_type != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_ICMPV6_TYPE;
  }

  if (match->masks.icmpv6_code != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_ICMPV6_CODE;
  }
  if (match->masks.mpls_label != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_MPLS_LABEL;
  }
  if (match->masks.mpls_bos != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_MPLS_BOS;
  }
  if (match->masks.mpls_tc != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_MPLS_TC;
  }
  if (match->masks.ofdpa_mpls_l2_port != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_MPLS_L2_PORT;
  }
  if (match->masks.ofdpa_ovid != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_OVID;
  }
  if (match->masks.ofdpa_vrf != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_VRF;
  }
  if (match->masks.ofdpa_qos_index != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_QOS_INDEX;
  }
  if (match->masks.ofdpa_lmep_id != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_LMEP_ID;
  }
  if (match->masks.ofdpa_mpls_ttl != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_MPLS_TTL;
  }
  if (match->masks.ofdpa_bfd_discriminator != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_BFD_DISCRIMINATOR;
  }
  if (match->masks.ofdpa_mpls_data_first_nibble != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_MPLS_DATA_FIRST_NIBBLE;
  }
  if (match->masks.ofdpa_mpls_ach_channel != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_MPLS_ACH_CHANNEL;
  }
  if (match->masks.ofdpa_mpls_next_label_is_gal != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_MPLS_NEXT_LABEL_IS_GAL;
  }
  if (match->masks.ofdpa_oam_y1731_mdl != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_OAM_Y1731_MDL;
  }
  if (match->masks.ofdpa_oam_y1731_opcode != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_OAM_Y1731_OPCODE;
  }
  if (match->masks.ofdpa_color_actions_index != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_COLOR_ACTIONS_INDEX;
  }
  if (match->masks.ofdpa_txfcl != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_TXFCL;
  }
  if (match->masks.ofdpa_rxfcl != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_RXFCL;
  }
  if (match->masks.ofdpa_rx_timestamp != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_RX_TIMESTAMP;
  }
  if (match->masks.ofdpa_l3_in_port != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_L3_IN_PORT;
  }
  if (match->masks.ofdpa_protection_index != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_PROTECTION_INDEX;
  }
  if (match->masks.ofdpa_color != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_COLOR;
  }
  if (match->masks.ofdpa_traffic_class != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_TC;
  }
  if (match->masks.ofdpa_allow_vlan_translation != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_OFDPA_ALLOW_VLAN_TRANSLATION;
  }
  if (match->masks.onf_actset_output != 0)
  {
    *ind_ofdpa_match_fields_bitmask |= IND_ONF_ACTSET_OUTPUT;
  }
  LOG_TRACE("match_fields_bitmask is 0x%llX", *ind_ofdpa_match_fields_bitmask);
}

/* Get the flow match criteria from of_match */

static indigo_error_t ind_ofdpa_match_fields_masks_get(const of_match_t *match, ofdpaFlowEntry_t *flow)
{
  indigo_error_t err = INDIGO_ERROR_NONE;
  ind_ofdpa_fields_t ind_ofdpa_match_fields_bitmask;

  ind_ofdpa_populate_flow_bitmask(match, &ind_ofdpa_match_fields_bitmask);

  switch (flow->tableId)
  {
    case OFDPA_FLOW_TABLE_ID_INGRESS_PORT:
      if ((ind_ofdpa_match_fields_bitmask| IND_OFDPA_ING_PORT_FLOW_MATCH_BITMAP) != IND_OFDPA_ING_PORT_FLOW_MATCH_BITMAP)
      {
        err = INDIGO_ERROR_COMPAT;
      }
      else if ((ind_ofdpa_match_fields_bitmask & IND_OFDPA_ING_PORT_FLOW_MATCH_MAND_BITMAP)
                != IND_OFDPA_ING_PORT_FLOW_MATCH_MAND_BITMAP)
      {
        err = INDIGO_ERROR_COMPAT;
      }
      else
      {
        flow->flowData.ingressPortFlowEntry.match_criteria.inPort        = match->fields.in_port;
        flow->flowData.ingressPortFlowEntry.match_criteria.inPortMask    = match->masks.in_port;
        flow->flowData.ingressPortFlowEntry.match_criteria.tunnelId      = match->fields.tunnel_id;
        flow->flowData.ingressPortFlowEntry.match_criteria.tunnelIdMask  = match->masks.tunnel_id;
        flow->flowData.ingressPortFlowEntry.match_criteria.etherType     = match->fields.eth_type;
        flow->flowData.ingressPortFlowEntry.match_criteria.etherTypeMask = match->masks.eth_type;
        flow->flowData.ingressPortFlowEntry.match_criteria.lmepId        = match->fields.ofdpa_lmep_id;
        flow->flowData.ingressPortFlowEntry.match_criteria.lmepIdMask    = match->masks.ofdpa_lmep_id;
      }
      break;

    case OFDPA_FLOW_TABLE_ID_INJECTED_OAM:
      if ((ind_ofdpa_match_fields_bitmask| IND_OFDPA_INJECTED_OAM_FLOW_MATCH_BITMAP) != IND_OFDPA_INJECTED_OAM_FLOW_MATCH_BITMAP)
      {
       err = INDIGO_ERROR_COMPAT;
      }
      else if ((ind_ofdpa_match_fields_bitmask & IND_OFDPA_INJECTED_OAM_FLOW_MATCH_MAND_BITMAP)
                != IND_OFDPA_INJECTED_OAM_FLOW_MATCH_MAND_BITMAP)
      {
       err = INDIGO_ERROR_COMPAT;
      }
      else
      {
        flow->flowData.injectedOamFlowEntry.match_criteria.lmepId = match->fields.ofdpa_lmep_id;
      }
      break;

    case OFDPA_FLOW_TABLE_ID_VLAN:
      if ((ind_ofdpa_match_fields_bitmask| IND_OFDPA_VLAN_FLOW_MATCH_BITMAP) != IND_OFDPA_VLAN_FLOW_MATCH_BITMAP)
      {
        err = INDIGO_ERROR_COMPAT;
      }
      else if ((ind_ofdpa_match_fields_bitmask & IND_OFDPA_VLAN_FLOW_MATCH_MAND_BITMAP)
                != IND_OFDPA_VLAN_FLOW_MATCH_MAND_BITMAP)
      {
        err = INDIGO_ERROR_COMPAT;
      }
      else
      {
        flow->flowData.vlanFlowEntry.match_criteria.inPort        = match->fields.in_port;
        flow->flowData.vlanFlowEntry.match_criteria.vlanId        = match->fields.vlan_vid & (OFDPA_VID_PRESENT | OFDPA_VID_EXACT_MASK);
        flow->flowData.vlanFlowEntry.match_criteria.vlanIdMask    = match->masks.vlan_vid & (OFDPA_VID_PRESENT | OFDPA_VID_EXACT_MASK);
      }
      break;

    case OFDPA_FLOW_TABLE_ID_VLAN_1:
      if ((ind_ofdpa_match_fields_bitmask| IND_OFDPA_VLAN1_FLOW_MATCH_BITMAP) != IND_OFDPA_VLAN1_FLOW_MATCH_BITMAP)
      {
        err = INDIGO_ERROR_COMPAT;
      }
      else if ((ind_ofdpa_match_fields_bitmask & IND_OFDPA_VLAN1_FLOW_MATCH_MAND_BITMAP)
                != IND_OFDPA_VLAN1_FLOW_MATCH_MAND_BITMAP)
      {
        err = INDIGO_ERROR_COMPAT;
      }
      else
      {
        flow->flowData.vlan1FlowEntry.match_criteria.inPort        = match->fields.in_port;
        flow->flowData.vlan1FlowEntry.match_criteria.vlanId        = match->fields.vlan_vid & (OFDPA_VID_PRESENT | OFDPA_VID_EXACT_MASK);
        flow->flowData.vlan1FlowEntry.match_criteria.ovid          = match->fields.ofdpa_ovid;
      }
      break;

    case OFDPA_FLOW_TABLE_ID_MAINTENANCE_POINT:
      if ((ind_ofdpa_match_fields_bitmask| IND_OFDPA_MP_FLOW_MATCH_BITMAP) != IND_OFDPA_MP_FLOW_MATCH_BITMAP)
      {
        err = INDIGO_ERROR_COMPAT;
      }
      else if ((ind_ofdpa_match_fields_bitmask & IND_OFDPA_MP_FLOW_MATCH_MAND_BITMAP)
                != IND_OFDPA_MP_FLOW_MATCH_MAND_BITMAP)
      {
        err = INDIGO_ERROR_COMPAT;
      }
      else
      {
        flow->flowData.mpFlowEntry.match_criteria.etherType          = match->fields.eth_type;
        flow->flowData.mpFlowEntry.match_criteria.etherTypeMask      = match->masks.eth_type;
        flow->flowData.mpFlowEntry.match_criteria.oamY1731Mdl        = match->fields.ofdpa_oam_y1731_mdl;
        flow->flowData.mpFlowEntry.match_criteria.oamY1731MdlMask    = match->masks.ofdpa_oam_y1731_mdl & OFDPA_OAM_Y1731_MDL_EXACT_MASK;
        flow->flowData.mpFlowEntry.match_criteria.oamY1731Opcode     = match->fields.ofdpa_oam_y1731_opcode;
        flow->flowData.mpFlowEntry.match_criteria.oamY1731OpcodeMask = match->masks.ofdpa_oam_y1731_opcode;
        flow->flowData.mpFlowEntry.match_criteria.inPort             = match->fields.in_port;
        flow->flowData.mpFlowEntry.match_criteria.vlanId             = match->fields.vlan_vid & (OFDPA_VID_PRESENT | OFDPA_VID_EXACT_MASK);
        flow->flowData.mpFlowEntry.match_criteria.vlanIdMask         = match->masks.vlan_vid & (OFDPA_VID_PRESENT | OFDPA_VID_EXACT_MASK);

        memcpy(flow->flowData.mpFlowEntry.match_criteria.destMac.addr, &match->fields.eth_dst, OF_MAC_ADDR_BYTES);
        memcpy(flow->flowData.mpFlowEntry.match_criteria.destMacMask.addr, &match->masks.eth_dst, OF_MAC_ADDR_BYTES);
      }
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_L2_PORT:
      if ((ind_ofdpa_match_fields_bitmask| IND_OFDPA_MPLS_L2_PORT_FLOW_MATCH_BITMAP) != IND_OFDPA_MPLS_L2_PORT_FLOW_MATCH_BITMAP)
      {
        err = INDIGO_ERROR_COMPAT;
      }
      else if ((ind_ofdpa_match_fields_bitmask & IND_OFDPA_MPLS_L2_PORT_FLOW_MATCH_MAND_BITMAP)
                != IND_OFDPA_MPLS_L2_PORT_FLOW_MATCH_MAND_BITMAP)
      {
        err = INDIGO_ERROR_COMPAT;
      }
      else
      {
        flow->flowData.mplsL2PortFlowEntry.match_criteria.mplsL2Port     = match->fields.ofdpa_mpls_l2_port;
        flow->flowData.mplsL2PortFlowEntry.match_criteria.mplsL2PortMask = match->masks.ofdpa_mpls_l2_port;
        flow->flowData.mplsL2PortFlowEntry.match_criteria.etherType      = match->fields.eth_type;
        flow->flowData.mplsL2PortFlowEntry.match_criteria.etherTypeMask  = match->masks.eth_type;
        flow->flowData.mplsL2PortFlowEntry.match_criteria.tunnelId       = match->fields.tunnel_id;
      }
      break;

    case OFDPA_FLOW_TABLE_ID_TERMINATION_MAC:
      if ((ind_ofdpa_match_fields_bitmask| IND_OFDPA_TERM_MAC_FLOW_MATCH_BITMAP) != IND_OFDPA_TERM_MAC_FLOW_MATCH_BITMAP)
      {
        err = INDIGO_ERROR_COMPAT;
      }
      else if ((ind_ofdpa_match_fields_bitmask & IND_OFDPA_TERM_MAC_FLOW_MATCH_MAND_BITMAP)
                != IND_OFDPA_TERM_MAC_FLOW_MATCH_MAND_BITMAP)
      {
        err = INDIGO_ERROR_COMPAT;
      }
      else
      {
        flow->flowData.terminationMacFlowEntry.match_criteria.inPort     = match->fields.in_port;
        flow->flowData.terminationMacFlowEntry.match_criteria.inPortMask = match->masks.in_port;
        flow->flowData.terminationMacFlowEntry.match_criteria.etherType  = match->fields.eth_type;

        memcpy(flow->flowData.terminationMacFlowEntry.match_criteria.destMac.addr, &match->fields.eth_dst, OF_MAC_ADDR_BYTES);
        memcpy(flow->flowData.terminationMacFlowEntry.match_criteria.destMacMask.addr, &match->masks.eth_dst, OF_MAC_ADDR_BYTES);

        flow->flowData.terminationMacFlowEntry.match_criteria.vlanId     = match->fields.vlan_vid;
        flow->flowData.terminationMacFlowEntry.match_criteria.vlanIdMask = match->masks.vlan_vid & (OFDPA_VID_PRESENT | OFDPA_VID_EXACT_MASK);
      }
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_0:
    case OFDPA_FLOW_TABLE_ID_MPLS_1:
    case OFDPA_FLOW_TABLE_ID_MPLS_2:
      if ((ind_ofdpa_match_fields_bitmask| IND_OFDPA_MPLS_FLOW_MATCH_BITMAP) != IND_OFDPA_MPLS_FLOW_MATCH_BITMAP)
      {
        err = INDIGO_ERROR_COMPAT;
      }
      else if ((ind_ofdpa_match_fields_bitmask & IND_OFDPA_MPLS_FLOW_MATCH_MAND_BITMAP)
                != IND_OFDPA_MPLS_FLOW_MATCH_MAND_BITMAP)
      {
        err = INDIGO_ERROR_COMPAT;
      }
      else
      {
        flow->flowData.mplsFlowEntry.match_criteria.etherType               = match->fields.eth_type;
        flow->flowData.mplsFlowEntry.match_criteria.mplsBos                 = match->fields.mpls_bos;
        flow->flowData.mplsFlowEntry.match_criteria.mplsLabel               = match->fields.mpls_label;
        flow->flowData.mplsFlowEntry.match_criteria.inPort                  = match->fields.in_port;
        flow->flowData.mplsFlowEntry.match_criteria.inPortMask              = match->masks.in_port;
        flow->flowData.mplsFlowEntry.match_criteria.mplsTtl                 = match->fields.ofdpa_mpls_ttl;
        flow->flowData.mplsFlowEntry.match_criteria.mplsTtlMask             = match->masks.ofdpa_mpls_ttl;
        flow->flowData.mplsFlowEntry.match_criteria.mplsDataFirstNibble     = match->fields.ofdpa_mpls_data_first_nibble;
        flow->flowData.mplsFlowEntry.match_criteria.mplsDataFirstNibbleMask = match->masks.ofdpa_mpls_data_first_nibble;
        flow->flowData.mplsFlowEntry.match_criteria.mplsAchChannel          = match->fields.ofdpa_mpls_ach_channel;
        flow->flowData.mplsFlowEntry.match_criteria.mplsAchChannelMask      = match->masks.ofdpa_mpls_ach_channel;
        flow->flowData.mplsFlowEntry.match_criteria.nextLabelIsGal          = match->fields.ofdpa_mpls_next_label_is_gal;
        flow->flowData.mplsFlowEntry.match_criteria.nextLabelIsGalMask      = match->masks.ofdpa_mpls_next_label_is_gal;
        flow->flowData.mplsFlowEntry.match_criteria.destIp4                 = match->fields.ipv4_dst;
        flow->flowData.mplsFlowEntry.match_criteria.destIp4Mask             = match->masks.ipv4_dst;

        memcpy(&flow->flowData.mplsFlowEntry.match_criteria.destIp6, &match->fields.ipv6_dst, OF_IPV6_BYTES);
        memcpy(&flow->flowData.mplsFlowEntry.match_criteria.destIp6Mask, &match->masks.ipv6_dst, OF_IPV6_BYTES);

        flow->flowData.mplsFlowEntry.match_criteria.ipProto        = match->fields.ip_proto;
        flow->flowData.mplsFlowEntry.match_criteria.ipProtoMask    = match->masks.ip_proto;
        flow->flowData.mplsFlowEntry.match_criteria.udpSrcPort     = match->fields.udp_src;
        flow->flowData.mplsFlowEntry.match_criteria.udpSrcPortMask = match->masks.udp_src;
        flow->flowData.mplsFlowEntry.match_criteria.udpDstPort     = match->fields.udp_dst;
        flow->flowData.mplsFlowEntry.match_criteria.udpDstPortMask = match->masks.udp_dst;
      }
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_MAINTENANCE_POINT:
      if ((ind_ofdpa_match_fields_bitmask| IND_OFDPA_MPLS_MP_FLOW_MATCH_BITMAP) != IND_OFDPA_MPLS_MP_FLOW_MATCH_BITMAP)
      {
        err = INDIGO_ERROR_COMPAT;
      }
      else if ((ind_ofdpa_match_fields_bitmask & IND_OFDPA_MPLS_MP_FLOW_MATCH_MAND_BITMAP)
                != IND_OFDPA_MPLS_MP_FLOW_MATCH_MAND_BITMAP)
      {
        err = INDIGO_ERROR_COMPAT;
      }
      else
      {
        flow->flowData.mplsMpFlowEntry.match_criteria.lmepId         = match->fields.ofdpa_lmep_id;
        flow->flowData.mplsMpFlowEntry.match_criteria.oamY1731Opcode = match->fields.ofdpa_oam_y1731_opcode;
        flow->flowData.mplsMpFlowEntry.match_criteria.etherType      = match->fields.eth_type;
      }
      break;

    case OFDPA_FLOW_TABLE_ID_UNICAST_ROUTING:
      if ((ind_ofdpa_match_fields_bitmask| IND_OFDPA_UCAST_ROUTING_FLOW_MATCH_BITMAP) != IND_OFDPA_UCAST_ROUTING_FLOW_MATCH_BITMAP)
      {
        err = INDIGO_ERROR_COMPAT;
      }
      else if (((ind_ofdpa_match_fields_bitmask & IND_OFDPA_UCAST_ROUTINGV4_FLOW_MATCH_MAND_BITMAP)
                  != IND_OFDPA_UCAST_ROUTINGV4_FLOW_MATCH_MAND_BITMAP) &&
               ((ind_ofdpa_match_fields_bitmask & IND_OFDPA_UCAST_ROUTINGV6_FLOW_MATCH_MAND_BITMAP)
                  != IND_OFDPA_UCAST_ROUTINGV6_FLOW_MATCH_MAND_BITMAP))
      {
        err = INDIGO_ERROR_COMPAT;
      }
      else
      {
        flow->flowData.unicastRoutingFlowEntry.match_criteria.etherType  = match->fields.eth_type;
        flow->flowData.unicastRoutingFlowEntry.match_criteria.vrf        = match->fields.ofdpa_vrf;
        flow->flowData.unicastRoutingFlowEntry.match_criteria.vrfMask    = match->masks.ofdpa_vrf;
        flow->flowData.unicastRoutingFlowEntry.match_criteria.dstIp4     = match->fields.ipv4_dst;
        flow->flowData.unicastRoutingFlowEntry.match_criteria.dstIp4Mask = match->masks.ipv4_dst;

        memcpy(&flow->flowData.unicastRoutingFlowEntry.match_criteria.dstIp6, &match->fields.ipv6_dst, OF_IPV6_BYTES);
        memcpy(&flow->flowData.unicastRoutingFlowEntry.match_criteria.dstIp6Mask, &match->masks.ipv6_dst, OF_IPV6_BYTES);
      }
      break;

    case OFDPA_FLOW_TABLE_ID_MULTICAST_ROUTING:
      if ((ind_ofdpa_match_fields_bitmask| IND_OFDPA_MCAST_ROUTING_FLOW_MATCH_BITMAP) != IND_OFDPA_MCAST_ROUTING_FLOW_MATCH_BITMAP)
      {
        err = INDIGO_ERROR_COMPAT;
      }
      else if (((ind_ofdpa_match_fields_bitmask & IND_OFDPA_MCAST_ROUTINGV4_FLOW_MATCH_MAND_BITMAP)
                  != IND_OFDPA_MCAST_ROUTINGV4_FLOW_MATCH_MAND_BITMAP) &&
               ((ind_ofdpa_match_fields_bitmask & IND_OFDPA_MCAST_ROUTINGV6_FLOW_MATCH_MAND_BITMAP)
                  != IND_OFDPA_MCAST_ROUTINGV6_FLOW_MATCH_MAND_BITMAP))
      {
        err = INDIGO_ERROR_COMPAT;
      }
      else
      {
        flow->flowData.multicastRoutingFlowEntry.match_criteria.etherType  = match->fields.eth_type;
        flow->flowData.multicastRoutingFlowEntry.match_criteria.vlanId     = match->fields.vlan_vid;
        flow->flowData.multicastRoutingFlowEntry.match_criteria.vrf        = match->fields.ofdpa_vrf;
        flow->flowData.multicastRoutingFlowEntry.match_criteria.vrfMask    = match->masks.ofdpa_vrf;
        flow->flowData.multicastRoutingFlowEntry.match_criteria.srcIp4     = match->fields.ipv4_src;
        flow->flowData.multicastRoutingFlowEntry.match_criteria.srcIp4Mask = match->masks.ipv4_src;
        flow->flowData.multicastRoutingFlowEntry.match_criteria.dstIp4     = match->fields.ipv4_dst;

        memcpy(flow->flowData.multicastRoutingFlowEntry.match_criteria.srcIp6.s6_addr, match->fields.ipv6_src.addr, OF_IPV6_BYTES);
        memcpy(flow->flowData.multicastRoutingFlowEntry.match_criteria.srcIp6Mask.s6_addr, match->masks.ipv6_src.addr, OF_IPV6_BYTES);
        memcpy(flow->flowData.multicastRoutingFlowEntry.match_criteria.dstIp6.s6_addr, match->fields.ipv6_dst.addr, OF_IPV6_BYTES);
      }
      break;

    case OFDPA_FLOW_TABLE_ID_BRIDGING:
      if ((ind_ofdpa_match_fields_bitmask| IND_OFDPA_BRIDGING_FLOW_MATCH_BITMAP) != IND_OFDPA_BRIDGING_FLOW_MATCH_BITMAP)
      {
        err = INDIGO_ERROR_COMPAT;
      }
      else
      {
        flow->flowData.bridgingFlowEntry.match_criteria.vlanId       = match->fields.vlan_vid;
        flow->flowData.bridgingFlowEntry.match_criteria.vlanIdMask   = match->masks.vlan_vid & (OFDPA_VID_PRESENT | OFDPA_VID_EXACT_MASK);
        flow->flowData.bridgingFlowEntry.match_criteria.tunnelId     = match->fields.tunnel_id;
        flow->flowData.bridgingFlowEntry.match_criteria.tunnelIdMask = match->masks.tunnel_id;

        memcpy(flow->flowData.bridgingFlowEntry.match_criteria.destMac.addr, &match->fields.eth_dst, OF_MAC_ADDR_BYTES);
        memcpy(flow->flowData.bridgingFlowEntry.match_criteria.destMacMask.addr, &match->masks.eth_dst, OF_MAC_ADDR_BYTES);
      }
      break;

    case OFDPA_FLOW_TABLE_ID_L2_POLICER:
      if ((ind_ofdpa_match_fields_bitmask| IND_OFDPA_L2_POLICER_FLOW_MATCH_BITMAP) != IND_OFDPA_L2_POLICER_FLOW_MATCH_BITMAP)
      {
       err = INDIGO_ERROR_COMPAT;
      }
      else if ((ind_ofdpa_match_fields_bitmask & IND_OFDPA_L2_POLICER_FLOW_MATCH_MAND_BITMAP)
                != IND_OFDPA_L2_POLICER_FLOW_MATCH_MAND_BITMAP)
      {
       err = INDIGO_ERROR_COMPAT;
      }
      else
      {
        flow->flowData.l2PolicerFlowEntry.match_criteria.tunnelId       = match->fields.tunnel_id;
        flow->flowData.l2PolicerFlowEntry.match_criteria.mplsL2Port     = match->fields.ofdpa_mpls_l2_port;
        flow->flowData.l2PolicerFlowEntry.match_criteria.mplsL2PortMask = match->masks.ofdpa_mpls_l2_port;
      }
      break;

    case OFDPA_FLOW_TABLE_ID_L2_POLICER_ACTIONS:
      if ((ind_ofdpa_match_fields_bitmask| IND_OFDPA_L2_POLICER_ACTIONS_FLOW_MATCH_BITMAP) != IND_OFDPA_L2_POLICER_ACTIONS_FLOW_MATCH_BITMAP)
      {
       err = INDIGO_ERROR_COMPAT;
      }
      else if ((ind_ofdpa_match_fields_bitmask & IND_OFDPA_L2_POLICER_ACTIONS_FLOW_MATCH_MAND_BITMAP)
                != IND_OFDPA_L2_POLICER_ACTIONS_FLOW_MATCH_MAND_BITMAP)
      {
       err = INDIGO_ERROR_COMPAT;
      }
      else
      {
        flow->flowData.l2PolicerActionsFlowEntry.match_criteria.color             = match->fields.ofdpa_color;
        flow->flowData.l2PolicerActionsFlowEntry.match_criteria.colorActionsIndex = match->fields.ofdpa_color_actions_index;
      }
      break;

    case OFDPA_FLOW_TABLE_ID_PORT_DSCP_TRUST:
    case OFDPA_FLOW_TABLE_ID_TUNNEL_DSCP_TRUST:
    case OFDPA_FLOW_TABLE_ID_MPLS_DSCP_TRUST:
      if ((ind_ofdpa_match_fields_bitmask| IND_OFDPA_DSCP_TRUST_FLOW_MATCH_BITMAP) != IND_OFDPA_DSCP_TRUST_FLOW_MATCH_BITMAP)
      {
        err = INDIGO_ERROR_COMPAT;
      }
      else if ((ind_ofdpa_match_fields_bitmask & IND_OFDPA_DSCP_TRUST_FLOW_MATCH_MAND_BITMAP)
                != IND_OFDPA_DSCP_TRUST_FLOW_MATCH_MAND_BITMAP)
      {
        err = INDIGO_ERROR_COMPAT;
      }
      else
      {
        flow->flowData.dscpTrustFlowEntry.match_criteria.qosIndex       = match->fields.ofdpa_qos_index;
        flow->flowData.dscpTrustFlowEntry.match_criteria.dscpValue      = match->fields.ip_dscp;
        flow->flowData.dscpTrustFlowEntry.match_criteria.mplsL2Port     = match->fields.ofdpa_mpls_l2_port;
        flow->flowData.dscpTrustFlowEntry.match_criteria.mplsL2PortMask = match->masks.ofdpa_mpls_l2_port;
      }
      break;

    case OFDPA_FLOW_TABLE_ID_PORT_PCP_TRUST:
    case OFDPA_FLOW_TABLE_ID_TUNNEL_PCP_TRUST:
    case OFDPA_FLOW_TABLE_ID_MPLS_PCP_TRUST:
      if ((ind_ofdpa_match_fields_bitmask| IND_OFDPA_PCP_TRUST_FLOW_MATCH_BITMAP) != IND_OFDPA_PCP_TRUST_FLOW_MATCH_BITMAP)
      {
        err = INDIGO_ERROR_COMPAT;
      }
      else if ((ind_ofdpa_match_fields_bitmask & IND_OFDPA_PCP_TRUST_FLOW_MATCH_MAND_BITMAP)
                != IND_OFDPA_PCP_TRUST_FLOW_MATCH_MAND_BITMAP)
      {
        err = INDIGO_ERROR_COMPAT;
      }
      else
      {
        flow->flowData.pcpTrustFlowEntry.match_criteria.qosIndex       = match->fields.ofdpa_qos_index;
        flow->flowData.pcpTrustFlowEntry.match_criteria.pcpValue       = match->fields.vlan_pcp;
        flow->flowData.pcpTrustFlowEntry.match_criteria.dei            = match->fields.ofdpa_dei;
        flow->flowData.pcpTrustFlowEntry.match_criteria.mplsL2Port     = match->fields.ofdpa_mpls_l2_port;
        flow->flowData.pcpTrustFlowEntry.match_criteria.mplsL2PortMask = match->masks.ofdpa_mpls_l2_port;
      }
      break;

    case OFDPA_FLOW_TABLE_ID_ACL_POLICY:
      if ((ind_ofdpa_match_fields_bitmask| IND_OFDPA_ACL_POLICY_FLOW_MATCH_BITMAP) != IND_OFDPA_ACL_POLICY_FLOW_MATCH_BITMAP)
      {
        err = INDIGO_ERROR_COMPAT;
      }
      else
      {
        flow->flowData.policyAclFlowEntry.match_criteria.inPort         = match->fields.in_port;
        flow->flowData.policyAclFlowEntry.match_criteria.inPortMask     = match->masks.in_port;
        flow->flowData.policyAclFlowEntry.match_criteria.mplsL2Port     = match->fields.ofdpa_mpls_l2_port;
        flow->flowData.policyAclFlowEntry.match_criteria.mplsL2PortMask = match->masks.ofdpa_mpls_l2_port;

        memcpy(flow->flowData.policyAclFlowEntry.match_criteria.srcMac.addr, &match->fields.eth_src, OF_MAC_ADDR_BYTES);
        memcpy(flow->flowData.policyAclFlowEntry.match_criteria.srcMacMask.addr, &match->masks.eth_src, OF_MAC_ADDR_BYTES);
        memcpy(flow->flowData.policyAclFlowEntry.match_criteria.destMac.addr, &match->fields.eth_dst, OF_MAC_ADDR_BYTES);
        memcpy(flow->flowData.policyAclFlowEntry.match_criteria.destMacMask.addr, &match->masks.eth_dst, OF_MAC_ADDR_BYTES);

        flow->flowData.policyAclFlowEntry.match_criteria.etherType     = match->fields.eth_type;
        flow->flowData.policyAclFlowEntry.match_criteria.etherTypeMask = match->masks.eth_type;
        flow->flowData.policyAclFlowEntry.match_criteria.vlanId        = match->fields.vlan_vid;
        flow->flowData.policyAclFlowEntry.match_criteria.vlanIdMask    = match->masks.vlan_vid & (OFDPA_VID_PRESENT | OFDPA_VID_EXACT_MASK);
        flow->flowData.policyAclFlowEntry.match_criteria.vlanPcp       = match->fields.vlan_pcp;
        flow->flowData.policyAclFlowEntry.match_criteria.vlanPcpMask   = match->masks.vlan_pcp;
        flow->flowData.policyAclFlowEntry.match_criteria.vlanDei       = match->fields.ofdpa_dei;
        flow->flowData.policyAclFlowEntry.match_criteria.vlanDeiMask   = match->masks.ofdpa_dei;
        flow->flowData.policyAclFlowEntry.match_criteria.tunnelId      = match->fields.tunnel_id;
        flow->flowData.policyAclFlowEntry.match_criteria.tunnelIdMask  = match->masks.tunnel_id;
        flow->flowData.policyAclFlowEntry.match_criteria.vrf           = match->fields.ofdpa_vrf;
        flow->flowData.policyAclFlowEntry.match_criteria.vrfMask       = match->masks.ofdpa_vrf;
        flow->flowData.policyAclFlowEntry.match_criteria.sourceIp4     = match->fields.ipv4_src;
        flow->flowData.policyAclFlowEntry.match_criteria.sourceIp4Mask = match->masks.ipv4_src;
        flow->flowData.policyAclFlowEntry.match_criteria.destIp4       = match->fields.ipv4_dst;
        flow->flowData.policyAclFlowEntry.match_criteria.destIp4Mask   = match->masks.ipv4_dst;

        memcpy(flow->flowData.policyAclFlowEntry.match_criteria.sourceIp6.s6_addr, match->fields.ipv6_src.addr, OF_IPV6_BYTES);
        memcpy(flow->flowData.policyAclFlowEntry.match_criteria.sourceIp6Mask.s6_addr, match->masks.ipv6_src.addr, OF_IPV6_BYTES);
        memcpy(flow->flowData.policyAclFlowEntry.match_criteria.destIp6.s6_addr, match->fields.ipv6_dst.addr, OF_IPV6_BYTES);
        memcpy(flow->flowData.policyAclFlowEntry.match_criteria.destIp6Mask.s6_addr, match->masks.ipv6_dst.addr, OF_IPV6_BYTES);

        flow->flowData.policyAclFlowEntry.match_criteria.ipv4ArpSpa     = match->fields.arp_spa;
        flow->flowData.policyAclFlowEntry.match_criteria.ipv4ArpSpaMask = match->masks.arp_spa;
        flow->flowData.policyAclFlowEntry.match_criteria.ipProto        = match->fields.ip_proto;
        flow->flowData.policyAclFlowEntry.match_criteria.ipProtoMask    = match->masks.ip_proto;
        flow->flowData.policyAclFlowEntry.match_criteria.dscp           = match->fields.ip_dscp;
        flow->flowData.policyAclFlowEntry.match_criteria.dscpMask       = match->masks.ip_dscp;
        flow->flowData.policyAclFlowEntry.match_criteria.ecn            = match->fields.ip_ecn;
        flow->flowData.policyAclFlowEntry.match_criteria.ecnMask        = match->masks.ip_ecn;

        if (match->fields.ip_proto == IPPROTO_TCP)
        {
          flow->flowData.policyAclFlowEntry.match_criteria.srcL4Port      = match->fields.tcp_src;
          flow->flowData.policyAclFlowEntry.match_criteria.srcL4PortMask  = match->masks.tcp_src;
          flow->flowData.policyAclFlowEntry.match_criteria.destL4Port     = match->fields.tcp_dst;
          flow->flowData.policyAclFlowEntry.match_criteria.destL4PortMask = match->masks.tcp_dst;
        }
        else if (match->fields.ip_proto == IPPROTO_UDP)
        {
          flow->flowData.policyAclFlowEntry.match_criteria.srcL4Port      = match->fields.udp_src;
          flow->flowData.policyAclFlowEntry.match_criteria.srcL4PortMask  = match->masks.udp_src;
          flow->flowData.policyAclFlowEntry.match_criteria.destL4Port     = match->fields.udp_dst;
          flow->flowData.policyAclFlowEntry.match_criteria.destL4PortMask = match->masks.udp_dst;
        }
        else if (match->fields.ip_proto == IPPROTO_SCTP)
        {
          flow->flowData.policyAclFlowEntry.match_criteria.srcL4Port      = match->fields.sctp_src;
          flow->flowData.policyAclFlowEntry.match_criteria.srcL4PortMask  = match->masks.sctp_src;
          flow->flowData.policyAclFlowEntry.match_criteria.destL4Port     = match->fields.sctp_dst;
          flow->flowData.policyAclFlowEntry.match_criteria.destL4PortMask = match->masks.sctp_dst;
        }

        if (match->fields.ip_proto == IPPROTO_ICMP)
        {
          flow->flowData.policyAclFlowEntry.match_criteria.icmpType     = match->fields.icmpv4_type;
          flow->flowData.policyAclFlowEntry.match_criteria.icmpTypeMask = match->masks.icmpv4_type;
          flow->flowData.policyAclFlowEntry.match_criteria.icmpCode     = match->fields.icmpv4_code;
          flow->flowData.policyAclFlowEntry.match_criteria.icmpCodeMask = match->masks.icmpv4_code;
        }
        else if (match->fields.ip_proto == IPPROTO_ICMPV6)
        {
          flow->flowData.policyAclFlowEntry.match_criteria.icmpType     = match->fields.icmpv6_type;
          flow->flowData.policyAclFlowEntry.match_criteria.icmpTypeMask = match->masks.icmpv6_type;
          flow->flowData.policyAclFlowEntry.match_criteria.icmpCode     = match->fields.icmpv6_code;
          flow->flowData.policyAclFlowEntry.match_criteria.icmpCodeMask = match->masks.icmpv6_code;
        }

        flow->flowData.policyAclFlowEntry.match_criteria.ipv6FlowLabel     = match->fields.ipv6_flabel;
        flow->flowData.policyAclFlowEntry.match_criteria.ipv6FlowLabelMask = match->masks.ipv6_flabel;
      }
      break;

    case OFDPA_FLOW_TABLE_ID_COLOR_BASED_ACTIONS:
      if ((ind_ofdpa_match_fields_bitmask| IND_OFDPA_COLOR_BASED_ACTIONS_FLOW_MATCH_BITMAP) != IND_OFDPA_COLOR_BASED_ACTIONS_FLOW_MATCH_BITMAP)
      {
        err = INDIGO_ERROR_COMPAT;
      }
      else if ((ind_ofdpa_match_fields_bitmask & IND_OFDPA_COLOR_BASED_ACTIONS_FLOW_MATCH_MAND_BITMAP)
                != IND_OFDPA_COLOR_BASED_ACTIONS_FLOW_MATCH_MAND_BITMAP)
      {
        err = INDIGO_ERROR_COMPAT;
      }
      else
      {
        flow->flowData.colorActionsFlowEntry.match_criteria.color = match->fields.ofdpa_color;
        flow->flowData.colorActionsFlowEntry.match_criteria.index  = match->fields.ofdpa_color_actions_index;
      }
      break;

    case OFDPA_FLOW_TABLE_ID_EGRESS_VLAN:
      if ((ind_ofdpa_match_fields_bitmask| IND_OFDPA_EGRESS_VLAN_FLOW_MATCH_BITMAP) != IND_OFDPA_EGRESS_VLAN_FLOW_MATCH_BITMAP)
      {
        err = INDIGO_ERROR_COMPAT;
      }
      else if ((ind_ofdpa_match_fields_bitmask & IND_OFDPA_EGRESS_VLAN_FLOW_MATCH_MAND_BITMAP)
                != IND_OFDPA_EGRESS_VLAN_FLOW_MATCH_MAND_BITMAP)
      {
        err = INDIGO_ERROR_COMPAT;
      }
      else
      {
        flow->flowData.egressVlanFlowEntry.match_criteria.outPort = match->fields.onf_actset_output;
        flow->flowData.egressVlanFlowEntry.match_criteria.vlanId  = match->fields.vlan_vid;
        flow->flowData.egressVlanFlowEntry.match_criteria.allowVlanTranslation = match->fields.ofdpa_allow_vlan_translation;
      }
      break;

    case OFDPA_FLOW_TABLE_ID_EGRESS_VLAN_1:
      if ((ind_ofdpa_match_fields_bitmask| IND_OFDPA_EGRESS_VLAN1_FLOW_MATCH_BITMAP) != IND_OFDPA_EGRESS_VLAN1_FLOW_MATCH_BITMAP)
      {
        err = INDIGO_ERROR_COMPAT;
      }
      else if ((ind_ofdpa_match_fields_bitmask & IND_OFDPA_EGRESS_VLAN1_FLOW_MATCH_MAND_BITMAP)
                != IND_OFDPA_EGRESS_VLAN1_FLOW_MATCH_MAND_BITMAP)
      {
        err = INDIGO_ERROR_COMPAT;
      }
      else
      {
        flow->flowData.egressVlan1FlowEntry.match_criteria.outPort = match->fields.onf_actset_output;
        flow->flowData.egressVlan1FlowEntry.match_criteria.vlanId  = match->fields.vlan_vid;
        flow->flowData.egressVlan1FlowEntry.match_criteria.ovid    = match->fields.ofdpa_ovid;
      }
      break;

    case OFDPA_FLOW_TABLE_ID_EGRESS_MAINTENANCE_POINT:
      if ((ind_ofdpa_match_fields_bitmask| IND_OFDPA_EGRESS_MP_FLOW_MATCH_BITMAP) != IND_OFDPA_EGRESS_MP_FLOW_MATCH_BITMAP)
      {
        err = INDIGO_ERROR_COMPAT;
      }
      else if ((ind_ofdpa_match_fields_bitmask & IND_OFDPA_EGRESS_MP_FLOW_MATCH_MAND_BITMAP)
              != IND_OFDPA_EGRESS_MP_FLOW_MATCH_MAND_BITMAP)
      {
        err = INDIGO_ERROR_COMPAT;
      }
      else
      {
        flow->flowData.egressMpFlowEntry.match_criteria.outPort            = match->fields.onf_actset_output;
        flow->flowData.egressMpFlowEntry.match_criteria.vlanId             = match->fields.vlan_vid;
        flow->flowData.egressMpFlowEntry.match_criteria.vlanIdMask         = match->masks.vlan_vid & (OFDPA_VID_PRESENT | OFDPA_VID_EXACT_MASK);
        flow->flowData.egressMpFlowEntry.match_criteria.etherType          = match->fields.eth_type;
        flow->flowData.egressMpFlowEntry.match_criteria.etherTypeMask      = match->masks.eth_type;
        flow->flowData.egressMpFlowEntry.match_criteria.oamY1731Mdl        = match->fields.ofdpa_oam_y1731_mdl;
        flow->flowData.egressMpFlowEntry.match_criteria.oamY1731MdlMask    = match->masks.ofdpa_oam_y1731_mdl & OFDPA_OAM_Y1731_MDL_EXACT_MASK;
        flow->flowData.egressMpFlowEntry.match_criteria.oamY1731Opcode     = match->fields.ofdpa_oam_y1731_opcode;
        flow->flowData.egressMpFlowEntry.match_criteria.oamY1731OpcodeMask = match->masks.ofdpa_oam_y1731_opcode;

        memcpy(flow->flowData.egressMpFlowEntry.match_criteria.destMac.addr, &match->fields.eth_dst, OF_MAC_ADDR_BYTES);
        memcpy(flow->flowData.egressMpFlowEntry.match_criteria.destMacMask.addr, &match->masks.eth_dst, OF_MAC_ADDR_BYTES);
      }
      break;

    case OFDPA_FLOW_TABLE_ID_EGRESS_DSCP_PCP_REMARK:
      if ((ind_ofdpa_match_fields_bitmask| IND_OFDPA_EGRESS_DSCP_PCP_REM_FLOW_MATCH_BITMAP)
            != IND_OFDPA_EGRESS_DSCP_PCP_REM_FLOW_MATCH_BITMAP)
      {
        err = INDIGO_ERROR_COMPAT;
      }
      else if ((ind_ofdpa_match_fields_bitmask & IND_OFDPA_EGRESS_DSCP_PCP_REM_FLOW_MATCH_MAND_BITMAP)
                != IND_OFDPA_EGRESS_DSCP_PCP_REM_FLOW_MATCH_MAND_BITMAP)
      {
        err = INDIGO_ERROR_COMPAT;
      }
      else
      {
        flow->flowData.egressDscpPcpRemarkFlowEntry.match_criteria.etherType      = match->fields.eth_type;
        flow->flowData.egressDscpPcpRemarkFlowEntry.match_criteria.etherTypeMask  = match->masks.eth_type;
        flow->flowData.egressDscpPcpRemarkFlowEntry.match_criteria.outPort        = match->fields.onf_actset_output;
        flow->flowData.egressDscpPcpRemarkFlowEntry.match_criteria.trafficClass   = match->fields.ofdpa_traffic_class;
        flow->flowData.egressDscpPcpRemarkFlowEntry.match_criteria.color          = match->fields.ofdpa_color;
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

static indigo_error_t ind_ofdpa_translate_openflow_actions(of_object_id_t type, of_list_action_t *actions, ofdpaFlowEntry_t *flow)
{
  of_action_t act;
  of_port_no_t port_no;
  int rv;

  OF_LIST_ACTION_ITER(actions, &act, rv)
  {
    LOG_TRACE("action %s for table %d", of_object_id_str[act.header.object_id], flow->tableId);
    switch (act.header.object_id)
    {
      case OF_ACTION_OUTPUT:
      {
        of_action_output_port_get(&act.output, &port_no);
        switch (flow->tableId)
        {
          case OFDPA_FLOW_TABLE_ID_TERMINATION_MAC:
            flow->flowData.terminationMacFlowEntry.outputPort = port_no;
            break;
          case OFDPA_FLOW_TABLE_ID_BRIDGING:
            switch (type)
            {
              case OF_INSTRUCTION_APPLY_ACTIONS:
                flow->flowData.bridgingFlowEntry.outputPort = port_no;
                break;
              case OF_INSTRUCTION_WRITE_ACTIONS:
                flow->flowData.bridgingFlowEntry.tunnelLogicalPort = port_no;
                break;
              default:
                break;
            }
            break;
          case OFDPA_FLOW_TABLE_ID_MAINTENANCE_POINT:
            flow->flowData.mpFlowEntry.outputPort = port_no;
            break;
          case OFDPA_FLOW_TABLE_ID_MPLS_MAINTENANCE_POINT:
            flow->flowData.mplsMpFlowEntry.outputPort = port_no;
            break;
          case OFDPA_FLOW_TABLE_ID_UNICAST_ROUTING:
            flow->flowData.unicastRoutingFlowEntry.outputPort = port_no;
            break;
          case OFDPA_FLOW_TABLE_ID_ACL_POLICY:
            flow->flowData.policyAclFlowEntry.outputPort = port_no;
            break;
          case OFDPA_FLOW_TABLE_ID_COLOR_BASED_ACTIONS:
            flow->flowData.colorActionsFlowEntry.outputPort = port_no;
            break;
          case OFDPA_FLOW_TABLE_ID_EGRESS_MAINTENANCE_POINT:
            flow->flowData.egressMpFlowEntry.outputPort = port_no;
            break;
          default:
            LOG_TRACE("Unsupported output port action for Table: %d", flow->tableId);
            return INDIGO_ERROR_COMPAT;
        }
        break;
      }
      case OF_ACTION_SET_FIELD:
      {
        /* FIXUP: loci does not yet support the OXM field in the set-field action */
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
        LOG_TRACE("set-field oxm %s for table %d", of_object_id_str[oxm.header.object_id], flow->tableId);
        switch (oxm.header.object_id)
        {
          case OF_OXM_TUNNEL_ID:
          {
            uint64_t tunnel_id;
            of_oxm_tunnel_id_value_get(&oxm.tunnel_id, &tunnel_id);
            if (flow->tableId == OFDPA_FLOW_TABLE_ID_VLAN)
            {
              flow->flowData.vlanFlowEntry.tunnelIdAction = 1;
              flow->flowData.vlanFlowEntry.tunnelId = tunnel_id;
            }
            else if (flow->tableId == OFDPA_FLOW_TABLE_ID_VLAN_1)
            {
              flow->flowData.vlan1FlowEntry.tunnelIdAction = 1;
              flow->flowData.vlan1FlowEntry.tunnelId = tunnel_id;
            }
            else if (flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_0 ||
                    flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_1 ||
                    flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_2)
            {
              flow->flowData.mplsFlowEntry.tunnelIdAction = 1;
              flow->flowData.mplsFlowEntry.tunnelId = tunnel_id;
            }
            else if (flow->tableId == OFDPA_FLOW_TABLE_ID_INJECTED_OAM)
            {
              flow->flowData.injectedOamFlowEntry.tunnelIdAction = 1;
              flow->flowData.injectedOamFlowEntry.tunnelId = tunnel_id;
            }
            else
            {
              LOG_ERROR("Unsupported set-field oxm %s for table %d", of_object_id_str[oxm.header.object_id], flow->tableId);
              return INDIGO_ERROR_COMPAT;
            }
            break;
          }
          case OF_OXM_OFDPA_MPLS_L2_PORT:
          {
            uint32_t mpls_l2_port;
            of_oxm_ofdpa_mpls_l2_port_value_get(&oxm.ofdpa_mpls_l2_port, &mpls_l2_port);
            if (flow->tableId == OFDPA_FLOW_TABLE_ID_VLAN)
            {
              flow->flowData.vlanFlowEntry.mplsL2PortAction = 1;
              flow->flowData.vlanFlowEntry.mplsL2Port = mpls_l2_port;
            }
            else if (flow->tableId == OFDPA_FLOW_TABLE_ID_VLAN_1)
            {
              flow->flowData.vlan1FlowEntry.mplsL2PortAction = 1;
              flow->flowData.vlan1FlowEntry.mplsL2Port = mpls_l2_port;
            }
            else if (flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_0 ||
                     flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_1 ||
                     flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_2)
            {
              flow->flowData.mplsFlowEntry.mplsL2PortAction = 1;
              flow->flowData.mplsFlowEntry.mplsL2Port = mpls_l2_port;
            }
            else if (flow->tableId == OFDPA_FLOW_TABLE_ID_INJECTED_OAM)
            {
              flow->flowData.injectedOamFlowEntry.mplsL2PortAction = 1;
              flow->flowData.injectedOamFlowEntry.mplsL2Port = mpls_l2_port;
            }
            else
            {
              LOG_ERROR("Unsupported set-field oxm %s for table %d", of_object_id_str[oxm.header.object_id], flow->tableId);
              return INDIGO_ERROR_COMPAT;
            }
            break;
          }
          case OF_OXM_MPLS_LABEL:
          {
            uint32_t mpls_label;
            of_oxm_mpls_label_value_get(&oxm.mpls_label, &mpls_label);
            if (flow->tableId == OFDPA_FLOW_TABLE_ID_INJECTED_OAM)
            {
              flow->flowData.injectedOamFlowEntry.pushMplsLabelHdr = 1;
              flow->flowData.injectedOamFlowEntry.mplsLabel = mpls_label;
            }
            else
            {
              LOG_ERROR("Unsupported set-field oxm %s for table %d", of_object_id_str[oxm.header.object_id], flow->tableId);
              return INDIGO_ERROR_COMPAT;
            }
            break;
          }
          case OF_OXM_MPLS_BOS:
          {
            uint8_t mpls_bos;
            of_oxm_mpls_bos_value_get(&oxm.mpls_bos, &mpls_bos);
            if (flow->tableId == OFDPA_FLOW_TABLE_ID_INJECTED_OAM)
            {
              flow->flowData.injectedOamFlowEntry.pushMplsLabelHdr = 1;
              flow->flowData.injectedOamFlowEntry.mplsBOS = mpls_bos;
            }
            else
            {
              LOG_ERROR("Unsupported set-field oxm %s for table %d", of_object_id_str[oxm.header.object_id], flow->tableId);
              return INDIGO_ERROR_COMPAT;
            }
            break;
          }
          case OF_OXM_OFDPA_MPLS_TTL:
          {
            uint8_t ofdpa_mpls_ttl;
            of_oxm_ofdpa_mpls_ttl_value_get(&oxm.ofdpa_mpls_ttl, &ofdpa_mpls_ttl);
            if (flow->tableId == OFDPA_FLOW_TABLE_ID_INJECTED_OAM)
            {
              flow->flowData.injectedOamFlowEntry.mplsTTLAction = 1;
              flow->flowData.injectedOamFlowEntry.mplsTTL = ofdpa_mpls_ttl;
            }
            else
            {
              LOG_ERROR("Unsupported set-field oxm %s for table %d", of_object_id_str[oxm.header.object_id], flow->tableId);
              return INDIGO_ERROR_COMPAT;
            }
            break;
          }
          case OF_OXM_OFDPA_MPLS_DATA_FIRST_NIBBLE:
          {
            uint8_t mpls_data_first_nibble;
            of_oxm_ofdpa_mpls_data_first_nibble_value_get(&oxm.ofdpa_mpls_data_first_nibble, &mpls_data_first_nibble);
            if (flow->tableId == OFDPA_FLOW_TABLE_ID_INJECTED_OAM)
            {
              flow->flowData.injectedOamFlowEntry.mplsDataFirstNibbleAction = 1;
              flow->flowData.injectedOamFlowEntry.mplsDataFirstNibble = mpls_data_first_nibble;
            }
            else
            {
              LOG_ERROR("Unsupported set-field oxm %s for table %d", of_object_id_str[oxm.header.object_id], flow->tableId);
              return INDIGO_ERROR_COMPAT;
            }
            break;
          }
          case OF_OXM_OFDPA_MPLS_ACH_CHANNEL:
          {
            uint16_t mpls_ach_channel;
            of_oxm_ofdpa_mpls_ach_channel_value_get(&oxm.ofdpa_mpls_ach_channel, &mpls_ach_channel);
            if (flow->tableId == OFDPA_FLOW_TABLE_ID_INJECTED_OAM)
            {
              flow->flowData.injectedOamFlowEntry.mplsAchChannelAction = 1;
              flow->flowData.injectedOamFlowEntry.mplsAchChannel = mpls_ach_channel;
            }
            else
            {
              LOG_ERROR("Unsupported set-field oxm %s for table %d", of_object_id_str[oxm.header.object_id], flow->tableId);
              return INDIGO_ERROR_COMPAT;
            }
            break;
          }
          case OF_OXM_OFDPA_L3_IN_PORT:
          {
            uint32_t l3_in_port;
            of_oxm_ofdpa_l3_in_port_value_get(&oxm.ofdpa_l3_in_port, &l3_in_port);
            if (flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_1 ||
                flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_2)
            {
              //flow->flowData.mplsFlowEntry.l3_in_port = l3_in_port;
              //flow->flowData.mplsFlowEntry.l3_in_portAction = 1;
            }
            else
            {
              LOG_ERROR("Unsupported set-field oxm %s for table %d", of_object_id_str[oxm.header.object_id], flow->tableId);
              return INDIGO_ERROR_COMPAT;
            }
            break;
          }
          case OF_OXM_OFDPA_VRF:
          {
            uint16_t vrf;
            of_oxm_ofdpa_vrf_value_get(&oxm.ofdpa_vrf, &vrf);
            if (flow->tableId == OFDPA_FLOW_TABLE_ID_INGRESS_PORT)
            {
              flow->flowData.ingressPortFlowEntry.vrfAction = 1;
              flow->flowData.ingressPortFlowEntry.vrf = vrf;
            }
            else if (flow->tableId == OFDPA_FLOW_TABLE_ID_VLAN)
            {
              flow->flowData.vlanFlowEntry.vrfAction = 1;
              flow->flowData.vlanFlowEntry.vrf = vrf;
            }
            else if (flow->tableId == OFDPA_FLOW_TABLE_ID_VLAN_1)
            {
              flow->flowData.vlan1FlowEntry.vrfAction = 1;
              flow->flowData.vlan1FlowEntry.vrf = vrf;
            }
            else if (flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_0 ||
                     flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_1 ||
                     flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_2)
            {
              flow->flowData.mplsFlowEntry.vrfAction = 1;
              flow->flowData.mplsFlowEntry.vrf = vrf;
            }
            else
            {
              LOG_ERROR("Unsupported set-field oxm %s for table %d", of_object_id_str[oxm.header.object_id], flow->tableId);
              return INDIGO_ERROR_COMPAT;
            }
            break;
          }
          case OF_OXM_OFDPA_OVID:
          {
            uint16_t vlan_vid;
            of_oxm_ofdpa_ovid_value_get(&oxm.ofdpa_ovid, &vlan_vid);
            if (flow->tableId == OFDPA_FLOW_TABLE_ID_VLAN)
            {
              flow->flowData.vlanFlowEntry.ovidAction = 1;
              flow->flowData.vlanFlowEntry.ovid = vlan_vid;
            }
            else if (flow->tableId == OFDPA_FLOW_TABLE_ID_EGRESS_VLAN)
            {
              flow->flowData.egressVlanFlowEntry.ovidAction = 1;
              flow->flowData.egressVlanFlowEntry.ovid = vlan_vid;
            }
            else
            {
              LOG_ERROR("Unsupported set-field oxm %s for table %d", of_object_id_str[oxm.header.object_id], flow->tableId);
              return INDIGO_ERROR_COMPAT;
            }
            break;
          }
          case OF_OXM_VLAN_VID:
          {
            uint16_t vlan_vid;
            of_oxm_vlan_vid_value_get(&oxm.vlan_vid, &vlan_vid);
            if (flow->tableId == OFDPA_FLOW_TABLE_ID_VLAN)
            {
              if (flow->flowData.vlanFlowEntry.pushVlan2Action == 1)
              {
                flow->flowData.vlanFlowEntry.newVlanId2 = vlan_vid;
                flow->flowData.vlanFlowEntry.setVlanId2Action = 1;
              }
              else
              {
                flow->flowData.vlanFlowEntry.newVlanId = vlan_vid;
                flow->flowData.vlanFlowEntry.setVlanIdAction = 1;
              }
            }
            else if (flow->tableId == OFDPA_FLOW_TABLE_ID_EGRESS_VLAN)
            {
              if (flow->flowData.egressVlanFlowEntry.pushVlan2Action == 1)
              {
                flow->flowData.egressVlanFlowEntry.newVlanId2 = vlan_vid;
                flow->flowData.egressVlanFlowEntry.setVlanId2Action = 1;
              }
              else
              {
                flow->flowData.egressVlanFlowEntry.newVlanId = vlan_vid;
                flow->flowData.egressVlanFlowEntry.setVlanIdAction = 1;
              }
            }
            else if (flow->tableId == OFDPA_FLOW_TABLE_ID_VLAN_1)
            {
              flow->flowData.vlan1FlowEntry.newVlanId = vlan_vid;
            }
            else if (flow->tableId == OFDPA_FLOW_TABLE_ID_EGRESS_VLAN_1)
            {
              if (flow->flowData.egressVlan1FlowEntry.pushVlan2Action == 1)
              {
                flow->flowData.egressVlan1FlowEntry.newVlanId2 = vlan_vid;
                flow->flowData.egressVlan1FlowEntry.setVlanId2Action = 1;
              }
              else
              {
                flow->flowData.egressVlan1FlowEntry.newVlanId = vlan_vid;
                flow->flowData.egressVlan1FlowEntry.setVlanIdAction = 1;
              }
            }
            else if (flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_0 ||
                    flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_1 ||
                    flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_2)
            {
              flow->flowData.mplsFlowEntry.vlanIdAction = 1;
              flow->flowData.mplsFlowEntry.vlanId = vlan_vid;
            }
            else
            {
              LOG_ERROR("Unsupported set-field oxm %s for table %d", of_object_id_str[oxm.header.object_id], flow->tableId);
              return INDIGO_ERROR_COMPAT;
            }
            break;
          }
          case OF_OXM_VLAN_PCP:
          {
            uint8_t vlan_pcp;
            of_oxm_vlan_pcp_value_get(&oxm.vlan_pcp, &vlan_pcp);
            if (flow->tableId == OFDPA_FLOW_TABLE_ID_L2_POLICER_ACTIONS)
            {
              flow->flowData.l2PolicerActionsFlowEntry.vlanPcpAction = 1;
              flow->flowData.l2PolicerActionsFlowEntry.vlanPcp = vlan_pcp;
            }
            else if (flow->tableId == OFDPA_FLOW_TABLE_ID_COLOR_BASED_ACTIONS)
            {
              flow->flowData.colorActionsFlowEntry.vlanPcpAction = 1;
              flow->flowData.colorActionsFlowEntry.vlanPcp = vlan_pcp;
            }
            else if (flow->tableId == OFDPA_FLOW_TABLE_ID_ACL_POLICY)
            {
              flow->flowData.policyAclFlowEntry.vlanPcpAction = 1;
              flow->flowData.policyAclFlowEntry.vlanPcp = vlan_pcp;
            }
            else if (flow->tableId == OFDPA_FLOW_TABLE_ID_EGRESS_DSCP_PCP_REMARK)
            {
              flow->flowData.egressDscpPcpRemarkFlowEntry.vlanPcpAction = 1;
              flow->flowData.egressDscpPcpRemarkFlowEntry.vlanPcp = vlan_pcp;
            }
            else
            {
              LOG_ERROR("Unsupported set-field oxm %s for table %d", of_object_id_str[oxm.header.object_id], flow->tableId);
              return INDIGO_ERROR_COMPAT;
            }
            break;
          }
          case OF_OXM_OFDPA_DEI:
          {
            uint8_t vlan_dei;
            of_oxm_ofdpa_dei_value_get(&oxm.ofdpa_dei, &vlan_dei);
            if (flow->tableId == OFDPA_FLOW_TABLE_ID_EGRESS_DSCP_PCP_REMARK)
            {
              flow->flowData.egressDscpPcpRemarkFlowEntry.vlanDeiAction = 1;
              flow->flowData.egressDscpPcpRemarkFlowEntry.vlanDei = vlan_dei;
            }
            else
            {
              LOG_ERROR("Unsupported set-field oxm %s for table %d", of_object_id_str[oxm.header.object_id], flow->tableId);
            }
            break;
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
            if (flow->tableId == OFDPA_FLOW_TABLE_ID_L2_POLICER_ACTIONS)
            {
              flow->flowData.l2PolicerActionsFlowEntry.dscpAction = 1;
              flow->flowData.l2PolicerActionsFlowEntry.dscp = ip_dscp;
            }
            else if (flow->tableId == OFDPA_FLOW_TABLE_ID_COLOR_BASED_ACTIONS)
            {
              flow->flowData.colorActionsFlowEntry.dscpAction = 1;
              flow->flowData.colorActionsFlowEntry.dscp = ip_dscp;
            }
            else if (flow->tableId == OFDPA_FLOW_TABLE_ID_ACL_POLICY)
            {
              flow->flowData.policyAclFlowEntry.dscpAction = 1;
              flow->flowData.policyAclFlowEntry.dscp = ip_dscp;
            }
            else if (flow->tableId == OFDPA_FLOW_TABLE_ID_EGRESS_DSCP_PCP_REMARK)
            {
              flow->flowData.egressDscpPcpRemarkFlowEntry.dscpAction = 1;
              flow->flowData.egressDscpPcpRemarkFlowEntry.dscp = ip_dscp;
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
            if (flow->tableId == OFDPA_FLOW_TABLE_ID_L2_POLICER_ACTIONS)
            {
              flow->flowData.l2PolicerActionsFlowEntry.ecnAction = 1;
              flow->flowData.l2PolicerActionsFlowEntry.ecn = ip_ecn;
            }
            else if (flow->tableId == OFDPA_FLOW_TABLE_ID_COLOR_BASED_ACTIONS)
            {
              flow->flowData.colorActionsFlowEntry.ecnAction = 1;
              flow->flowData.colorActionsFlowEntry.ecn = ip_ecn;
            }
            else if (flow->tableId == OFDPA_FLOW_TABLE_ID_ACL_POLICY)
            {
              flow->flowData.policyAclFlowEntry.ecnAction = 1;
              flow->flowData.policyAclFlowEntry.ecn = ip_ecn;
            }
            else
            {
              LOG_ERROR("Unsupported set-field oxm %s for table %d", of_object_id_str[oxm.header.object_id], flow->tableId);
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
          case OF_OXM_OFDPA_TRAFFIC_CLASS:
          {
            uint8_t tc;
            of_oxm_ofdpa_traffic_class_value_get(&oxm.ofdpa_traffic_class, &tc);
            if ((flow->tableId == OFDPA_FLOW_TABLE_ID_PORT_DSCP_TRUST) ||
                     (flow->tableId == OFDPA_FLOW_TABLE_ID_TUNNEL_DSCP_TRUST) ||
                     (flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_DSCP_TRUST))
            {
              flow->flowData.dscpTrustFlowEntry.trafficClass = tc;
            }
            else if ((flow->tableId == OFDPA_FLOW_TABLE_ID_PORT_PCP_TRUST) ||
                     (flow->tableId == OFDPA_FLOW_TABLE_ID_TUNNEL_PCP_TRUST) ||
                     (flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_PCP_TRUST))
            {
              flow->flowData.pcpTrustFlowEntry.trafficClass = tc;
            }
            else if (flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_0 ||
                    flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_1 ||
                    flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_2)
            {
              flow->flowData.mplsFlowEntry.trafficClass = tc;
              flow->flowData.mplsFlowEntry.trafficClassAction = 1;
            }
            else if (flow->tableId == OFDPA_FLOW_TABLE_ID_COLOR_BASED_ACTIONS)
            {
              flow->flowData.colorActionsFlowEntry.trafficClass = tc;
              flow->flowData.colorActionsFlowEntry.trafficClassAction = 1;
            }
            else if (flow->tableId == OFDPA_FLOW_TABLE_ID_L2_POLICER_ACTIONS)
            {
              flow->flowData.l2PolicerActionsFlowEntry.trafficClassAction = 1;
              flow->flowData.l2PolicerActionsFlowEntry.trafficClass = tc;
            }
            else if (flow->tableId == OFDPA_FLOW_TABLE_ID_ACL_POLICY)
            {
              flow->flowData.policyAclFlowEntry.trafficClass = tc;
              flow->flowData.policyAclFlowEntry.trafficClassAction = 1;
            }
            else
            {
              LOG_ERROR("Unsupported set-field oxm %s for table %d", of_object_id_str[oxm.header.object_id], flow->tableId);
              return INDIGO_ERROR_COMPAT;
            }
            break;
          }
          case OF_OXM_OFDPA_COLOR:
          {
            uint8_t color;
            of_oxm_ofdpa_color_value_get(&oxm.ofdpa_color, &color);
            if ((flow->tableId == OFDPA_FLOW_TABLE_ID_PORT_DSCP_TRUST) ||
                     (flow->tableId == OFDPA_FLOW_TABLE_ID_TUNNEL_DSCP_TRUST) ||
                     (flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_DSCP_TRUST))
            {
              flow->flowData.dscpTrustFlowEntry.color = color;
            }
            else if ((flow->tableId == OFDPA_FLOW_TABLE_ID_PORT_PCP_TRUST) ||
                     (flow->tableId == OFDPA_FLOW_TABLE_ID_TUNNEL_PCP_TRUST) ||
                     (flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_PCP_TRUST))
            {
              flow->flowData.pcpTrustFlowEntry.color = color;
            }
            else if (flow->tableId == OFDPA_FLOW_TABLE_ID_ACL_POLICY)
            {
              flow->flowData.policyAclFlowEntry.color = color;
              flow->flowData.policyAclFlowEntry.colorAction = 1;
            }
            else
            {
              LOG_ERROR("Unsupported set-field oxm %s for table %d", of_object_id_str[oxm.header.object_id], flow->tableId);
              return INDIGO_ERROR_COMPAT;
            }
            break;
          }
          case OF_OXM_OFDPA_COLOR_ACTIONS_INDEX:
          {
            uint32_t ofdpa_color_actions_index;
            of_oxm_ofdpa_color_actions_index_value_get(&oxm.ofdpa_color_actions_index, &ofdpa_color_actions_index);

            if (flow->tableId == OFDPA_FLOW_TABLE_ID_L2_POLICER)
            {
              flow->flowData.l2PolicerFlowEntry.colorActionsIndex = ofdpa_color_actions_index;
            }
            else if (flow->tableId == OFDPA_FLOW_TABLE_ID_L2_POLICER_ACTIONS)
            {
              flow->flowData.l2PolicerActionsFlowEntry.colorActionsIndex = 0;
            }
            else
            {
              LOG_ERROR("Unsupported set-field oxm %s for table %d", of_object_id_str[oxm.header.object_id], flow->tableId);
              return INDIGO_ERROR_COMPAT;
            }
            break;
          }
          case OF_OXM_OFDPA_QOS_INDEX:
          {
            uint8_t qos_index;
            of_oxm_ofdpa_qos_index_value_get(&oxm.ofdpa_qos_index, &qos_index);
            if (flow->tableId == OFDPA_FLOW_TABLE_ID_INGRESS_PORT)
            {
              flow->flowData.ingressPortFlowEntry.qosIndex = qos_index;
              flow->flowData.ingressPortFlowEntry.qosIndexAction = 1;
            }
            else if (flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_L2_PORT)
            {
              flow->flowData.mplsL2PortFlowEntry.qosIndex = qos_index;
              flow->flowData.mplsL2PortFlowEntry.qosIndexAction = 1;
            }
            else if (flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_0 ||
                    flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_1 ||
                    flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_2)
            {
              flow->flowData.mplsFlowEntry.qosIndex = qos_index;
              flow->flowData.mplsFlowEntry.qosIndexAction = 1;
            }
            else
            {
              LOG_ERROR("Unsupported set-field oxm %s for table %d", of_object_id_str[oxm.header.object_id], flow->tableId);
              return INDIGO_ERROR_COMPAT;
            }
            break;
          }
          case OF_OXM_OFDPA_LMEP_ID:
          {
            uint32_t lmep_id;
            of_oxm_ofdpa_lmep_id_value_get(&oxm.ofdpa_lmep_id, &lmep_id);
            if (flow->tableId == OFDPA_FLOW_TABLE_ID_MAINTENANCE_POINT)
            {
              flow->flowData.mpFlowEntry.lmepId = lmep_id;
              flow->flowData.mpFlowEntry.lmepIdAction = 1;
            }
            else if ((flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_0) ||
                     (flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_1) ||
                     (flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_2))
            {
              flow->flowData.mplsFlowEntry.lmepId = lmep_id;
              flow->flowData.mplsFlowEntry.lmepIdAction = 1;
            }
            else if (flow->tableId == OFDPA_FLOW_TABLE_ID_EGRESS_MAINTENANCE_POINT)
            {
              flow->flowData.egressMpFlowEntry.lmepId = lmep_id;
              flow->flowData.egressMpFlowEntry.lmepIdAction = 1;
            }
            else
            {
              LOG_ERROR("Unsupported set-field oxm %s for table %d", of_object_id_str[oxm.header.object_id], flow->tableId);
              return INDIGO_ERROR_COMPAT;
            }
            break;
          }
          case OF_OXM_OFDPA_PROTECTION_INDEX:
          {
            uint8_t protection_index;
            of_oxm_ofdpa_protection_index_value_get(&oxm.ofdpa_protection_index, &protection_index);
            if (flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_0 ||
                    flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_1 ||
                    flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_2)
            {
              flow->flowData.mplsFlowEntry.protectionId = protection_index;
              flow->flowData.mplsFlowEntry.protectionIdAction = 1;
            }
            else
            {
              LOG_ERROR("Unsupported set-field oxm %s for table %d", of_object_id_str[oxm.header.object_id], flow->tableId);
              return INDIGO_ERROR_COMPAT;
            }
            break;
          }
          case OF_OXM_OFDPA_MPLS_TYPE:
          {
            uint16_t mpls_type;
            of_oxm_ofdpa_mpls_type_value_get(&oxm.ofdpa_mpls_type, &mpls_type);
            if (flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_0 ||
                    flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_1 ||
                    flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_2)
            {
              flow->flowData.mplsFlowEntry.mplsType = mpls_type;
              flow->flowData.mplsFlowEntry.mplsTypeAction = 1;
            }
            else
            {
              LOG_ERROR("Unsupported set-field oxm %s for table %d", of_object_id_str[oxm.header.object_id], flow->tableId);
              return INDIGO_ERROR_COMPAT;
            }
            break;
          }
          default:
            LOG_ERROR("unsupported set-field oxm %s for table %d", of_object_id_str[oxm.header.object_id], flow->tableId);
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
        break;
      }
      case OF_ACTION_SET_VLAN_PCP:
      {
        uint8_t vlan_pcp;
        of_action_set_vlan_pcp_vlan_pcp_get(&act.set_vlan_pcp, &vlan_pcp);
        break;
      }
      case OF_ACTION_POP_VLAN:
        if (flow->tableId == OFDPA_FLOW_TABLE_ID_VLAN)
        {
          flow->flowData.vlanFlowEntry.popVlanAction = 1;
        }
        else if (flow->tableId == OFDPA_FLOW_TABLE_ID_EGRESS_VLAN)
        {
          flow->flowData.egressVlanFlowEntry.popVlanAction = 1;
        }
        else if (flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_0 ||
                flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_1 ||
                flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_2)
        {
          flow->flowData.mplsFlowEntry.popVlanAction = 1;
        }
        else
        {
          LOG_ERROR("Unsupported action %s for table %d", of_object_id_str[act.header.object_id], flow->tableId);
          return INDIGO_ERROR_COMPAT;
        }
        break;
      case OF_ACTION_STRIP_VLAN:
      {
        break;
      }
      case OF_ACTION_PUSH_VLAN:
      {
        uint16_t eth_type;
        of_action_push_vlan_ethertype_get(&act.push_vlan, &eth_type);

        if (eth_type != ETH_P_8021Q && eth_type != 0x88a8)
        {
          LOG_ERROR("unsupported eth_type 0x%04x in action %s", eth_type,
                    of_object_id_str[act.header.object_id]);
          return INDIGO_ERROR_COMPAT;
        }
        if (flow->tableId == OFDPA_FLOW_TABLE_ID_VLAN)
        {
          flow->flowData.vlanFlowEntry.pushVlan2Action = 1;
          flow->flowData.vlanFlowEntry.newTpid2 = eth_type;
        }
        else if (flow->tableId == OFDPA_FLOW_TABLE_ID_EGRESS_VLAN)
        {
          flow->flowData.egressVlanFlowEntry.pushVlan2Action = 1;
          flow->flowData.egressVlanFlowEntry.newTpid2 = eth_type;
        }
        else if (flow->tableId == OFDPA_FLOW_TABLE_ID_VLAN_1)
        {
          flow->flowData.vlan1FlowEntry.pushVlan2Action = 1;
          flow->flowData.vlan1FlowEntry.newTpid2 = eth_type;
        }
        else if (flow->tableId == OFDPA_FLOW_TABLE_ID_EGRESS_VLAN_1)
        {
          flow->flowData.egressVlan1FlowEntry.pushVlan2Action = 1;
          flow->flowData.egressVlan1FlowEntry.newTpid2 = eth_type;
        }
        else
        {
          LOG_ERROR("Unsupported action %s for table %d", of_object_id_str[act.header.object_id], flow->tableId);
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
      case OF_ACTION_POP_MPLS:
      {
        if (flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_0 ||
            flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_1 ||
            flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_2)
        {
          flow->flowData.mplsFlowEntry.popLabelAction = 1;
          uint16_t ether_type;
          of_action_pop_mpls_ethertype_get(&act.pop_mpls, &ether_type);
          flow->flowData.mplsFlowEntry.newEtherType = ether_type;
        }
        else
        {
          LOG_ERROR("Unsupported action %s for table %d", of_object_id_str[act.header.object_id], flow->tableId);
          return INDIGO_ERROR_COMPAT;
        }
        break;
      }

      case OF_ACTION_PUSH_MPLS:
      {
        if (flow->tableId == OFDPA_FLOW_TABLE_ID_INJECTED_OAM)
        {
          //flow->flowData.injectedOamFlowEntry.pushLabelAction = 1;
          uint16_t ether_type;
          of_action_push_mpls_ethertype_get(&act.push_mpls, &ether_type);
          //flow->flowData.injectedOamFlowEntry.newEtherType = ether_type;
        }
        else
        {
          LOG_ERROR("Unsupported action %s for table %d", of_object_id_str[act.header.object_id], flow->tableId);
          return INDIGO_ERROR_COMPAT;
        }
        break;
      }

      case OF_ACTION_OFDPA_PUSH_CW:
      {
        if (flow->tableId == OFDPA_FLOW_TABLE_ID_INJECTED_OAM)
        {
          //flow->flowData.injectedOamFlowEntry.pushCwAction = 1;
        }
        else
        {
          LOG_ERROR("Unsupported action %s for table %d", of_object_id_str[act.header.object_id], flow->tableId);
          return INDIGO_ERROR_COMPAT;
        }
        break;
      }

      case OF_ACTION_DEC_MPLS_TTL:
      {
        if (flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_0 ||
            flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_1 ||
            flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_2)
        {
          flow->flowData.mplsFlowEntry.decrementTtlAction = 1;
        }
        else
        {
          LOG_ERROR("Unsupported action %s for table %d", of_object_id_str[act.header.object_id], flow->tableId);
          return INDIGO_ERROR_COMPAT;
        }
        break;
      }
      case OF_ACTION_COPY_TTL_IN:
      {
        if (flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_0 ||
            flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_1 ||
            flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_2)
        {
          flow->flowData.mplsFlowEntry.copyTtlInAction = 1;
        }
        else
        {
          LOG_ERROR("Unsupported action %s for table %d", of_object_id_str[act.header.object_id], flow->tableId);
          return INDIGO_ERROR_COMPAT;
        }
        break;
      }
      case OF_ACTION_OFDPA_POP_L2HDR:
      {
        if (flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_0 ||
            flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_1 ||
            flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_2)
        {
          flow->flowData.mplsFlowEntry.popL2HeaderAction = 1;
        }
        else
        {
          LOG_ERROR("Unsupported action %s for table %d", of_object_id_str[act.header.object_id], flow->tableId);
          return INDIGO_ERROR_COMPAT;
        }
        break;
      }
      case OF_ACTION_OFDPA_POP_CW:
      {
        if (flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_0 ||
            flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_1 ||
            flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_2)
        {
          flow->flowData.mplsFlowEntry.popCwAction = 1;
        }
        else
        {
          LOG_ERROR("Unsupported action %s for table %d", of_object_id_str[act.header.object_id], flow->tableId);
          return INDIGO_ERROR_COMPAT;
        }
        break;
      }
      case OF_ACTION_OFDPA_COPY_TC_IN:
      {
        if (flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_0 ||
            flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_1 ||
            flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_2)
        {
          flow->flowData.mplsFlowEntry.copyTcInAction = 1;
        }
        else
        {
          LOG_ERROR("Unsupported action %s for table %d", of_object_id_str[act.header.object_id], flow->tableId);
          return INDIGO_ERROR_COMPAT;
        }
        break;
      }
      case OF_ACTION_OFDPA_OAM_LM_RX_COUNT:
      {
        uint32_t lmepId;
        of_action_ofdpa_oam_lm_rx_count_lmep_id_get(&act.ofdpa_oam_lm_rx_count, &lmepId);
        if (flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_0 ||
            flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_1 ||
            flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_2)
        {
          flow->flowData.mplsFlowEntry.oamLmRxCountAction = 1;
          flow->flowData.mplsFlowEntry.lmepId = lmepId;
        }
        else if (flow->tableId == OFDPA_FLOW_TABLE_ID_EGRESS_MAINTENANCE_POINT)
        {
          flow->flowData.egressMpFlowEntry.oamLmRxCountAction = 1;
          flow->flowData.egressMpFlowEntry.lmepId = lmepId;
        }
        else
        {
          LOG_ERROR("Unsupported action %s for table %d", of_object_id_str[act.header.object_id], flow->tableId);
          return INDIGO_ERROR_COMPAT;
        }
        break;
      }
      case OF_ACTION_OFDPA_OAM_LM_TX_COUNT:
      {
        uint32_t lmepId;
        of_action_ofdpa_oam_lm_tx_count_lmep_id_get(&act.ofdpa_oam_lm_tx_count, &lmepId);
        if (flow->tableId == OFDPA_FLOW_TABLE_ID_MAINTENANCE_POINT)
        {
          flow->flowData.mpFlowEntry.oamLmTxCountAction = 1;
          flow->flowData.mpFlowEntry.lmepId = lmepId;
        }
        else
        {
          LOG_ERROR("Unsupported action %s for table %d", of_object_id_str[act.header.object_id], flow->tableId);
          return INDIGO_ERROR_COMPAT;
        }
        break;
      }
      case OF_ACTION_OFDPA_SET_COUNTER_FIELDS:
      {
        uint32_t lmepId;
        of_action_ofdpa_set_counter_fields_lmep_id_get(&act.ofdpa_set_counter_fields, &lmepId);
        if (flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_MAINTENANCE_POINT)
        {
          flow->flowData.mplsMpFlowEntry.oamSetCounterFieldsAction = 1;
          flow->flowData.mplsMpFlowEntry.lmepId = lmepId;
        }
        else if (flow->tableId == OFDPA_FLOW_TABLE_ID_EGRESS_MAINTENANCE_POINT)
        {
          flow->flowData.egressMpFlowEntry.oamSetCounterFieldsAction = 1;
          flow->flowData.egressMpFlowEntry.lmepId = lmepId;
        }
        else
        {
          LOG_ERROR("Unsupported action %s for table %d", of_object_id_str[act.header.object_id], flow->tableId);
          return INDIGO_ERROR_COMPAT;
        }
        break;
      }
      case OF_ACTION_OFDPA_CHECK_DROP_STATUS:
      {
        uint16_t drop_type;
        uint32_t drop_index;
        of_action_ofdpa_check_drop_status_drop_index_get(&act.ofdpa_check_drop_status, &drop_index);
        of_action_ofdpa_check_drop_status_drop_type_get(&act.ofdpa_check_drop_status, &drop_type);
        if (flow->tableId == OFDPA_FLOW_TABLE_ID_MAINTENANCE_POINT)
        {
          if (0 == drop_type)
          {
            flow->flowData.mpFlowEntry.checkDropStatusAction = 1;
            flow->flowData.mpFlowEntry.lmepId = drop_index;
          }
        }
        else if (flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_0 ||
            flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_1 ||
            flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_2)
        {
          if (0 == drop_type)
          {
            flow->flowData.mplsFlowEntry.checkDropStatusAction = 1;
            flow->flowData.mplsFlowEntry.lmepId = drop_index;
          }
          else if (1 == drop_type)
          {
            flow->flowData.mplsFlowEntry.checkDropStatus2Action = 1;
            flow->flowData.mplsFlowEntry.dropIndex = drop_index;
            flow->flowData.mplsFlowEntry.dropType = drop_type;
          }
        }
        else if (flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_MAINTENANCE_POINT)
        {
          if (0 == drop_type)
          {
            flow->flowData.mplsMpFlowEntry.checkDropStatusAction = 1;
            flow->flowData.mplsMpFlowEntry.lmepId = drop_index;
          }
        }
        else
        {
          LOG_ERROR("Unsupported action %s for table %d", of_object_id_str[act.header.object_id], flow->tableId);
          return INDIGO_ERROR_COMPAT;
        }
        break;
      }
      case OFDPA_ACT_MPLS_SET_QOS_FROM_TABLE:
      {
        uint8_t qos_index;
        of_action_ofdpa_set_mpls_qos_from_table_qos_index_get(&act.ofdpa_set_mpls_qos_from_table, &qos_index);
        break;
      }
      case OFDPA_ACT_SET_MPLS_PCPDEI_FROM_VPN_TABLE:
      case OFDPA_ACT_SET_MPLS_TC_FROM_TUNNEL_TABLE:
      case OFDPA_ACT_SET_MPLS_PCPDEI_FROM_TUNNEL_TABLE:
      case OFDPA_ACT_CLASS_BASED_TX_COUNT:
      case OFDPA_ACT_CLASS_BASED_RX_COUNT:
      case OFDPA_ACT_COLOR_BASED_TX_COUNT:
      case OFDPA_ACT_COLOR_BASED_RX_COUNT:
      {
        break;
      }
      case OF_ACTION_GROUP:
      {
        uint32_t group_id;
        of_action_group_group_id_get(&act.group, &group_id);
        switch (flow->tableId)
        {
          case OFDPA_FLOW_TABLE_ID_MPLS_L2_PORT:
            flow->flowData.mplsL2PortFlowEntry.groupId = group_id;
            break;

          case OFDPA_FLOW_TABLE_ID_MPLS_0:
          case OFDPA_FLOW_TABLE_ID_MPLS_1:
          case OFDPA_FLOW_TABLE_ID_MPLS_2:
            flow->flowData.mplsFlowEntry.groupID = group_id;
            break;

          case OFDPA_FLOW_TABLE_ID_UNICAST_ROUTING:
            flow->flowData.unicastRoutingFlowEntry.groupID = group_id;
            break;

          case OFDPA_FLOW_TABLE_ID_MULTICAST_ROUTING:
            flow->flowData.multicastRoutingFlowEntry.groupID = group_id;
            break;

          case OFDPA_FLOW_TABLE_ID_BRIDGING:
            flow->flowData.bridgingFlowEntry.groupID = group_id;
            break;

          case OFDPA_FLOW_TABLE_ID_ACL_POLICY:
            flow->flowData.policyAclFlowEntry.groupID = group_id;
            break;

          default:
            LOG_ERROR("Unsupported action %s for table %d", of_object_id_str[act.header.object_id], flow->tableId);
            return INDIGO_ERROR_COMPAT;
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
        switch (flow->tableId)
        {
          case OFDPA_FLOW_TABLE_ID_INGRESS_PORT:
          case OFDPA_FLOW_TABLE_ID_PORT_DSCP_TRUST:
          case OFDPA_FLOW_TABLE_ID_PORT_PCP_TRUST:
          case OFDPA_FLOW_TABLE_ID_TUNNEL_DSCP_TRUST:
          case OFDPA_FLOW_TABLE_ID_TUNNEL_PCP_TRUST:
          case OFDPA_FLOW_TABLE_ID_INJECTED_OAM:
          case OFDPA_FLOW_TABLE_ID_VLAN:
          case OFDPA_FLOW_TABLE_ID_VLAN_1:
          case OFDPA_FLOW_TABLE_ID_MAINTENANCE_POINT:
          case OFDPA_FLOW_TABLE_ID_MPLS_L2_PORT:
          case OFDPA_FLOW_TABLE_ID_MPLS_DSCP_TRUST:
          case OFDPA_FLOW_TABLE_ID_MPLS_PCP_TRUST:
          case OFDPA_FLOW_TABLE_ID_TERMINATION_MAC:
          case OFDPA_FLOW_TABLE_ID_MPLS_0:
          case OFDPA_FLOW_TABLE_ID_MPLS_1:
          case OFDPA_FLOW_TABLE_ID_MPLS_2:
          case OFDPA_FLOW_TABLE_ID_MPLS_MAINTENANCE_POINT:
          case OFDPA_FLOW_TABLE_ID_BRIDGING:
          case OFDPA_FLOW_TABLE_ID_L2_POLICER:
          case OFDPA_FLOW_TABLE_ID_L2_POLICER_ACTIONS:
          case OFDPA_FLOW_TABLE_ID_ACL_POLICY:
          case OFDPA_FLOW_TABLE_ID_COLOR_BASED_ACTIONS:
          case OFDPA_FLOW_TABLE_ID_EGRESS_VLAN:
          case OFDPA_FLOW_TABLE_ID_EGRESS_VLAN_1:
          case OFDPA_FLOW_TABLE_ID_EGRESS_MAINTENANCE_POINT:
          case OFDPA_FLOW_TABLE_ID_EGRESS_DSCP_PCP_REMARK:
          case OFDPA_FLOW_TABLE_ID_UNICAST_ROUTING:
            break;
          case OFDPA_FLOW_TABLE_ID_MULTICAST_ROUTING:
          default:
            LOG_ERROR("Unsupported instruction %s for flow table %d.", of_object_id_str[inst.header.object_id], flow->tableId);
            return INDIGO_ERROR_COMPAT;
        }

        of_instruction_apply_actions_actions_bind(&inst.apply_actions, &openflow_actions);
        if ((err = ind_ofdpa_translate_openflow_actions(inst.header.object_id, &openflow_actions, flow)) < 0)
        {
          return err;
        }
        break;
      case OF_INSTRUCTION_WRITE_ACTIONS:
        switch (flow->tableId)
        {
          case OFDPA_FLOW_TABLE_ID_VLAN:
          case OFDPA_FLOW_TABLE_ID_VLAN_1:
          case OFDPA_FLOW_TABLE_ID_MPLS_L2_PORT:
          case OFDPA_FLOW_TABLE_ID_MPLS_0:
          case OFDPA_FLOW_TABLE_ID_MPLS_1:
          case OFDPA_FLOW_TABLE_ID_MPLS_2:
          case OFDPA_FLOW_TABLE_ID_UNICAST_ROUTING:
          case OFDPA_FLOW_TABLE_ID_MULTICAST_ROUTING:
          case OFDPA_FLOW_TABLE_ID_BRIDGING:
          case OFDPA_FLOW_TABLE_ID_L2_POLICER_ACTIONS:
          case OFDPA_FLOW_TABLE_ID_ACL_POLICY:
          case OFDPA_FLOW_TABLE_ID_COLOR_BASED_ACTIONS:
          case OFDPA_FLOW_TABLE_ID_EGRESS_MAINTENANCE_POINT:
          case OFDPA_FLOW_TABLE_ID_EGRESS_DSCP_PCP_REMARK:
            break;
          case OFDPA_FLOW_TABLE_ID_INGRESS_PORT:
          case OFDPA_FLOW_TABLE_ID_TERMINATION_MAC:
          case OFDPA_FLOW_TABLE_ID_INJECTED_OAM:
          case OFDPA_FLOW_TABLE_ID_L2_POLICER:
          default:
            LOG_ERROR("Unsupported instruction %s for flow table %d.", of_object_id_str[inst.header.object_id], flow->tableId);
            return INDIGO_ERROR_COMPAT;
        }
        of_instruction_write_actions_actions_bind(&inst.write_actions, &openflow_actions);
        if ((err = ind_ofdpa_translate_openflow_actions(inst.header.object_id, &openflow_actions, flow)) < 0)
        {
          return err;
        }
        break;
      case OF_INSTRUCTION_CLEAR_ACTIONS:
        switch (flow->tableId)
        {
          case OFDPA_FLOW_TABLE_ID_MAINTENANCE_POINT:
            flow->flowData.mpFlowEntry.clearAction = 1;
            break;
          case OFDPA_FLOW_TABLE_ID_L2_POLICER_ACTIONS:
            flow->flowData.l2PolicerActionsFlowEntry.clearAction = 1;
            break;
          case OFDPA_FLOW_TABLE_ID_MPLS_1:
            flow->flowData.mplsFlowEntry.clearAction = 1;
            break;
          case OFDPA_FLOW_TABLE_ID_MPLS_MAINTENANCE_POINT:
            flow->flowData.mplsMpFlowEntry.clearAction = 1;
            break;
          case OFDPA_FLOW_TABLE_ID_ACL_POLICY:
            flow->flowData.policyAclFlowEntry.clearAction = 1;
            break;
          case OFDPA_FLOW_TABLE_ID_COLOR_BASED_ACTIONS:
            flow->flowData.colorActionsFlowEntry.clearAction = 1;
            break;
          case OFDPA_FLOW_TABLE_ID_EGRESS_VLAN:
            //flow->flowData.egressVlanFlowEntry.clearAction = 1;
            break;
          case OFDPA_FLOW_TABLE_ID_EGRESS_VLAN_1:
            //flow->flowData.egressVlan1FlowEntry.clearAction = 1;
            break;
          case OFDPA_FLOW_TABLE_ID_EGRESS_MAINTENANCE_POINT:
            flow->flowData.egressMpFlowEntry.clearAction = 1;
            break;
          default:
            LOG_ERROR("Unsupported instruction %s for flow table %d.", of_object_id_str[inst.header.object_id], flow->tableId);
            return INDIGO_ERROR_COMPAT;
        }
        break;
      case OF_INSTRUCTION_GOTO_TABLE:
        of_instruction_goto_table_table_id_get(&inst.goto_table, &next_table_id);

        switch (flow->tableId)
        {
          case OFDPA_FLOW_TABLE_ID_INGRESS_PORT:
            flow->flowData.ingressPortFlowEntry.gotoTableId = next_table_id;
            break;
          case OFDPA_FLOW_TABLE_ID_INJECTED_OAM:
            flow->flowData.injectedOamFlowEntry.gotoTableId = next_table_id;
            break;
          case OFDPA_FLOW_TABLE_ID_VLAN:
            flow->flowData.vlanFlowEntry.gotoTableId = next_table_id;
            break;
          case OFDPA_FLOW_TABLE_ID_VLAN_1:
            flow->flowData.vlan1FlowEntry.gotoTableId = next_table_id;
            break;
          case OFDPA_FLOW_TABLE_ID_MAINTENANCE_POINT:
            flow->flowData.mpFlowEntry.gotoTableId = next_table_id;
            break;
          case OFDPA_FLOW_TABLE_ID_MPLS_L2_PORT:
            flow->flowData.mplsL2PortFlowEntry.gotoTableId = next_table_id;
            break;
          case OFDPA_FLOW_TABLE_ID_TERMINATION_MAC:
            flow->flowData.terminationMacFlowEntry.gotoTableId = next_table_id;
            break;
          case OFDPA_FLOW_TABLE_ID_MPLS_0:
          case OFDPA_FLOW_TABLE_ID_MPLS_1:
          case OFDPA_FLOW_TABLE_ID_MPLS_2:
            flow->flowData.mplsFlowEntry.gotoTableId = next_table_id;
            break;
          case OFDPA_FLOW_TABLE_ID_MPLS_MAINTENANCE_POINT:
            flow->flowData.mplsMpFlowEntry.gotoTableId = next_table_id;
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
          case OFDPA_FLOW_TABLE_ID_L2_POLICER:
            flow->flowData.l2PolicerFlowEntry.gotoTableId = next_table_id;
            break;
          case OFDPA_FLOW_TABLE_ID_L2_POLICER_ACTIONS:
            flow->flowData.l2PolicerActionsFlowEntry.gotoTableId = next_table_id;
            break;
          case OFDPA_FLOW_TABLE_ID_EGRESS_VLAN:
            flow->flowData.egressVlanFlowEntry.gotoTableId = next_table_id;
            break;
          case OFDPA_FLOW_TABLE_ID_EGRESS_VLAN_1:
            flow->flowData.egressVlan1FlowEntry.gotoTableId = next_table_id;
            break;
          case OFDPA_FLOW_TABLE_ID_PORT_DSCP_TRUST:
          case OFDPA_FLOW_TABLE_ID_TUNNEL_DSCP_TRUST:
          case OFDPA_FLOW_TABLE_ID_MPLS_DSCP_TRUST:
            flow->flowData.dscpTrustFlowEntry.gotoTableId = next_table_id;
            break;
          case OFDPA_FLOW_TABLE_ID_PORT_PCP_TRUST:
          case OFDPA_FLOW_TABLE_ID_TUNNEL_PCP_TRUST:
          case OFDPA_FLOW_TABLE_ID_MPLS_PCP_TRUST:
            flow->flowData.pcpTrustFlowEntry.gotoTableId = next_table_id;
            break;
          case OFDPA_FLOW_TABLE_ID_EGRESS_DSCP_PCP_REMARK:
            flow->flowData.egressDscpPcpRemarkFlowEntry.gotoTableId = next_table_id;
            break;
          case OFDPA_FLOW_TABLE_ID_ACL_POLICY:
            LOG_ERROR("Unsupported instruction %s for flow table %d.", of_object_id_str[inst.header.object_id], flow->tableId);
            return INDIGO_ERROR_COMPAT;
          default:
            LOG_ERROR("Unsupported instruction %s for flow table %d.", of_object_id_str[inst.header.object_id], flow->tableId);
            return INDIGO_ERROR_COMPAT;
            break;
        }
        break;
      case OF_INSTRUCTION_METER:
        of_instruction_meter_meter_id_get(&inst.meter, &meter_id);
        LOG_ERROR("Unsupported instruction: meter_id.");
        break;
      default:
        LOG_ERROR("Invalid instruction.");
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
            LOG_TRACE("Unsupported output port 0x%x", port_no);
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
  uint32_t tableId, tableCount = 0;

  LOG_TRACE("%s() called", __FUNCTION__);

  if (features_reply->version < OF_VERSION_1_3)
  {
    LOG_ERROR("Unsupported OpenFlow version 0x%x.", features_reply->version);
    return INDIGO_ERROR_VERSION;
  }

  /* Number of tables supported by datapath. */
  for (tableId = 0; tableId < 255; tableId++)
  {
    if (ofdpaFlowTableSupported(tableId) == OFDPA_E_NONE)
    {
      tableCount++;
    }
  }
  of_features_reply_n_tables_set(features_reply, tableCount);

  return INDIGO_ERROR_NONE;
}

indigo_error_t indigo_fwd_flow_create(indigo_cookie_t flow_id,
                                      of_flow_add_t *flow_add,
                                      uint8_t *table_id)
{
  indigo_error_t err = INDIGO_ERROR_NONE;
  OFDPA_ERROR_t ofdpa_rv = OFDPA_E_NONE;
  ofdpaFlowEntry_t flow;
  uint16_t priority;
  uint16_t idle_timeout, hard_timeout;
  of_match_t of_match;

  LOG_TRACE("Flow create called");

  if (flow_add->version < OF_VERSION_1_3)
  {
    LOG_ERROR("OpenFlow version 0x%x unsupported", flow_add->version);
    return INDIGO_ERROR_VERSION;
  }

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
  if (of_flow_add_match_get(flow_add, &of_match) < 0)
  {
    LOG_ERROR("Error getting openflow match criteria.");
    return INDIGO_ERROR_UNKNOWN;
  }

  /* Get the match fields and masks from LOCI match structure */
  err = ind_ofdpa_match_fields_masks_get(&of_match, &flow);
  if (err != INDIGO_ERROR_NONE)
  {
    LOG_ERROR("Error getting match fields and masks. (err = %d)", err);
    return err;
  }

  /* Get the instructions set from the LOCI flow add object */
  err = ind_ofdpa_instructions_get(flow_add, &flow);
  if (err != INDIGO_ERROR_NONE)
  {
    LOG_TRACE("Failed to get flow instructions. (err = %d)", err);
    return err;
  }
  /* Submit the changes to ofdpa */
  ofdpa_rv = ofdpaFlowAdd(&flow);
  if (ofdpa_rv != OFDPA_E_NONE)
  {
    LOG_TRACE("Failed to add flow. (ofdpa_rv = %d)", ofdpa_rv);
  }
  else
  {
    LOG_TRACE("Flow added successfully. (ofdpa_rv = %d)", ofdpa_rv);
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
    LOG_TRACE("Failed to get flow instructions. (err = %d)", err);
    return err;
  }

  /* Submit the changes to ofdpa */
  ofdpa_rv = ofdpaFlowModify(&flow);
  if (ofdpa_rv!= OFDPA_E_NONE)
  {
    LOG_TRACE("Failed to modify flow. (ofdpa_rv = %d)", ofdpa_rv);
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
      LOG_ERROR("Request to delete non-existent flow. (ofdpa_rv = %d)", ofdpa_rv);
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
    LOG_ERROR("Failed to delete flow. (ofdpa_rv = %d)", ofdpa_rv);
  }
  else
  {
    LOG_TRACE("Flow deleted successfully. (ofdpa_rv = %d)", ofdpa_rv);
  }

  return (indigoConvertOfdpaRv(ofdpa_rv));
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

    LOG_TRACE("Flow stats get successful. (ofdpa_rv = %d)", ofdpa_rv);
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
  LOG_ERROR("indigo_fwd_table_mod() unsupported.");
  return;
}

indigo_error_t indigo_fwd_table_stats_get(of_table_stats_request_t *table_stats_request,
                                          of_table_stats_reply_t **table_stats_reply)
{
  of_version_t version = table_stats_request->version;
  uint32_t xid;
  uint32_t i;
  ofdpaFlowTableInfo_t tableInfo;
  of_table_stats_entry_t entry[1];
  of_table_stats_reply_t *reply;
  of_list_table_stats_entry_t list[1];


  if (table_stats_request->version < OF_VERSION_1_3)
  {
    LOG_ERROR("Unsupported OpenFlow version 0x%x.", version);
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

  of_table_stats_reply_entries_bind(*table_stats_reply, list);


  for (i = 0; i < 255; i++)
  {
    if (ofdpaFlowTableSupported(i) == OFDPA_E_NONE)
    {
      of_table_stats_entry_init(entry, version, -1, 1);
      (void)of_list_table_stats_entry_append_bind(list, entry);

      /* Table Id */
      of_table_stats_entry_table_id_set(entry, i);

      /* Number of entries in the table */
      if (ofdpaFlowTableInfoGet(i, &tableInfo) == OFDPA_E_NONE)
      {
        of_table_stats_entry_active_count_set(entry, tableInfo.numEntries);
      }

      /* Number of packets looked up in table not supported. */
      of_table_stats_entry_lookup_count_set(entry, 0);

      /* Number of packets that hit table not supported. */
      of_table_stats_entry_matched_count_set(entry, 0);
    }
  }

  return(INDIGO_ERROR_NONE);
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
    LOG_TRACE("Packet sent out of output port (%d) successfully. (ofdpa_rv = %d)", packetOutActions.outputPort, ofdpa_rv);
  }

  return (indigoConvertOfdpaRv(ofdpa_rv));
}

indigo_error_t indigo_fwd_experimenter(of_experimenter_t *experimenter,
                                       indigo_cxn_id_t cxn_id)
{
  indigo_error_t err = INDIGO_ERROR_NONE;
  of_experimenter_ofdpa_t *obj = experimenter;
  uint32_t subtype;

  ofdpa_mpls_set_qos_action_mod_msg_t *mpls_set_qos_action;
  ofdpa_oam_dataplane_ctr_mod_msg_t *oam_dataplane_ctr;
  ofdpa_oam_drop_status_mod_msg_t *oam_drop_status;
  ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *mpls_tunnel_label_remark;

  LOG_TRACE("Action tables called");

  if (obj->version < OF_VERSION_1_3)
  {
    LOG_ERROR("OpenFlow version 0x%x unsupported", obj->version);
    return INDIGO_ERROR_VERSION;
  }

  of_experimenter_ofdpa_subtype_get(obj, &subtype);

  switch (subtype)
  {
    case OFDPA_ACTION_TABLE_TYPE_MPLS_SET_QOS:
      mpls_set_qos_action = experimenter;
      err = indigo_set_mpls_qos(mpls_set_qos_action);
      break;

    case OFDPA_ACTION_TABLE_TYPE_OAM_DATAPLANE_COUNTER:
      oam_dataplane_ctr = experimenter;
      err = indigo_oam_dataplane(oam_dataplane_ctr);
      break;

    case OFDPA_ACTION_TABLE_TYPE_DROP_STATUS:
      oam_drop_status = experimenter;
      err = indigo_drop_status(oam_drop_status);
      break;

    case OFDPA_ACTION_TABLE_TYPE_MPLS_VPN_LABEL_REMARK:
    case OFDPA_ACTION_TABLE_TYPE_MPLS_TUNNEL_LABEL_REMARK:
      mpls_tunnel_label_remark = experimenter;
      err = indigo_remark_action(mpls_tunnel_label_remark);
      break;
    default:
    LOG_ERROR("experimenter subtype 0x%x unsupported", subtype);
    return INDIGO_ERROR_NOT_SUPPORTED;
  }
 return err;
}

static indigo_error_t indigo_set_mpls_qos(ofdpa_mpls_set_qos_action_mod_msg_t *mpls_set_qos_action)
{
  OFDPA_ERROR_t ofdpa_rv = OFDPA_E_NONE;
  uint8_t qosIndex;
  uint8_t mpls_tc;
  uint8_t trafficClass;
  OFDPA_QOS_COLORS_t color;
  uint32_t mod_command;
  ofdpaMplsQosEntry_t mplsQosEntry;

  ofdpa_mpls_set_qos_action_mod_msg_mod_command_get(mpls_set_qos_action, &mod_command);
  ofdpa_mpls_set_qos_action_mod_msg_qos_index_get(mpls_set_qos_action, &qosIndex);
  ofdpa_mpls_set_qos_action_mod_msg_mpls_tc_get(mpls_set_qos_action, &mpls_tc);
  ofdpa_mpls_set_qos_action_mod_msg_traffic_class_get(mpls_set_qos_action, &trafficClass);
  ofdpa_mpls_set_qos_action_mod_msg_color_get(mpls_set_qos_action, (uint8_t*)&color);

  mplsQosEntry.qosIndex = qosIndex;
  mplsQosEntry.mpls_tc = mpls_tc;
  mplsQosEntry.trafficClass = trafficClass;
  mplsQosEntry.color = color;

  switch (mod_command)
  {
    case OFDPA_MSG_MOD_ADD:
      ofdpa_rv = ofdpaMplsQosActionAdd(&mplsQosEntry);
      if (ofdpa_rv != OFDPA_E_NONE)
      {
        LOG_ERROR("Failed to add table entry. (ofdpa_rv = %d)", ofdpa_rv);
      }
      else
      {
        LOG_TRACE("Table entry added successfully. (ofdpa_rv = %d)", ofdpa_rv);
      }
      break;

    case OFDPA_MSG_MOD_MODIFY:
      ofdpa_rv = ofdpaMplsQosActionDelete(&mplsQosEntry);
      if (ofdpa_rv != OFDPA_E_NONE)
      {
        LOG_ERROR("Failed to delete  table entry. (ofdpa_rv = %d)", ofdpa_rv);
        break;
      }
      else
      {
        LOG_TRACE("Table entry deleted successfully. (ofdpa_rv = %d)", ofdpa_rv);
      }
      ofdpa_rv = ofdpaMplsQosActionAdd(&mplsQosEntry);
      if (ofdpa_rv != OFDPA_E_NONE)
      {
        LOG_ERROR("Failed to add flow. (ofdpa_rv = %d)", ofdpa_rv);
      }
      else
      {
        LOG_TRACE("Table entryw added successfully. (ofdpa_rv = %d)", ofdpa_rv);
      }
      break;

    case OFDPA_MSG_MOD_DELETE:
      ofdpa_rv = ofdpaMplsQosActionDelete(&mplsQosEntry);
      if (ofdpa_rv != OFDPA_E_NONE)
      {
        LOG_ERROR("Failed to delete flow. (ofdpa_rv = %d)", ofdpa_rv);
        break;
      }
      else
      {
        LOG_TRACE("Table entry deleted successfully. (ofdpa_rv = %d)", ofdpa_rv);
      }
      break;

    default:
      LOG_ERROR("Wrong action table command. (mod_command = %d)", mod_command);
      return INDIGO_ERROR_UNKNOWN;
  }
  return (indigoConvertOfdpaRv(ofdpa_rv));
}

static
indigo_error_t indigo_oam_dataplane(ofdpa_oam_dataplane_ctr_mod_msg_t *oam_dataplane_ctr)
{
  OFDPA_ERROR_t ofdpa_rv = OFDPA_E_NONE;
  uint32_t mod_command;
  uint32_t lmepId;
  uint8_t trafficClass;

  ofdpa_oam_dataplane_ctr_mod_msg_mod_command_get(oam_dataplane_ctr, &mod_command);
  ofdpa_oam_dataplane_ctr_mod_msg_lmep_id_get(oam_dataplane_ctr, &lmepId);
  ofdpa_oam_dataplane_ctr_mod_msg_traffic_class_get(oam_dataplane_ctr, &trafficClass);

  switch (mod_command)
    {
      case OFDPA_MSG_MOD_ADD:
      ofdpa_rv = ofdpaOamDataCounterAdd(lmepId, trafficClass);
        if (ofdpa_rv != OFDPA_E_NONE)
        {
          LOG_ERROR("Failed to add table entry. (ofdpa_rv = %d)", ofdpa_rv);
        }
        else
        {
          LOG_TRACE("Table entry added successfully. (ofdpa_rv = %d)", ofdpa_rv);
        }
        break;

      case OFDPA_MSG_MOD_MODIFY:
        ofdpa_rv = ofdpaOamDataCounterDelete(lmepId, trafficClass);
        if (ofdpa_rv != OFDPA_E_NONE)
        {
          LOG_ERROR("Failed to delete table entry. (ofdpa_rv = %d)", ofdpa_rv);
          break;
        }
        else
        {
          LOG_TRACE("Table entry deleted successfully. (ofdpa_rv = %d)", ofdpa_rv);
        }

        ofdpa_rv = ofdpaOamDataCounterAdd(lmepId, trafficClass);
        if (ofdpa_rv != OFDPA_E_NONE)
        {
          LOG_ERROR("Failed to add table entry. (ofdpa_rv = %d)", ofdpa_rv);
        }
        else
        {
          LOG_TRACE("Table entry added successfully. (ofdpa_rv = %d)", ofdpa_rv);
        }
        break;

      case OFDPA_MSG_MOD_DELETE:
        ofdpa_rv = ofdpaOamDataCounterDelete(lmepId, trafficClass);
        if (ofdpa_rv != OFDPA_E_NONE)
        {
          LOG_ERROR("Failed to delete table entry. (ofdpa_rv = %d)", ofdpa_rv);
        }
        else
        {
          LOG_TRACE("Table entry deleted successfully. (ofdpa_rv = %d)", ofdpa_rv);
        }
        break;

      default:
        LOG_ERROR("Wrong action table command. (mod_command = %d)", mod_command);
        return INDIGO_ERROR_UNKNOWN;
    }
  return (indigoConvertOfdpaRv(ofdpa_rv));
}

static
indigo_error_t indigo_drop_status(ofdpa_oam_drop_status_mod_msg_t *oam_drop_status)
{
  OFDPA_ERROR_t ofdpa_rv = OFDPA_E_NONE;
  uint32_t mod_command;
  uint32_t lmepId;
  uint8_t type;
  uint8_t dropAction;
  ofdpaDropStatusEntry_t dropEntry;

  ofdpa_oam_drop_status_mod_msg_mod_command_get(oam_drop_status, &mod_command);
  ofdpa_oam_drop_status_mod_msg_index_get(oam_drop_status, &lmepId);
  ofdpa_oam_drop_status_mod_msg_entry_type_get(oam_drop_status, &type);
  ofdpa_oam_drop_status_mod_msg_drop_status_get(oam_drop_status, &dropAction);

  dropEntry.lmepId = lmepId;
  dropEntry.type = type;
  dropEntry.dropAction = dropAction;

  switch (mod_command)
  {
    case OFDPA_MSG_MOD_ADD:
      ofdpa_rv = ofdpaDropStatusAdd(&dropEntry);
      if (ofdpa_rv != OFDPA_E_NONE)
      {
        LOG_ERROR("Failed to add table entry. (ofdpa_rv = %d)", ofdpa_rv);
      }
      else
      {
        LOG_TRACE("Table entry added successfully. (ofdpa_rv = %d)", ofdpa_rv);
      }
      break;

    case OFDPA_MSG_MOD_MODIFY:
      ofdpa_rv = ofdpaDropStatusDelete(lmepId);
      if (ofdpa_rv != OFDPA_E_NONE)
      {
        LOG_ERROR("Failed to delete table entry. (ofdpa_rv = %d)", ofdpa_rv);
        break;
      }
      else
      {
        LOG_TRACE("Table entry deleted successfully. (ofdpa_rv = %d)", ofdpa_rv);
      }
      break;

    case OFDPA_MSG_MOD_DELETE:
      ofdpa_rv = ofdpaDropStatusDelete(lmepId);
      if (ofdpa_rv != OFDPA_E_NONE)
      {
        LOG_ERROR("Failed to delete table entry. (ofdpa_rv = %d)", ofdpa_rv);
      }
      else
      {
        LOG_TRACE("Table entry deleted successfully. (ofdpa_rv = %d)", ofdpa_rv);
      }
      break;

    default:
      LOG_ERROR("Wrong action table command. (mod_command = %d)", mod_command);
      return INDIGO_ERROR_UNKNOWN;
  }
  return (indigoConvertOfdpaRv(ofdpa_rv));
}

static
indigo_error_t indigo_remark_action(ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *mpls_tunnel_label_remark)
{
  OFDPA_ERROR_t ofdpa_rv = OFDPA_E_NONE;
  OFDPA_ACTION_TABLE_TYPE_t   actionTableType;
  uint32_t                    index;
  uint8_t                     trafficClass;
  OFDPA_QOS_COLORS_t          color;

  uint8_t                     remarkData;  // MPLS_TC or DSCP
  uint8_t                     vlanPcp;
  uint8_t                     vlanDei;

  ofdpaRemarkActionEntry_t    remarkActionEntry;
  uint32_t                    mod_command;

  of_experimenter_ofdpa_subtype_get(mpls_tunnel_label_remark, (uint32_t*)&actionTableType);
  ofdpa_mpls_tunnel_label_remark_action_mod_msg_mod_command_get(mpls_tunnel_label_remark, &mod_command);
  ofdpa_mpls_tunnel_label_remark_action_mod_msg_index_get(mpls_tunnel_label_remark, &index);
  ofdpa_mpls_tunnel_label_remark_action_mod_msg_traffic_class_get(mpls_tunnel_label_remark, &trafficClass);
  ofdpa_mpls_tunnel_label_remark_action_mod_msg_color_get(mpls_tunnel_label_remark, (uint8_t*)&color);
  ofdpa_mpls_tunnel_label_remark_action_mod_msg_mpls_tc_get(mpls_tunnel_label_remark, &remarkData);
  ofdpa_mpls_tunnel_label_remark_action_mod_msg_vlan_pcp_get(mpls_tunnel_label_remark, &vlanPcp);
  ofdpa_mpls_tunnel_label_remark_action_mod_msg_vlan_dei_get(mpls_tunnel_label_remark, &vlanDei);

  remarkActionEntry.actionTableType = actionTableType;
  remarkActionEntry.index = index;
  remarkActionEntry.trafficClass = trafficClass;
  remarkActionEntry.color = color;
  remarkActionEntry.actions.remarkData = remarkData;
  remarkActionEntry.actions.vlanPcp = vlanPcp;
  remarkActionEntry.actions.vlanDei = vlanDei;

  switch (mod_command)
  {
    case OFDPA_MSG_MOD_ADD:
      ofdpa_rv = ofdpaRemarkActionAdd(&remarkActionEntry);
      if (ofdpa_rv != OFDPA_E_NONE)
      {
        LOG_ERROR("Failed to add table entry. (ofdpa_rv = %d)", ofdpa_rv);
      }
      else
      {
        LOG_TRACE("Table entry added successfully. (ofdpa_rv = %d)", ofdpa_rv);
      }
    break;

    case OFDPA_MSG_MOD_MODIFY:
      ofdpa_rv = ofdpaRemarkActionDelete(&remarkActionEntry);
      if (ofdpa_rv != OFDPA_E_NONE)
      {
        LOG_ERROR("Failed to delete table entry. (ofdpa_rv = %d)", ofdpa_rv);
        break;
      }
      else
      {
        LOG_TRACE("Table entry deleted successfully. (ofdpa_rv = %d)", ofdpa_rv);
      }
      break;

      ofdpa_rv = ofdpaRemarkActionAdd(&remarkActionEntry);
      if (ofdpa_rv != OFDPA_E_NONE)
      {
        LOG_ERROR("Failed to add table entry. (ofdpa_rv = %d)", ofdpa_rv);
      }
      else
      {
        LOG_TRACE("Table entry added successfully. (ofdpa_rv = %d)", ofdpa_rv);
      }
      break;

    case OFDPA_MSG_MOD_DELETE:
      ofdpa_rv = ofdpaRemarkActionDelete(&remarkActionEntry);
      if (ofdpa_rv != OFDPA_E_NONE)
      {
        LOG_ERROR("Failed to delete table entry. (ofdpa_rv = %d)", ofdpa_rv);
      }
      else
      {
        LOG_TRACE("Table entry deleted successfully. (ofdpa_rv = %d)", ofdpa_rv);
      }
      break;

    default:
      LOG_ERROR("Wrong action table command. (mod_command = %d)", mod_command);
      return INDIGO_ERROR_UNKNOWN;
  }
  return (indigoConvertOfdpaRv(ofdpa_rv));
}

void indigo_set_mpls_qos_get_multipart(ofdpa_mpls_set_qos_action_multipart_request_t *request,
        ofdpa_mpls_set_qos_action_multipart_reply_t *reply)
{
  OFDPA_ERROR_t ofdpa_rv = OFDPA_E_NONE;

  uint8_t qosIndex;
  uint8_t mpls_tc;

  ofdpaMplsQosEntry_t mplsQosEntry;

  ofdpa_mpls_set_qos_action_multipart_request_qos_index_get(request, &qosIndex);
  ofdpa_mpls_set_qos_action_multipart_request_mpls_tc_get(request, &mpls_tc);

  ofdpa_rv = ofdpaMplsQosActionEntryGet(qosIndex, mpls_tc, &mplsQosEntry);
  if (ofdpa_rv != OFDPA_E_NONE)
  {
    LOG_ERROR("Failed to read table entry. (ofdpa_rv = %d)", ofdpa_rv);
  }
  else
  {
    ofdpa_mpls_set_qos_action_multipart_reply_qos_index_set(reply, mplsQosEntry.qosIndex);
    ofdpa_mpls_set_qos_action_multipart_reply_mpls_tc_set(reply, mplsQosEntry.mpls_tc);
    ofdpa_mpls_set_qos_action_multipart_reply_traffic_class_set(reply, mplsQosEntry.trafficClass);
    ofdpa_mpls_set_qos_action_multipart_reply_color_set(reply, mplsQosEntry.color);
    LOG_TRACE("Table entry read successfully. (ofdpa_rv = %d)", ofdpa_rv);
  }
}

void indigo_oam_dataplane_get_multipart(ofdpa_oam_dataplane_ctr_multipart_request_t *request,
        ofdpa_oam_dataplane_ctr_multipart_reply_t *reply)
{
  OFDPA_ERROR_t ofdpa_rv = OFDPA_E_NONE;

  uint32_t lmepId;
  uint8_t traffic_class;

  ofdpaOamDataCounterIndex_t ofdpa_index;
  uint32_t TxFCl;
  uint32_t RxFCl;


  ofdpa_oam_dataplane_ctr_multipart_request_lmep_id_get(request, &lmepId);
  ofdpa_oam_dataplane_ctr_multipart_request_traffic_class_get(request, &traffic_class);
  ofdpa_index.lmepId = lmepId;
  ofdpa_index.trafficClass = traffic_class;

  ofdpa_rv = ofdpaOamDataCountersLMGet(ofdpa_index, &TxFCl, &RxFCl);
  if (ofdpa_rv != OFDPA_E_NONE)
  {
    LOG_ERROR("Failed to read table entry. (ofdpa_rv = %d)", ofdpa_rv);
  }
  else
  {
    ofdpa_oam_dataplane_ctr_multipart_reply_lmep_id_set(reply, lmepId);
    ofdpa_oam_dataplane_ctr_multipart_request_traffic_class_set(reply, traffic_class);
    ofdpa_oam_dataplane_ctr_multipart_reply_receive_packets_set(reply, RxFCl);
    ofdpa_oam_dataplane_ctr_multipart_reply_transmit_packets_set(reply, TxFCl);
    LOG_TRACE("Table entry read successfully. (ofdpa_rv = %d)", ofdpa_rv);
  }
}

void indigo_drop_status_get_multipart(ofdpa_oam_drop_status_multipart_request_t *request,
        ofdpa_oam_drop_status_multipart_reply_t *reply)
{
  OFDPA_ERROR_t ofdpa_rv = OFDPA_E_NONE;
  uint32_t lmepId;
  ofdpaDropStatusEntry_t dropEntry;

  ofdpa_oam_drop_status_multipart_request_index_get(request, &lmepId);

  ofdpa_rv = ofdpaDropStatusGet(lmepId, &dropEntry);
  if (ofdpa_rv != OFDPA_E_NONE)
  {
    LOG_ERROR("Failed to read table entry. (ofdpa_rv = %d)", ofdpa_rv);
  }
  else
  {
    ofdpa_oam_drop_status_multipart_reply_index_set(reply, dropEntry.lmepId);
    ofdpa_oam_drop_status_multipart_reply_entry_type_set(reply, dropEntry.type);
    ofdpa_oam_drop_status_multipart_reply_drop_status_set(reply, dropEntry.dropAction);
    LOG_TRACE("Table entry read successfully. (ofdpa_rv = %d)", ofdpa_rv);
  }
}

void indigo_remark_action_get_multipart(ofdpa_mpls_vpn_label_remark_action_multipart_request_t *request,
        ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *reply)
{
  OFDPA_ERROR_t ofdpa_rv = OFDPA_E_NONE;
  ofdpaRemarkActionEntry_t    remarkEntry;

  OFDPA_ACTION_TABLE_TYPE_t   actionTableType;
  uint32_t                    index;
  uint8_t                     trafficClass;
  OFDPA_QOS_COLORS_t          color;
  uint8_t                     remarkData;
  uint8_t                     vlanPcp;
  uint8_t                     vlanDei;

  ofdpa_mpls_vpn_label_remark_action_multipart_request_subtype_get(request, &actionTableType);
  ofdpa_mpls_vpn_label_remark_action_multipart_request_index_get(request, &index);
  ofdpa_mpls_vpn_label_remark_action_multipart_request_traffic_class_get(request, &trafficClass);
  ofdpa_mpls_vpn_label_remark_action_multipart_request_color_get(request, (uint8_t*)&color);
  ofdpa_mpls_vpn_label_remark_action_multipart_request_traffic_class_get(request, &remarkData);
  ofdpa_mpls_vpn_label_remark_action_multipart_request_vlan_pcp_get(request, &vlanPcp);
  ofdpa_mpls_vpn_label_remark_action_multipart_request_vlan_dei_get(request, &vlanDei);

  ofdpa_rv = ofdpaRemarkActionEntryGet(&remarkEntry);
  if (ofdpa_rv != OFDPA_E_NONE)
  {
    LOG_ERROR("Failed to read table entry. (ofdpa_rv = %d)", ofdpa_rv);
  }
  else
  {
    ofdpa_mpls_vpn_label_remark_action_multipart_request_subtype_set(reply, actionTableType);
    ofdpa_mpls_vpn_label_remark_action_multipart_request_index_set(reply, index);
    ofdpa_mpls_vpn_label_remark_action_multipart_request_traffic_class_set(reply, trafficClass);
    ofdpa_mpls_vpn_label_remark_action_multipart_request_color_set(reply, color);
    ofdpa_mpls_vpn_label_remark_action_multipart_request_traffic_class_set(reply, remarkData);
    ofdpa_mpls_vpn_label_remark_action_multipart_request_vlan_pcp_set(reply, vlanPcp);
    ofdpa_mpls_vpn_label_remark_action_multipart_request_vlan_dei_set(reply, vlanDei);
    LOG_TRACE("Table entry read successfully. (ofdpa_rv = %d)", ofdpa_rv);
  }
}

indigo_error_t indigo_fwd_expiration_enable_set(int is_enabled)
{
  LOG_TRACE("indigo_fwd_expiration_enable_set() unsupported.");
  return INDIGO_ERROR_NOT_SUPPORTED;
}

indigo_error_t indigo_fwd_expiration_enable_get(int *is_enabled)
{
  LOG_TRACE("indigo_fwd_expiration_enable_get() unsupported.");
  return INDIGO_ERROR_NOT_SUPPORTED;
}

void ind_ofdpa_flow_event_receive(void)
{
  int flowTableId;
  ofdpaFlowEvent_t flowEventData;

  LOG_TRACE("Reading Flow Events");

  for (flowTableId = 0; flowTableId < 255; flowTableId++)
  {
    if (ofdpaFlowTableSupported(flowTableId) == OFDPA_E_NONE)
    {
      memset(&flowEventData, 0, sizeof(flowEventData));
      flowEventData.flowMatch.tableId = flowTableId;

      while (ofdpaFlowEventNextGet(&flowEventData) == OFDPA_E_NONE)
      {
        if (flowEventData.eventMask & OFDPA_FLOW_EVENT_HARD_TIMEOUT)
        {
          LOG_TRACE("Received flow event on hard timeout.");
          ind_core_flow_expiry_handler(flowEventData.flowMatch.cookie,
                                       INDIGO_FLOW_REMOVED_HARD_TIMEOUT);
        }
        else
        {
          LOG_TRACE("Received flow event on idle timeout.");
          ind_core_flow_expiry_handler(flowEventData.flowMatch.cookie,
                                       INDIGO_FLOW_REMOVED_IDLE_TIMEOUT);
        }
      }
    }
  }
  return;
}

static void ind_ofdpa_key_to_match(uint32_t portNum, of_match_t *match)
{
  memset(match, 0, sizeof(*match));

  /* We only populate the masks for this OF version */
  match->version = ofagent_of_version;
  match->fields.in_port = portNum;
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
  of_packet_in_cookie_set(of_packet_in, 0xffffffffffffffffLL);

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

/* It has been copied from modules/OFStateManager/utest/main.c */

void
indigo_fwd_pipeline_get(of_desc_str_t pipeline)
{
    LOG_TRACE("fwd switch pipeline get");
    strcpy(pipeline, "ofdpa_pipeline");
}

indigo_error_t
indigo_fwd_pipeline_set(of_desc_str_t pipeline)
{
    LOG_ERROR("fwd switch pipeline set: %s", pipeline);
    return INDIGO_ERROR_NOT_SUPPORTED;
}

void
indigo_fwd_pipeline_stats_get(of_desc_str_t **pipeline, int *num_pipelines)
{
    LOG_TRACE("fwd switch pipeline stats get");
    *num_pipelines = 0;
}
