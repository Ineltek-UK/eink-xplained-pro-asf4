/**
 * \file
 * 
 * \brief UC8173 Eink Display Driver Component
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
#include <eink/drivers/uc8173/uc8173.h>

/**
 * \brief Initializes the UC8173 Eink display driver.
 *
 * Call this function to initialize the hardware interface and the UC8173 display
 * driver. When initialization is done the display is ready to receive data.
 */
void uc8173_init(void)
{
    /* Initialize delay routine */
    delay_init(SysTick);

    /* Initialize the interface */
    eink_interface_init(0);

    /* Reset display */
    uc8173_hard_reset();
    
    /* Wait for BUSY output to return HIGH */
    uc8173_wait_for_busy_low();
}

/**
 * \brief Set the UC8173 configuration registers.
 *
 * Sets all the necessary configuration for the UC8173 Eink display driver to communicate
 * with the MCU and sets the necessary variables from the config struct to allow the 
 * display to work.
 *
 * \param *config The configuration to write to the display driver.
 */
void uc8173_set_config(
        struct uc8173_config *const config)
{
    uint8_t eink_data[5];
    
    uc8173_global_instance.display_config = *config;
    
	if((config->display_rotation == ROTATE_90) || (config->display_rotation == ROTATE_270)) {
		uc8173_global_instance.display_width   = 128;
		uc8173_global_instance.display_height  = 256;
	} else {
		uc8173_global_instance.display_width   = 256;
		uc8173_global_instance.display_height  = 128;
	}
	
    /* Booster Soft Start Control */
    eink_data[0] = 0x17;
	eink_data[1] = 0x17;
	eink_data[2] = 0x26;
	eink_write_data(UC8173_BTST, eink_data, 3);
	
    /* Power Settings */
	eink_data[0] = 0x03;
	eink_data[1] = 0x00;
	eink_data[2] = 0x2B;
	eink_data[3] = 0x2B;
	eink_data[4] = 0x08;
	eink_write_data(UC8173_PWR, eink_data, 5);
	
	eink_write_data(UC8173_PON, 0, 0);
	uc8173_wait_for_busy_low();
	
    /* Panel Settings */	
	eink_data[0] = 0x03;
	eink_data[1] = 0x86;
	eink_write_data(UC8173_PSR, eink_data, 2);

    /* Power OFF Sequence Settings */
	eink_data[0] = 0x00;
	eink_write_data(UC8173_PFS, eink_data, 1);
	
    /* PLL Control */
	eink_data[0] = 0x48;
	eink_write_data(UC8173_LPRD, eink_data, 1);

    /* Temperature Sensor Enable */
	eink_data[0] = 0x00; /* Internal Sensor Enabled */
	eink_write_data(UC8173_TSE, eink_data, 1);

    /* VCOM & DI Settings */
	eink_data[0] = 0xE1;
	eink_data[1] = 0x20;
	eink_data[2] = 0x10;
	eink_write_data(UC8173_CDI, eink_data, 3);

    /* Resolution Settings */
	eink_data[0] = 0x7F; /* H = 127 */
	eink_data[1] = 0x00;
	eink_data[2] = 0xFF; /* W = 255 */
	eink_write_data(UC8173_TRES, eink_data, 3);
	
	eink_data[0] = 0xCF;
	eink_data[1] = 0xAF;
	eink_data[2] = 0x00;
	eink_data[3] = 0x00;
	eink_data[4] = 0x03;
	eink_write_data(UC8173_GDS, eink_data, 5);
  
	uc8173_measure_vcom();

	eink_data[0] = 0x02;
	eink_write_data(UC8173_LVSEL, eink_data, 1);
	
	eink_data[0] = 0x02;
	eink_data[1] = 0x02;
	eink_write_data(UC8173_GBS, eink_data, 2);
	
	eink_data[0] = 0x02;
	eink_data[1] = 0x02;
	eink_write_data(UC8173_GSS, eink_data, 2);
	
	//eink_data[0] = 0x1F;
	//eink_write_data(0xDF, eink_data, 1); /* Command missing from sample code */
	//uc8173_wait_for_busy_low();
	
	eink_write_data(UC8173_POF, 0, 0);
	uc8173_wait_for_busy_high();
}

/**
 * \brief Measures and set the VCOM value.
 *
 * Measure the VCOM voltage from the UC8173's internal mechanism and sets the 
 * VDCS and VBDS setting.
 */
void uc8173_measure_vcom(void)
{
	uint8_t vcom_value = 0, eink_data[1];
	
	/* Auto measure VCOM */
	eink_data[0] = 0x11;
	eink_write_data(UC8173_AMV, eink_data, 1);
	uc8173_wait_for_busy_low();

	/* Read the measured VCOM value using 'VV' command */
	eink_read_data(UC8173_VV, &vcom_value, 1);
	vcom_value = vcom_value + 12;

	eink_data[0] = vcom_value;
	eink_write_data(UC8173_VDCS, eink_data, 1);
	eink_write_data(UC8173_VBDS, eink_data, 1);

}