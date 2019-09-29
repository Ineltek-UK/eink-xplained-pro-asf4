/**
 * \file
 * 
 * \brief SSD1608 Eink Display Driver Component
 * 
 * Eink Software Library
 * Microchip ASF4 Variant - release 1.4 - October 2019
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
#include <eink/drivers/ssd1608/ssd1608.h>

/**
 * \brief Initializes the SSD1608 Eink display driver.
 *
 * Call this function to initialize the hardware interface and the SSD1608 display
 * driver. When initialization is done the display is ready to receive data.
 */
void ssd1608_init(void)
{
    /* Initialize delay routine */
    delay_init(SysTick);

    /* Initialize the interface */
    eink_interface_init(0);

    /* Reset display */
    ssd1608_hard_reset();
    
    /* Wait for BUSY output to return LOW */
    ssd1608_wait_for_busy();

    /* Set the HINK part number as unset */
    ssd1608_global_instance.display_config.display_part_no = SSD1608_HINK_UNSET;
}

//! \name SSD1608 Instant Update waveform for HINK-E0154A05
//@{
const uint8_t SSD1608_A05_IU_LUT[]={
    0x10, 0x18, 0x18, 0x28, 0x18, 0x18, 0x18, 0x18, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x12, 0x22, 0x13, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00
};
//@}

//! \name SSD1608 Instant Update waveform for HINK-E0154A45
//@{
const uint8_t SSD1608_A45_IU_LUT[]={
    0x24, 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x10, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x11, 0x11, 0x11, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00
};
//@}

/**
 * \brief Set the SSD1608 configuration registers.
 *
 * Sets all the necessary configuration for the SSD1608 Eink display driver to communicate
 * with the MCU and sets the necessary variables from the config struct to allow the 
 * display to work.
 *
 * \param *config The configuration to write to the display driver.
 */
void ssd1608_set_config(
        struct ssd1608_config *const config)
{
    uint8_t eink_data[5];
    
    if(ssd1608_global_instance.display_config.display_part_no != SSD1608_HINK_UNSET)
    {
        /* HINK part number already set, ignore the value in the config struct to ensure validity */
        config->display_part_no = ssd1608_global_instance.display_config.display_part_no;
    }
    
    /* Save display resolution and rotation into SSD1608 instance */
    ssd1608_global_instance.display_config = *config;
    ssd1608_global_instance.display_width   = 200;
    ssd1608_global_instance.display_height  = 200;
    
    /* Gate Driving Voltage */
    eink_data[0] = 0xEA;
    eink_write_data(SSD1608_GDV, eink_data, 1);

    /* Source Driving Voltage */
    eink_data[0] = 0x0A;
    eink_write_data(SSD1608_SDV, eink_data, 1);

    /* Set Gate Line Width */
    eink_data[0] = 0x0B;
    eink_write_data(SSD1608_GLW, eink_data, 1);

    /* Border Waveform Control */
    eink_data[0] = 0x33;
    eink_write_data(SSD1608_BWC, eink_data, 1);

    /* Driver Output Control */
    eink_data[0] = 0xC7;
    eink_data[1] = 0x00;
    eink_data[2] = 0x00;
    eink_write_data(SSD1608_DOC, eink_data, 3);

    /* Booster Soft Start Control */
    eink_data[0] = 0xD7;
    eink_data[1] = 0xD6;
    eink_data[2] = 0x9D;
    eink_write_data(SSD1608_BSSC, eink_data, 3);

    /* Write VCOM value */
    if(config->display_part_no == SSD1608_HINK_E0154A45)
    {
        eink_data[0] = 0x50;
    } else {
        eink_data[0] = 0x7D;
    }
    eink_write_data(SSD1608_WVCOM, eink_data, 1);

    /* Set Dummy Line Period */
    eink_data[0] = 0x12;
    eink_write_data(SSD1608_DLP, eink_data, 1);

    /* Data Entry Mode Setting */
    eink_data[0] = 0x00;
    eink_write_data(SSD1608_DEM, eink_data, 1);
    
    /* Set RAM X Start Position */
    eink_data[0] = 0x18;
    eink_data[1] = 0x00;
    eink_write_data(SSD1608_RXAS, eink_data, 2);

    /* Set RAM Y Start Position */
    eink_data[0] = 0xC7;
    eink_data[1] = 0x00;
    eink_data[2] = 0x00;
    eink_data[3] = 0x00;
    eink_write_data(SSD1608_RYAS, eink_data, 4);
    
    /* Set RAM X Address Counter */
    eink_data[0] = 0x18;
    eink_write_data(SSD1608_RXC, eink_data, 1);
    
    /* Set RAM Y Address Counter */
    eink_data[0] = 0xC7;
    eink_data[1] = 0x00;
    eink_write_data(SSD1608_RYC, eink_data, 2);

    if(config->display_part_no == SSD1608_HINK_E0154A45)
    {
        eink_write_data(SSD1608_WLUT, SSD1608_A45_IU_LUT, 30);
    } else {
        eink_write_data(SSD1608_WLUT, SSD1608_A05_IU_LUT, 30);
    }
    ssd1608_wait_for_busy();   
}