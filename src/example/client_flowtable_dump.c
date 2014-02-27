/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2003-2014
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
* @filename     client_flowtable_dump.c
*
* @purpose      Dump flow tables. Uses RPC calls.
*
* @component    Unit Test
*
* @comments
*
* @create
*
* @end
*
**********************************************************************/
#include "ofdpa_api.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <argp.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define DEFAULT_COUNT        0

const char *argp_program_version = "client_flowtable_dump v1.1";

/* The options we understand. */
static struct argp_option options[] =
{
  { "count",   'c', "COUNT",     0, "Number of entries from start of table. (0 for all)",       0 },
  { 0 }
};

static int count;
static int tableId;

struct
{
  OFDPA_FLOW_TABLE_ID_t type;
  char *name;
} tableNameList[] =
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

/* Parse a single option. */
static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
  switch (key)
  {
    case 'c':                           /* count */
      errno = 0;
      count = strtoul(arg, NULL, 0);
      if (errno != 0)
      {
        argp_error(state, "Invalid count \"%s\"", arg);
        return errno;
      }
      break;

    case ARGP_KEY_ARG:
      errno = 0;
      tableId = strtoul(arg, NULL, 0);
      if (errno != 0)
      {
        argp_error(state, "Invalid table ID \"%s\"", arg);
        return errno;
      }
      switch (tableId)
      {
        case OFDPA_FLOW_TABLE_ID_INGRESS_PORT:
        case OFDPA_FLOW_TABLE_ID_VLAN:
        case OFDPA_FLOW_TABLE_ID_TERMINATION_MAC:
        case OFDPA_FLOW_TABLE_ID_UNICAST_ROUTING:
        case OFDPA_FLOW_TABLE_ID_MULTICAST_ROUTING:
        case OFDPA_FLOW_TABLE_ID_BRIDGING:
        case OFDPA_FLOW_TABLE_ID_ACL_POLICY:
          break;
        default:
          argp_error(state, "Invalid table ID \"%s\"", arg);
          return ARGP_ERR_UNKNOWN;
          break;
      }
      break;

    case ARGP_KEY_NO_ARGS:
      argp_error(state, "Table ID must be specified.");
      return ARGP_ERR_UNKNOWN;
      break;

    case ARGP_KEY_END:
      break;

    default:
      return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

char *flowTableNameGet(OFDPA_FLOW_TABLE_ID_t tableId)
{
  int i;

  for (i=0; i < TABLE_NAME_LIST_SIZE; i++)
  {
    if (tableNameList[i].type == tableId)
    {
      return(tableNameList[i].name);
    }
  }
  return("[Table name not found]");
}

void printFlowEntry(ofdpaFlowEntry_t *flow)
{
  char addrBuf[20];
  char addrMaskBuf[20];
  struct in_addr ipv4Addr;

  switch (flow->tableId)
  {
  case OFDPA_FLOW_TABLE_ID_INGRESS_PORT:
    {
      ofdpaIngressPortFlowEntry_t *flowData;
      ofdpaIngressPortFlowMatch_t *match;

      flowData = &flow->flowData.ingressPortFlowEntry;
      match = &flowData->match_criteria;

      /* match criteria */
      printf("  inPort:mask = %d:%03x", match->inPort, match->inPortMask);

      /* instructions */
      printf(" |");
      printf(" GoTo = %d (%s)", flowData->gotoTableId, flowTableNameGet(flowData->gotoTableId));
    }
    break;

  case OFDPA_FLOW_TABLE_ID_VLAN:
    {
      ofdpaVlanFlowEntry_t *flowData;
      ofdpaVlanFlowMatch_t *match;

      flowData = &flow->flowData.vlanFlowEntry;
      match = &flowData->match_criteria;

      printf(" inPort = %d", match->inPort);
      printf(" vlanId/mask = %x/%x", match->vlanId, match->vlanIdMask);

      /* instructions */
      printf(" |");
      printf(" GoTo = %d (%s)", flowData->gotoTableId, flowTableNameGet(flowData->gotoTableId));

      printf(" newVlanId = %d", flowData->newVlanId);
    }
    break;

  case OFDPA_FLOW_TABLE_ID_TERMINATION_MAC:
    {
      ofdpaTerminationMacFlowEntry_t *flowData;
      ofdpaTerminationMacFlowMatch_t *match;

      flowData = &flow->flowData.terminationMacFlowEntry;
      match = &flowData->match_criteria;

      printf(" inPort:mask = %d:%03x", match->inPort, match->inPortMask);
      printf(" etherType = %04x", match->etherType);
      printf(" destMac:mask = %2.2x%2.2x.%2.2x%2.2x.%2.2x%2.2x:%2.2x%2.2x.%2.2x%2.2x.%2.2x%2.2x",
             match->destMac.addr[0], match->destMac.addr[1],
             match->destMac.addr[2], match->destMac.addr[3],
             match->destMac.addr[4], match->destMac.addr[5],
             match->destMacMask.addr[0], match->destMacMask.addr[1],
             match->destMacMask.addr[2], match->destMacMask.addr[3],
             match->destMacMask.addr[4], match->destMacMask.addr[5]);
      printf(" vlanId:mask = %d:%03x", match->vlanId, match->vlanIdMask);

      /* instructions */
      printf(" |");
      printf(" GoTo = %d (%s)", flowData->gotoTableId, flowTableNameGet(flowData->gotoTableId));

      printf(" outPort = %d", flowData->outputPort);
    }
    break;

  case OFDPA_FLOW_TABLE_ID_UNICAST_ROUTING:
    {
      ofdpaUnicastRoutingFlowEntry_t *flowData;
      ofdpaUnicastRoutingFlowMatch_t *match;

      flowData = &flow->flowData.unicastRoutingFlowEntry;
      match = &flowData->match_criteria;

      /* match criteria */
      printf(" etherType = %04x", match->etherType);

      ipv4Addr.s_addr = htonl(match->dstIp4);
      inet_ntop(AF_INET, &ipv4Addr, addrBuf, sizeof(addrBuf));
      ipv4Addr.s_addr = htonl(match->dstIp4Mask);
      inet_ntop(AF_INET, &ipv4Addr, addrMaskBuf, sizeof(addrMaskBuf));
      printf(" dstIp4 = %s/%s", addrBuf, addrMaskBuf);

      inet_ntop(AF_INET6, &match->dstIp6, addrBuf, sizeof(addrBuf));
      inet_ntop(AF_INET6, &match->dstIp6Mask, addrMaskBuf, sizeof(addrMaskBuf));
      printf(" dstIp6 = %s/%s", addrBuf, addrMaskBuf);

      /* instructions */
      printf(" |");
      printf(" GoTo = %d (%s)", flowData->gotoTableId, flowTableNameGet(flowData->gotoTableId));
      printf(" groupId = 0x%08x", flowData->groupID);
    }
    break;

  case OFDPA_FLOW_TABLE_ID_MULTICAST_ROUTING:
    {
      ofdpaMulticastRoutingFlowEntry_t *flowData;
      ofdpaMulticastRoutingFlowMatch_t *match;

      flowData = &flow->flowData.multicastRoutingFlowEntry;
      match = &flowData->match_criteria;

      /* match criteria */
      printf(" etherType = %04x", match->etherType);
      printf(" vlanId = %d", match->vlanId);

      ipv4Addr.s_addr = htonl(match->srcIp4);
      inet_ntop(AF_INET, &ipv4Addr, addrBuf, sizeof(addrBuf));
      ipv4Addr.s_addr = htonl(match->srcIp4Mask);
      inet_ntop(AF_INET, &ipv4Addr, addrMaskBuf, sizeof(addrMaskBuf));
      printf(" srcIp4 = %s/%s", addrBuf, addrMaskBuf);

      ipv4Addr.s_addr = htonl(match->dstIp4);
      inet_ntop(AF_INET, &ipv4Addr, addrBuf, sizeof(addrBuf));
      printf(" dstIp4 = %s", addrBuf);

      inet_ntop(AF_INET6, &match->srcIp6, addrBuf, sizeof(addrBuf));
      inet_ntop(AF_INET6, &match->srcIp6Mask, addrMaskBuf, sizeof(addrMaskBuf));
      printf(" srcIp6 = %s/%s", addrBuf, addrMaskBuf);

      inet_ntop(AF_INET6, &match->dstIp6, addrBuf, sizeof(addrBuf));
      printf(" dstIp6 = %s", addrBuf);

      /* instructions */
      printf(" |");
      printf(" GoTo = %d (%s)", flowData->gotoTableId, flowTableNameGet(flowData->gotoTableId));
      printf(" groupId = 0x%08x", flowData->groupID);
    }
    break;

  case OFDPA_FLOW_TABLE_ID_BRIDGING:
    {
      ofdpaBridgingFlowEntry_t *flowData;
      ofdpaBridgingFlowMatch_t *match;

      flowData = &flow->flowData.bridgingFlowEntry;
      match = &flowData->match_criteria;

      /* match criteria */
      if (match->vlanId)
        printf(" vlanId = %d", match->vlanId);
      else
        printf(" tunnelId = %d", match->tunnelId);

      printf(" destMac:mask = %2.2x%2.2x.%2.2x%2.2x.%2.2x%2.2x:%2.2x%2.2x.%2.2x%2.2x.%2.2x%2.2x",
             match->destMac.addr[0], match->destMac.addr[1],
             match->destMac.addr[2], match->destMac.addr[3],
             match->destMac.addr[4], match->destMac.addr[5],
             match->destMacMask.addr[0], match->destMacMask.addr[1],
             match->destMacMask.addr[2], match->destMacMask.addr[3],
             match->destMacMask.addr[4], match->destMacMask.addr[5]);

      /* instructions */
      printf(" |");
      printf(" GoTo = %d (%s)", flowData->gotoTableId, flowTableNameGet(flowData->gotoTableId));
      printf(" groupId = 0x%08x", flowData->groupID);
      if (flowData->tunnelLogicalPort)
      {
        printf(" tunnelLogicalPort = 0x%08x", flowData->tunnelLogicalPort);
      }
      printf(" outPort = %d", flowData->outputPort);
    }
    break;

  case OFDPA_FLOW_TABLE_ID_ACL_POLICY:
    {
      ofdpaPolicyAclFlowEntry_t *flowData;
      ofdpaPolicyAclFlowMatch_t *match;

      flowData = &flow->flowData.policyAclFlowEntry;
      match = &flowData->match_criteria;

      /* match criteria */

      printf(" inPort:mask = %d:%03x", match->inPort, match->inPortMask);
      printf(" srcMac:mask = %2.2x%2.2x.%2.2x%2.2x.%2.2x%2.2x:%2.2x%2.2x.%2.2x%2.2x.%2.2x%2.2x",
             match->srcMac.addr[0], match->srcMac.addr[1],
             match->srcMac.addr[2], match->srcMac.addr[3],
             match->srcMac.addr[4], match->srcMac.addr[5],
             match->srcMacMask.addr[0], match->srcMacMask.addr[1],
             match->srcMacMask.addr[2], match->srcMacMask.addr[3],
             match->srcMacMask.addr[4], match->srcMacMask.addr[5]);
      printf(" destMac:mask = %2.2x%2.2x.%2.2x%2.2x.%2.2x%2.2x:%2.2x%2.2x.%2.2x%2.2x.%2.2x%2.2x",
             match->destMac.addr[0], match->destMac.addr[1],
             match->destMac.addr[2], match->destMac.addr[3],
             match->destMac.addr[4], match->destMac.addr[5],
             match->destMacMask.addr[0], match->destMacMask.addr[1],
             match->destMacMask.addr[2], match->destMacMask.addr[3],
             match->destMacMask.addr[4], match->destMacMask.addr[5]);
      printf(" etherType = %04x", match->etherType);
      if (match->vlanId)
        printf(" vlanId:mask = %d:%x", match->vlanId, match->vlanIdMask);
      else
        printf(" tunnelId = %d", match->tunnelId);
      if (match->vlanPcp)
        printf(" vlanPcp:mask = %d:%x", match->vlanPcp, match->vlanPcpMask);

      ipv4Addr.s_addr = htonl(match->sourceIp4);
      inet_ntop(AF_INET, &ipv4Addr, addrBuf, sizeof(addrBuf));
      ipv4Addr.s_addr = htonl(match->sourceIp4Mask);
      inet_ntop(AF_INET, &ipv4Addr, addrMaskBuf, sizeof(addrMaskBuf));
      printf(" srcIp4 = %s/%s", addrBuf, addrMaskBuf);

      ipv4Addr.s_addr = htonl(match->destIp4);
      inet_ntop(AF_INET, &ipv4Addr, addrBuf, sizeof(addrBuf));
      ipv4Addr.s_addr = htonl(match->destIp4Mask);
      inet_ntop(AF_INET, &ipv4Addr, addrMaskBuf, sizeof(addrMaskBuf));
      printf(" dstIp4 = %s/%s", addrBuf, addrMaskBuf);

      inet_ntop(AF_INET6, &match->sourceIp6, addrBuf, sizeof(addrBuf));
      inet_ntop(AF_INET6, &match->sourceIp6Mask, addrMaskBuf, sizeof(addrMaskBuf));
      printf(" srcIp6 = %s/%s", addrBuf, addrMaskBuf);

      inet_ntop(AF_INET6, &match->destIp6, addrBuf, sizeof(addrBuf));
      inet_ntop(AF_INET6, &match->destIp6Mask, addrMaskBuf, sizeof(addrMaskBuf));
      printf(" dstIp6 = %s/%s", addrBuf, addrMaskBuf);

      printf(" DSCP = %u", match->dscp);
      printf(" IP Protocol = 0x%2.2x", match->ipProto);
      printf(" Source L4 Port = %u", match->srcL4Port);
      printf(" Destination L4 Port = %u", match->destL4Port);
      printf(" ICMP Type = %u", match->icmpType);
      printf(" ICMP Code = %u", match->icmpCode);

      /* instructions */
      printf(" |");
      if (flowData->groupID)
      {
         printf(" Set output group ID = 0x%8x", flowData->groupID);
      }
      if (flowData->queueIDAction)
      {
         printf(" Set CoS queue = %d", flowData->queueID);
      }
      if (flowData->vlanPcpAction)
      {
         printf(" Set VLAN priority = %d", flowData->vlanPcp);
      }
      if (flowData->dscpAction)
      {
         printf(" Set DSCP = %d", flowData->dscp);
      }

      printf(" outPort = %d", flowData->outputPort);

    }
    break;

  default:
    /* unknown table ID */
    break;

  }

  printf(" |");
  printf(" priority = %d", flow->priority);
  printf(" cookie = %llu", (unsigned long long int)flow->cookie);
  printf(" hard_time = %d", flow->hard_time);
  printf(" idle_time = %d", flow->idle_time);

  printf("\r\n");
}

int main(int argc, char *argv[])
{
  int i, j;
  int rc;
  char              client_name[] = "ofdpa flowtable_dump client";
  char docBuffer[300];
  char argsDocBuffer[300];
  char buffer[300];
  ofdpaFlowEntry_t flow;
  ofdpaFlowTableInfo_t info;

  /* Our argp parser. */
  struct argp argp =
    {
      .args_doc = argsDocBuffer,
      .doc      = docBuffer,
      .options  = options,
      .parser   = parse_opt,
    };

  count = DEFAULT_COUNT;

  strcpy(argsDocBuffer, "table_ID");

  strcpy(docBuffer, "Prints all entries in the specified OF-DPA flow table.\vDefault values:\n");
  i = strlen(docBuffer);
  i += sprintf(&docBuffer[i], "COUNT     = %d\n", DEFAULT_COUNT);
  i += sprintf(&docBuffer[i], "Valid flow table IDs:\n");
  for (j=0; j < TABLE_NAME_LIST_SIZE; j++)
  {
    i += sprintf(&docBuffer[i], "  %d - %s\n", tableNameList[j].type, tableNameList[j].name);
  }
  i += sprintf(&docBuffer[i], "\n");

  /* Parse our arguments; every option seen by `parse_opt' will be reflected in
     `arguments'. */
  argp_parse(&argp, argc, argv, 0, 0, 0);

  rc = ofdpaClientInitialize(client_name);
  if (rc != OFDPA_E_NONE)
  {
    return rc;
  }

  memset(&flow, 0, sizeof(flow));
  flow.tableId = tableId;

  if (count != 0)
  {
    sprintf(buffer, "up to %d", count);
  }
  else
  {
    sprintf(buffer, "all");
  }

  memset(&info, 0, sizeof(info));

  rc = ofdpaFlowTableInfoGet(tableId, &info);
  if (rc != OFDPA_E_NONE)
  {
    printf("  Could not retrieve OF-DPA table info with ID %d.\r\n", tableId);
    return rc;
  }

  printf("Retrieving %s flow table entries from OF-DPA table ID %d (%s):\r\n", buffer, tableId, flowTableNameGet(tableId));
  printf("Max entries = %d, Current entries = %d.\r\n ", info.maxEntries, info.numEntries);

  i = 0;
  while ((ofdpaFlowNextGet(&flow, &flow) == OFDPA_E_NONE) &&
         ((count == 0) || (i < count)))
  {
    printFlowEntry(&flow);
    i++;
  }
  if (i == 0)
  {
    printf("  No entries found.\r\n");
  }

  return rc;
}
