/****************************************************************************
 * apps/system/uorb/sensor/adc.c
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <sensor/adc.h>

/****************************************************************************
 * Private Functions
 ****************************************************************************/

#ifdef CONFIG_DEBUG_UORB
static void print_sensor_adc_raw_message(FAR const struct orb_metadata *meta,
                                     FAR const void *buffer)
{
  FAR const struct ads7953_raw_msg *message = buffer;
  const orb_abstime now = orb_absolute_time();

  uorbinfo_raw("%s:\ttimestamp: %" PRIu64 " (%" PRIu64 " us ago) ",
               meta->o_name, message->timestamp, now - message->timestamp);
  for(int i =0; i < 8; i++)
  {
    uorbinfo_raw("Temp Chan%i: %i; ", i, message->temp_chan[i]); /* Printed temperature channel raw data */
  }
  for(int i = 0; i < 7; i++)
  {
    uorbinfo_raw("Volt Chan%i: %i", i, message->volts_chan[i]);
  }
}

static void print_sat_temperature_message(FAR const struct orb_metadata *meta,
                                     FAR const void *buffer)
{
  FAR const struct sat_temp_msg *message = buffer;
  const orb_abstime now = orb_absolute_time();

  uorbinfo_raw("%s:\ttimestamp: %" PRIu64 " (%" PRIu64 " us ago) ",
               meta->o_name, message->timestamp, now - message->timestamp);
  uorbinfo_raw("BPB temp: %0.3f "
               "Batt Temp: %0.3f "
               "Ant Temp: %0.3f "
               "+Z Temp: %0.3f "
               "Temp_2: %0.3f "
               "Temp_3: %0.3f "
               "Temp_4: %0.3f "
               "Temp_5: %0.3f ",
               message->temp_bpb, message->batt_temp, message->temp_ant,
               message->temp_z_pos, message->temp_2, message->temp_3,
               message->temp_4, message->temp_5);
}

static void print_sat_voltage_message(FAR const struct orb_metadata *meta,
                                     FAR const void *buffer)
{
  FAR const struct sat_volts_msg *message = buffer;
  const orb_abstime now = orb_absolute_time();

  uorbinfo_raw("%s:\ttimestamp: %" PRIu64 " (%" PRIu64 " us ago) ",
               meta->o_name, message->timestamp, now - message->timestamp);
  uorbinfo_raw("sp1 Volts: %0.3f "
               "sp2 Volts: %0.3f "
               "sp3 Volts: %0.3f "
               "sp4 Volts: %0.3f "
               "sp5 Volts: %0.3f "
               "Raw Volts: %0.3f "
               "SolT Volts: %0.3f ",
               message->volt_sp1, message->volt_sp2, message->volt_sp3,
               message->volt_sp4, message->volt_sp5, message->volt_raw,
               message->volt_SolT);
}

static void print_sat_int_adc_message(FAR const struct orb_metadata *meta,
                                      FAR const void *buffer)
{
  FAR const struct sat_int_adc_msg *message = buffer;
  const orb_abstime now = orb_absolute_time();

  uorbinfo_raw("%s:\ttimestamp: %" PRIu64" (%"PRIu64 " us ago) ",
                meta->o_name, message->timestamp, now - message->timestamp);
}
#endif

/****************************************************************************
 * Public Data
 ****************************************************************************/

ORB_DEFINE(ads7953_raw_msg, struct ads7953_raw_msg, print_sensor_adc_raw_message);
ORB_DEFINE(sat_temp_msg, struct sat_temp_msg, print_sat_temperature_message);
ORB_DEFINE(sat_volts_msg, struct sat_volts_msg, print_sat_voltage_message);
ORB_DEFINE(sat_int_adc_msg, struct sat_int_adc_msg, print_sat_int_adc_message);
