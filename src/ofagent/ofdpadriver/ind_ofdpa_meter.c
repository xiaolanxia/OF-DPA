/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2014-2016
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
* @filename   ind_ofdpa_meter.c
*
* @purpose    OF-DPA Driver for Indigo
*
* @component  OF-DPA
*
* @comments   none
*
* @create     12 Aug 2014
*
* @end
*
**********************************************************************/
#include "indigo/forwarding.h"
#include "ind_ofdpa_util.h"
#include "ind_ofdpa_log.h"

#ifdef OFDPA_FIXUP
indigo_error_t indigo_fwd_meter_add(uint32_t id, uint16_t flag, of_list_meter_band_t *meters)
{
  indigo_error_t err = INDIGO_ERROR_NONE;
  of_meter_band_t of_meter_band;
  int rv;
  OFDPA_ERROR_t ofdpa_rv = OFDPA_E_NONE;
  ofdpaMeterEntry_t meter;

  memset(&meter, 0, sizeof(meter));

  LOG_TRACE("meter_add: id %d, flag 0x%x",id, flag);
  OF_LIST_METER_BAND_ITER(meters, &of_meter_band, rv)
  {
    switch (of_meter_band.header.object_id) {
      case OF_METER_BAND_DROP: {
        uint32_t rate, burst;
        of_meter_band_drop_rate_get(&of_meter_band.drop, &rate);
        of_meter_band_drop_burst_size_get(&of_meter_band.drop, &burst);
        LOG_TRACE("meter_band: %d, %d",rate, burst);
        break;
      }
      case OF_METER_BAND_DSCP_REMARK: {
        uint32_t rate, burst;
        uint8_t prec_level;
        of_meter_band_dscp_remark_rate_get(&of_meter_band.dscp_remark, &rate);
        of_meter_band_dscp_remark_burst_size_get(&of_meter_band.dscp_remark, &burst);
        of_meter_band_dscp_remark_prec_level_get(&of_meter_band.dscp_remark, &prec_level);
        LOG_TRACE("meter_band: %d, %d, %d",rate, burst, prec_level);
        break;
      }
      case OF_METER_BAND_OFDPA_COLOR_SET: {
        uint32_t rate, burst;
        uint8_t  mode, color_aware, color;

        if ((flag & OF_METER_FLAG_KBPS) == OF_METER_FLAG_KBPS)
        {
          meter.u.tcmParameters.tcmRateUnit = OFDPA_METER_RATE_KBPS;
        }
        else if ((flag & OF_METER_FLAG_PKTPS) == OF_METER_FLAG_PKTPS)
        {
          meter.u.tcmParameters.tcmRateUnit = OFDPA_METER_RATE_PKTPS;
        }

        of_meter_band_ofdpa_color_set_rate_get(&of_meter_band.ofdpa_color_set, &rate);
        of_meter_band_ofdpa_color_set_burst_size_get(&of_meter_band.ofdpa_color_set, &burst);
        of_meter_band_ofdpa_color_set_mode_get(&of_meter_band.ofdpa_color_set, &mode);
        of_meter_band_ofdpa_color_set_color_aware_get(&of_meter_band.ofdpa_color_set, &color_aware);
        of_meter_band_ofdpa_color_set_color_get(&of_meter_band.ofdpa_color_set, &color);
        LOG_TRACE("meter_band: %d, %d, %d, %d, %d",rate, burst, mode, color_aware, color);

        meter.meterType = OFDPA_METER_TYPE_TCM;
        meter.u.tcmParameters.tcmMode = mode;
        meter.u.tcmParameters.colorAwareMode = color_aware;
        if (color == 1) /* Yellow */
        {
          meter.u.tcmParameters.yellowRate = rate;
          meter.u.tcmParameters.yellowBurst = burst;
        }
        else if (color == 2) /* Red */
        {
          meter.u.tcmParameters.redRate = rate;
          meter.u.tcmParameters.redBurst = burst;
        }

        break;
      }
      default:
          LOG_ERROR("unsupported meter_band %d", of_meter_band.header.object_id);
          return INDIGO_ERROR_COMPAT;
    }

  }

  if (meter.meterType == OFDPA_METER_TYPE_TCM)
  {
    /* Submit the changes to ofdpa */
    ofdpa_rv = ofdpaMeterAdd(id, &meter);
    if (ofdpa_rv != OFDPA_E_NONE)
    {
      LOG_ERROR("Failed to add Meter. (ofdpa_rv = %d)", ofdpa_rv);
    }
    else
    {
      LOG_TRACE("Meter added successfully. (ofdpa_rv = %d)", ofdpa_rv);
    }

    rv = indigoConvertOfdpaRv(ofdpa_rv);
  }

  return err;
}
indigo_error_t indigo_fwd_meter_modify(uint32_t id, uint16_t flag, of_list_meter_band_t *meters)
{
  indigo_error_t err = INDIGO_ERROR_NONE;

  LOG_TRACE("meter_mod: id %d", id);

  err = indigo_fwd_meter_delete(id);
  if (err != INDIGO_ERROR_NONE)
  {
    return err;
  }
  err = indigo_fwd_meter_add(id, flag, meters);
  if (err != INDIGO_ERROR_NONE)
  {
    return err;
  }

  return err;
}
indigo_error_t indigo_fwd_meter_delete(uint32_t id)
{
  OFDPA_ERROR_t ofdpa_rv = OFDPA_E_NONE;

  LOG_TRACE("meter_del: id %d",id);

  /* Submit the changes to ofdpa */
  ofdpa_rv = ofdpaMeterDelete(id);
  if (ofdpa_rv != OFDPA_E_NONE)
  {
    LOG_ERROR("Failed to delete meter. (ofdpa_rv = %d)", ofdpa_rv);
  }
  else
  {
    LOG_TRACE("Meter deleted successfully. (ofdpa_rv = %d)", ofdpa_rv);
  }

  return (indigoConvertOfdpaRv(ofdpa_rv));;
}
#endif
