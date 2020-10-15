/**
 * \file
 * 
 * \brief UC8177 Eink Display Driver Component
 * 
 * Eink Software Library
 * Microchip ASF4 Variant - release 1.6 - July 2020
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
#include <eink/drivers/uc8177/uc8177.h>

/**
 * \brief Initializes the UC8177 Eink display driver.
 *
 * Call this function to initialize the hardware interface and the UC8177 display
 * driver. When initialization is done the display is ready to receive data.
 */
void uc8177_init(void)
{
    /* Initialize delay routine */
    delay_init(SysTick);

    /* Initialize the interface */
    eink_interface_init(0);

    /* Reset display */
    uc8177_hard_reset();
    
    /* Wait for BUSY output to return HIGH */
    uc8177_wait_for_busy_low();
}

/**
 * \brief Set the UC8177 configuration registers.
 *
 * Sets all the necessary configuration for the UC8177 Eink display driver to communicate
 * with the MCU and sets the necessary variables from the config struct to allow the 
 * display to work.
 *
 * \param *config The configuration to write to the display driver.
 */
void uc8177_set_config(
        struct uc8177_config *const config)
{
    uint8_t eink_data[5];
    
    uc8177_global_instance.display_config = *config;
    
    /* Power Settings */
    eink_data[0] = 0x03; /* Internal DC/DC function for Gate/Source Power */
    eink_data[1] = 0x03; /* Gate Voltage level +-20V */
    eink_data[2] = 0x00; /* 0x00 2.4V */
    eink_data[3] = 0x00; /* VSH = 15V, VSL = 15V */
    eink_write_data(UC8177_PWR, eink_data, 4);
    
    /* Panel Settings */
    if(config->display_rotation == ROTATE_270) {
        uc8177_global_instance.display_width   = 600;
        uc8177_global_instance.display_height  = 480;
        eink_data[0] = 0x21; //Rotate 270
    } else if(config->display_rotation == ROTATE_180) {
        uc8177_global_instance.display_width   = 480;
        uc8177_global_instance.display_height  = 600;
        eink_data[0] = 0x21; //Rotate 180
    } else if(config->display_rotation == ROTATE_90) {
        uc8177_global_instance.display_width   = 600;
        uc8177_global_instance.display_height  = 480;
        eink_data[0] = 0x25; //Rotate 90
    } else {
        uc8177_global_instance.display_width   = 480;
        uc8177_global_instance.display_height  = 600;
        eink_data[0] = 0x25; //Rotate 0
    }
    eink_data[1] = 0x00;
    eink_write_data(UC8177_PSR, eink_data, 2);
    
    eink_data[0] = 0x82;
    eink_write_data(0x26, eink_data, 1);
    
    /* Power OFF Sequence Settings */
    eink_data[0] = 0x03; /* 4 Frames */
    eink_write_data(UC8177_PFS, eink_data, 1);
    
    /* Booster Soft Start Control */
    eink_data[0] = 0xEF;
    eink_data[1] = 0xEF;
    eink_data[2] = 0x28;
    eink_write_data(UC8177_BTST, eink_data, 3);
    
    /* GD Order Settings */
    //eink_data[0] = 0x02;
    //eink_write_data(UC8177_GDOS, eink_data, 1);
    
    /* PLL Control */
    eink_data[0] = 0x0E; /* 20 MHz */
    eink_write_data(UC8177_PLL, eink_data, 1);
    
    /* Temperature Sensor Enable */
    eink_data[0] = 0x00; /* Internal Sensor Enabled */
    eink_write_data(UC8177_TSE, eink_data, 1);

    /* VCOM & DI Settings */
    eink_data[0] = 0x21;
    eink_data[1] = 0x22;
    eink_write_data(UC8177_CDI, eink_data, 2);
    
    /* TCON Settings */
    eink_data[0] = 0x3F;
    eink_data[1] = 0x09;
    eink_data[2] = 0x2D;
    eink_write_data(UC8177_TCON, eink_data, 3);
    
    /* Resolution Settings */
    eink_data[0] = 0x02; /* H = 600 */
    eink_data[1] = 0x58;
    eink_data[2] = 0x01; /* W = 480*/
    eink_data[3] = 0xE0;
    eink_write_data(UC8177_TRES, eink_data, 4);
    
    /* GD Order Settings */
    //eink_data[0] = 0x02;
    //eink_write_data(UC8177_GDOS, eink_data, 1);
    
    /* VCM DC Settings */
    eink_data[0] = 0x30;
    eink_write_data(UC8177_VDCS, eink_data, 1);
    
    uc8177_send_wf_lut();
}