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
* @filename     group_app.c
*
* @purpose      Add Groups. Uses RPC calls.
*
* @component    OF-DPA
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
#include <argp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <libgen.h>

#include "ofdpa_api.h"
#include "ofdpa_datatypes.h"

#define VERSION              1.0

#define ARG_DELETE "delete"
#define ARG_LIST   "list"
#define ARG_MODIFY "modify"

#define KEY_GROUPID               1
#define KEY_L2INTFBUCKETID        2
#define KEY_L2INTFOUTPORT         3
#define KEY_L2INTFPOPVLAN         4
#define KEY_L3INTFBUCKETID        5
#define KEY_L3INTFSRCMAC          6
#define KEY_L3INTFVLANID          7
#define KEY_L3INTFREFGID          8
#define KEY_L3UCASTBUCKETID       9
#define KEY_L3UCASTDSTMAC         10
#define KEY_L3UCASTSRCMAC         11
#define KEY_L3UCASTVLANID         12
#define KEY_L3UCASTREFGID         13
#define KEY_REFGROUPSBUCKETID     14
#define KEY_REFGROUPSREFGID       15
#define KEY_L2REWRITEBUCKETID     16
#define KEY_L2REWRITEDSTMAC       17
#define KEY_L2REWRITESRCMAC       18
#define KEY_L2REWRITEVLANID       19
#define KEY_L2REWRITEREFGID       20
#define KEY_GROUPENTRYSTATS       21
#define KEY_GROUPBUCKETTABLE      22
#define KEY_GROUPBUCKETENTRYSTATS 23
#define KEY_GROUPTABLE            24
#define KEY_L2OVERLAYBUCKETID     25
#define KEY_L2OVERLAYOUTPORT      26

typedef enum
{
  ARG_GROUP_L2INTF = 0,
  ARG_GROUP_L3INTF,
  ARG_GROUP_L3UCAST,
  ARG_GROUP_REFERENCEGROUPS,
  ARG_GROUP_L2REWRITE,
  ARG_GROUP_L2OVERLAY,
  ARG_GROUP_LAST
} arg_group_t;

typedef struct
{
  ofdpaGroupBucketEntry_t groupBucket;
  int groupTable;
  int groupEntryStats;
  int groupBucketTable;
  int groupBucketEntryStats;
  int groupBucketEntryStatsFlag;
  arg_group_t groupBucketFlag;
  int list;
  int modify;
  int delete;
} arguments_t;

/* The options we understand. */
static struct argp_option options[] =
{
  {"groupid",         KEY_GROUPID,                "GROUPID",     0, "Add a Group Entry.",                                         },

  {0,                 0,                          0,             0, "L2 Interface Group Bucket Entry:",                           },
  {"l2intfbktid",     KEY_L2INTFBUCKETID,         "INDEX",       0, "L2 Interface Group Bucket Index.",                           },
  {"outport",         KEY_L2INTFOUTPORT,          "OUTPUTPORT",  0, "Output port of an L2 Interface Group.",                      },
  {"popvlan",         KEY_L2INTFPOPVLAN,          "POPVLAN",     0, "Pop the VLAN tag before sending the packet.",                },

  {0,                 0,                          0,             0, "L3 Interface Group Bucket Entry:",                           },
  {"l3intfbktid",     KEY_L3INTFBUCKETID,         "INDEX",       0, "L3 Interface Group Bucket Index.",                           },
  {"l3intfsrcMac",    KEY_L3INTFSRCMAC,           "MAC",         0, "Source MAC corresponding to the L3 output interface.",       },
  {"l3intfvlanId",    KEY_L3INTFVLANID,           "VID",         0, "VLAN ID.",                                                   },
  {"l3intfrefGID",    KEY_L3INTFREFGID,           "GID",         0, "Reference Group ID.",                                        },

  {0,                 0,                          0,             0, "L3 Unicast Group Bucket Entry:",                             },
  {"l3ucastbktid",    KEY_L3UCASTBUCKETID,        "INDEX",       0, "L3 Unicast Group Bucket Index.",                             },
  {"l3ucastdstMac",   KEY_L3UCASTDSTMAC,          "MAC",         0, "Destination MAC corresponding to the L3 output interface.",  },
  {"l3ucastsrcMac",   KEY_L3UCASTSRCMAC,          "MAC",         0, "Source MAC corresponding to the L3 output interface.",       },
  {"l3ucastvlanId",   KEY_L3UCASTVLANID,          "VID",         0, "VLAN ID.",                                                   },
  {"l3ucastrefGID",   KEY_L3UCASTREFGID,          "GID",         0, "Reference Group ID.",                                        },

  {0,                 0,                          0,             0, "L2Multicast/L2Flood/L3Multicast/L3ECMP Group Bucket Entry:", },
  {"refgroupsbktid",  KEY_REFGROUPSBUCKETID,      "INDEX",       0, "L2Multicast/L2Flood/L3Multicast/L3ECMP Bucket index.",       },
  {"refgroupsrefGID", KEY_REFGROUPSREFGID,        "GID",         0, "Reference Group ID.",                                        },

  {0,                 0,                          0,             0, "L2 Rewrite Group Bucket Entry:",                             },
  {"l2rewritebktid",  KEY_L2REWRITEBUCKETID,      "INDEX",       0, "L2 Rewrite Group Bucket Index.",                             },
  {"l2rewritedstMac", KEY_L2REWRITEDSTMAC,        "MAC",         0, "Re-write the destination MAC.",                              },
  {"l2rewritesrcMac", KEY_L2REWRITESRCMAC,        "MAC",         0, "Re-write the Source MAC.",                                   },
  {"l2rewritevlanId", KEY_L2REWRITEVLANID,        "VID",         0, "Re-write the VLAN ID.",                                      },
  {"l2rewriterefGID", KEY_L2REWRITEREFGID,        "GID",         0, "Reference Group ID.",                                        },

  {0,                  0,                         0,             0, "L2 Overlay Group Bucket Entry:",                             },
  {"l2overlaybktid",   KEY_L2OVERLAYBUCKETID,     "INDEX",       0, "L2 Overlay Group Bucket Index.",                             },
  {"l2overlayoutport", KEY_L2OVERLAYOUTPORT,      "OUTPUTPORT",  0, "Access or Tunnel Endpoint Logical output port.",             },

  {0,                  0,                         0,             0, "Other Options:",                                             },
  {"groupentrystats",  KEY_GROUPENTRYSTATS,       0,             0, "List Group Entry statistics.",                               },
  {"groupbuckettable", KEY_GROUPBUCKETTABLE,      0,             0, "List Group Bucket table.",                                   },
  {"groupbktstats",    KEY_GROUPBUCKETENTRYSTATS, "INDEX",       0, "List Group Bucket Entry statistics.",                        },

  { 0 }
};

/* Parse a single option. */
static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
  /* Get the INPUT argument from `argp_parse', which we
     know is a pointer to our arguments structure. */
  arguments_t *arguments = state->input;
  uint32_t count = 0;
  uint32_t i = 0;
  union
  {
    unsigned char  bytes[6];
    unsigned short shorts[3];
  } mac;

  switch(key)
  {
    case KEY_GROUPID:
      errno = 0;
      arguments->groupBucket.groupId = strtoul(arg, NULL, 0);
      if (errno != 0)
      {
        argp_error(state, "Invalid group ID \"%s\"", arg);
        return errno;
      }
      arguments->groupTable = 1;
      break;
    case KEY_L2INTFBUCKETID:
    case KEY_L3INTFBUCKETID:
    case KEY_L3UCASTBUCKETID:
    case KEY_REFGROUPSBUCKETID:
    case KEY_L2REWRITEBUCKETID:
    case KEY_L2OVERLAYBUCKETID:
      errno = 0;
      arguments->groupBucket.bucketIndex = strtoul(arg, NULL, 0);
      if (errno != 0)
      {
        argp_error(state, "Invalid bucket index \"%s\"", arg);
        return errno;
      }
      if (key == KEY_L2INTFBUCKETID)
      {
        arguments->groupBucketFlag |= (1 << ARG_GROUP_L2INTF);
      }
      else if (key == KEY_L3INTFBUCKETID)
      {
        arguments->groupBucketFlag |= (1 << ARG_GROUP_L3INTF);
      }
      else if (key == KEY_L3UCASTBUCKETID)
      {
        arguments->groupBucketFlag |= (1 << ARG_GROUP_L3UCAST);
      }
      else if (key == KEY_REFGROUPSBUCKETID)
      {
        arguments->groupBucketFlag |= (1 << ARG_GROUP_REFERENCEGROUPS);
      }
      else if (key == KEY_L2REWRITEBUCKETID)
      {
        arguments->groupBucketFlag |= (1 << ARG_GROUP_L2REWRITE);
      }
      else if (key == KEY_L2OVERLAYBUCKETID)
      {
        arguments->groupBucketFlag |= (1 << ARG_GROUP_L2OVERLAY);
      }
      arguments->groupTable = 0;
      break;
    case KEY_L2INTFOUTPORT:
      errno = 0;
      arguments->groupBucket.bucketData.l2Interface.outputPort = strtoul(arg, NULL, 0);
      if (errno != 0)
      {
        argp_error(state, "Invalid output port \"%s\"", arg);
        return errno;
      }
      arguments->groupBucketFlag |= (1 << ARG_GROUP_L2INTF);
      arguments->groupTable = 0;
      break;
    case KEY_L2INTFPOPVLAN:
      errno = 0;
      arguments->groupBucket.bucketData.l2Interface.popVlanTag = strtoul(arg, NULL, 0);
      if (errno != 0)
      {
        argp_error(state, "Invalid parameter to pop vlan \"%s\"", arg);
        return errno;
      }
      arguments->groupBucketFlag |= (1 << ARG_GROUP_L2INTF);
      arguments->groupTable = 0;
      break;
    case KEY_L3INTFSRCMAC:
      errno = 0;
      if (6 != sscanf(arg, " %2hhx:%2hhx:%2hhx:%2hhx:%2hhx:%2hhx ",
                      &mac.bytes[0], &mac.bytes[1], &mac.bytes[2], &mac.bytes[3], &mac.bytes[4], &mac.bytes[5]))
      {
        if (6 != sscanf(arg, " %2hhx-%2hhx-%2hhx-%2hhx-%2hhx-%2hhx ",
                        &mac.bytes[0], &mac.bytes[1], &mac.bytes[2], &mac.bytes[3], &mac.bytes[4], &mac.bytes[5]))
        {
          if (3 != sscanf(arg, " %4hx.%4hx.%4hx ", &mac.shorts[0], &mac.shorts[1], &mac.shorts[2]))
          {
            argp_error(state, "Invalid source MAC address \"%s\"", arg);
            return errno;
          }
          else
          {
            /* successfully converted address string to short int values, need to handle endian issues here */
            mac.shorts[0] = htons(mac.shorts[0]);
            mac.shorts[1] = htons(mac.shorts[1]);
            mac.shorts[2] = htons(mac.shorts[2]);
          }
        }
      }
      memcpy(arguments->groupBucket.bucketData.l3Interface.srcMac.addr, mac.bytes,
             sizeof(arguments->groupBucket.bucketData.l3Interface.srcMac.addr));

      arguments->groupBucketFlag |= (1 << ARG_GROUP_L3INTF);
      arguments->groupTable = 0;
      break;
    case KEY_L3INTFVLANID:
      errno = 0;
      arguments->groupBucket.bucketData.l3Interface.vlanId = strtoul(arg, NULL, 0);
      if (errno != 0)
      {
        argp_error(state, "Invalid vlan ID \"%s\"", arg);
        return errno;
      }
      arguments->groupBucketFlag |= (1 << ARG_GROUP_L3INTF);
      arguments->groupTable = 0;
      break;
    case KEY_L3INTFREFGID:
    case KEY_L3UCASTREFGID:
    case KEY_REFGROUPSREFGID:
    case KEY_L2REWRITEREFGID:
      errno = 0;
      arguments->groupBucket.referenceGroupId = strtoul(arg, NULL, 0);
      if (errno != 0)
      {
        argp_error(state, "Invalid reference group ID \"%s\"", arg);
        return errno;
      }
      if (key == KEY_L3INTFREFGID)
      {
        arguments->groupBucketFlag |= (1 << ARG_GROUP_L3INTF);
      }
      else if (key == KEY_L3UCASTREFGID)
      {
        arguments->groupBucketFlag |= (1 << ARG_GROUP_L3UCAST);
      }
      else if (key == KEY_REFGROUPSREFGID)
      {
        arguments->groupBucketFlag |= (1 << ARG_GROUP_REFERENCEGROUPS);
      }
      else if (key == KEY_L2REWRITEREFGID)
      {
        arguments->groupBucketFlag |= (1 << ARG_GROUP_L2REWRITE);
      }
      arguments->groupTable = 0;
      break;
    case KEY_L3UCASTDSTMAC:
      errno = 0;
      if (6 != sscanf(arg, " %2hhx:%2hhx:%2hhx:%2hhx:%2hhx:%2hhx ",
                      &mac.bytes[0], &mac.bytes[1], &mac.bytes[2], &mac.bytes[3], &mac.bytes[4], &mac.bytes[5]))
      {
        if (6 != sscanf(arg, " %2hhx-%2hhx-%2hhx-%2hhx-%2hhx-%2hhx ",
                        &mac.bytes[0], &mac.bytes[1], &mac.bytes[2], &mac.bytes[3], &mac.bytes[4], &mac.bytes[5]))
        {
          if (3 != sscanf(arg, " %4hx.%4hx.%4hx ", &mac.shorts[0], &mac.shorts[1], &mac.shorts[2]))
          {
            argp_error(state, "Invalid destination MAC address \"%s\"", arg);
            return errno;
          }
          else
          {
            /* successfully converted address string to short int values, need to handle endian issues here */
            mac.shorts[0] = htons(mac.shorts[0]);
            mac.shorts[1] = htons(mac.shorts[1]);
            mac.shorts[2] = htons(mac.shorts[2]);
          }
        }
      }
      memcpy(arguments->groupBucket.bucketData.l3Unicast.dstMac.addr, mac.bytes,
             sizeof(arguments->groupBucket.bucketData.l3Unicast.dstMac.addr));
      arguments->groupBucketFlag |= (1 << ARG_GROUP_L3UCAST);
      arguments->groupTable = 0;
      break;
    case KEY_L3UCASTSRCMAC:
      errno = 0;
      if (6 != sscanf(arg, " %2hhx:%2hhx:%2hhx:%2hhx:%2hhx:%2hhx ",
                      &mac.bytes[0], &mac.bytes[1], &mac.bytes[2], &mac.bytes[3], &mac.bytes[4], &mac.bytes[5]))
      {
        if (6 != sscanf(arg, " %2hhx-%2hhx-%2hhx-%2hhx-%2hhx-%2hhx ",
                        &mac.bytes[0], &mac.bytes[1], &mac.bytes[2], &mac.bytes[3], &mac.bytes[4], &mac.bytes[5]))
        {
          if (3 != sscanf(arg, " %4hx.%4hx.%4hx ", &mac.shorts[0], &mac.shorts[1], &mac.shorts[2]))
          {
            argp_error(state, "Invalid source MAC address \"%s\"", arg);
            return errno;
          }
          else
          {
            /* successfully converted address string to short int values, need to handle endian issues here */
            mac.shorts[0] = htons(mac.shorts[0]);
            mac.shorts[1] = htons(mac.shorts[1]);
            mac.shorts[2] = htons(mac.shorts[2]);
          }
        }
      }
      memcpy(arguments->groupBucket.bucketData.l3Unicast.srcMac.addr, mac.bytes,
             sizeof(arguments->groupBucket.bucketData.l3Unicast.srcMac.addr));
      arguments->groupBucketFlag |= (1 << ARG_GROUP_L3UCAST);
      arguments->groupTable = 0;
      break;
    case KEY_L3UCASTVLANID:
      errno = 0;
      arguments->groupBucket.bucketData.l3Unicast.vlanId = strtoul(arg, NULL, 0);
      if (errno != 0)
      {
        argp_error(state, "Invalid vlan ID \"%s\"", arg);
        return errno;
      }
      arguments->groupBucketFlag |= (1 << ARG_GROUP_L3UCAST);
      arguments->groupTable = 0;
      break;
    case KEY_L2REWRITEDSTMAC:
      errno = 0;
      if (6 != sscanf(arg, " %2hhx:%2hhx:%2hhx:%2hhx:%2hhx:%2hhx ",
                      &mac.bytes[0], &mac.bytes[1], &mac.bytes[2], &mac.bytes[3], &mac.bytes[4], &mac.bytes[5]))
      {
        if (6 != sscanf(arg, " %2hhx-%2hhx-%2hhx-%2hhx-%2hhx-%2hhx ",
                        &mac.bytes[0], &mac.bytes[1], &mac.bytes[2], &mac.bytes[3], &mac.bytes[4], &mac.bytes[5]))
        {
          if (3 != sscanf(arg, " %4hx.%4hx.%4hx ", &mac.shorts[0], &mac.shorts[1], &mac.shorts[2]))
          {
            argp_error(state, "Invalid destination MAC address \"%s\"", arg);
            return errno;
          }
          else
          {
            /* successfully converted address string to short int values, need to handle endian issues here */
            mac.shorts[0] = htons(mac.shorts[0]);
            mac.shorts[1] = htons(mac.shorts[1]);
            mac.shorts[2] = htons(mac.shorts[2]);
          }
        }
      }

      memcpy(arguments->groupBucket.bucketData.l2Rewrite.dstMac.addr, mac.bytes,
             sizeof(arguments->groupBucket.bucketData.l2Rewrite.dstMac.addr));
      arguments->groupBucketFlag |= (1 << ARG_GROUP_L2REWRITE);
      arguments->groupTable = 0;
      break;
    case KEY_L2REWRITESRCMAC:
      errno = 0;
      if (6 != sscanf(arg, " %2hhx:%2hhx:%2hhx:%2hhx:%2hhx:%2hhx ",
                      &mac.bytes[0], &mac.bytes[1], &mac.bytes[2], &mac.bytes[3], &mac.bytes[4], &mac.bytes[5]))
      {
        if (6 != sscanf(arg, " %2hhx-%2hhx-%2hhx-%2hhx-%2hhx-%2hhx ",
                        &mac.bytes[0], &mac.bytes[1], &mac.bytes[2], &mac.bytes[3], &mac.bytes[4], &mac.bytes[5]))
        {
          if (3 != sscanf(arg, " %4hx.%4hx.%4hx ", &mac.shorts[0], &mac.shorts[1], &mac.shorts[2]))
          {
            argp_error(state, "Invalid source MAC address \"%s\"", arg);
            return errno;
          }
        }
      }

      memcpy(arguments->groupBucket.bucketData.l2Rewrite.srcMac.addr, mac.bytes,
             sizeof(arguments->groupBucket.bucketData.l2Rewrite.srcMac.addr));
      arguments->groupBucketFlag |= (1 << ARG_GROUP_L2REWRITE);
      arguments->groupTable = 0;
      break;
    case KEY_L2REWRITEVLANID:
      arguments->groupBucket.bucketData.l2Rewrite.vlanId = strtoul(arg, NULL, 0);
      if (errno != 0)
      {
        argp_error(state, "Invalid vlan ID \"%s\"", arg);
        return errno;
      }
      arguments->groupBucketFlag |= (1 << ARG_GROUP_L2REWRITE);
      arguments->groupTable = 0;
      break;
    case KEY_GROUPENTRYSTATS:
      arguments->groupEntryStats = 1;
      arguments->groupTable = 0;
      break;
    case KEY_GROUPBUCKETTABLE:
      arguments->groupBucketTable = 1;
      arguments->groupTable = 0;
      break;
    case KEY_GROUPBUCKETENTRYSTATS:
      arguments->groupBucketEntryStats = strtoul(arg, NULL, 0);
      if (errno != 0)
      {
        argp_error(state, "Invalid vlan ID \"%s\"", arg);
        return errno;
      }
      arguments->groupBucketEntryStatsFlag = 1;
      arguments->groupTable = 0;
      break;
    case KEY_L2OVERLAYOUTPORT:
      errno = 0;
      arguments->groupBucket.bucketData.l2Overlay.outputPort = strtoul(arg, NULL, 0);
      if (errno != 0)
      {
        argp_error(state, "Invalid output port \"%s\"", arg);
        return errno;
      }
      arguments->groupBucketFlag |= (1 << ARG_GROUP_L2OVERLAY);
      arguments->groupTable = 0;
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
      else if (0 == strcasecmp(ARG_MODIFY, arg))
      {
        arguments->modify = 1;
      }
      else
      {
        argp_error(state, "Unknown option \"%s\"", arg);
      }
      break;
    case ARGP_KEY_NO_ARGS:
      break;
    case ARGP_KEY_END:
      if ((arguments->list == 0) && ((arguments->groupEntryStats == 1)  ||
                                     (arguments->groupBucketTable == 1) ||
                                     (arguments->groupBucketEntryStatsFlag == 1)))
      {
        argp_error(state, "Use 'list' followed by the option.");
        return EINVAL;
      }

      /* Check if the correct parameters for a group */
      for (i = ARG_GROUP_L2INTF; i < ARG_GROUP_LAST; i++)
      {
        if (arguments->groupBucketFlag & (1 << i))
        {
          count++;
        }
        if (count > 1)
        {
          argp_error(state, "Invalid option for the group bucket trying to configure.");
          return EINVAL;
        }
      }
      break;
    default:
      return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

void groupAdd(uint32_t groupId)
{
  OFDPA_ERROR_t result;
  ofdpaGroupEntry_t group;
  uint32_t groupType;

  result = ofdpaGroupTypeGet(groupId, &groupType);
  if (result != OFDPA_E_NONE)
  {
    printf("Bad return code trying to get Group Type. (result = %d)\n", result);
    return;
  }
  else
  {
    result = ofdpaGroupEntryInit(groupType, &group);
    if (result != OFDPA_E_NONE)
    {
      printf("Bad return code trying to initialize Group Entry. (result = %d)\n", result);
      return;
    }
  }

  group.groupId = groupId;

  result = ofdpaGroupAdd(&group);
  if (result != OFDPA_E_NONE)
  {
    printf("Bad return code trying to Add a Group Entry. (result = %d)\n", result);
  }
  else
  {
    printf("Group entry added successfully \n");
  }

  return;
}

void groupDelete(uint32_t groupId)
{
  OFDPA_ERROR_t result;

  result = ofdpaGroupDelete(groupId);
  if (result != OFDPA_E_NONE)
  {
    printf("Bad return code trying to delete a Group Entry. (result = %d)\n", result);
  }
  else
  {
    printf("Group entry deleted successfully \n");
  }

  return;
}

void groupTableList(void)
{
  OFDPA_ERROR_t result;
  uint32_t groupId = 0;
  ofdpaGroupEntry_t nextGroup;
  char              buf[200];

  result = OFDPA_E_NONE;
  groupId = 0;

  printf("Group Table List:\n");

  while (result == OFDPA_E_NONE)
  {
    result = ofdpaGroupNextGet(groupId, &nextGroup);
    if (result == OFDPA_E_NONE)
    {
      ofdpaGroupDecode(nextGroup.groupId, buf, sizeof(buf));
      printf("\tgroupId = 0x%x (%s)\r\n", nextGroup.groupId, buf);
      groupId = nextGroup.groupId;
    }
  }
  return;
}

void groupStatistics(uint32_t groupId)
{
  OFDPA_ERROR_t result;
  ofdpaGroupEntryStats_t groupStats;

  result = ofdpaGroupStatsGet(groupId, &groupStats);
  if (result != OFDPA_E_NONE)
  {
    printf("Bad return code trying to display stats of a Group Entry. (result = %d)\n", result);
  }
  else
  {
    printf("Group Statistics:\n");
    printf("\tduration: %d, refCount:%d \n", groupStats.duration, groupStats.refCount);
  }
  return;
}

void groupBucketTypeL2InterfaceEntryAdd(arguments_t arguments)
{
  OFDPA_ERROR_t result;
  ofdpaGroupBucketEntry_t groupBucket;
  uint32_t groupType;

  result = ofdpaGroupTypeGet(arguments.groupBucket.groupId, &groupType);
  if (result != OFDPA_E_NONE)
  {
    printf("Bad return code trying to get Group Type. (result = %d)\n", result);
    return;
  }
  else
  {
    result = ofdpaGroupBucketEntryInit(groupType, &groupBucket);
    if (result != OFDPA_E_NONE)
    {
      printf("Bad return code trying to initialize an L2 Interface Group Bucket Entry. (result = %d)\n", result);
      return;
    }
  }

  groupBucket.groupId = arguments.groupBucket.groupId;
  groupBucket.bucketIndex = arguments.groupBucket.bucketIndex;
  groupBucket.bucketData.l2Interface.outputPort = arguments.groupBucket.bucketData.l2Interface.outputPort;
  groupBucket.bucketData.l2Interface.popVlanTag = arguments.groupBucket.bucketData.l2Interface.popVlanTag;

  result = ofdpaGroupBucketEntryAdd(&groupBucket);
  if (result != OFDPA_E_NONE)
  {
    printf("Bad return code trying to add an L2 Interface Group Bucket Entry. (result = %d)\n", result);
  }
  else
  {
    printf("Group bucket entry added successfully \n");
  }
  return;
}

void groupBucketTypeL2InterfaceEntryModify(arguments_t *arguments)
{
  OFDPA_ERROR_t result;
  ofdpaGroupBucketEntry_t groupBucket;

  groupBucket.groupId = arguments->groupBucket.groupId;
  groupBucket.bucketIndex = arguments->groupBucket.bucketIndex;
  groupBucket.bucketData.l2Interface.outputPort = arguments->groupBucket.bucketData.l2Interface.outputPort;
  groupBucket.bucketData.l2Interface.popVlanTag = arguments->groupBucket.bucketData.l2Interface.popVlanTag;

  result = ofdpaGroupBucketEntryModify(&groupBucket);
  if (result != OFDPA_E_NONE)
  {
    printf("Bad return code trying to modify an L2 Interface Group Bucket Entry. (result = %d)\n", result);
  }
  else
  {
    printf("Group bucket entry modified successfully \n");
  }
  return;
}

void groupBucketTypeL3InterfaceEntryAdd(arguments_t arguments)
{
  OFDPA_ERROR_t result;
  ofdpaGroupBucketEntry_t groupBucket;
  uint32_t groupType;

  result = ofdpaGroupTypeGet(arguments.groupBucket.groupId, &groupType);
  if (result != OFDPA_E_NONE)
  {
    printf("Bad return code trying to get Group Type. (result = %d)\n", result);
    return;
  }
  else
  {
    result = ofdpaGroupBucketEntryInit(groupType, &groupBucket);
    if (result != OFDPA_E_NONE)
    {
      printf("Bad return code trying to initialize an L3 Interface Group Bucket Entry. (result = %d)\n", result);
      return;
    }
  }

  groupBucket.groupId = arguments.groupBucket.groupId;
  groupBucket.bucketIndex = arguments.groupBucket.bucketIndex;
  groupBucket.referenceGroupId = arguments.groupBucket.referenceGroupId;

  groupBucket.bucketData.l3Interface.vlanId = arguments.groupBucket.bucketData.l3Interface.vlanId;

  memcpy(&groupBucket.bucketData.l3Interface.srcMac, &arguments.groupBucket.bucketData.l3Interface.srcMac,
         sizeof(arguments.groupBucket.bucketData.l3Interface.srcMac));

  result = ofdpaGroupBucketEntryAdd(&groupBucket);
  if (result != OFDPA_E_NONE)
  {
    printf("Bad return code trying to add an L3 Interface Group Bucket Entry. (result = %d)\n", result);
  }
  else
  {
    printf("Group bucket entry added successfully \n");
  }
  return;
}

void groupBucketTypeL3InterfaceEntryModify(arguments_t arguments)
{
#ifdef NOT_SUPPORTED
  OFDPA_ERROR_t result;
  ofdpaGroupBucketEntry_t groupBucket;

  groupBucket.groupId = arguments.groupBucket.groupId;
  groupBucket.bucketIndex = arguments.groupBucket.bucketIndex;
  groupBucket.referenceGroupId = arguments.groupBucket.referenceGroupId;

  groupBucket.bucketData.l3Interface.vlanId = arguments.groupBucket.bucketData.l3Interface.vlanId;

  memcpy(&groupBucket.bucketData.l3Interface.srcMac, &arguments.groupBucket.bucketData.l3Interface.srcMac,
         sizeof(arguments.groupBucket.bucketData.l3Interface.srcMac));

  result = ofdpaGroupBucketEntryModify(&groupBucket);
  if (result != OFDPA_E_NONE)
  {
    printf("Bad return code trying to modify an L3 Interface Group Bucket Entry. (result = %d)\n", result);
  }
  else
  {
    printf("Group bucket entry modified successfully \n");
  }
#else
  printf("\r\n%s: Not supported!\n", __FUNCTION__);
#endif
  return;
}

void groupBucketTypeL3UnicastEntryAdd(arguments_t arguments)
{
  OFDPA_ERROR_t result;
  ofdpaGroupBucketEntry_t groupBucket;
  uint32_t groupType;

  result = ofdpaGroupTypeGet(arguments.groupBucket.groupId, &groupType);
  if (result != OFDPA_E_NONE)
  {
    printf("Bad return code trying to get Group Type. (result = %d)\n", result);
    return;
  }
  else
  {
    result = ofdpaGroupBucketEntryInit(groupType, &groupBucket);
    if (result != OFDPA_E_NONE)
    {
      printf("Bad return code trying to initialize an L3 Unicast Group Bucket Entry. (result = %d)\n", result);
      return;
    }
  }

  groupBucket.groupId = arguments.groupBucket.groupId;
  groupBucket.bucketIndex = arguments.groupBucket.bucketIndex;
  groupBucket.referenceGroupId = arguments.groupBucket.referenceGroupId;
  groupBucket.bucketData.l3Unicast.vlanId = arguments.groupBucket.bucketData.l3Unicast.vlanId;

  memcpy(&groupBucket.bucketData.l3Unicast.dstMac, &arguments.groupBucket.bucketData.l3Unicast.dstMac,
         sizeof(groupBucket.bucketData.l3Unicast.dstMac));

  memcpy(&groupBucket.bucketData.l3Unicast.srcMac, &arguments.groupBucket.bucketData.l3Unicast.srcMac,
         sizeof(arguments.groupBucket.bucketData.l3Unicast.srcMac));


  result = ofdpaGroupBucketEntryAdd(&groupBucket);
  if (result != OFDPA_E_NONE)
  {
    printf("Bad return code trying to add an L3 Unicast Group Bucket Entry. (result = %d)\n", result);
  }
  else
  {
    printf("Group bucket entry added successfully \n");
  }
  return;
}

void groupBucketTypeL3UnicastEntryModify(arguments_t arguments)
{
  OFDPA_ERROR_t result;
  ofdpaGroupBucketEntry_t groupBucket;

  groupBucket.groupId = arguments.groupBucket.groupId;
  groupBucket.bucketIndex = arguments.groupBucket.bucketIndex;
  groupBucket.referenceGroupId = arguments.groupBucket.referenceGroupId;
  groupBucket.bucketData.l3Unicast.vlanId = arguments.groupBucket.bucketData.l3Unicast.vlanId;

  memcpy(&groupBucket.bucketData.l3Unicast.dstMac, &arguments.groupBucket.bucketData.l3Unicast.dstMac,
         sizeof(arguments.groupBucket.bucketData.l3Unicast.dstMac));

  memcpy(&groupBucket.bucketData.l3Unicast.srcMac, &arguments.groupBucket.bucketData.l3Unicast.srcMac,
         sizeof(arguments.groupBucket.bucketData.l3Unicast.dstMac));

  result = ofdpaGroupBucketEntryModify(&groupBucket);
  if (result != OFDPA_E_NONE)
  {
    printf("Bad return code trying to modify an L3 Unicast Group Bucket Entry. (result = %d)\n", result);
  }
  else
  {
    printf("Group bucket entry modified successfully \n");
  }
  return;
}

void groupBucketTypeL2RewriteEntryAdd(arguments_t arguments)
{
  OFDPA_ERROR_t result;
  ofdpaGroupBucketEntry_t groupBucket;
  uint32_t groupType;

  result = ofdpaGroupTypeGet(arguments.groupBucket.groupId, &groupType);
  if (result != OFDPA_E_NONE)
  {
    printf("Bad return code trying to get Group Type. (result = %d)\n", result);
    return;
  }
  else
  {
    result = ofdpaGroupBucketEntryInit(groupType, &groupBucket);
    if (result != OFDPA_E_NONE)
    {
      printf("Bad return code trying to initialize an L2 Rewrite Group Bucket Entry. (result = %d)\n", result);
      return;
    }
  }

  groupBucket.groupId = arguments.groupBucket.groupId;
  groupBucket.bucketIndex = arguments.groupBucket.bucketIndex;
  groupBucket.referenceGroupId = arguments.groupBucket.referenceGroupId;
  groupBucket.bucketData.l2Rewrite.vlanId = arguments.groupBucket.bucketData.l2Rewrite.vlanId;

  memcpy(&groupBucket.bucketData.l2Rewrite.dstMac, &arguments.groupBucket.bucketData.l2Rewrite.dstMac,
         sizeof(arguments.groupBucket.bucketData.l2Rewrite.dstMac));

  memcpy(&groupBucket.bucketData.l2Rewrite.srcMac, &arguments.groupBucket.bucketData.l2Rewrite.srcMac,
         sizeof(arguments.groupBucket.bucketData.l2Rewrite.srcMac));

  result = ofdpaGroupBucketEntryAdd(&groupBucket);
  if (result != OFDPA_E_NONE)
  {
    printf("Bad return code trying to add an L2 Rewrite Group Bucket Entry. (result = %d)\n", result);
  }
  else
  {
    printf("Group bucket entry added successfully \n");
  }
  return;
}

void groupBucketTypeL2OverlayEntryAdd(arguments_t arguments)
{
  OFDPA_ERROR_t result;
  ofdpaGroupBucketEntry_t groupBucket;
  uint32_t groupType;

  result = ofdpaGroupTypeGet(arguments.groupBucket.groupId, &groupType);
  if (result != OFDPA_E_NONE)
  {
    printf("Bad return code trying to get Group Type. (result = %d)\n", result);
    return;
  }
  else
  {
    result = ofdpaGroupBucketEntryInit(groupType, &groupBucket);
    if (result != OFDPA_E_NONE)
    {
      printf("Bad return code trying to initialize an L2 Overlay Group Bucket Entry. (result = %d)\n", result);
      return;
    }
  }

  groupBucket.groupId = arguments.groupBucket.groupId;
  groupBucket.bucketIndex = arguments.groupBucket.bucketIndex;
  groupBucket.bucketData.l2Overlay.outputPort = arguments.groupBucket.bucketData.l2Overlay.outputPort;

  result = ofdpaGroupBucketEntryAdd(&groupBucket);
  if (result != OFDPA_E_NONE)
  {
    printf("Bad return code trying to add an L2 Overlay Group Bucket Entry. (result = %d)\n", result);
  }
  else
  {
    printf("Group bucket entry added successfully \n");
  }
  return;
}

void groupBucketTypeL2RewriteEntryModify(arguments_t arguments)
{
#ifdef NOT_SUPPORTED
  OFDPA_ERROR_t result;
  ofdpaGroupBucketEntry_t groupBucket;

  groupBucket.groupId = arguments.groupBucket.groupId;
  groupBucket.bucketIndex = arguments.groupBucket.bucketIndex;
  groupBucket.referenceGroupId = arguments.groupBucket.referenceGroupId;
  groupBucket.bucketData.l2Rewrite.vlanId = arguments.groupBucket.bucketData.l2Rewrite.vlanId;

  memcpy(&groupBucket.bucketData.l2Rewrite.dstMac, &arguments.groupBucket.bucketData.l2Rewrite.dstMac,
         sizeof(arguments.groupBucket.bucketData.l2Rewrite.dstMac));

  memcpy(&groupBucket.bucketData.l2Rewrite.srcMac, &arguments.groupBucket.bucketData.l2Rewrite.srcMac,
         sizeof(arguments.groupBucket.bucketData.l2Rewrite.srcMac));

  result = ofdpaGroupBucketEntryModify(&groupBucket);
  if (result != OFDPA_E_NONE)
  {
    printf("Bad return code trying to modify an L2 Rewrite Group Bucket Entry. (result = %d)\n", result);
  }
  else
  {
    printf("Group bucket entry modified successfully \n");
  }
#else
  printf("\r\n%s: Not supported!\n", __FUNCTION__);
#endif
  return;
}

void groupBucketRefGroupAdd(arguments_t arguments)
{
  OFDPA_ERROR_t result;
  ofdpaGroupBucketEntry_t groupBucket;
  uint32_t groupType;

  result = ofdpaGroupTypeGet(arguments.groupBucket.groupId, &groupType);
  if (result != OFDPA_E_NONE)
  {
    printf("Bad return code trying to get Group Type. (result = %d)\n", result);
    return;
  }
  else
  {
    result = ofdpaGroupBucketEntryInit(groupType, &groupBucket);
    if (result != OFDPA_E_NONE)
    {
      printf("Bad return code trying to initialize an Reference Group Bucket Entry. (result = %d)\n", result);
      return;
    }
  }

  groupBucket.groupId = arguments.groupBucket.groupId;
  groupBucket.bucketIndex = arguments.groupBucket.bucketIndex;
  groupBucket.referenceGroupId = arguments.groupBucket.referenceGroupId;

  result = ofdpaGroupBucketEntryAdd(&groupBucket);
   if (result != OFDPA_E_NONE)
  {
    printf("Bad return code trying to add a Group Bucket Entry. (result = %d)\n", result);
  }
  else
  {
    printf("Group bucket entry added successfully \n");
  }
  return;
}

void groupBucketTableList(int groupId)
{
  OFDPA_ERROR_t result = OFDPA_E_NONE;
  ofdpaGroupBucketEntry_t nextBucketEntry;
  int tempGroupId = groupId;
  int tempBucketIndex = 0;
  uint32_t groupType;
  char              buf[200];

  memset(&nextBucketEntry, 0, sizeof(nextBucketEntry));

  result = ofdpaGroupBucketEntryFirstGet(tempGroupId, &nextBucketEntry);

  ofdpaGroupTypeGet(groupId, &groupType);

  if (result == OFDPA_E_NONE)
  {
    ofdpaGroupDecode(tempGroupId, buf, sizeof(buf));
    printf("Bucket entry list for group id = 0x%08x (%s):\r\n", tempGroupId, buf);
  }

  while (result == OFDPA_E_NONE)
  {
    printf("    bucket index = %d\r\n", nextBucketEntry.bucketIndex);
    switch (groupType)
    {
      case OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE:
        printf("\tOutputPort = %d\r\n",
               nextBucketEntry.bucketData.l2Interface.outputPort);

        printf("\tPopVlanTag = %d\r\n",
               nextBucketEntry.bucketData.l2Interface.popVlanTag);
        break;

      case OFDPA_GROUP_ENTRY_TYPE_L3_INTERFACE:
        printf("\tRefGroupId = 0x%x\r\n", nextBucketEntry.referenceGroupId);

        printf("\tVLANId = %d\r\n",
               nextBucketEntry.bucketData.l3Interface.vlanId);

        printf("\tMAC: %2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X\n",
               nextBucketEntry.bucketData.l3Interface.srcMac.addr[0],
               nextBucketEntry.bucketData.l3Interface.srcMac.addr[1],
               nextBucketEntry.bucketData.l3Interface.srcMac.addr[2],
               nextBucketEntry.bucketData.l3Interface.srcMac.addr[3],
               nextBucketEntry.bucketData.l3Interface.srcMac.addr[4],
               nextBucketEntry.bucketData.l3Interface.srcMac.addr[5]
               );
        break;

      case OFDPA_GROUP_ENTRY_TYPE_L3_UNICAST:
        printf("\tRefGroupId = 0x%x\r\n", nextBucketEntry.referenceGroupId);
        printf("\tVLANId = %d\r\n", nextBucketEntry.bucketData.l3Unicast.vlanId);

        printf("\tSrc MAC: %2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X\n",
               nextBucketEntry.bucketData.l3Unicast.srcMac.addr[0],
               nextBucketEntry.bucketData.l3Unicast.srcMac.addr[1],
               nextBucketEntry.bucketData.l3Unicast.srcMac.addr[2],
               nextBucketEntry.bucketData.l3Unicast.srcMac.addr[3],
               nextBucketEntry.bucketData.l3Unicast.srcMac.addr[4],
               nextBucketEntry.bucketData.l3Unicast.srcMac.addr[5]
               );

        printf("\tDst MAC: %2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X\n",
               nextBucketEntry.bucketData.l3Unicast.dstMac.addr[0],
               nextBucketEntry.bucketData.l3Unicast.dstMac.addr[1],
               nextBucketEntry.bucketData.l3Unicast.dstMac.addr[2],
               nextBucketEntry.bucketData.l3Unicast.dstMac.addr[3],
               nextBucketEntry.bucketData.l3Unicast.dstMac.addr[4],
               nextBucketEntry.bucketData.l3Unicast.dstMac.addr[5]
               );
        break;

      case OFDPA_GROUP_ENTRY_TYPE_L2_REWRITE:
          printf("\tRefGroupId = 0x%x\r\n", nextBucketEntry.referenceGroupId);
          printf("\tVLANId = %d\r\n", nextBucketEntry.bucketData.l2Rewrite.vlanId);

          printf("\tSrc MAC: %2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X\n",
                 nextBucketEntry.bucketData.l2Rewrite.srcMac.addr[0],
                 nextBucketEntry.bucketData.l2Rewrite.srcMac.addr[1],
                 nextBucketEntry.bucketData.l2Rewrite.srcMac.addr[2],
                 nextBucketEntry.bucketData.l2Rewrite.srcMac.addr[3],
                 nextBucketEntry.bucketData.l2Rewrite.srcMac.addr[4],
                 nextBucketEntry.bucketData.l2Rewrite.srcMac.addr[5]
                 );

          printf("\tDst MAC: %2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X\n",
                 nextBucketEntry.bucketData.l2Rewrite.dstMac.addr[0],
                 nextBucketEntry.bucketData.l2Rewrite.dstMac.addr[1],
                 nextBucketEntry.bucketData.l2Rewrite.dstMac.addr[2],
                 nextBucketEntry.bucketData.l2Rewrite.dstMac.addr[3],
                 nextBucketEntry.bucketData.l2Rewrite.dstMac.addr[4],
                 nextBucketEntry.bucketData.l2Rewrite.dstMac.addr[5]
               );
        break;

      case OFDPA_GROUP_ENTRY_TYPE_L2_OVERLAY:
        printf("\tOutputPort = 0x%x\r\n",
               nextBucketEntry.bucketData.l2Overlay.outputPort);

        break;

      case OFDPA_GROUP_ENTRY_TYPE_L2_MULTICAST:
      case OFDPA_GROUP_ENTRY_TYPE_L2_FLOOD:
      case OFDPA_GROUP_ENTRY_TYPE_L3_MULTICAST:
      case OFDPA_GROUP_ENTRY_TYPE_L3_ECMP:
        printf("\tRefGroupId = 0x%x\r\n", nextBucketEntry.referenceGroupId);
        break;

      default:
        printf("Unknown Group type \n");
        break;
    }


    tempBucketIndex = nextBucketEntry.bucketIndex;

    result = ofdpaGroupBucketEntryNextGet(tempGroupId, tempBucketIndex,
                                          &nextBucketEntry);

  }
  return;
}


void groupBucketTableStatistics(int groupId, int bucketIndex)
{
  printf("\t%s: Not Supported !\r\n", __FUNCTION__);
  return;
}

void groupBucketEntryDelete(int groupId, int bucketIndex)
{
  OFDPA_ERROR_t result;

  result = ofdpaGroupBucketEntryDelete(groupId, bucketIndex);
  if (result != OFDPA_E_NONE)
  {
    printf("Bad return code trying to delete a Group Bucket Entry. (result = %d)\n", result);
  }
  else
  {
    printf("Group Bucket Entry deleted successfully. \n");
  }
  return;
}



int main(int argc, char *argv[])
{
  OFDPA_ERROR_t  rc;
  char           client_name[20] = "ofdpa client group";
  char           docBuffer[300];
  char           versionBuf[100];
  arguments_t    arguments;

  memset(&arguments, 0, sizeof(arguments));


  /* Our argp parser. */
  struct argp argp =
    {
      .doc     = docBuffer,
      .options = options,
      .parser  = parse_opt,
      .args_doc = "[" ARG_DELETE "] [" ARG_LIST "] ["ARG_MODIFY"]",
    };

  sprintf(versionBuf, "%s v%.1f", basename(strdup(__FILE__)), VERSION);
  argp_program_version = versionBuf;

  strcpy(docBuffer, "\nAdds, deletes or lists groups and group buckets.\n");

  /* Parse our arguments; every option seen by `parse_opt' will be reflected in
     `arguments'. */
  argp_parse(&argp, argc, argv, 0, 0, &arguments);

  rc = ofdpaClientInitialize(client_name);
  if (rc != OFDPA_E_NONE)
  {
    return rc;
  }


  if (arguments.list)
  {
    if (arguments.groupTable)
    {
      groupTableList();
    }
    else if (arguments.groupEntryStats)
    {
      groupStatistics(arguments.groupBucket.groupId);
    }
    else if (arguments.groupBucketTable)
    {
      groupBucketTableList(arguments.groupBucket.groupId);
    }
    else if (arguments.groupBucketEntryStatsFlag)
    {
      groupBucketTableStatistics(arguments.groupBucket.groupId,
                                 arguments.groupBucket.bucketIndex);
    }
    else
    {
      /* List the groups created */
      groupTableList();
    }
  }
  else if (arguments.delete)
  {
    if (arguments.groupTable)
    {
      groupDelete(arguments.groupBucket.groupId);
    }
    else
    {
      groupBucketEntryDelete(arguments.groupBucket.groupId,
                             arguments.groupBucket.bucketIndex);
    }

  }
  else if (arguments.modify)
  {
    if (arguments.groupBucketFlag & (1 << ARG_GROUP_L2INTF))
    {
      groupBucketTypeL2InterfaceEntryModify(&arguments);
    }
    else if (arguments.groupBucketFlag & (1 << ARG_GROUP_L3INTF))
    {
      groupBucketTypeL3InterfaceEntryModify(arguments);
    }
    else if (arguments.groupBucketFlag & (1 << ARG_GROUP_L3UCAST))
    {
      groupBucketTypeL3UnicastEntryModify(arguments);
    }
    else if (arguments.groupBucketFlag & (1 << ARG_GROUP_L2REWRITE))
    {
      groupBucketTypeL2RewriteEntryModify(arguments);
    }
    else
    {
      printf("\r\nNot supported!\n");
    }
  }
  else
  {
    if (arguments.groupTable)
    {
      groupAdd(arguments.groupBucket.groupId);
    }
    else if (arguments.groupBucketFlag & (1 << ARG_GROUP_L2INTF))
    {
      groupBucketTypeL2InterfaceEntryAdd(arguments);
    }
    else if (arguments.groupBucketFlag & (1 << ARG_GROUP_L3INTF))
    {
      groupBucketTypeL3InterfaceEntryAdd(arguments);
    }
    else if (arguments.groupBucketFlag & (1 << ARG_GROUP_L3UCAST))
    {
      groupBucketTypeL3UnicastEntryAdd(arguments);
    }
    else if (arguments.groupBucketFlag & (1 << ARG_GROUP_L2REWRITE))
    {
      groupBucketTypeL2RewriteEntryAdd(arguments);
    }
    else if (arguments.groupBucketFlag & (1 << ARG_GROUP_L2OVERLAY))
    {
      groupBucketTypeL2OverlayEntryAdd(arguments);
    }
    else if (arguments.groupBucketFlag & (1 << ARG_GROUP_REFERENCEGROUPS))
    {
      groupBucketRefGroupAdd(arguments);
    }
    else
    {
      groupTableList();
    }
  }

  return rc;
}
