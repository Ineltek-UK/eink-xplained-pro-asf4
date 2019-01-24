/**
 * \file
 * 
 * \brief SSD1608 Eink Display Driver Component
 * 
 * Eink Software Library
 * Microchip ASF4 Variant - release 1.2 - January 2019
 *
 * \note Not currently implemented.
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
#ifndef SSD1608_H_INCLUDED
#define SSD1608_H_INCLUDED

#include <compiler.h>
#include <hal_gpio.h>
#include <hal_delay.h>
#include <eink/drivers/eink_driver.h>

// Controller and Eink configuration file
#include <atmel_start_pins.h>

#ifdef __cplusplus
extern "C" {
#endif

//! \name SSD1608 Register Map defines
//@{
#define SSD1608_DOC         0x0001 //Command: Driver Output Control
#define SSD1608_GDV         0x0003 //Command: Gate Driving Voltage Control
#define SSD1608_SDV         0x0004 //Command: Source Driving Voltage Control
#define SSD1608_DC          0x0007 //Command: Display Control
#define SSD1608_BSSC        0x000C //Command: Booster Soft Start Control
#define SSD1608_DS          0x0010 //Command: Deep Sleep Mode
#define SSD1608_DEM         0x0011 //Command: Data Entry Mode Setting
#define SSD1608_SWRESET     0x0012 //Command: Software Register
#define SSD1608_TSC         0x001A //Command: Temperature Sensor Control
#define SSD1608_MA          0x0020 //Command: Master Activation
#define SSD1608_DUC1        0x0021 //Command: Display Update Control 1
#define SSD1608_DUC2        0x0022 //Command: Display Update Control 2
#define SSD1608_WR          0x0024 //Command: Write RAM
#define SSD1608_WVCOM       0x002C //Command: Write VCOM Register
#define SSD1608_WLUT        0x0032 //Command: Write LUT register
#define SSD1608_DLP         0x003A //Command: Set Dummy Line Period
#define SSD1608_GLW         0x003B //Command: Set Gate line width
#define SSD1608_BWC         0x003C //Command: Border Waveform Control
#define SSD1608_RXAS        0x0044 //Command: Set RAM X - address Start / End position
#define SSD1608_RYAS        0x0045 //Command: Set Ram Y- address Start / End position
#define SSD1608_RXC         0x004E //Command: Set RAM X address counter
#define SSD1608_RYC         0x004F //Command: Set RAM Y address counter
//@}


/**
 * \brief SSD1608 Configuration structure.
 *
 * Used in software library for initialization function to set SSD1608
 * configuration. Automatically maps settings to correct values and registers.
 */
struct ssd1608_config {

};

/**
 * \brief SSD1608 Driver component software device instance structure.
 *
 * Used to retain software state information of an associated display.
 *
 * \note The fields of this structure should not be altered by the user
 *       application; they are reserved for module-internal use only.
 */
struct ssd1608_module {
    /** Display settings */
    
    /** Display width */
    eink_x_coordinate                           display_width;
    /** Display height */
    eink_y_coordinate                           display_height;
};
volatile struct ssd1608_module ssd1608_global_instance;

//! \name SSD1608 reset
//@{

/**
 * \brief Perform a hard reset of the display driver
 *
 * This functions will reset the SSD1608 by setting the reset pin LOW for 1ms.
 */
static inline void ssd1608_hard_reset(void)
{
    gpio_set_pin_level(EINK_RST, 0);
    delay_ms(1);
    gpio_set_pin_level(EINK_RST, 1);
    delay_ms(1);
}
//@}


//! \name SSD1608 busy check
//@{

/**
 * \brief Checks if the display driver is busy
 *
 * This functions will read the SSD1608's busy status by reading the BUSY pin and will
 * hold the MCU until it returns LOW again.
 */
static inline void ssd1608_wait_for_busy(void)
{
    while(gpio_get_pin_level(EINK_BUSY));
}
//@}

/** \name Configuration and Initialization
 * @{
 */

/**
 *  \brief Initializes SSD1608 configuration structure to defaults.
 */
static inline void ssd1608_get_config_defaults(
        struct ssd1608_config *const config)
{
   
}

void ssd1608_set_config(
        const struct ssd1608_config *config);

/** @} */

//! \name SSD1608 initialization function
//@{
void ssd1608_init(void);
//@}

#ifdef __cplusplus
}
#endif

#endif /* SSD1608_H_INCLUDED */