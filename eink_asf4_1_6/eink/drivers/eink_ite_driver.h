/**
 * \file
 * 
 * \brief Eink ITE Display Driver Component
 * 
 * Eink Software Library
 * Microchip ASF4 Variant - release 1.6 - October 2020
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
#ifndef EINK_ITE_DRIVER_H_
#define EINK_ITE_DRIVER_H_

#include <compiler.h>
#include <hal_gpio.h>
#include <hal_delay.h>
#ifdef EINK_HOST_RDY
#include <hal_spi_m_sync.h>
#endif
#include "driver_init.h"

// Controller and Eink configuration file
#include <atmel_start_pins.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Eink ITE Driver Configuration structure.
 *
 * Used in Eink ITE Driver to ensure the correct functionality is used
 * based on the display being used.
 */
struct eink_ite_config {
    void (*wait_for_hrdy)(void);            /**< Pointer to driver wait for busy function. */
#ifdef EINK_HOST_RDY
    struct io_descriptor *spi_io;           /** SPI IO Descriptor */
#endif
};

/**
 * \brief Pointer to the Eink ITE Driver Configuration structure.
 */
struct eink_ite_config *ptr_eink_ite_config;

/**
 * \name Global Driver functions
 * 
 * Global functions linked to the eink_ite_config structure. Ensures correct
 * display is addressed (after being initialized) and gives a nice function
 * to use. 
 */
/**
 * \brief Wait For Host Ready function.
 */
static void eink_ite_wait_for_hrdy(void) {
    ptr_eink_ite_config->wait_for_hrdy();
}

/**
 * \brief Initializes the E Ink + ITE hardware interface.
 *
 * Initializes the display controller hardware interface, regardless
 * of driver IC, interface or display.
 *
 * \note SPI interface is initialized by Atmel START as configuration will
 * be different depending on MCU and SERCOM port being used. 
 */
static void eink_ite_interface_init(void)
{
    /* Set up the Eink interface outputs */
    gpio_set_pin_direction(EINK_RST, GPIO_DIRECTION_OUT);
#ifdef EINK_SRAM_CSB
    gpio_set_pin_direction(EINK_SRAM_CSB, GPIO_DIRECTION_OUT);
#endif
#ifdef EINK_SPI_CSB
    gpio_set_pin_direction(EINK_SPI_CSB, GPIO_DIRECTION_OUT);
#endif

    /* Set up the Eink initial outputs */
#ifdef EINK_SPI_CSB
    gpio_set_pin_level(EINK_SPI_CSB,  1);  /* Set SPI CSB HIGH to disable communication to connected display */
#endif
#ifdef EINK_SRAM_CSB
    gpio_set_pin_level(EINK_SRAM_CSB, 1);  /* Set SRAM CSB HIGH to disable communication to external SRAM */
#endif
    gpio_set_pin_level(EINK_RST, 1);       /* Set RST HIGH since reset is active low */

    /* Set up the HOST READY input */
#ifdef EINK_SPI_CSB
    gpio_set_pin_direction(EINK_HOST_RDY, GPIO_DIRECTION_IN);
    gpio_set_pin_pull_mode(EINK_HOST_RDY, GPIO_PULL_DOWN);
#endif

    /* Setup PORT pin functions */
#ifdef EINK_SPI_CSB
    gpio_set_pin_function(EINK_SPI_CSB, GPIO_PIN_FUNCTION_OFF);
#endif
#ifdef EINK_SRAM_CSB
    gpio_set_pin_function(EINK_SRAM_CSB, GPIO_PIN_FUNCTION_OFF);
#endif
#ifdef EINK_RST
    gpio_set_pin_function(EINK_RST, GPIO_PIN_FUNCTION_OFF);
#endif
#ifdef EINK_HOST_RDY
    gpio_set_pin_function(EINK_HOST_RDY, GPIO_PIN_FUNCTION_OFF);
#endif
}

/** \name Eink display driver data transfer functions.
 * @{
 */
void _eink_ite_write_data(
        uint16_t command,
        uint16_t *data,
        uint32_t data_bytes,
        uint8_t fast_write);

void eink_ite_write_data(
        uint16_t command,
        uint16_t *data,
        uint32_t data_bytes);

void eink_ite_write_data_fast(
        uint16_t command,
        uint16_t *data,
        uint32_t data_bytes);

void eink_ite_read_data(
        uint16_t command,
        uint16_t *data_send,
        uint32_t data_send_bytes,
        uint16_t *data_get,
        uint32_t data_get_bytes);

void eink_ite_send_raw_hword(
        uint16_t send_hword);

void eink_ite_send_raw_hword_fast(
        uint16_t send_hword);

void eink_ite_read_raw_hword(
        uint16_t *read_hword);
/** @} */

void eink_ite_set_bit(
        uint8_t *byte,
        uint8_t bit_number,
        bool bit_value);

#ifdef __cplusplus
}
#endif

#endif /* EINK_ITE_DRIVER_H_ */