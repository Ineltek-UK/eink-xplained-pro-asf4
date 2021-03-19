/**
 * \file
 * 
 * \brief SSD1677 Eink Display Driver Component
 * 
 * Eink Software Library
 * Microchip ASF4 Variant - release 1.6 - October 2020
 * 
 * \author George Sephton
 * 
 * \copyright Copyright (c) 2020 Ineltek Ltd. All rights reserved.
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
#include <eink/drivers/ssd1677/ssd1677.h>

/**
 * \brief Initializes the SSD1677 Eink display driver 
 *
 * Call this function to initialize the hardware interface and the SSD1677 display
 * driver. When initialization is done the display is ready to receive data.
 */
void ssd1677_init(void)
{
    /* Initialize delay routine */
    delay_init(SysTick);

    /* Initialize the interface */
    eink_interface_init(1);

    /* Reset display */
    ssd1677_hard_reset();

    /* Wait for BUSY output to return HIGH */
    ssd1677_wait_for_busy();

    /* Software Reset */
    eink_write_data(SSD1677_SWRESET, 0, 0);

    /* Wait for BUSY output to return LOW */
    ssd1677_wait_for_busy();
}

/**
 * \brief Set the SSD1677 configuration registers
 *
 * Sets all the necessary configuration for the SSD1677 display driver to communicate
 * with the MCU and sets the necessary variables from the config struct to allow the 
 * display to work.
 *
 * \param *config The configuration to write to the SSD1677
 */
void ssd1677_set_config(
        const struct ssd1677_config *config)
{
    uint8_t eink_data[10];
    
    eink_data[0] = 0xF7;
    eink_write_data(SSD1677_AUTO_RAM_RED, eink_data, 1);
    ssd1677_wait_for_busy();
    
    eink_data[0] = 0xF7;
    eink_write_data(SSD1677_AUTO_RAM_BW, eink_data, 1);
    ssd1677_wait_for_busy();
    
    //Gate Settings
    eink_data[0] = 0xDF;
    eink_data[1] = 0x01;
    eink_data[2] = 0x00;
    eink_write_data(SSD1677_GATE_SET, eink_data, 3);
    
    //Set Gate Voltage
    eink_data[0] = 0x00;
    eink_write_data(SSD1677_VGH, eink_data, 1);
    
    //Set Source Voltage
    eink_data[0] = 0x41;
    eink_data[1] = 0xA8;
    eink_data[2] = 0x32;
    eink_write_data(SSD1677_VSHL, eink_data, 3);
    
    //Data Entry Sequence
    eink_data[0] = 0x03;
    eink_write_data(SSD1677_DEY, eink_data, 1);
    
    //Border Settings
    eink_data[0] = 0x00;
    eink_write_data(SSD1677_BDW, eink_data, 1);

    //Booster Strength
    eink_data[0] = 0xAE;
    eink_data[1] = 0xC7;
    eink_data[2] = 0xC3;
    eink_data[3] = 0xC0;
    eink_data[4] = 0xC0;
    eink_write_data(SSD1677_BTST, eink_data, 5);
    
    //Temperature Settings
    eink_data[0] = 0x80;
    eink_write_data(SSD1677_TSC, eink_data, 1);
    
    //Set VCOM Value
    eink_data[0] = 0x44;
    eink_write_data(SSD1677_VCOM_WT, eink_data, 1);
    
    //Display Options
    /*eink_data[0] = 0x00;
    eink_data[1] = 0xFF;
    eink_data[2] = 0xFF;
    eink_data[3] = 0xFF;
    eink_data[4] = 0xFF;
    eink_data[5] = 0x4F;
    eink_data[6] = 0xFF;
    eink_data[7] = 0xFF;
    eink_data[8] = 0xFF;
    eink_data[9] = 0xFF;*/
	eink_data[0] = 0x00;
	eink_data[1] = 0x00;
	eink_data[2] = 0x00;
	eink_data[3] = 0x00;
	eink_data[4] = 0x00;
	eink_data[5] = 0x00;
	eink_data[6] = 0x00;
	eink_data[7] = 0x00;
	eink_data[8] = 0x00;
	eink_data[9] = 0x00;
    eink_write_data(SSD1677_DIS_OPT, eink_data, 10);

    //X RAM Start/End Position
    eink_data[0] = 0x00;
    eink_data[1] = 0x00;
    eink_data[2] = 0x17;
    eink_data[3] = 0x01;
    eink_write_data(SSD1677_X_ADDR, eink_data, 4);

    //Y RAM Start/End Position
    eink_data[0] = 0x00;
    eink_data[1] = 0x00;
    eink_data[2] = 0xDF;
    eink_data[3] = 0x01;
    eink_write_data(SSD1677_Y_ADDR, eink_data, 4);

    eink_data[0] = 0x00;
    eink_data[1] = 0x00;
    eink_write_data(SSD1677_X_ADDRC, eink_data, 2);

    eink_data[0] = 0x00;
    eink_data[1] = 0x00;
    eink_write_data(SSD1677_Y_ADDRC, eink_data, 2);

    /* Copy configuration into UC8151 instance */
    ssd1677_global_instance.panel_settings = *config;
    
    /* Save display resolution into UC8151 instance */
    if((config->display_rotation == ROTATE_90) || (config->display_rotation == ROTATE_270)) {
        ssd1677_global_instance.display_width   = 480;
        ssd1677_global_instance.display_height  = 280;
    } else {
        ssd1677_global_instance.display_width   = 280;
        ssd1677_global_instance.display_height  = 480;
    }
}