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
* @filename     ind_ofdpa_util.h 
*
* @purpose      This file contains utility functions for Indigo
*
* @component    OF-DPA
*
* @comments     none
*
* @create       11 Nov 2013
*
* @end
*
**********************************************************************/
#include <indigo/error.h>
#include <loci/of_match.h>
#include <loci/loci.h>
#include <ofdpa_api.h>
#include <linux/if_ether.h>

#define IND_OFDPA_IP_DSCP_MASK     0xfc
#define IND_OFDPA_IP_ECN_MASK      0x03 
#define IND_OFDPA_IPV6_FLABEL_MASK 0x000fffff 

#define IND_OFDPA_DEFAULT_SOURCEIP4MASK 0xFFFFFFFFu
#define IND_OFDPA_DEFAULT_DESTIP4MASK   0xFFFFFFFFu

#define IND_OFDPA_NANO_SEC 1000000000


typedef struct  indTableNameList
{
  OFDPA_FLOW_TABLE_ID_t type;
  char *name;
} indTableNameList_t;

typedef struct indPacketOutActions_s
{
  uint32_t outputPort;
  uint32_t pipeline;
}indPacketOutActions_t;


indigo_error_t indigoConvertOfdpaRv(OFDPA_ERROR_t result);


/* Short hand logging macros */
#define LOG_ERROR AIM_LOG_ERROR
#define LOG_WARN AIM_LOG_WARN
#define LOG_INFO AIM_LOG_INFO
#define LOG_VERBOSE AIM_LOG_VERBOSE
#define LOG_TRACE AIM_LOG_TRACE

typedef struct ind_ofdpa_group_bucket_s
{
  uint32_t        vlanId;
  of_mac_addr_t   srcMac;  
  of_mac_addr_t   dstMac;  
  uint32_t        outputPort;
  uint32_t        popVlanTag;
  uint32_t        referenceGroupId;
} ind_ofdpa_group_bucket_t;

typedef enum ind_ofdpa_fields_s
{
  IND_OFDPA_VLANID =     1 << 0,
  IND_OFDPA_SRCMAC =     1 << 1,
  IND_OFDPA_DSTMAC =     1 << 2,
  IND_OFDPA_POPVLAN =    1 << 3,
  IND_OFDPA_PORT =       1 << 4,
  IND_OFDPA_REFGROUP =   1 << 5,
  IND_OFDPA_ETHER_TYPE = 1 << 6,
  IND_OFDPA_IPV4_DST =   1 << 7,
  IND_OFDPA_IPV4_SRC =   1 << 8,
  IND_OFDPA_IPV6_DST =   1 << 9,
  IND_OFDPA_IPV6_SRC =   1 << 10,
  IND_OFDPA_TUNNEL_ID =  1 << 11, /* TBD in indigo */
  IND_OFDPA_VLAN_PCP =   1 << 12,
  IND_OFDPA_IPV4_ARP_SPA = 1 << 13,
  IND_OFDPA_IP_PROTO =     1 << 14,
  IND_OFDPA_IP_DSCP =         1 << 15,
  IND_OFDPA_IP_ECN =          1 << 16,
  IND_OFDPA_TCP_L4_SRC_PORT =  1 << 17,
  IND_OFDPA_TCP_L4_DST_PORT =  1 << 18,
  IND_OFDPA_UDP_L4_SRC_PORT =  1 << 19,
  IND_OFDPA_UDP_L4_DST_PORT =  1 << 20,
  IND_OFDPA_SCTP_L4_SRC_PORT =  1 << 21,
  IND_OFDPA_SCTP_L4_DST_PORT =  1 << 22,
  IND_OFDPA_ICMPV4_TYPE =    1 << 23,
  IND_OFDPA_ICMPV4_CODE =    1 << 24,
  IND_OFDPA_IPV6_FLOW_LABEL = 1 << 25,
  IND_OFDPA_ICMPV6_TYPE =    1 << 26,
  IND_OFDPA_ICMPV6_CODE =    1 << 27
  

}ind_ofdpa_fields_t;

#define IND_OFDPA_L2INTERFACE_BITMAP   (IND_OFDPA_POPVLAN | IND_OFDPA_PORT)

#define IND_OFDPA_L2REWRITE_BITMAP     (IND_OFDPA_VLANID | IND_OFDPA_SRCMAC | \
                                        IND_OFDPA_DSTMAC | IND_OFDPA_REFGROUP)

#define IND_OFDPA_L3UNICAST_BITMAP     (IND_OFDPA_VLANID | IND_OFDPA_SRCMAC | \
                                        IND_OFDPA_DSTMAC | IND_OFDPA_REFGROUP)

#define IND_OFDPA_L3INTERFACE_BITMAP   (IND_OFDPA_VLANID | IND_OFDPA_SRCMAC | \
                                        IND_OFDPA_REFGROUP)

#define IND_OFDPA_L2OVERLAY_BITMAP     (IND_OFDPA_PORT)

#define IND_OFDPA_L2MULTICAST_BITMAP   (IND_OFDPA_REFGROUP)

#define IND_OFDPA_L2FLOOD_BITMAP       (IND_OFDPA_REFGROUP)

#define IND_OFDPA_L3MULTICAST_BITMAP   (IND_OFDPA_REFGROUP)

#define IND_OFDPA_L3ECMP_BITMAP        (IND_OFDPA_REFGROUP)

#define IND_OFDPA_ING_PORT_FLOW_MATCH_BITMAP (IND_OFDPA_PORT)

#define IND_OFDPA_VLAN_FLOW_MATCH_BITMAP  (IND_OFDPA_PORT | IND_OFDPA_VLANID)

#define IND_OFDPA_TERM_MAC_FLOW_MATCH_BITMAP (IND_OFDPA_PORT | IND_OFDPA_ETHER_TYPE | \
                                              IND_OFDPA_DSTMAC | IND_OFDPA_VLANID)

#define IND_OFDPA_BRIDGING_FLOW_MATCH_BITMAP (IND_OFDPA_VLANID | IND_OFDPA_TUNNEL_ID | \
                                              IND_OFDPA_DSTMAC)

#define IND_OFDPA_UCAST_ROUTING_FLOW_MATCH_BITMAP  (IND_OFDPA_ETHER_TYPE | IND_OFDPA_IPV4_DST | \
                                                    IND_OFDPA_IPV6_DST)

#define IND_OFDPA_MCAST_ROUTING_FLOW_MATCH_BITMAP   (IND_OFDPA_ETHER_TYPE | IND_OFDPA_VLANID | \
                                                     IND_OFDPA_IPV4_SRC | IND_OFDPA_IPV4_DST | \
                                                     IND_OFDPA_IPV6_SRC | IND_OFDPA_IPV6_DST)

#define IND_OFDPA_ACL_POLICY_FLOW_MATCH_BITMAP    (IND_OFDPA_PORT | IND_OFDPA_ETHER_TYPE |       \
                                                   IND_OFDPA_SRCMAC | IND_OFDPA_DSTMAC |         \
                                                   IND_OFDPA_VLANID | IND_OFDPA_TUNNEL_ID |       \
                                                   IND_OFDPA_VLAN_PCP | IND_OFDPA_IPV4_SRC |     \
                                                   IND_OFDPA_IPV4_DST | IND_OFDPA_IPV6_SRC |     \
                                                   IND_OFDPA_IPV6_DST | IND_OFDPA_IPV4_ARP_SPA | \
                                                   IND_OFDPA_IP_PROTO | IND_OFDPA_IP_DSCP |         \
                                                   IND_OFDPA_IP_ECN | IND_OFDPA_TCP_L4_SRC_PORT |       \
                                                   IND_OFDPA_TCP_L4_DST_PORT | IND_OFDPA_UDP_L4_SRC_PORT | \
                                                   IND_OFDPA_UDP_L4_DST_PORT | IND_OFDPA_ICMPV4_TYPE | \
                                                   IND_OFDPA_ICMPV4_CODE | IND_OFDPA_IPV6_FLOW_LABEL | \
                                                   IND_OFDPA_SCTP_L4_SRC_PORT | IND_OFDPA_SCTP_L4_DST_PORT | \
                                                   IND_OFDPA_ICMPV6_CODE | IND_OFDPA_ICMPV6_TYPE)


extern ind_ofdpa_fields_t ind_ofdpa_match_fields_bitmask;
indigo_error_t indigoConvertOfdpaRv(OFDPA_ERROR_t result);

void ind_ofdpa_port_event_receive(void);
void ind_ofdpa_flow_event_receive(void);
void ind_ofdpa_pkt_receive(void);


