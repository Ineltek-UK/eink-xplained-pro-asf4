/**
 * \file
 * 
 * \brief Eink Display Driver Component
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
#ifndef EINK_DRIVER_H_
#define EINK_DRIVER_H_

#include <compiler.h>
#include <hal_gpio.h>
#include <hal_delay.h>

// Controller and Eink configuration file
#include <atmel_start_pins.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  \brief Eink X Coordinate typdef.
 */
typedef uint16_t eink_x_coordinate;
/**
 *  \brief Eink Y Coordinate typdef.
 */
typedef uint16_t eink_y_coordinate;
/**
 *  \brief Eink Partial Window X Coordinate typdef.
 */
typedef uint16_t eink_pw_x_coordinate;
/**
 *  \brief Eink Partial Window Y Coordinate typdef.
 */
typedef uint16_t eink_pw_y_coordinate;
/**
 *  \brief Eink Coordinate typdef.
 */
typedef uint16_t eink_coordinate;

/**
 * \brief Display Rotation enum.
 *
 * Automatically deals with Scan and Gate direction to ensure correct rotation.
 * 
 * \note 0 degrees of rotation  for the ED029TC1 has the display horizontal,
 * with the FPC on the right of the display.
 */
enum eink_display_rotation {
    ROTATE_0,
    ROTATE_90,
    ROTATE_180,
    ROTATE_270
};

/**
 * \brief Booster Switch enum.
 *
 * - Booster OFF : register data are kept, and SEG/BG/VCOM are kept 0V or floating.
 * - Booster ON (Default) : When SHD_N become LOW, charge pump will be turned OFF,
 *   register and SRAM data will keep until VDD OFF, and SD output and VCOM will
 *   remain previous condition. SHD_N may have two conditions: 0V or floating.
 */
enum eink_uc_psr_booster_switch {
    BOOSTER_OFF,
    BOOSTER_ON
};

/**
 * \brief Source Power Selection enum.
 *
 * - External source power from VDH/VDL pins.
 * - Internal DC/DC function for generating VDH/VDL.
 */
enum eink_uc_pwr_source_power_sel {
    SOURCE_EXTERNAL_POWER,
    SOURCE_INTERNAL_DC_DC
};

/**
 * \brief Gate power selection enum.
 *
 * - External gate power from VGH/VGL pins.
 * - Internal DC/DC function for generating VGH/VGL.
 */
enum eink_uc_pwr_gate_power_sel {
    GATE_EXTERNAL_POWER,
    GATE_INTERNAL_DC_DC
};

/**
 * \brief VGH / VGL Voltage Level enum.
 *
 * Used for UC8159:
 * - VGH = 20V, VGL = -20V
 * - VGH = 19V, VGL = -19V (Default)
 * - VGH = 18V, VGL = -18V
 * - VGH = 17V, VGL = -17V
 *
 * Used for UC8151:
 * - VGH = 16V, VGL = -16V (Default)
 * - VGH = 15V, VGL = -15V
 * - VGH = 14V, VGL = -14V
 * - VGH = 13V, VGL = -13V
 *
 * \note Driver IC will not initialize if an incorrect value is selected.
 */
enum eink_uc_pwr_vg_voltage_level {
    VGx_20V,
    VGx_19V,
    VGx_18V,
    VGx_17V,
    VGx_16V,
    VGx_15V,
    VGx_14V,
    VGx_13V
};

/**
 * \brief Power Off Sequence enum.
 */
enum eink_uc_power_off_sequence {
    PFS_1_FRAME,
    PFS_2_FRAMES,
    PFS_3_FRAMES,
    PFS_4_FRAMES
};

/**
 * \brief Temperature Sensor Selector enum.
 *
 * - Internal (Default)
 * - External
 */
enum temperature_sensor {
    INTERNAL_TEMPERATURE_SENSOR,
    EXTERNAL_TEMPERATURE_SENSOR
};

/**
 * \brief Temperature Sensor Enable Configuration structure.
 *
 * This command selects Internal or External temperature sensor and allows
 * temperature offset to be configured.
 */
struct eink_uc_temperature_sensor_settings {
    enum temperature_sensor                     sensor_select;
    int8_t                                      temperature_offset;           /**< Temperature offset is signed 8-bit int (-8 to +7) */
};

/**
 * \brief Initializes the Eink hardware interface.
 *
 * \note Initializes the display controller hardware interface, regardless
 * of driver IC, interface or display. Takes definitions from conf_eink.h.
 */
static void eink_interface_init(uint8_t f_init)
{
    /* Set up the Eink interface outputs */
    gpio_set_pin_direction(EINK_BS, GPIO_DIRECTION_OUT);
    gpio_set_pin_direction(EINK_CSB, GPIO_DIRECTION_OUT);
    gpio_set_pin_direction(EINK_DC, GPIO_DIRECTION_OUT);
    gpio_set_pin_direction(EINK_RST, GPIO_DIRECTION_OUT);

    gpio_set_pin_direction(EINK_SPI_SDA, GPIO_DIRECTION_OUT);
    gpio_set_pin_direction(EINK_SPI_SCL, GPIO_DIRECTION_OUT);
    
    if(f_init) {
#ifdef EINK_FMSDO
        gpio_set_pin_direction(EINK_FMSDO, GPIO_DIRECTION_IN);
        gpio_set_pin_pull_mode(EINK_FMSDO, GPIO_PULL_UP);
        gpio_set_pin_function(EINK_FMSDO, GPIO_PIN_FUNCTION_OFF);
#endif
#ifdef EINK_MFCSB
        gpio_set_pin_direction(EINK_MFCSB, GPIO_DIRECTION_OUT);
        gpio_set_pin_level(EINK_MFCSB, 1); /* Set EINK_MFCSB HIGH to disable communication to on-board flash */
        gpio_set_pin_function(EINK_MFCSB, GPIO_PIN_FUNCTION_OFF);
#endif
    }
    
    /* Set up the Eink initial outputs */
    gpio_set_pin_level(EINK_BS, 0); /* Set BS LOW for 4 wire SPI interface */
    gpio_set_pin_level(EINK_CSB, 1); /* Set CSB HIGH to disable communication to connected display */
    gpio_set_pin_level(EINK_DC, 1); /* Set DC HIGH by default */
    gpio_set_pin_level(EINK_RST, 1); /* Set RST HIGH since reset is active low */

    gpio_set_pin_level(EINK_SPI_SDA, 0); /* Set SDA LOW for initial state */
    gpio_set_pin_level(EINK_SPI_SCL, 0); /* Set SCL LOW for initial state */

    /* Set up the BUSY input */
    gpio_set_pin_direction(EINK_BUSY, GPIO_DIRECTION_IN);
    gpio_set_pin_pull_mode(EINK_BUSY, GPIO_PULL_UP);

    /* Setup PORT pin functions */
    gpio_set_pin_function(EINK_BS, GPIO_PIN_FUNCTION_OFF);
    gpio_set_pin_function(EINK_CSB, GPIO_PIN_FUNCTION_OFF);
    gpio_set_pin_function(EINK_DC, GPIO_PIN_FUNCTION_OFF);
    gpio_set_pin_function(EINK_RST, GPIO_PIN_FUNCTION_OFF);
    
    gpio_set_pin_function(EINK_SPI_SDA, GPIO_PIN_FUNCTION_OFF);
    gpio_set_pin_function(EINK_SPI_SCL, GPIO_PIN_FUNCTION_OFF);
    
    gpio_set_pin_function(EINK_BUSY, GPIO_PIN_FUNCTION_OFF);
}

/** \name Eink display driver data transfer functions.
 * @{
 */
void eink_write_data(
uint8_t command,
        uint8_t *data,
        uint32_t data_bytes);
        
void eink_read_data(
        uint8_t command,
        uint8_t *data);
        
void eink_send_raw_byte(
        uint8_t send_byte,
        uint8_t f_send);
/** @} */

/** \name Eink flash data transfer functions.
 * @{
 */        
void eink_f_read_data(
        uint8_t command,
        uint8_t *data);
/** @} */

void eink_set_bit(
        uint8_t *byte,
        uint8_t bit_number,
        bool bit_value);

#ifdef __cplusplus
}
#endif

#endif /* EINK_DRIVER_H_ */