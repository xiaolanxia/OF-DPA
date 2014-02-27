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
* @filename     client_event.c
*
* @purpose      Receives events from OFDPA server. Uses RPC calls.
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "ofdpa_api.h"

const char *argp_program_version = "client_event v1.0";

/* Arguments as follows:
 * argv[1] - how long to block on receive, in seconds. -1 to block forever.
 */
int main(int argc, char *argv[])
{
  char client_name[] = "ofdpa event client";
  OFDPA_ERROR_t rc;
  int32_t wait;

  struct timeval timeout;
  struct timeval *tv = NULL;

  time_t startTime;
  time_t currentTime;
  int32_t elapsedSeconds;

  ofdpaFlowEntry_t flow;
  ofdpaFlowEvent_t flowEventData;

  ofdpaPortEvent_t portEventData;
  int i;

  if (argc == 2)
  {
    wait = atoi(argv[1]);
  }
  else
  {
    wait = -1;
  }

  rc = ofdpaClientInitialize(client_name);
  if (rc != OFDPA_E_NONE)
  {
    return rc;
  }

  if (wait >= 0)
  {
    timeout.tv_sec = wait;
    timeout.tv_usec = 0;
    tv = &timeout;
    printf("\nclient_event: using wait time %d seconds for event\n", wait);
  }
  else
    printf("\nclient_event: waiting indefinitely for events\n");

  /* first clear out out any existing flows in the VLAN table */
  memset(&flow, 0, sizeof(flow));
  flow.tableId = OFDPA_FLOW_TABLE_ID_VLAN;

  /* wait if OFDPA process not ready for RPC */
  while (ofdpaFlowNextGet(&flow, &flow) == OFDPA_E_RPC)
  {
    usleep(1000000);
  }
  do
  {
    ofdpaFlowDelete(&flow);
  } while (ofdpaFlowNextGet(&flow, &flow) == OFDPA_E_NONE);

  /* build and add some flows with various hard timeout values */
  memset(&flow, 0, sizeof(flow));
  flow.tableId = OFDPA_FLOW_TABLE_ID_VLAN;
  flow.flowData.vlanFlowEntry.gotoTableId = OFDPA_FLOW_TABLE_ID_TERMINATION_MAC;
  flow.flowData.vlanFlowEntry.match_criteria.inPort = 1;
  flow.flowData.vlanFlowEntry.match_criteria.vlanId = 100 | OFDPA_VID_PRESENT;
  flow.flowData.vlanFlowEntry.match_criteria.vlanIdMask = OFDPA_VID_PRESENT | OFDPA_VID_EXACT_MASK;
  flow.hard_time = 5;

  if (ofdpaFlowAdd(&flow) != OFDPA_E_NONE)
  {
    printf("client_event: ofdpaFlowAdd failed for first flow: rc = %d\n", rc);
    /* no sense in continuing if we cannot add the first test flow */
    return(-1);
  }

  /* build up a second flow with a later hard time value */
  flow.flowData.vlanFlowEntry.match_criteria.vlanId++;
  flow.hard_time = 15;
  if (ofdpaFlowAdd(&flow) != OFDPA_E_NONE)
  {
    printf("client_event: ofdpaFlowAdd failed for second flow: rc = %d\n", rc);
    /* forge ahead */
  }

  /* build up a flow with a idle time value that expires before the second hard time flow */
  flow.flowData.vlanFlowEntry.match_criteria.vlanId++;
  flow.hard_time = 0;
  flow.idle_time = 10;
  if (ofdpaFlowAdd(&flow) != OFDPA_E_NONE)
  {
    printf("client_event: ofdpaFlowAdd failed: rc = %d\n", rc);
    /* forge ahead */
  }

  /* build up a flow with a idle time value AND a hard time value to see
     if we can get both event types valid for the same flow simultaneously
   */
  flow.flowData.vlanFlowEntry.match_criteria.vlanId++;
  flow.hard_time = 12;
  flow.idle_time = 12;
  if (ofdpaFlowAdd(&flow) != OFDPA_E_NONE)
  {
    printf("client_event: ofdpaFlowAdd failed: rc = %d\n", rc);
    /* forge ahead */
  }
  flow.idle_time = 0;

  /* build up a group of flows with the same hard time value to see if we can
     generate multiple flow events sharing the same event notification
   */
  flow.hard_time = 20;
  for (i=0; i<10; i++)
  {
    flow.flowData.vlanFlowEntry.match_criteria.vlanId++;
    if (ofdpaFlowAdd(&flow) != OFDPA_E_NONE)
    {
      printf("client_event: ofdpaFlowAdd failed in flow add loop: rc = %d, i = %d\n", rc, i);
      /* forge ahead */
    }
  }

  printf("client_event: waiting for event notifications...\n");
  time(&startTime);
  while ((rc = ofdpaEventReceive(tv)) == OFDPA_E_NONE)
  {
    time(&currentTime);
    elapsedSeconds = currentTime - startTime;
    printf("client_event: received event notification (elapsed time: %d seconds)\n",
           elapsedSeconds);

    memset(&flowEventData, 0, sizeof(flowEventData));
    flowEventData.flowMatch.tableId = OFDPA_FLOW_TABLE_ID_VLAN;

    while (ofdpaFlowEventNextGet(&flowEventData) == OFDPA_E_NONE)
    {
      printf("client_event: retrieved aged flow: tableId = %d, eventMask = %x\n",
             flowEventData.flowMatch.tableId, flowEventData.eventMask);
      printf("    flow data: inPort = %d, vlanId = %d\n",
             flowEventData.flowMatch.flowData.vlanFlowEntry.match_criteria.inPort,
             flowEventData.flowMatch.flowData.vlanFlowEntry.match_criteria.vlanId & OFDPA_VID_EXACT_MASK);

      if ((rc = ofdpaFlowDelete(&flowEventData.flowMatch)) != OFDPA_E_NONE)
        printf("client_event: ofdpaFlowDelete rc = %d\n", rc);
      else
        printf("client_event: successfully deleted aged flow\n");
    }

    memset(&portEventData, 0, sizeof(portEventData));
    while (ofdpaPortEventNextGet(&portEventData) == OFDPA_E_NONE)
    {
      printf("client_event: retrieved port event: port no = %d, eventMask = 0x%x, state = %d\n",
             portEventData.portNum, portEventData.eventMask, portEventData.state);
    }
  }

  printf("client_event: ofdpaEventReceive rc = %d\n", rc);

  return 0;
}
