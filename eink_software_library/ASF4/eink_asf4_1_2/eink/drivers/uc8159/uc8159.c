/**
 * \file
 * 
 * \brief UC8159 Eink Display Driver Component
 * 
 * Eink Software Library
 * Microchip ASF4 Variant - release 1.2 - January 2019
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
#include <eink/drivers/uc8159/uc8159.h>

/**
 * \brief Measures and set the VCOM value.
 *
 * Measure the VCOM voltage from the UC8159's internal mechanism and sets the 
 * VCOM_DC setting.
 */

void uc8159_measure_vcom(void)
{
    uint8_t vcom_value = 0, eink_data[1], i;
#ifdef EINK_MFCSB
#ifdef EINK_FMSDO
    /* Send the 'POWER ON' command */
    eink_write_data(UC8159_PON, 0, 0);
    uc8159_wait_for_busy();

    /*
     * Send the 'Auto Measure VCOM' command
     * AMVT  = 0b00 - 3 seconds
     * AMVX  = 0    - Measure VCOM without XON function.
     * AMVS  = 0    - Source output 0V during Auto Measure VCOM period
     * AMV   = 0    - Get VCOM value with the VV command
     * AMVE  = 1    - Trigger auto VCOM sensing
     */ 
    //eink_data[0] = 0x01;
    //eink_write_data(UC8159_AMV, eink_data, 1);
    //uc8159_wait_for_busy();

    /* Read the measured VCOM value using 'VV' command */
    //eink_read_data(UC8159_VV, &vcom_value);
    
    /* Read VCOM value from on-board flash address 0x00006400 */
    eink_data[0] = 0x01;
    eink_write_data(UC8159_DAM, eink_data, 1);
    
    vcom_value = 0;
    /* Set MFCSB LOW to enable communication to the flash */
    gpio_set_pin_level(EINK_MFCSB, 0);
    
    eink_send_raw_byte(0x03, 1);
    
    /* Send the read address */
    eink_send_raw_byte(0x00, 1);
    eink_send_raw_byte(0x64, 1);
    eink_send_raw_byte(0x00, 1);
    
    /* Read the returned data */
    for (i=0;i<8;i++) {
        gpio_set_pin_level(EINK_SPI_SCL, 1);
        if (gpio_get_pin_level(EINK_FMSDO)) {
            vcom_value |= 1;
        }
        gpio_set_pin_level(EINK_SPI_SCL, 0);
        if(i!=7) vcom_value = vcom_value << 1;
    }
    
    /* Set MFCSB HIGH to end communication to the flash */
    gpio_set_pin_level(EINK_MFCSB, 1);
    /* Disable communication to on-board flash */
    eink_data[0] = 0x00;
    eink_write_data(UC8159_DAM, eink_data, 1);
    
    /* Set the measured VCOM value using 'VDCS' command */
    eink_data[0] = vcom_value;
    eink_write_data(UC8159_VDCS, eink_data, 1);

    /* Send the 'POWER OFF' command */
    eink_write_data(UC8159_POF, 0, 0);
    uc8159_wait_for_busy();
#endif
#endif
}

/**
 * \brief Initializes the UC8159 Eink display driver 
 *
 * Call this function to initialize the hardware interface and the UC8159 display
 * driver. When initialization is done the display is ready to receive data.
 */
void uc8159_init(void)
{
    /* Initialize delay routine */
    delay_init(SysTick);

    /* Initialize the interface */
    eink_interface_init(1);

    /* Reset display */
    uc8159_hard_reset();
    
    /* Wait for BUSY output to return HIGH */
    uc8159_wait_for_busy();
}

/**
 * \brief Set the UC8159 configuration registers
 *
 * Sets all the necessary configuration for the UC8159 display driver to communicate
 * with the MCU and sets the necessary variables from the config struct to allow the 
 * display to work.
 *
 * \param *config The configuration to write to the UC8159
 */
void uc8159_set_config(
        const struct uc8159_config *config)
{
    uint8_t eink_data[5];
    struct uc8159_panel_settings psr_config;

    psr_config = config->panel_settings;
    uc8159_set_psr_config(&psr_config);

    eink_data[0] = 0x06;
    eink_data[0] = eink_data[0] << 1;
    
    /* Translate the power settings structure to the required bytes to be sent to the display */
    switch(config->power_settings.source_power) {
        case SOURCE_EXTERNAL_POWER:     eink_data[0] |= 0x00; break;
        case SOURCE_INTERNAL_DC_DC:     eink_data[0] |= 0x01; break;
    }
    eink_data[0] = eink_data[0] << 1;
    switch(config->power_settings.gate_power) {
        case GATE_EXTERNAL_POWER:       eink_data[0] |= 0x00; break;
        case GATE_INTERNAL_DC_DC:       eink_data[0] |= 0x01; break;
    }
    eink_data[0] = eink_data[0] << 1;
    switch(config->power_settings.lv_source_power) {
        case LV_SOURCE_EXTERNAL_POWER:  eink_data[0] |= 0x00; break;
        case LV_SOURCE_INTERNAL_DC_DC:  eink_data[0] |= 0x01; break;
    }
    
    switch (config->power_settings.vg_voltage_level) {
        case VGx_20V:                   eink_data[1] = 0x00; break;
        case VGx_19V:                   eink_data[1] = 0x01; break;
        case VGx_18V:                   eink_data[1] = 0x02; break;
        case VGx_17V:                   eink_data[1] = 0x03; break;
    }
    
    if( (config->power_settings.vg_voltage_level == VGx_16V) | (config->power_settings.vg_voltage_level == VGx_15V) | (config->power_settings.vg_voltage_level == VGx_14V) | (config->power_settings.vg_voltage_level == VGx_13V) )
    {
        //Error, invalid Internal VGH/VGL voltage level selection.
        while(1);   
    }
        
    eink_data[2] = 0x05;
    eink_data[3] = 0x05;
    /* Send the PWR register to the display */
    eink_write_data(UC8159_PWR, eink_data, 4);
    
    eink_data[0] = 0x03;
    eink_write_data(UC8159_TSSET, eink_data, 1);
    
    /* Translate the power off sequence structure to the required byte to be sent to the display */
    switch (config->power_off_sequence) {
        case PFS_1_FRAME:               eink_data[0] = 0x00; break;
        case PFS_2_FRAMES:              eink_data[0] = 0x10; break;
        case PFS_3_FRAMES:              eink_data[0] = 0x20; break;
        case PFS_4_FRAMES:              eink_data[0] = 0x30; break;
    }
    /* Send the PFS register to the display */
    eink_write_data(UC8159_PFS, eink_data, 1);
    
    /* Send the default booster soft start structure (phase A) to the display */
    eink_data[0] = 0xC7;
    /* Send the default booster soft start structure (phase B) to the display */
    eink_data[1] = 0xCC;
    /* Send the default booster soft start structure (phase C) to the display */
    eink_data[2] = 0x28;
    /* Send the BTST register to the display */
    eink_write_data(UC8159_BTST, eink_data, 3);
    
    /* Set the PLL clock frequency to 50Hz */
    eink_data[0] = 0x3C;
    /* Send the BTST register to the display */
    eink_write_data(UC8159_PLL, eink_data, 3);
    
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
    eink_write_data(UC8159_TSE, eink_data, 1);
    
    eink_data[0] = 0x37;
    eink_write_data(UC8159_CDI, eink_data, 1);
    
    eink_data[0] = 0x22;
    eink_write_data(UC8159_TCON, eink_data, 1);
    
    /* Disable external flash */
    eink_data[0] = 0x00;
    eink_write_data(UC8159_DAM, eink_data, 1);
    
    uc8159_measure_vcom();
}

/**
 * \brief Set the UC8159 Panel Settings Register (PSR).
 *
 * Sets the correct values of the display so the UC8159 is set up to 
 * talk to the correct display. Sets the rotation and resolution and 
 * tells the controller which waveforms to use.
 *
 * \param *psr_config The PSR structure to write to the PSR register.
 */
void uc8159_set_psr_config(
        const struct uc8159_panel_settings *psr_config)
{
    eink_x_coordinate uc8159_display_width;
    eink_y_coordinate uc8159_display_height;
    
    /* Copy configuration into UC8159 instance */
    uc8159_global_instance.panel_settings = *psr_config;
    
    /* Save display resolution into UC8159 instance */
    switch (psr_config->display_resolution) {
        case RES_640x480:
            if((psr_config->display_rotation == ROTATE_90) || (psr_config->display_rotation == ROTATE_270)) {
                uc8159_global_instance.display_width   = 480;
                uc8159_global_instance.display_height  = 640;
            } else {
                uc8159_global_instance.display_width   = 640;
                uc8159_global_instance.display_height  = 480;
            }
            uc8159_display_width  = 640;
            uc8159_display_height = 480;
            break;
        case RES_600x450:
            if((psr_config->display_rotation == ROTATE_90) || (psr_config->display_rotation == ROTATE_270)) {
                uc8159_global_instance.display_width   = 450;
                uc8159_global_instance.display_height  = 600;
            } else {
                uc8159_global_instance.display_width   = 600;
                uc8159_global_instance.display_height  = 450;
            }
            uc8159_display_width  = 600;
            uc8159_display_height = 450;
            break;
        case RES_640x448:
            if((psr_config->display_rotation == ROTATE_90) || (psr_config->display_rotation == ROTATE_270)) {
                uc8159_global_instance.display_width   = 448;
                uc8159_global_instance.display_height  = 640;
            } else {
                uc8159_global_instance.display_width   = 640;
                uc8159_global_instance.display_height  = 448;
            }
            uc8159_display_width  = 640;
            uc8159_display_height = 448;
            break;
        case RES_600x448:
            if((psr_config->display_rotation == ROTATE_90) || (psr_config->display_rotation == ROTATE_270)) {
                uc8159_global_instance.display_width   = 448;
                uc8159_global_instance.display_height  = 600;
            } else {
                uc8159_global_instance.display_width   = 600;
                uc8159_global_instance.display_height  = 448;
            }
            uc8159_display_width  = 600;
            uc8159_display_height = 448;
            break;
    }
    
    uint8_t eink_data[5];

    /* Set the TRES register to the display */
    eink_data[0] = ((uc8159_display_width >> 8) & 0x03);
    eink_data[1] = (uc8159_display_width & 0xFF);
    eink_data[2] = ((uc8159_display_height >> 8) & 0x03);
    eink_data[3] = (uc8159_display_height & 0xFF);
    eink_write_data(UC8159_TRES, eink_data, 4);

    /* Translate the panel settings structure to the required bytes to be sent to the display */
    switch (psr_config->display_resolution) {
        case RES_640x480:               eink_data[0] = 0x00; break;
        case RES_600x450:               eink_data[0] = 0x01; break;
        case RES_640x448:               eink_data[0] = 0x02; break;
        case RES_600x448:               eink_data[0] = 0x03; break;
    }
    eink_data[0] = eink_data[0] << 4;

    switch (psr_config->display_rotation) {
        case ROTATE_0:                  eink_data[0] |= 0b11; break;
        case ROTATE_90:                 eink_data[0] |= 0b10; break;
        case ROTATE_180:                eink_data[0] |= 0b00; break;
        case ROTATE_270:                eink_data[0] |= 0b01; break;
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
    eink_write_data(UC8159_PSR, eink_data, 1);
}