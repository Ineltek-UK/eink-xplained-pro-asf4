/**
 * \file
 * 
 * \brief UC8151 Eink Display Driver Component
 * 
 * Eink Software Library
 * Microchip ASF4 Variant - release 1.3 - July 2019
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
#ifndef UC8151_H_INCLUDED
#define UC8151_H_INCLUDED

#include <compiler.h>
#include <hal_gpio.h>
#include <hal_delay.h>
#include <eink/drivers/eink_driver.h>

// Controller and Eink configuration file
#include <atmel_start_pins.h>

#ifdef __cplusplus
extern "C" {
#endif

//! \name UC8151 Register Map defines.
//@{
#define UC8151_PSR          0x0000
#define UC8151_PWR          0x0001
#define UC8151_POF          0x0002
#define UC8151_PFS          0x0003
#define UC8151_PON          0x0004
#define UC8151_PMES         0x0005
#define UC8151_BTST         0x0006
#define UC8151_DSLP         0x0007
#define UC8151_DTM1         0x0010
#define UC8151_DSP          0x0011
#define UC8151_DRF          0x0012
#define UC8151_DTM2         0x0013
#define UC8151_LUT_VCOM     0x0020
#define UC8151_LUT_WW       0x0021
#define UC8151_LUT_BW       0x0022
#define UC8151_LUT_WB       0x0023
#define UC8151_LUT_BB       0x0024
#define UC8151_PLL          0x0030
#define UC8151_TSC          0x0040
#define UC8151_TSE          0x0041
#define UC8151_TSR          0x0043
#define UC8151_TSW          0x0042
#define UC8151_CDI          0x0050
#define UC8151_LPD          0x0051
#define UC8151_TCON         0x0060
#define UC8151_TRES         0x0061
#define UC8151_REV          0x0070
#define UC8151_FLG          0x0071
#define UC8151_AMV          0x0080
#define UC8151_VV           0x0081
#define UC8151_VDCS         0x0082
#define UC8151_PTL          0x0090
#define UC8151_PTIN         0x0091
#define UC8151_PTOU         0x0092
#define UC8151_PGM          0x00A0
#define UC8151_APG          0x00A1
#define UC8151_ROTP         0x00A2
#define UC8151_CCSET        0x00E0
#define UC8151_PWS          0x00E3
#define UC8151_TSSET        0x00E5
//@}

/**
 * \name UC8151 Partial Scan defines.
 * 
 * \note Partial Window refresh is not currently implemented.
 * @{
 */
#define PTL_HRST            0
#define PTL_HRED            1
#define PTL_VRST_1          2
#define PTL_VRST_2          3
#define PTL_VRED_1          4
#define PTL_VRED_2          5
#define PTL_PT_SCAN         6
/** @} */

/**
 * \brief Display Resolution enum.
 *
 * \note As the UC8151 is used for multiple displays, ensure the correct resolution
 * for the screen being used.
 */
enum uc8151_psr_display_resolution {
    RES_96x230,
    RES_96x252,
    RES_128x296,
    RES_160x296
};

/**
 * \brief LUT Selection enum.
 *
 * Look Up Table for waveforms are stored in a OTP on the screen, but can also be 
 * loaded into a register on the display driver. This allows for custom waveforms
 * to be used to alter the display refresh pattern.
 */
enum uc8151_psr_lut_selection {
    LUT_FROM_OTP,
    LUT_FROM_REGISTER
};

/**
 * \brief Display Colors enum.
 */
enum uc8151_psr_display_colours {
    B_W_PIXELS,
    B_W_R_PIXELS,
};

/**
 * \brief Panel Settings configuration structure.
 * 
 * Register 0x00 - PSR
 */
struct uc8151_panel_settings {
    enum uc8151_psr_display_resolution          display_resolution;
    enum uc8151_psr_lut_selection               lut_selection;
    enum uc8151_psr_display_colours             display_colours;
    enum eink_display_rotation                  display_rotation;
    enum eink_uc_psr_booster_switch             booster_switch;
};

/**
 * \brief VCOM Voltage Level enum.
 *
 * - VCOMH=VDH+VCOMDC, VCOML=VHL+VCOMDC
 * - VCOML=VGH, VCOML=VGL
 */
enum uc8151_pwr_vcom_voltage_level {
    VCOMx_VDx_DC_VCOMD,
    VCOMx_VGx
};

/**
 * \brief Power Settings configuration structure.
 * 
 * Register 0x01 - PWR
 *
 * See datasheet for information on voltage values. It is recommended to leave
 * these values set to their defaults.
 */
struct uc8151_power_settings {
    enum eink_uc_pwr_source_power_sel           source_power;
    enum eink_uc_pwr_gate_power_sel             gate_power;
    
    enum uc8151_pwr_vcom_voltage_level          vcom_voltage_level;
    enum eink_uc_pwr_vg_voltage_level           vg_voltage_level;
    
    uint8_t                                     pwr_vdh_level;   /**< Power selection for Black/White pixels. \note See datasheet for information on VDH voltage values. */
    uint8_t                                     pwr_vdl_level;   /**< Power selection for Black/White pixels. \note See datasheet for information on VDL voltage values. */
    uint8_t                                     pwr_vdhr_level;  /**< Power selection for Red pixels. \note See datasheet for information on VDHR voltage values. */
};

/**
 * \brief Soft Start Period enum.
 */
enum soft_start_period {
    SS_PERIOD_10MS,
    SS_PERIOD_20MS,
    SS_PERIOD_30MS,
    SS_PERIOD_40MS
};

/**
 * \brief Driving Strength enum.
 */
enum soft_start_drive_strength {
    STRENGTH_1,
    STRENGTH_2,
    STRENGTH_3,
    STRENGTH_4,
    STRENGTH_5,
    STRENGTH_6,
    STRENGTH_7,
    STRENGTH_8
};

/**
 * \brief Minimum OFF Time enum.
 *
 * Minimum OFF time setting of GDR.
 */
enum soft_start_min_off_time {
    MIN_OFF_TIME_270_NS,
    MIN_OFF_TIME_340_NS,
    MIN_OFF_TIME_400_NS,
    MIN_OFF_TIME_540_NS,
    MIN_OFF_TIME_800_NS,
    MIN_OFF_TIME_1540_NS,
    MIN_OFF_TIME_3340_NS,
    MIN_OFF_TIME_6580_NS
};

/**
 * \brief Booster Soft Start configuration structure.
 * 
 * Register 0x06 - BTST
 */
struct uc8151_booster_soft_start {
    enum soft_start_period                      a_soft_start_period;  /**< Soft start period of phase A. \note Default: 10mS*/
    enum soft_start_drive_strength              a_drive_strength;     /**< Driving strength of phase A. \note Default: 3*/
    enum soft_start_min_off_time                a_min_off_time;       /**< Minimum OFF time setting of GDR in phase A. \note Default: 6.58uS*/
    
    enum soft_start_period                      b_soft_start_period;  /**< Soft start period of phase B. \note Default: 10mS*/
    enum soft_start_drive_strength              b_drive_strength;     /**< Driving strength of phase B. \note Default: 3*/
    enum soft_start_min_off_time                b_min_off_time;       /**< Minimum OFF time setting of GDR in phase B. \note Default: 6.58uS*/
    
    enum soft_start_drive_strength              c_drive_strength;     /**< Driving strength of phase C. \note Default: 3*/
    enum soft_start_min_off_time                c_min_off_time;       /**< Minimum OFF time setting of GDR in phase C. \note Default: 6.58uS*/
};

/**
 * \brief UC8151 Configuration structure.
 *
 * Used in software library for initialization function to set UC8151
 * configuration. Automatically maps settings to correct values and registers.
 */
struct uc8151_config {
    struct uc8151_panel_settings                panel_settings;
    struct uc8151_power_settings                power_settings;
    struct uc8151_booster_soft_start            booster_soft_start;
    enum eink_uc_power_off_sequence             power_off_sequence;           /**< Power off sequence setting (PFS - 0x03). \note Default: 1 frame*/
    struct eink_uc_temperature_sensor_settings  temperature_sensor_settings;
};

/**
 * \brief Partial Window Update configuration enum.
 *
 * Enables partial window updates over full window update.
 * 
 * \note Partial Window refresh is not currently implemented. 
 */
enum uc8151_partial_window_status {
    PARTIAL_WINDOW_ENABLED,
    PARTIAL_WINDOW_DISABLED
};

/**
 * \brief UC8151 Driver component software device instance structure.
 *
 * Used to retain software state information of an associated display.
 *
 * \note The fields of this structure should not be altered by the user
 *       application; they are reserved for module-internal use only.
 */
struct uc8151_module {
    /** Display settings */
    struct uc8151_panel_settings                panel_settings;
    /** Display width */
    eink_x_coordinate                           display_width;
    /** Display height */
    eink_y_coordinate                           display_height;
    /** Partial Window settings
     *
     * \note Partial Window refresh is not currently implemented.
     */
    enum uc8151_partial_window_status           partial_window_en;
    /** Partial Window width
     *
     * \note Partial Window refresh is not currently implemented.
     */
    eink_pw_x_coordinate                        partial_window_width;
    /** Partial Window height
     *
     * \note Partial Window refresh is not currently implemented.
     */
    eink_pw_y_coordinate                        partial_window_height;
};
volatile struct uc8151_module uc8151_global_instance;

//! \name UC8151 reset
//@{
    
/**
 * \brief Perform a hard reset of the display driver.
 *
 * This functions will reset the UC8151 by setting the reset pin low for 1ms.
 */
static inline void uc8151_hard_reset(void)
{
    gpio_set_pin_level(EINK_RST, 0);
    delay_ms(1);
    gpio_set_pin_level(EINK_RST, 1);
    delay_ms(1);
}
//@}


//! \name UC8151 busy check
//@{

/**
 * \brief Checks if the display driver is busy.
 *
 * This functions will read the UC8151's busy status by reading the BUSY pin and will
 * hold the MCU until it returns HIGH again.
 */
static inline void uc8151_wait_for_busy(void)
{
    while(!gpio_get_pin_level(EINK_BUSY));
}
//@}

/** \name Configuration and Initialization
 * @{
 */

/**
 *  \brief Initializes UC8151 configuration structure to defaults.
 */
static inline void uc8151_get_config_defaults(
        struct uc8151_config *const config)
{
    /* Default configuration values */
    config->panel_settings.display_resolution = RES_128x296;
    config->panel_settings.lut_selection = LUT_FROM_OTP;
    config->panel_settings.display_colours = B_W_PIXELS;
    config->panel_settings.display_rotation = ROTATE_0;
    config->panel_settings.booster_switch = BOOSTER_ON;
    
    config->power_settings.source_power = SOURCE_INTERNAL_DC_DC;
    config->power_settings.gate_power = GATE_INTERNAL_DC_DC;
    config->power_settings.vcom_voltage_level = VCOMx_VDx_DC_VCOMD;
    config->power_settings.vg_voltage_level = VGx_16V;
    config->power_settings.pwr_vdh_level = 0x2B;
    config->power_settings.pwr_vdl_level = 0x2B;
    config->power_settings.pwr_vdhr_level = 0x2B;
    
    config->booster_soft_start.a_soft_start_period = SS_PERIOD_10MS;
    config->booster_soft_start.a_drive_strength = STRENGTH_3;
    config->booster_soft_start.a_min_off_time = MIN_OFF_TIME_6580_NS;
    config->booster_soft_start.b_soft_start_period = SS_PERIOD_10MS;
    config->booster_soft_start.b_drive_strength = STRENGTH_3;
    config->booster_soft_start.b_min_off_time = MIN_OFF_TIME_6580_NS;
    config->booster_soft_start.c_drive_strength = STRENGTH_3;
    config->booster_soft_start.c_min_off_time = MIN_OFF_TIME_6580_NS;
    
    config->power_off_sequence = PFS_1_FRAME;
    
    config->temperature_sensor_settings.sensor_select = INTERNAL_TEMPERATURE_SENSOR;
    config->temperature_sensor_settings.temperature_offset = 0;
}

void uc8151_set_config(
        const struct uc8151_config *config);


void uc8151_set_psr_config(
        const struct uc8151_panel_settings *psr_config);
/** @} */

//! \name UC8151 initialization function
//@{
void uc8151_measure_vcom(void);

void uc8151_init(void);
//@}

#ifdef __cplusplus
}
#endif

#endif /* UC8151_H_INCLUDED */