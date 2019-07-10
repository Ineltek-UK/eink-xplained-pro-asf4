/**
 * \file
 * 
 * \brief UC8151 Eink Display Driver Component
 * 
 * Eink Software Library
 * Microchip ASF4 Variant - release 1.3 - July 2019
 * 
 * \author George Sephton
 * 
 * \copyright Copyright (c) 2019 Ineltek Ltd. All rights reserved.
 * 
 * \page eink_xplained_license License
 *
 * \brief Software License and Disclaimer
 *
 * \section software_license Software License
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * For a copy of the GNU General Public License, see
 * see <https://www.gnu.org/licenses/>
 *
 * \subsection software_license_logo Logo Use
 *
 * Use of the Ineltek logo, packaged with this software library, is not subject
 * to the above GPL software license and is property of Ineltek GmbH and it subsidiaries.
 * It may not be used for commercial use and may not be used to endorse or promote products
 * without specific prior written permission.
 *
 * \subsection software_license_conditions Conditions
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    software license, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    software license, this list of conditions and the following disclaimer
 *    in the documentation and/or other materials provided with the distribution.
 *
 * 3. The names, trademarks or logos of Eink, Ineltek or Microchip, and any combination
 *    thereof, may not be used to endorse or promote products derived from this
 *    software without specific prior written permission. They are also subject to their
 *    own copyrights and trademarks that may not be expressed here.
 *
 * \section disclaimer Disclaimer
 *
 * THIS SOFTWARE IS PROVIDED BY INELTEK "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL INELTEK BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * 
 */
#include <eink/drivers/uc8151/uc8151.h>
 
/**
 * \brief Measures and set the VCOM value.
 *
 * Measure the VCOM voltage from the UC8151's internal mechanism and sets the 
 * VCOM_DC setting.
 */

void uc8151_measure_vcom(void)
{
    uint8_t vcom_value = 0, eink_data[1];
    
    /* Send the 'POWER ON' command */
    eink_write_data(UC8151_PON, 0, 0);
    uc8151_wait_for_busy();

    /*
     * Send the 'Auto Measure VCOM' command
     * AMVT  = 0b00 - 3 seconds
     * XON   = 0    - Gate normally scan during Auto Measure VCOM period
     * AMVS  = 0    - Source output 0V during Auto Measure VCOM period
     * AMV   = 0    - Get VCOM value with the VV command
     * AMVE  = 1    - Trigger auto VCOM sensing
     */ 
    eink_data[0] = 0x01;
    eink_write_data(UC8151_AMV, eink_data, 1);
    uc8151_wait_for_busy();

    /* Send the 'POWER OFF' command */
    eink_write_data(UC8151_POF, 0, 0);

    /* Read the measured VCOM value using 'VV' command */
    eink_read_data(UC8151_VV, &vcom_value);
    
    /* Set the measured VCOM value using 'VDCS' command */
    eink_data[0] = vcom_value;
    eink_write_data(UC8151_VDCS, eink_data, 1);
    uc8151_wait_for_busy();
}

/**
 * \brief Initializes the UC8151 Eink display driver 
 *
 * Call this function to initialize the hardware interface and the UC8151 display
 * driver. When initialization is done the display is ready to receive data.
 */
void uc8151_init(void)
{
    /* Initialize delay routine */
    delay_init(SysTick);

    /* Initialize the interface */
    eink_interface_init(0);

    /* Reset display */
    uc8151_hard_reset();
    
    /* Wait for BUSY output to return HIGH */
    uc8151_wait_for_busy();
}

/**
 * \brief Set the UC8151 configuration registers
 *
 * Sets all the necessary configuration for the UC8151 display driver to communicate
 * with the MCU and sets the necessary variables from the config struct to allow the 
 * display to work.
 *
 * \param *config The configuration to write to the UC8151
 */
void uc8151_set_config(
        const struct uc8151_config *config)
{        
    uint8_t eink_data[5];
    struct uc8151_panel_settings psr_config;

    /* Translate the power settings structure to the required bytes to be sent to the display */
    switch(config->power_settings.source_power) {
        case SOURCE_EXTERNAL_POWER:     eink_data[0] = 0x00; break;
        case SOURCE_INTERNAL_DC_DC:     eink_data[0] = 0x01; break;
    }
    eink_data[0] = eink_data[0] << 1;
    switch(config->power_settings.gate_power) {
        case GATE_EXTERNAL_POWER:       eink_data[0] |= 0x00; break;
        case GATE_INTERNAL_DC_DC:       eink_data[0] |= 0x01; break;
    }
        
    switch(config->power_settings.vcom_voltage_level) {
        case VCOMx_VDx_DC_VCOMD:        eink_data[1] = 0x00; break;
        case VCOMx_VGx:                 eink_data[1] = 0x01; break;
    }
    eink_data[1] = eink_data[1] << 2;
    switch (config->power_settings.vg_voltage_level) {
        case VGx_16V:                   eink_data[1] |= 0x00; break;
        case VGx_15V:                   eink_data[1] |= 0x01; break;
        case VGx_14V:                   eink_data[1] |= 0x02; break;
        case VGx_13V:                   eink_data[1] |= 0x03; break;
    }
    
    if( (config->power_settings.vg_voltage_level == VGx_17V) | (config->power_settings.vg_voltage_level == VGx_18V) | (config->power_settings.vg_voltage_level == VGx_19V) | (config->power_settings.vg_voltage_level == VGx_20V) )
    {
        //Error, invalid Internal VGH/VGL voltage level selection.
        while(1);   
    }
        
    eink_data[2] = config->power_settings.pwr_vdh_level;
    eink_data[3] = config->power_settings.pwr_vdl_level;
    eink_data[4] = config->power_settings.pwr_vdhr_level;
    /* Send the PWR register to the display */
    eink_write_data(UC8151_PWR, eink_data, 5);

    psr_config = config->panel_settings;
    uc8151_set_psr_config(&psr_config);
    
    /* Send the 'POWER ON' command */
    eink_write_data(UC8151_PON, 0, 0);
    /* Wait for voltage levels to stabilize */
    uc8151_wait_for_busy();

    /* Translate the booster soft start structure (phase A) to the required bytes to be sent to the display */
    switch (config->booster_soft_start.a_soft_start_period) {
        case SS_PERIOD_10MS:            eink_data[0] = 0x00; break;
        case SS_PERIOD_20MS:            eink_data[0] = 0x01; break;
        case SS_PERIOD_30MS:            eink_data[0] = 0x02; break;
        case SS_PERIOD_40MS:            eink_data[0] = 0x03; break;
    }
    eink_data[0] = eink_data[0] << 3;
    switch (config->booster_soft_start.a_drive_strength) {
        case STRENGTH_1:                eink_data[0] |= 0x00; break;
        case STRENGTH_2:                eink_data[0] |= 0x01; break;
        case STRENGTH_3:                eink_data[0] |= 0x02; break;
        case STRENGTH_4:                eink_data[0] |= 0x03; break;
        case STRENGTH_5:                eink_data[0] |= 0x04; break;
        case STRENGTH_6:                eink_data[0] |= 0x05; break;
        case STRENGTH_7:                eink_data[0] |= 0x06; break;
        case STRENGTH_8:                eink_data[0] |= 0x07; break;
    }
    eink_data[0] = eink_data[0] << 3;
    switch (config->booster_soft_start.a_min_off_time) {
        case MIN_OFF_TIME_270_NS:       eink_data[0] |= 0x00; break;
        case MIN_OFF_TIME_340_NS:       eink_data[0] |= 0x01; break;
        case MIN_OFF_TIME_400_NS:       eink_data[0] |= 0x02; break;
        case MIN_OFF_TIME_540_NS:       eink_data[0] |= 0x03; break;
        case MIN_OFF_TIME_800_NS:       eink_data[0] |= 0x04; break;
        case MIN_OFF_TIME_1540_NS:      eink_data[0] |= 0x05; break;
        case MIN_OFF_TIME_3340_NS:      eink_data[0] |= 0x06; break;
        case MIN_OFF_TIME_6580_NS:      eink_data[0] |= 0x07; break;
    }
        
    /* Translate the booster soft start structure (phase B) to the required bytes to be sent to the display */
    switch (config->booster_soft_start.a_soft_start_period) {
        case SS_PERIOD_10MS:            eink_data[1] = 0x00; break;
        case SS_PERIOD_20MS:            eink_data[1] = 0x01; break;
        case SS_PERIOD_30MS:            eink_data[1] = 0x02; break;
        case SS_PERIOD_40MS:            eink_data[1] = 0x03; break;
    }
    eink_data[1] = eink_data[1] << 3;
    switch (config->booster_soft_start.a_drive_strength) {
        case STRENGTH_1:                eink_data[1] |= 0x00; break;
        case STRENGTH_2:                eink_data[1] |= 0x01; break;
        case STRENGTH_3:                eink_data[1] |= 0x02; break;
        case STRENGTH_4:                eink_data[1] |= 0x03; break;
        case STRENGTH_5:                eink_data[1] |= 0x04; break;
        case STRENGTH_6:                eink_data[1] |= 0x05; break;
        case STRENGTH_7:                eink_data[1] |= 0x06; break;
        case STRENGTH_8:                eink_data[1] |= 0x07; break;
    }
    eink_data[1] = eink_data[1] << 3;
    switch (config->booster_soft_start.a_min_off_time) {
        case MIN_OFF_TIME_270_NS:       eink_data[1] |= 0x00; break;
        case MIN_OFF_TIME_340_NS:       eink_data[1] |= 0x01; break;
        case MIN_OFF_TIME_400_NS:       eink_data[1] |= 0x02; break;
        case MIN_OFF_TIME_540_NS:       eink_data[1] |= 0x03; break;
        case MIN_OFF_TIME_800_NS:       eink_data[1] |= 0x04; break;
        case MIN_OFF_TIME_1540_NS:      eink_data[1] |= 0x05; break;
        case MIN_OFF_TIME_3340_NS:      eink_data[1] |= 0x06; break;
        case MIN_OFF_TIME_6580_NS:      eink_data[1] |= 0x07; break;
    }
    
    /* Translate the booster soft start structure (phase C) to the required bytes to be sent to the display */
    switch (config->booster_soft_start.a_drive_strength) {
        case STRENGTH_1:                eink_data[2] = 0x00; break;
        case STRENGTH_2:                eink_data[2] = 0x01; break;
        case STRENGTH_3:                eink_data[2] = 0x02; break;
        case STRENGTH_4:                eink_data[2] = 0x03; break;
        case STRENGTH_5:                eink_data[2] = 0x04; break;
        case STRENGTH_6:                eink_data[2] = 0x05; break;
        case STRENGTH_7:                eink_data[2] = 0x06; break;
        case STRENGTH_8:                eink_data[2] = 0x07; break;
    }
    eink_data[2] = eink_data[2] << 3;
    switch (config->booster_soft_start.a_min_off_time) {
        case MIN_OFF_TIME_270_NS:       eink_data[2] |= 0x00; break;
        case MIN_OFF_TIME_340_NS:       eink_data[2] |= 0x01; break;
        case MIN_OFF_TIME_400_NS:       eink_data[2] |= 0x02; break;
        case MIN_OFF_TIME_540_NS:       eink_data[2] |= 0x03; break;
        case MIN_OFF_TIME_800_NS:       eink_data[2] |= 0x04; break;
        case MIN_OFF_TIME_1540_NS:      eink_data[2] |= 0x05; break;
        case MIN_OFF_TIME_3340_NS:      eink_data[2] |= 0x06; break;
        case MIN_OFF_TIME_6580_NS:      eink_data[2] |= 0x07; break;
    }
    /* Send the BTST register to the display */
    eink_write_data(UC8151_BTST, eink_data, 3);
    
    /* Translate the power off sequence structure to the required byte to be sent to the display */
    switch (config->power_off_sequence) {
        case PFS_1_FRAME:               eink_data[0] = 0x00; break;
        case PFS_2_FRAMES:              eink_data[0] = 0x10; break;
        case PFS_3_FRAMES:              eink_data[0] = 0x20; break;
        case PFS_4_FRAMES:              eink_data[0] = 0x30; break;
    }
    /* Send the PFS register to the display */
    eink_write_data(UC8151_PFS, eink_data, 1);
    
    /* Translate the temperature sensor enable structure to the required byte to be sent to the display */
    switch (config->temperature_sensor_settings.sensor_select) {
        case INTERNAL_TEMPERATURE_SENSOR: eink_data[0] = 0x0; break;
        case EXTERNAL_TEMPERATURE_SENSOR: eink_data[0] = 0x1; break;
    }
    eink_data[0] = eink_data[0] << 4;
    /* Parse 8-bit signed integer into 4-bit signed integer for temperature offset */
    if (config->temperature_sensor_settings.temperature_offset > 7) {
        eink_data[0] |= 0x7;
    } else if((config->temperature_sensor_settings.temperature_offset <= 7) && (config->temperature_sensor_settings.temperature_offset >= 0)) {
        eink_data[0] |= config->temperature_sensor_settings.temperature_offset;
    } else if(config->temperature_sensor_settings.temperature_offset < -8) {
        eink_data[0] |= 0x8;
    } else {
        /* Convert negative numbers to 4-bit signed integer */
        eink_data[0] |= (config->temperature_sensor_settings.temperature_offset & 0x0F);
    }
    /* Send the TSE register to the display */
    eink_write_data(UC8151_TSE, eink_data, 1);

    eink_data[0] = 0x77;
    eink_write_data(UC8151_CDI, eink_data, 1);

    eink_data[0] = 0x22;
    eink_write_data(UC8151_TCON, eink_data, 1);

    /* Set the PLL control register - Frame rate 50Hz */
    eink_data[0] = 0x3C;
    eink_write_data(UC8151_CDI, eink_data, 1);

    eink_write_data(UC8151_POF, 0, 0);
    uc8151_wait_for_busy();
}

/**
 * \brief Set the UC8151 Panel Settings Register (PSR).
 *
 * Sets the correct values of the display so the UC8151 is set up to 
 * talk to the correct display. Sets the color, rotation, resolution and 
 * tells the controller which waveforms to use.
 *
 * \param *psr_config The PSR structure to write to the PSR register.
 */
void uc8151_set_psr_config(
        const struct uc8151_panel_settings *psr_config)
{    
    /* Copy configuration into UC8151 instance */
    uc8151_global_instance.panel_settings = *psr_config;
    
    /* Save display resolution into UC8151 instance */
    switch (psr_config->display_resolution) {
        case RES_96x230:
            if((psr_config->display_rotation == ROTATE_90) || (psr_config->display_rotation == ROTATE_270)) {
                uc8151_global_instance.display_width   = 96;
                uc8151_global_instance.display_height  = 230;
            } else {
                uc8151_global_instance.display_width   = 230;
                uc8151_global_instance.display_height  = 96;
            }
            break;
        case RES_96x252:
            if((psr_config->display_rotation == ROTATE_90) || (psr_config->display_rotation == ROTATE_270)) {
                uc8151_global_instance.display_width   = 96;
                uc8151_global_instance.display_height  = 252;
            } else {
                uc8151_global_instance.display_width   = 252;
                uc8151_global_instance.display_height  = 96;
            }
            break;
        case RES_128x296:
            if((psr_config->display_rotation == ROTATE_90) || (psr_config->display_rotation == ROTATE_270)) {
                uc8151_global_instance.display_width   = 128;
                uc8151_global_instance.display_height  = 296;
            } else {
                uc8151_global_instance.display_width   = 296;
                uc8151_global_instance.display_height  = 128;
            }
            break;
        case RES_160x296:
            if((psr_config->display_rotation == ROTATE_90) || (psr_config->display_rotation == ROTATE_270)) {
                uc8151_global_instance.display_width   = 160;
                uc8151_global_instance.display_height  = 296;
            } else {
                uc8151_global_instance.display_width   = 296;
                uc8151_global_instance.display_height  = 160;
            }
            break;
    }
    
    uint8_t eink_data[5];

    /* Translate the panel settings structure to the required bytes to be sent to the display */
    switch (psr_config->display_resolution) {
        case RES_96x230:                eink_data[0] = 0x00; break;
        case RES_96x252:                eink_data[0] = 0x01; break;
        case RES_128x296:               eink_data[0] = 0x02; break;
        case RES_160x296:               eink_data[0] = 0x03; break;
    }
    eink_data[0] = eink_data[0] << 1;
    switch (psr_config->lut_selection) {
        case LUT_FROM_OTP:              eink_data[0] |= 0x00; break;
        case LUT_FROM_REGISTER:         eink_data[0] |= 0x01; break;
    }
    eink_data[0] = eink_data[0] << 1;
    switch (psr_config->display_colours) {
        case B_W_R_PIXELS:              eink_data[0] |= 0x00; break;
        case B_W_PIXELS:                eink_data[0] |= 0x01; break;
    }
    eink_data[0] = eink_data[0] << 2;
    switch (psr_config->display_rotation) {
        case ROTATE_0:
        case ROTATE_270:                 eink_data[0] |= 0b00; break;
        case ROTATE_180:
        case ROTATE_90:                  eink_data[0] |= 0b11; break;
    }
    eink_data[0] = eink_data[0] << 1;
    switch (psr_config->booster_switch) {
        case BOOSTER_OFF:               eink_data[0] |= 0x00; break;
        case BOOSTER_ON:                eink_data[0] |= 0x01; break;
    }
    eink_data[0] = eink_data[0] << 1;
    /* Add the soft reset bit */
    eink_data[0] |= 0x01;
    /* Send the PSR register to the display */
    eink_write_data(UC8151_PSR, eink_data, 1);
}