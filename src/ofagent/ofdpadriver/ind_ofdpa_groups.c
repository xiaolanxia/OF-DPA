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
* @filename     ind_ofdpa_groups.c
*
* @purpose      OF-DPA Driver for Indigo
*
* @component    OF-DPA
*
* @comments     none
*
* @create       6 Nov 2013
*
* @end
*
**********************************************************************/

#include <indigo/forwarding.h>
#include <ind_ofdpa_util.h>
#include <ind_ofdpa_log.h>

static indigo_error_t
ind_ofdpa_translate_group_actions(of_list_action_t *actions, 
                                  ind_ofdpa_group_bucket_t *group_bucket,
                                  uint32_t *group_action_bitmap)
{
    of_action_t act;
    int rv;
    uint16_t vlanId;
    of_oxm_t oxm;

    OF_LIST_ACTION_ITER(actions, &act, rv) {
        switch (act.header.object_id) {
        case OF_ACTION_OUTPUT: {
            of_port_no_t port_no;
            of_action_output_port_get(&act.output, &port_no);
            switch (port_no) {
                case OF_PORT_DEST_CONTROLLER:
                case OF_PORT_DEST_FLOOD:
                case OF_PORT_DEST_ALL:
                case OF_PORT_DEST_USE_TABLE:
                case OF_PORT_DEST_LOCAL:
                case OF_PORT_DEST_IN_PORT:
                case OF_PORT_DEST_NORMAL:
                    LOG_ERROR("unsupported output port 0x%x", port_no);
                    return INDIGO_ERROR_COMPAT;
                default: 
                    group_bucket->outputPort = port_no;
                    *group_action_bitmap |= IND_OFDPA_PORT;
                  break;
            }
            break;
        }
        case OF_ACTION_SET_FIELD: {
            /* HACK loci does not yet support the OXM field in the set-field action */
            of_oxm_header_init(&oxm.header, act.header.version, 0, 1);
            oxm.header.wire_object = act.header.wire_object;
            oxm.header.wire_object.obj_offset += 4; /* skip action header */
            oxm.header.parent = &act.header;
            of_object_wire_init(&oxm.header, OF_OXM, 0);
            if (oxm.header.length == 0) {
                LOG_ERROR("failed to parse set-field action");
                return INDIGO_ERROR_COMPAT;
            }
            switch (oxm.header.object_id) {
                case OF_OXM_VLAN_VID: 
                    of_oxm_vlan_vid_value_get(&oxm.vlan_vid, &vlanId);
                    group_bucket->vlanId = vlanId;
                    *group_action_bitmap |= IND_OFDPA_VLANID;
                    break;
                
                case OF_OXM_ETH_SRC: 
                    of_oxm_eth_src_value_get(&oxm.eth_src, &group_bucket->srcMac);
                    *group_action_bitmap |= IND_OFDPA_SRCMAC;
                    break;
                
                case OF_OXM_ETH_DST: 
                    of_oxm_eth_dst_value_get(&oxm.eth_dst, &group_bucket->dstMac);
                    *group_action_bitmap |= IND_OFDPA_DSTMAC;
                    break;
                
                default:
                    LOG_ERROR("unsupported set-field oxm %s", of_object_id_str[oxm.header.object_id]);
                    return INDIGO_ERROR_COMPAT;
            }
            break;
        }
        case OF_ACTION_SET_DL_SRC: 
            of_action_set_dl_src_dl_addr_get(&act.set_dl_src, &group_bucket->srcMac);
            *group_action_bitmap |= IND_OFDPA_SRCMAC;
            break;

        case OF_ACTION_SET_DL_DST: 
            of_action_set_dl_dst_dl_addr_get(&act.set_dl_dst, &group_bucket->dstMac);
            *group_action_bitmap |= IND_OFDPA_DSTMAC;
            break;
        
        case OF_ACTION_SET_VLAN_VID: 
            of_action_set_vlan_vid_vlan_vid_get(&act.set_vlan_vid, &vlanId);
            group_bucket->vlanId = vlanId;
            *group_action_bitmap |= IND_OFDPA_VLANID;
            break;
        
        case OF_ACTION_POP_VLAN:
        case OF_ACTION_STRIP_VLAN: 
            group_bucket->popVlanTag = 1;
            *group_action_bitmap |= IND_OFDPA_POPVLAN;
            break;
        
        case OF_ACTION_GROUP: 
            of_action_group_group_id_get(&act.group, &group_bucket->referenceGroupId);
            *group_action_bitmap |= IND_OFDPA_REFGROUP;
            break;
        
        default:
            LOG_ERROR("unsupported action %s", of_object_id_str[act.header.object_id]);
            return INDIGO_ERROR_COMPAT;
        }
    }

    return INDIGO_ERROR_NONE;
}

static indigo_error_t
ind_ofdpa_translate_group_buckets(uint32_t group_id, 
                                  of_list_bucket_t *of_buckets,
                                  uint16_t command)
{
  indigo_error_t err;
  uint16_t bucket_index = 0;
  of_list_action_t of_actions;
  of_bucket_t of_bucket;
  ind_ofdpa_group_bucket_t group_bucket;
  int rv;
  uint32_t group_type;
  uint32_t group_action_bitmap = 0;
  ofdpaGroupEntry_t group_entry;
  ofdpaGroupBucketEntry_t group_bucket_entry;
  OFDPA_ERROR_t ofdpa_rv = OFDPA_E_FAIL;
  int group_added = 0;

  OF_LIST_BUCKET_ITER(of_buckets, &of_bucket, rv) 
  {
    of_bucket_actions_bind(&of_bucket, &of_actions);

    memset(&group_bucket, 0, sizeof(group_bucket));

    err = ind_ofdpa_translate_group_actions(
        &of_actions, &group_bucket, &group_action_bitmap);
    if (err < 0) 
    {
      LOG_ERROR("Error in translating group actions");
      if (group_added == 1) 
      {
        /* Delete the added group */
        (void)ofdpaGroupDelete(group_id);
      }
      return err;
    }

    ofdpaGroupTypeGet(group_id, &group_type);

    memset(&group_bucket_entry, 0, sizeof(group_bucket_entry));
    group_bucket_entry.groupId = group_id;
    group_bucket_entry.bucketIndex = bucket_index;

    err = INDIGO_ERROR_NONE;

    switch (group_type)
    {
      case OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE:
        if((group_action_bitmap | IND_OFDPA_L2INTERFACE_BITMAP) != IND_OFDPA_L2INTERFACE_BITMAP)
        {
          err = INDIGO_ERROR_COMPAT;
          break;
        }
        group_bucket_entry.bucketData.l2Interface.outputPort = group_bucket.outputPort;
        group_bucket_entry.bucketData.l2Interface.popVlanTag = group_bucket.popVlanTag;

        break;

      case OFDPA_GROUP_ENTRY_TYPE_L2_REWRITE:
        if((group_action_bitmap | IND_OFDPA_L2REWRITE_BITMAP) != IND_OFDPA_L2REWRITE_BITMAP)
        {
          err = INDIGO_ERROR_COMPAT;
          break;
        }

        group_bucket_entry.bucketData.l2Rewrite.vlanId = group_bucket.vlanId;

        memcpy(&group_bucket_entry.bucketData.l2Rewrite.srcMac,
               &group_bucket.srcMac, sizeof(group_bucket_entry.bucketData.l2Rewrite.srcMac));

        memcpy(&group_bucket_entry.bucketData.l2Rewrite.dstMac,
               &group_bucket.dstMac, sizeof(group_bucket_entry.bucketData.l2Rewrite.dstMac));

        group_bucket_entry.referenceGroupId = group_bucket.referenceGroupId;

        break;

      case OFDPA_GROUP_ENTRY_TYPE_L3_UNICAST:
        if((group_action_bitmap | IND_OFDPA_L3UNICAST_BITMAP) != IND_OFDPA_L3UNICAST_BITMAP)
        {
          err = INDIGO_ERROR_COMPAT;
          break;
        }

        group_bucket_entry.bucketData.l3Unicast.vlanId = group_bucket.vlanId;

        memcpy(&group_bucket_entry.bucketData.l3Unicast.srcMac,
               &group_bucket.srcMac, sizeof(group_bucket_entry.bucketData.l3Unicast.srcMac));

        memcpy(&group_bucket_entry.bucketData.l3Unicast.dstMac,
               &group_bucket.dstMac, sizeof(group_bucket_entry.bucketData.l3Unicast.dstMac));

        group_bucket_entry.referenceGroupId = group_bucket.referenceGroupId;

        break;

      case OFDPA_GROUP_ENTRY_TYPE_L3_INTERFACE:
        if((group_action_bitmap | IND_OFDPA_L3INTERFACE_BITMAP) != IND_OFDPA_L3INTERFACE_BITMAP)
        {
          err = INDIGO_ERROR_COMPAT;
          break;
        }

        group_bucket_entry.bucketData.l3Interface.vlanId = group_bucket.vlanId;

        memcpy(&group_bucket_entry.bucketData.l3Interface.srcMac,
               &group_bucket.srcMac, sizeof(group_bucket_entry.bucketData.l3Interface.srcMac));

        group_bucket_entry.referenceGroupId = group_bucket.referenceGroupId;

        break;

      case OFDPA_GROUP_ENTRY_TYPE_L2_OVERLAY:
        if((group_action_bitmap | IND_OFDPA_L2OVERLAY_BITMAP) != IND_OFDPA_L2OVERLAY_BITMAP)
        {
          err = INDIGO_ERROR_COMPAT;
          break;
        }

        group_bucket_entry.bucketData.l2Overlay.outputPort = group_bucket.outputPort;
        break;

      case OFDPA_GROUP_ENTRY_TYPE_L2_MULTICAST:
      case OFDPA_GROUP_ENTRY_TYPE_L2_FLOOD:
      case OFDPA_GROUP_ENTRY_TYPE_L3_MULTICAST:
      case OFDPA_GROUP_ENTRY_TYPE_L3_ECMP:
        if((group_action_bitmap | IND_OFDPA_REFGROUP) != IND_OFDPA_REFGROUP)
        {
          err = INDIGO_ERROR_COMPAT;
          break;
        }

        group_bucket_entry.referenceGroupId = group_bucket.referenceGroupId;
        break;

      default:
        err = INDIGO_ERROR_PARAM;
        LOG_ERROR("Invalid Group Type");
        break;
    }

    if (err != INDIGO_ERROR_NONE)
    {
      if (err == INDIGO_ERROR_COMPAT)
      {
        LOG_ERROR("Incompatible fields for Group Type");
      }
      if (group_added == 1) 
      {
        /* Delete the added group */
        (void)ofdpaGroupDelete(group_id);
      }
      return err; 
    }

    if (command == OF_GROUP_ADD) 
    {
      if (bucket_index == 0)
      {
        /* First Bucket; Add Group first*/
        group_entry.groupId = group_id;
        ofdpa_rv = ofdpaGroupAdd(&group_entry);
        if (ofdpa_rv != OFDPA_E_NONE)
        {
          LOG_ERROR("Error in adding Group, rv=%d",ofdpa_rv);
          break;
        }
        group_added = 1;
      }
    }
    else /* OF_GROUP_MODIFY */
    {
      if (bucket_index == 0)
      {
        /* First Bucket; Delete all buckets first */
        ofdpa_rv = ofdpaGroupBucketsDeleteAll(group_id);
        if (ofdpa_rv != OFDPA_E_NONE)
        {
          LOG_ERROR("Error in deleting Group buckets, rv=%d",ofdpa_rv);
          break;
        }
      }
    }

    ofdpa_rv = ofdpaGroupBucketEntryAdd(&group_bucket_entry);
    if (ofdpa_rv != OFDPA_E_NONE)
    {
      LOG_ERROR("Error in adding Group bucket, rv=%d",ofdpa_rv);
      if (group_added == 1) 
      {
        /* Delete the added group */
        (void)ofdpaGroupDelete(group_id);
      }
      else /* OF_GROUP_MODIFY */
      {
        /* Need to clean up and delete Group here as well.
           Will be done by the caller as the Group also needs to
           be deleted from the Indigo database. */
      }
      break;
    }

    bucket_index++;
  }

  return indigoConvertOfdpaRv(ofdpa_rv);
}

indigo_error_t indigo_fwd_group_add(uint32_t id, uint8_t group_type, of_list_bucket_t *buckets)
{
  indigo_error_t err;

  if ((group_type != OF_GROUP_TYPE_INDIRECT) &&
      (group_type != OF_GROUP_TYPE_ALL)) 
  {
    return INDIGO_ERROR_NOT_SUPPORTED;
  }

  err = ind_ofdpa_translate_group_buckets(id, buckets, OF_GROUP_ADD);

  return err;
}

indigo_error_t indigo_fwd_group_modify(uint32_t id, of_list_bucket_t *buckets)
{
  indigo_error_t err;

  err = ind_ofdpa_translate_group_buckets(id, buckets, OF_GROUP_MODIFY);

  return err;
}

#ifdef OFDPA_FIXUP
indigo_error_t indigo_fwd_group_delete(uint32_t id)
#else
void indigo_fwd_group_delete(uint32_t id)
#endif
{
  OFDPA_ERROR_t ofdpa_rv;

  ofdpa_rv = ofdpaGroupDelete(id);

  LOG_INFO("Group Delete returned %d",ofdpa_rv);
  
#ifdef OFDPA_FIXUP
  return indigoConvertOfdpaRv(ofdpa_rv);
#else
  return;
#endif
}

void indigo_fwd_group_stats_get(uint32_t id, of_group_stats_entry_t *entry)
{
  OFDPA_ERROR_t ofdpa_rv;
  ofdpaGroupEntryStats_t groupStats;

  memset(&groupStats, 0, sizeof(groupStats));
  ofdpa_rv = ofdpaGroupStatsGet(id, &groupStats);

  if (ofdpa_rv != OFDPA_E_NONE)
  {
    LOG_INFO("Failed to get Group stats, rv = %d",ofdpa_rv);
  }
  else
  {
    of_group_stats_entry_ref_count_set(entry, groupStats.refCount);
    of_group_stats_entry_duration_sec_set(entry, groupStats.duration);
  }

  return;
}


