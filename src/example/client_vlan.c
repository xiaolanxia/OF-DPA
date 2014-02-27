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
* @filename     client_vlan.c
*
* @purpose      Example code for VLAN Flow Table
*
* @component    Unit Test
*
* @comments
*
* @create       26 Apr 2013
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


#define DEFAULT_COUNT        1
#define DEFAULT_VLANID       0x1001
#define DEFAULT_VLANIDMASK   0x1fff
#define DEFAULT_INPORT       1
#define DEFAULT_NEWVLANID    1
#define DEFAULT_DISCARD      0
#define DEFAULT_DELETE       0
#define DEFAULT_LIST         0

#define VLANID_MAX        4096

#define ARG_DELETE        "delete"
#define ARG_LIST          "list"

typedef struct
{
  int      count;
  int      vlanId;
  int      vlanIdMask;
  int      inPort;
  int      newVlan;
  int      discard;
  int      delete;
  int      list;
} arguments_t;

/* The options we understand. */
static struct argp_option options[] =
{
  { "count",   'c', "COUNT",      0, "Number of flows to add.",                                  0 },
  { "vlan",    'v', "VLANID",     0, "The VLAN of the flow.",                                    0 },
  { "mask",    'm', "VLANIDMASK", 0, "The VLAN mask of the flow.",                               0 },
  { "intf",    'i', "INTF",       0, "The input interface of the flow.",                         0 },
  { 0,          0,  0,            0, "Actions:",                                                   },
  { "discard", 'd', 0,            0, "Discard matching flows.",                                    },
  { "newvlan", 'n', "VLANID",     0, "Data for the Set VLAN field action.",                        },
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

    case 'i':                           /* interface */
      errno = 0;
      arguments->inPort= strtoul(arg, NULL, 0);
      if (errno != 0)
      {
        argp_error(state, "Invalid interface\"%s\"", arg);
        return errno;
      }
      break;

    case 'v':                           /* VLAN */
      errno = 0;
      arguments->vlanId = strtoul(arg, NULL, 0);
      if (errno != 0)
      {
        argp_error(state, "Invalid VLAN ID \"%s\"", arg);
        return errno;
      }
      break;

    case 'm':                           /* VLAN mask */
      errno = 0;
      arguments->vlanIdMask = strtoul(arg, NULL, 0);
      if (errno != 0)
      {
        argp_error(state, "Invalid VLAN mask \"%s\"", arg);
        return errno;
      }
      break;

    case 'n':                           /* new VLAN */
      errno = 0;
      arguments->newVlan = strtoul(arg, NULL, 0);
      if (errno != 0)
      {
        argp_error(state, "Invalid VLAN ID \"%s\"", arg);
        return errno;
      }
      break;

    case 'd':                           /* discard */
      arguments->discard = 1;
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

static void displayVlan(ofdpaFlowEntry_t *flow)
{
  printf("\tVLANID     = %x\n", flow->flowData.vlanFlowEntry.match_criteria.vlanId);
  printf("\tVLANIDMASK = %x\n", flow->flowData.vlanFlowEntry.match_criteria.vlanIdMask);
  printf("\tINTERFACE  = %d\n", flow->flowData.vlanFlowEntry.match_criteria.inPort);
  printf("\tGO TABLE   = %d\n", flow->flowData.vlanFlowEntry.gotoTableId);
  printf("\tNEW VLAN   = %d\n", flow->flowData.vlanFlowEntry.newVlanId);
}

int main(int argc, char *argv[])
{
  int               i;
  OFDPA_ERROR_t     rc;
  char              client_name[20] = "ofdpa client";
  char              docBuffer[300];
  char              versionBuf[100];
  ofdpaFlowEntry_t  flow;
  arguments_t arguments =
    {
      .count      = DEFAULT_COUNT,
      .vlanId     = DEFAULT_VLANID,
      .vlanIdMask = DEFAULT_VLANIDMASK,
      .inPort     = DEFAULT_INPORT,
      .newVlan    = DEFAULT_NEWVLANID,
      .discard    = DEFAULT_DISCARD,
      .delete     = DEFAULT_DELETE,
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

  strcpy(docBuffer, "Adds a VLAN flow.\vDefault values:\n");
  i = strlen(docBuffer);
  i += snprintf(&docBuffer[i], sizeof(docBuffer) - i, "COUNT      = %d\n", DEFAULT_COUNT);
  i += snprintf(&docBuffer[i], sizeof(docBuffer) - i, "VLANID     = 0x%x\n", DEFAULT_VLANID);
  i += snprintf(&docBuffer[i], sizeof(docBuffer) - i, "VLANIDMASK = 0x%x\n", DEFAULT_VLANIDMASK);
  i += snprintf(&docBuffer[i], sizeof(docBuffer) - i, "INTERFACE  = %d\n", DEFAULT_INPORT);
  i += snprintf(&docBuffer[i], sizeof(docBuffer) - i, "NEW VLAN   = %d\n", DEFAULT_NEWVLANID);
  i += snprintf(&docBuffer[i], sizeof(docBuffer) - i, "DISCARD    = %s\n", (DEFAULT_DISCARD) ? "TRUE" : "FALSE");
  i += snprintf(&docBuffer[i], sizeof(docBuffer) - i, "\n");

  /* Parse our arguments; every option seen by `parse_opt' will be reflected in
     `arguments'. */
  argp_parse(&argp, argc, argv, 0, 0, &arguments);

  rc = ofdpaClientInitialize(client_name);
  if (rc != OFDPA_E_NONE)
  {
    return rc;
  }

  rc = ofdpaFlowEntryInit(OFDPA_FLOW_TABLE_ID_VLAN, &flow);
  if (rc != OFDPA_E_NONE)
  {
    printf("\r\nFailed to initialize Vlan Flow Table.(rc = %d)\n", rc);
    return rc;
  }
  if (0 == arguments.list)
  {
    flow.flowData.vlanFlowEntry.match_criteria.inPort = arguments.inPort;
    flow.flowData.vlanFlowEntry.match_criteria.vlanId = arguments.vlanId;
    flow.flowData.vlanFlowEntry.match_criteria.vlanIdMask = arguments.vlanIdMask;
    flow.flowData.vlanFlowEntry.newVlanId = arguments.newVlan;

    if (0 == arguments.discard)
    {
      flow.flowData.vlanFlowEntry.gotoTableId = OFDPA_FLOW_TABLE_ID_TERMINATION_MAC;
    }
  }

  printf("%s %u VLAN flows with the following parameters:\r\n", arguments.delete ? "Deleting" : (arguments.list ? "Listing" : "Adding"), arguments.count);
  displayVlan(&flow);

  if ((arguments.list))
    printf("\r\nVLAN is incremented in each additional flow.\r\n\r\n");

  for (i = 0; i < arguments.count; i++)
  {
    if (arguments.delete)
    {
      rc = ofdpaFlowDelete(&flow);
    }
    else if (arguments.list)
    {
      rc = ofdpaFlowNextGet(&flow, &flow);
      if (0 == rc)
      {
        printf("Flow number %d:\n", i);
        displayVlan(&flow);
      }
    }
    else
    {
      rc = ofdpaFlowAdd(&flow);
    }
    if (rc != 0)
    {
      if ((0 != arguments.list) &&
          (OFDPA_E_NOT_FOUND == rc))
      {
        printf("\r\nNo more entries found.\r\n");
      }
      else
      {
        printf("\r\nFailed to %s VLAN flow entry %u, rc = %d.\r\n", arguments.delete ? "delete" : (arguments.list ? "list" : "add"), i, rc);
      }
      break;
    }
    if (0 == arguments.list)
    {
      flow.flowData.vlanFlowEntry.match_criteria.vlanId++;
    }
  }

  return rc;
}
