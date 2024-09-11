/****************************************************************************
 * apps/system/uorb/sensor/adc.h
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

#ifndef __APPS_SYSTEM_UORB_SENSOR_ADC_H
#define __APPS_SYSTEM_UORB_SENSOR_ADC_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <uORB/uORB.h>
#include <nuttx/analog/adc.h>

struct ads7953_raw_msg
{
  uint64_t timestamp;
  uint16_t temp_chan[8];     // Temperature sensor channels data, ADC raw
  float temp_chan_volts[8];  // Temperature sensor channels data, volts
  uint16_t volts_chan[7];    // Satellite voltage channels data, ADC Raw
  float volts_chan_volts[7]; // satellite voltage channels in volts
  uint32_t status;
};

struct sat_temp_msg
{
  uint64_t timestamp;
  float batt_temp;
  float temp_bpb;
  float temp_ant;
  float temp_z_pos;
  float temp_5;
  float temp_4;
  float temp_3;
  float temp_2;
  uint32_t status;
};

struct sat_volts_msg
{
  uint64_t timestamp;
  float volt_sp1;
  float volt_sp2;
  float volt_sp3;
  float volt_sp4;
  float volt_sp5;
  float volt_raw;
  float volt_SolT;
  uint32_t status;
};

struct sat_int_adc_msg
{
  uint64_t timestamp;
  float C_batt;
  float C_SolT;
  float C_3v3_main;
  float C_3v3_com;
  float C_5v;
  float C_sp1;
  float C_sp2;
  float C_sp3;
  float C_sp4;
  float C_sp5;
  float C_3v3_2;
  float C_unreg;
  float C_raw;
  float C_4v;

  float volt_batt;
  uint32_t status;

};

/****************************************************************************
 * Public Data
 ****************************************************************************/

/* register this as object request broker structure */

ORB_DECLARE(ads7953_raw_msg);
ORB_DECLARE(sat_temp_msg);
ORB_DECLARE(sat_volts_msg);
ORB_DECLARE(sat_int_adc_msg);

#endif
