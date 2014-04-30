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
* @filename     client_cfg_purge.c
*
* @purpose      Program to walk and delete all entries in OF-DPA.
*               Uses RPC calls.
*
* @component    Unit Test
*
* @comments     DEVELOPMENT TESTING PURPOSES ONLY.
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
#include <libgen.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
  char              client_name[] = "ofdpa purge client";
  OFDPA_ERROR_t rc;
  uint32_t count;

  ofdpaFlowEntry_t flow;
  ofdpaFlowEntryStats_t flowStats;
  uint32_t tableIdIndex;
  OFDPA_FLOW_TABLE_ID_t flowTablePurgeOrderList[] = {OFDPA_FLOW_TABLE_ID_VLAN,
                                                     OFDPA_FLOW_TABLE_ID_TERMINATION_MAC,
                                                     OFDPA_FLOW_TABLE_ID_UNICAST_ROUTING,
                                                     OFDPA_FLOW_TABLE_ID_MULTICAST_ROUTING,
                                                     OFDPA_FLOW_TABLE_ID_BRIDGING,
                                                     OFDPA_FLOW_TABLE_ID_ACL_POLICY,
                                                     OFDPA_FLOW_TABLE_ID_INGRESS_PORT};
#define FLOW_TABLE_PURGE_LIST_NUM_ENTRIES  (sizeof(flowTablePurgeOrderList)/sizeof(OFDPA_FLOW_TABLE_ID_t))
  ofdpaGroupEntry_t group;
  uint32_t tunnelPortId;
  uint32_t ecmpNextHopId;
  uint32_t nextHopId;
  uint32_t tunnelId;
  ofdpaPortEvent_t portEventData;

  rc = ofdpaClientInitialize(client_name);
  if (rc != OFDPA_E_NONE)
  {
    printf("\nFailure calling ofdpaClientInitialize(). rc = %d", rc);
    return rc;
  }

  printf("Purging flow tables.\n");
  for (tableIdIndex = 0; tableIdIndex < FLOW_TABLE_PURGE_LIST_NUM_ENTRIES; tableIdIndex++)
  {
    ofdpaFlowEntryInit(flowTablePurgeOrderList[tableIdIndex], &flow);
    count = 0;
    if ((ofdpaFlowStatsGet(&flow, &flowStats) == OFDPA_E_NONE) ||
        (ofdpaFlowNextGet(&flow, &flow) == OFDPA_E_NONE))
    {
      do
      {
        if ((rc = ofdpaFlowDelete(&flow)) == OFDPA_E_NONE)
        {
          count++;
        }
        else
        {
          printf("\tError returned from odpaFlowDelete: rc = %d, tableId = %d\n",
                 rc, flow.tableId);
        }
      } while (ofdpaFlowNextGet(&flow, &flow) == OFDPA_E_NONE);
    }

    if (count != 0)
    {
      printf("\tDeleted %d flows from tableId %d\n", count, flow.tableId);
    }
    else
    {
      printf("\tNo entries found in tableId %d\n", flow.tableId);
    }
  }

  printf("Purging group table.\n");
  memset(&group, 0, sizeof(group));
  count = 0;
  while (ofdpaGroupNextGet(group.groupId, &group) == OFDPA_E_NONE)
  {
    if ((rc = ofdpaGroupDelete(group.groupId)) == OFDPA_E_NONE)
    {
      count++;
    }
    else
    {
      printf("\tError returned from ofdpaGroupDelete: rc = %d, groupId = 0x%08x\n",
             rc, group.groupId);
    }
  }
  if (count != 0)
  {
    printf("\tDeleted %d groups.\n", count);
  }
  else
  {
    printf("\tNo entries found\n");
  }

  printf("Purging logical tunnel port to tenant associations.\n");
  tunnelPortId = 0;
  ofdpaPortTypeSet(&tunnelPortId, OFDPA_PORT_TYPE_LOGICAL_TUNNEL);
  while (ofdpaTunnelPortNextGet(tunnelPortId, &tunnelPortId) == OFDPA_E_NONE)
  {
    count = 0;
    tunnelId = 0;
    while (ofdpaTunnelPortTenantNextGet(tunnelPortId, tunnelId, &tunnelId) == OFDPA_E_NONE)
    {
      if ((rc = ofdpaTunnelPortTenantDelete(tunnelPortId, tunnelId)) == OFDPA_E_NONE)
      {
        count++;
      }
      else
      {
        printf("\tError returned from ofdpaTunnelPortTenantDelete: rc = %d, tunnelPortId = 0x%08x, tunnelId = %d\n",
               rc, tunnelPortId, tunnelId);
      }
    }
    if (count != 0)
    {
      printf("\tDeleted %d tunnel logical port to tenant associations for port 0x%08x.\n", count, tunnelPortId);
    }
    else
    {
      printf("\tNo entries found for port 0x%08x.\n", tunnelPortId);
    }
  }

  printf("Purging logical tunnel ports.\n");
  tunnelPortId = 0;
  ofdpaPortTypeSet(&tunnelPortId, OFDPA_PORT_TYPE_LOGICAL_TUNNEL);
  count = 0;
  while (ofdpaTunnelPortNextGet(tunnelPortId, &tunnelPortId) == OFDPA_E_NONE)
  {
    if ((rc = ofdpaTunnelPortDelete(tunnelPortId)) == OFDPA_E_NONE)
    {
      count++;
    }
    else
    {
      printf("\tError returned from ofdpaTunnelPortDelete: rc = %d, tunnelPortId = 0x%08x\n",
             rc, tunnelPortId);
    }
  }
  if (count != 0)
  {
    printf("\tDeleted %d tunnel logical ports.\n", count);
  }
  else
  {
    printf("\tNo entries found\n");
  }

  printf("Purging tunnel ECMP next hop groups.\n");
  ecmpNextHopId = 0;
  count = 0;
  while (ofdpaTunnelEcmpNextHopGroupNextGet(ecmpNextHopId, &ecmpNextHopId) == OFDPA_E_NONE)
  {
    /* remove any member next hops from this group */
    nextHopId = 0;
    while ((rc = ofdpaTunnelEcmpNextHopGroupMemberNextGet(ecmpNextHopId, nextHopId, &nextHopId)) == OFDPA_E_NONE)
    {
      if ((rc = ofdpaTunnelEcmpNextHopGroupMemberDelete(ecmpNextHopId, nextHopId)) != OFDPA_E_NONE)
      {
        printf("\tError returned from ofdpaTunnelEcmpNextHopGroupMemberDelete: rc = %d, ecmpNextHopId = %d, nextHopId = %d\n",
               rc, ecmpNextHopId, nextHopId);
      }
    }

    if ((rc = ofdpaTunnelEcmpNextHopGroupDelete(ecmpNextHopId)) == OFDPA_E_NONE)
    {
      count++;
    }
    else
    {
      printf("\tError returned from ofdpaTunnelEcmpNextHopGroupDelete: rc = %d, nextHopId = 0x%08x\n",
             rc, nextHopId);
    }
  }
  if (count != 0)
  {
    printf("\tDeleted %d ECMP next hop groups.\n", count);
  }
  else
  {
    printf("\tNo entries found\n");
  }

  printf("Purging tenants.\n");
  tunnelId = 0;
  count = 0;
  while (ofdpaTunnelTenantNextGet(tunnelId, &tunnelId) == OFDPA_E_NONE)
  {
    if ((rc = ofdpaTunnelTenantDelete(tunnelId)) == OFDPA_E_NONE)
    {
      count++;
    }
    else
    {
      printf("\tError returned from ofdpaTunnelTenantDelete: rc = %d, tunnelId = %d\n",
             rc, tunnelId);
    }
  }
  if (count != 0)
  {
    printf("\tDeleted %d tenants.\n", count);
  }
  else
  {
    printf("\tNo entries found\n");
  }

  printf("Purging tunnel next hops.\n");
  nextHopId = 0;
  count = 0;
  while (ofdpaTunnelNextHopNextGet(nextHopId, &nextHopId) == OFDPA_E_NONE)
  {
    if ((rc = ofdpaTunnelNextHopDelete(nextHopId)) == OFDPA_E_NONE)
    {
      count++;
    }
    else
    {
      printf("\tError returned from ofdpaTunnelNextHopDelete: rc = %d, nextHopId = %d\n",
             rc, nextHopId);
    }
  }
  if (count != 0)
  {
    printf("\tDeleted %d next hops.\n", count);
  }
  else
  {
    printf("\tNo entries found\n");
  }

  /* retrieve all port events to allow logical ports to actually be deleted from database */
  printf("Retrieving pending logical port delete events.\n");
  memset(&portEventData, 0, sizeof(portEventData));
  count = 0;
  while (ofdpaPortEventNextGet(&portEventData) == OFDPA_E_NONE)
  {
    if (portEventData.eventMask & OFDPA_EVENT_PORT_DELETE)
    {
      count++;
    }
  }
  if (count != 0)
  {
    printf("\tRetrieved %d port delete events.\n", count);
  }
  else
  {
    printf("\tNo events found\n");
  }

  return(0);
}
