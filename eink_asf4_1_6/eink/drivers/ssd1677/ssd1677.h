/**
 * \file
 * 
 * \brief SSD1677 Eink Display Driver Component
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
#ifndef SSD1677_H_
#define SSD1677_H_

// Controller and Eink configuration file
#include <compiler.h>
#include <hal_gpio.h>
#include <hal_delay.h>
#include <eink/drivers/eink_driver.h>

#include <atmel_start_pins.h>

#ifdef __cplusplus
extern "C" {
#endif

//! \name SSD1677 Register Map defines.
//@{
#define SSD1677_GATE_SET        0x01
#define SSD1677_VGH             0x03
#define SSD1677_VSHL            0x04
#define SSD1677_GG_NOP          0x0B
#define SSD1677_BTST            0x0C
#define SSD1677_GSP             0x0F
#define SSD1677_DSP             0x10
#define SSD1677_DEY             0x11
#define SSD1677_SWRESET         0x12
#define SSD1677_TSC             0x18
#define SSD1677_WTM             0x1A
#define SSD1677_RTM             0x1B
#define SSD1677_ETM             0x1C
#define SSD1677_DSP_ACT         0x20
#define SSD1677_DSP_SEQ         0x22
#define SSD1677_RAM_BW          0x24
#define SSD1677_RAM_RED         0x26
#define SSD1677_VCOM_WT         0x2C
#define SSD1677_LUT_REG         0x32
#define SSD1677_DIS_OPT         0x37
#define SSD1677_BDW             0x3C
#define SSD1677_X_ADDR          0x44
#define SSD1677_Y_ADDR          0x45
#define SSD1677_AUTO_RAM_RED    0x46
#define SSD1677_AUTO_RAM_BW     0x47
#define SSD1677_X_ADDRC         0x4E
#define SSD1677_Y_ADDRC         0x4F
//@}

//! \name ED037TC1 Update Waveforms
//@{
static const uint8_t ED037TC1_A2_LUT[]={
    0x2A, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x2A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x2A, 0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x03, 0x0A, 0x00, 0x02, 0x06, 0x0A, 0x05, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x22, 0x22, 0x22, 0x22, 0x22
};

static const uint8_t ED037TC1_DU_LUT[]={
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x2A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0A, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x05, 0x00, 0x05, 0x03, 0x05, 0x05, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x22, 0x22, 0x22, 0x22, 0x22
};
//@}

/**
 * \brief SSD1677 Configuration structure.
 *
 * Used in software library for initialization function to set SSD1677
 * configuration. Automatically maps settings to correct values and registers.
 */
struct ssd1677_config {
    enum eink_display_rotation                  display_rotation;
    struct eink_uc_temperature_sensor_settings  temperature_sensor_settings;
    enum eink_update_mode                       update_mode;
};

/**
 * \brief SSD1677 Driver component software device instance structure.
 *
 * Used to retain software state information of an associated display.
 *
 * \note The fields of this structure should not be altered by the user
 *       application; they are reserved for module-internal use only.
 */
struct ssd1677_module {
    /** Display config */
    struct ssd1677_config                       panel_settings;
    /** Display width */
    eink_coordinate                             display_width;
    /** Display height */
    eink_coordinate                             display_height;
};
volatile struct ssd1677_module ssd1677_global_instance;

//! \name SSD1677 reset
//@{
    
/**
 * \brief Perform a hard reset of the display driver.
 *
 * This functions will reset the SSD1677 by setting the reset pin low for 1ms.
 */
static inline void ssd1677_hard_reset(void)
{
    gpio_set_pin_level(EINK_RST, 0);
    delay_ms(1);
    gpio_set_pin_level(EINK_RST, 1);
    delay_ms(1);
}
//@}


//! \name SSD1677 busy check
//@{

/**
 * \brief Checks if the display driver is busy.
 *
 * This functions will read the SSD1677's busy status by reading the BUSY pin and will
 * hold the MCU until it returns HIGH again.
 */
static inline void ssd1677_wait_for_busy(void)
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
 *  \brief Initializes SSD1677 configuration structure to defaults.
 */
static inline void ssd1677_get_config_defaults(
        struct ssd1677_config *const config)
{
    /* Default configuration values */
    config->display_rotation = ROTATE_0;
    
    config->temperature_sensor_settings.sensor_select = INTERNAL_TEMPERATURE_SENSOR;
    config->temperature_sensor_settings.temperature_offset = 0;
    
    config->update_mode = UPDATE_SLOW;
}

void ssd1677_set_config(
        const struct ssd1677_config *config);
/** @} */

//! \name SSD1677 initialization function
//@{
void ssd1677_init(void);
//@}

#ifdef __cplusplus
}
#endif

#endif /* SSD1677_H_ */