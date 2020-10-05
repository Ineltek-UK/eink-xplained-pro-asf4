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
#ifndef UC8173_H_INCLUDED
#define UC8173_H_INCLUDED

#include <compiler.h>
#include <hal_gpio.h>
#include <hal_delay.h>
#include <eink/drivers/eink_driver.h>

// Controller and Eink configuration file
#include <atmel_start_pins.h>

#ifdef __cplusplus
extern "C" {
#endif

//! \name UC8173 Register Map defines
//@{
#define UC8173_PSR         0x0000
#define UC8173_PWR         0x0001
#define UC8173_POF         0x0002
#define UC8173_PFS         0x0003
#define UC8173_PON         0x0004
#define UC8173_PMES        0x0005
#define UC8173_BTST        0x0006
#define UC8173_DSLP        0x0007
#define UC8173_SLP         0x0008
#define UC8173_DTM1        0x0010
#define UC8173_DRF         0x0012
#define UC8173_DTM2        0x0013
#define UC8173_DTMW        0x0014
#define UC8173_DTM3        0x0015
#define UC8173_DTM4        0x0016
#define UC8173_LUT_KWVCOM  0x0020
#define UC8173_LUT_RVCOM   0x0021
#define UC8173_LUT_KW      0x0022 
#define UC8173_LUT_RR      0x0023 
#define UC8173_LUT_RK      0x0024 
#define UC8173_LUT_RW      0x0025
#define UC8173_LUT_FT      0x0026
#define UC8173_LPRD        0x0030
#define UC8173_TSC         0x0040
#define UC8173_TSE         0x0041
#define UC8173_TSW         0x0042
#define UC8173_TSR         0x0043
#define UC8173_PBC         0x0044
#define UC8173_CDI         0x0050
#define UC8173_LPD         0x0051
#define UC8173_TRES        0x0061
#define UC8173_GDS         0x0062
#define UC8173_GBS         0x0063
#define UC8173_GSS         0x0064
#define UC8173_REV         0x0070
#define UC8173_FLG         0x0071
#define UC8173_AMV         0x0080
#define UC8173_VV          0x0081
#define UC8173_VDCS        0x0082
#define UC8173_EDS         0x0083
#define UC8173_VBDS        0x0084
#define UC8173_LVSEL       0x00E4
//@}

//! \name ED013TC1 Waveform LUT
//@{
static const uint8_t ED013TC1_WF_LUT[]={
    
};
//@}

/**
 * \brief UC8173 Configuration structure.
 *
 * Used in software library for initialization function to set UC8173
 * configuration. Automatically maps settings to correct values and registers.
 */
struct uc8173_config {
    enum eink_display_rotation                  display_rotation;
};

/**
 * \brief UC8173 Driver component software device instance structure.
 *
 * Used to retain software state information of an associated display.
 *
 * \note The fields of this structure should not be altered by the user
 *       application; they are reserved for module-internal use only.
 */
//struct uc8177_module {
//    /** Display configuration */
//    struct uc8177_config                        display_config;
//    /** Display width */
//    eink_coordinate                           display_width;
//    /** Display height */
//    eink_coordinate                           display_height;
//};
//volatile struct uc8177_module uc8177_global_instance;

//! \name UC8173 reset
//@{

/**
 * \brief Perform a hard reset of the display driver
 *
 * This functions will reset the UC8173 by setting the reset pin LOW for 1ms.
 */
static inline void uc8173_hard_reset(void)
{
    gpio_set_pin_level(EINK_RST, 0);
    delay_ms(1);
    gpio_set_pin_level(EINK_RST, 1);
    delay_ms(1);
}
//@}


//! \name UC8173 busy check
//@{

/**
 * \brief Checks if the display busy pin is HIGH
 *
 * This functions will read the UC8173's busy status by reading the BUSY pin and will
 * hold the MCU until it returns LOW again.
 */
static inline void uc8173_wait_for_busy_high(void)
{
#ifdef EINK_BUSY
    while(gpio_get_pin_level(EINK_BUSY));
#endif
}

/**
 * \brief Checks if the display busy pin is LOW
 *
 * This functions will read the UC8173's busy status by reading the BUSY pin and will
 * hold the MCU until it returns HIGH again.
 */
static inline void uc8173_wait_for_busy_low(void)
{
#ifdef EINK_BUSY
    while(!gpio_get_pin_level(EINK_BUSY));
#endif
}
//@}

#ifdef __cplusplus
}
#endif

#endif /* UC8173_H_INCLUDED */