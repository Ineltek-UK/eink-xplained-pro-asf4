/**
 * \file
 * 
 * \brief SSD1608 Eink Display Driver Component
 * 
 * Eink Software Library
 * Microchip ASF4 Variant - release 1.5 - January 2020
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
#define SSD1608_DOC         0x01
#define SSD1608_GDV         0x03
#define SSD1608_SDV         0x04
#define SSD1608_DC          0x07
#define SSD1608_BSSC        0x0C
#define SSD1608_DS          0x10
#define SSD1608_DEM         0x11
#define SSD1608_SWRESET     0x12
#define SSD1608_TSC         0x1A
#define SSD1608_MA          0x20
#define SSD1608_DUC1        0x21
#define SSD1608_DUC2        0x22
#define SSD1608_WR          0x24
#define SSD1608_WVCOM       0x2C
#define SSD1608_WLUT        0x32
#define SSD1608_DLP         0x3A
#define SSD1608_GLW         0x3B
#define SSD1608_BWC         0x3C
#define SSD1608_RXAS        0x44
#define SSD1608_RYAS        0x45
#define SSD1608_RXC         0x4E
#define SSD1608_RYC         0x4F
//@}

/**
 * \brief LUT Selection enum.
 *
 * Look Up Table for waveforms are stored in a OTP on the screen, but can also be 
 * loaded into a register on the display driver. This allows for custom waveforms
 * to be used to alter the display refresh pattern.
 */
enum ssd1608_lut_selection {
    SSD1608_LUT_OTP,
    SSD1608_LUT_REGISTER
};

/**
 * \brief HINK Part Selection enum.
 *
 * HINK Display part number - SSD1608 supports both the HINK-E0154A05 and the
 * HINK-E0154A45. The library needs to know which so it can adjust the
 * initialization accordingly.
 */
enum ssd1608_hink_part_no {
    SSD1608_HINK_E0154A05,
    SSD1608_HINK_E0154A45,
    SSD1608_HINK_UNSET
};

/**
 * \brief SSD1608 Configuration structure.
 *
 * Used in software library for initialization function to set SSD1608
 * configuration. Automatically maps settings to correct values and registers.
 */
struct ssd1608_config {
    enum eink_display_rotation                  display_rotation;
    enum ssd1608_lut_selection                  lut_selection;
    enum ssd1608_hink_part_no                   display_part_no;
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
    /** Display configuration */
    struct ssd1608_config                       display_config;
    /** Display width */
    eink_coordinate                           display_width;
    /** Display height */
    eink_coordinate                           display_height;
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
#ifdef EINK_BUSY
	while(gpio_get_pin_level(EINK_BUSY));
#endif
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
    /* Default configuration values */
    config->display_rotation = ROTATE_0;
    config->lut_selection = SSD1608_LUT_OTP;
    config->display_part_no = SSD1608_HINK_E0154A45;
}

void ssd1608_set_config(
        struct ssd1608_config *const config);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* SSD1608_H_INCLUDED */