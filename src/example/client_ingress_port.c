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
* @filename     client_ingress_port.c
*
* @purpose      Example code for Ingress Port Flow Table
*
* @component    Unit Test
*
* @comments
*
* @create       17 Jul 2013
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

#define VERSION              1.0

#define DEFAULT_COUNT         1
#define DEFAULT_TUNNEL         0
#define DEFAULT_DELETE         0
#define DEFAULT_LIST           0

#define ARG_DELETE        "delete"
#define ARG_LIST          "list"

typedef struct
{
  int      count;
  int      tunnel;
  int      delete;
  int      list;
} arguments_t;

/* The options we understand. */
static struct argp_option options[] =
{
  { "count",   'c', "COUNT",      0, "Number of flows to add.",                                     0 },
  { "tunnel",  't', 0,            0, "Specifies whether matching flow is sent to tunnel pipeline.", 0 },
  { 0 }
};

/* Parse a single option. */
static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
  /* Get the INPUT argument from `argp_parse', which we
     know is a pointer to our arguments structure. */
  arguments_t *arguments = state->input;

  switch (key)
  {
    case 'c':                           /* count */
      errno = 0;
      arguments->count = strtoul(arg, NULL, 0);
      if (errno != 0)
      {
        argp_error(state, "Invalid count \"%s\"", arg);
        return errno;
      }
      break;

    case 't':                           /* tunnel */
      arguments->tunnel = 1;
      break;

    case ARGP_KEY_ARG:
      if (0 == strcasecmp(ARG_DELETE, arg))
      {
        arguments->delete = 1;
      }
      else if (0 == strcasecmp(ARG_LIST, arg))
      {
        arguments->list = 1;
      }
      else
      {
        argp_error(state, "Unknown option \"%s\"", arg);
      }
      break;

    case ARGP_KEY_NO_ARGS:
    case ARGP_KEY_END:
      break;

    default:
      return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

static void displayIngressPort(ofdpaFlowEntry_t *flow)
{
  printf("\tINPORT     = 0x%08x\n", flow->flowData.ingressPortFlowEntry.match_criteria.inPort);
  printf("\tINPORTMASK = 0x%08x\n", flow->flowData.ingressPortFlowEntry.match_criteria.inPortMask);
  printf("\tGOTO TABLE = %d\n", flow->flowData.ingressPortFlowEntry.gotoTableId);
  printf("\tTUNNEL     = %s\n", (OFDPA_FLOW_TABLE_ID_VLAN != flow->flowData.ingressPortFlowEntry.gotoTableId) ? "TRUE" : "FALSE");
}

int main(int argc, char *argv[])
{
  int               i;
  OFDPA_ERROR_t     rc;
  char              client_name[20] = "ofdpa client";
  char              docBuffer[300];
  char              versionBuf[100];
  ofdpaFlowEntry_t  flow;
  ofdpaFlowEntryStats_t  flowStats;
  arguments_t arguments =
    {
      .count      = DEFAULT_COUNT,
      .tunnel     = DEFAULT_TUNNEL,
      .delete     = DEFAULT_DELETE,
      .list       = DEFAULT_LIST,
    };

  /* Our argp parser. */
  struct argp argp =
    {
      .doc      = docBuffer,
      .options  = options,
      .parser   = parse_opt,
      .args_doc = "[" ARG_DELETE "] [" ARG_LIST "]",
    };

  sprintf(versionBuf, "%s v%.1f", basename(strdup(__FILE__)), VERSION);
  argp_program_version = versionBuf;

  strcpy(docBuffer, "Adds an Ingress Port flow.\vDefault values:\n");
  i = strlen(docBuffer);
  i += snprintf(&docBuffer[i], sizeof(docBuffer) - i, "COUNT      = %d\n", DEFAULT_COUNT);
  i += snprintf(&docBuffer[i], sizeof(docBuffer) - i, "TUNNEL     = %s\n", (DEFAULT_TUNNEL) ? "TRUE" : "FALSE");
  i += snprintf(&docBuffer[i], sizeof(docBuffer) - i, "\n");

  /* Parse our arguments; every option seen by `parse_opt' will be reflected in
     `arguments'. */
  argp_parse(&argp, argc, argv, 0, 0, &arguments);

  rc = ofdpaClientInitialize(client_name);
  if (rc != OFDPA_E_NONE)
  {
    return rc;
  }

  rc = ofdpaFlowEntryInit(OFDPA_FLOW_TABLE_ID_INGRESS_PORT, &flow);
  if (rc != OFDPA_E_NONE)
  {
    printf("\r\nFailed to initialize flow entry.(rc = %d)\n", rc);
    return rc;
  }

  if (arguments.tunnel)
  {
     ofdpaPortTypeSet(&flow.flowData.ingressPortFlowEntry.match_criteria.inPort, OFDPA_PORT_TYPE_LOGICAL_TUNNEL);
     flow.flowData.ingressPortFlowEntry.gotoTableId = OFDPA_FLOW_TABLE_ID_BRIDGING;
  }
  else
  {
    ofdpaPortTypeSet(&flow.flowData.ingressPortFlowEntry.match_criteria.inPort, OFDPA_PORT_TYPE_PHYSICAL);
    flow.flowData.ingressPortFlowEntry.gotoTableId = OFDPA_FLOW_TABLE_ID_VLAN;
  }
  flow.flowData.ingressPortFlowEntry.match_criteria.inPortMask = OFDPA_INPORT_TYPE_MASK;

  printf("%s %u Ingress Port flows with the following parameters:\r\n", arguments.delete ? "Deleting" : (arguments.list ? "Listing" : "Adding"), arguments.count);
  displayIngressPort(&flow);

  if (arguments.list || arguments.delete)
  {
    i = 0;

    rc = ofdpaFlowStatsGet(&flow, &flowStats);
    if (rc != OFDPA_E_NONE)
    {
      rc = ofdpaFlowNextGet(&flow, &flow);
    }
    while (rc == OFDPA_E_NONE)
    {
      i++;
      if (arguments.list)
      {
        printf("Flow number %d.\r\n", i);
        displayIngressPort(&flow);
      }
      else if (arguments.delete)
      {
        rc = ofdpaFlowDelete(&flow);
        if (rc != 0)
        {
          printf("\r\nError deleting ingress port flow entry rc = %d.\r\n", rc);
          displayIngressPort(&flow);
        }
      }
      if ((arguments.count == 0) || (i < arguments.count))
      {
        rc = ofdpaFlowNextGet(&flow, &flow);
      }
      else
      {
        rc = OFDPA_E_NOT_FOUND;
      }
    }
  }
  else
  {
    for (i = 0; i < arguments.count; i++)
    {
      rc = ofdpaFlowAdd(&flow);

      if (rc != 0)
      {
        printf("\r\nFailed to add ingress port flow entry. rc = %d.\r\n", rc);
        displayIngressPort(&flow);
        break;
      }
    }
  }

  return rc;
}
