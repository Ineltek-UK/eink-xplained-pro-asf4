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
#ifndef UC8159_H_
#define UC8159_H_

#include <compiler.h>
#include <hal_gpio.h>
#include <hal_delay.h>
#include <eink/drivers/eink_driver.h>

// Controller and Eink configuration file
#include <atmel_start_pins.h>

#ifdef __cplusplus
extern "C" {
#endif

//! \name UC8159 Register Map defines.
//@{
#define UC8159_PSR          0x0000
#define UC8159_PWR          0x0001
#define UC8159_POF          0x0002
#define UC8159_PFS          0x0003
#define UC8159_PON          0x0004
#define UC8159_BTST         0x0006
#define UC8159_DSLP         0x0007
#define UC8159_DTM1         0x0010
#define UC8159_DSP          0x0011
#define UC8159_DRF          0x0012
#define UC8159_IPC          0x0013
#define UC8159_PLL          0x0030
#define UC8159_TSC          0x0040
#define UC8159_TSE          0x0041
#define UC8159_TSW          0x0042
#define UC8159_TSR          0x0043
#define UC8159_CDI          0x0050
#define UC8159_LPD          0x0051
#define UC8159_TCON         0x0060
#define UC8159_TRES         0x0061
#define UC8159_DAM          0x0065
#define UC8159_REV          0x0070
#define UC8159_FLG          0x0071
#define UC8159_AMV          0x0080
#define UC8159_VV           0x0081
#define UC8159_VDCS         0x0082
#define UC8159_PWS          0x00E3
#define UC8159_TSSET        0x00E5
//@}

/**
 * \brief Display Resolution enum.
 *
 * \note As the UC8159 is used for multiple displays, ensure the correct resolution
 * for the screen being used.
 */
enum uc8159_psr_display_resolution {
    RES_640x480,
    RES_600x450,
    RES_640x448,
    RES_600x448
};

/**
 * \brief Panel Settings configuration structure.
 * 
 * Register 0x00 - PSR
 */
struct uc8159_panel_settings {
    enum uc8159_psr_display_resolution          display_resolution;
    enum eink_display_rotation                  display_rotation;
    enum eink_uc_psr_booster_switch             booster_switch;
};

/**
 * \brief Source LV Power Selection enum.
 *
 * - External source LV power from VSH_LV/VSL_LV pins.
 * - Internal DC/DC function for generating LV power.
 */
enum uc8159_pwr_lv_source_power_sel {
    LV_SOURCE_EXTERNAL_POWER,
    LV_SOURCE_INTERNAL_DC_DC
};

/**
 * \brief Power Settings configuration structure.
 * 
 * Register 0x01 - PWR
 *
 * See datasheet for information on voltage values. It is recommended to leave
 * these values set to their defaults.
 */
struct uc8159_power_settings {
    enum eink_uc_pwr_source_power_sel           source_power;
    enum uc8159_pwr_lv_source_power_sel         lv_source_power;
    enum eink_uc_pwr_gate_power_sel             gate_power;
    enum eink_uc_pwr_vg_voltage_level           vg_voltage_level;
};

/**
 * \brief UC8159 Configuration structure.
 *
 * Used in software library for initialization function to set UC8159
 * configuration. Automatically maps settings to correct values and registers.
 */
struct uc8159_config {
    struct uc8159_panel_settings                panel_settings;
    struct uc8159_power_settings                power_settings;
    enum eink_uc_power_off_sequence             power_off_sequence;           /**< Power off sequence setting (PFS - 0x03). \note Default: 1 frame*/
    struct eink_uc_temperature_sensor_settings  temperature_sensor_settings;
};

/**
 * \brief UC8159 Driver component software device instance structure.
 *
 * Used to retain software state information of an associated display.
 *
 * \note The fields of this structure should not be altered by the user
 *       application; they are reserved for module-internal use only.
 */
struct uc8159_module {
    /** Display settings */
    struct uc8159_panel_settings                panel_settings;
    /** Display width */
    eink_x_coordinate                           display_width;
    /** Display height */
    eink_y_coordinate                           display_height;
};
volatile struct uc8159_module uc8159_global_instance;

//! \name UC8159 reset
//@{
    
/**
 * \brief Perform a hard reset of the display driver.
 *
 * This functions will reset the UC8159 by setting the reset pin low for 1ms.
 */
static inline void uc8159_hard_reset(void)
{
    gpio_set_pin_level(EINK_RST, 0);
    delay_ms(1);
    gpio_set_pin_level(EINK_RST, 1);
    delay_ms(1);
}
//@}


//! \name UC8159 busy check
//@{

/**
 * \brief Checks if the display driver is busy.
 *
 * This functions will read the UC8159's busy status by reading the BUSY pin and will
 * hold the MCU until it returns HIGH again.
 */
static inline void uc8159_wait_for_busy(void)
{
    while(!gpio_get_pin_level(EINK_BUSY));
}
//@}

/** \name Configuration and Initialization
 * @{
 */

/**
 *  \brief Initializes UC8159 configuration structure to defaults.
 */
static inline void uc8159_get_config_defaults(
struct uc8159_config *const config)
{
    /* Default configuration values */
    config->panel_settings.display_resolution = RES_600x448;
    config->panel_settings.display_rotation = ROTATE_0;
    config->panel_settings.booster_switch = BOOSTER_ON;
    
    config->power_settings.source_power = SOURCE_INTERNAL_DC_DC;
    config->power_settings.gate_power = GATE_INTERNAL_DC_DC;
    config->power_settings.lv_source_power = LV_SOURCE_INTERNAL_DC_DC;
    config->power_settings.vg_voltage_level = VGx_20V;
    
    config->power_off_sequence = PFS_1_FRAME;
    
    config->temperature_sensor_settings.sensor_select = INTERNAL_TEMPERATURE_SENSOR;
    config->temperature_sensor_settings.temperature_offset = 0;
}

void uc8159_set_config(
        const struct uc8159_config *config);


void uc8159_set_psr_config(
        const struct uc8159_panel_settings *psr_config);
/** @} */

//! \name UC8159 initialization function
//@{
void uc8159_measure_vcom(void);

void uc8159_init(void);
//@}

#ifdef __cplusplus
}
#endif

#endif /* UC8159_H_ */