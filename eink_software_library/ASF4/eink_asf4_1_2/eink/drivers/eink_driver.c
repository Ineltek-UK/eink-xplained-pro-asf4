/**
 * \file
 * 
 * \brief Eink Display Driver Component
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
#include <eink/drivers/eink_driver.h>

/**
 * \brief Writes a command and associated data to the Eink display driver 
 *
 * Sends data over the SPI interface, initially with a command followed by
 * a specified number of data bytes. Ensures the SPI interface is correctly
 * set before sending raw data.
 *
 * \param command Command byte to write.
 * \param *data Pointer to data array to write.
 * \param data_bytes Length of data array.
 */
void eink_write_data(
        uint8_t command,
        uint8_t *data,
        uint32_t data_bytes)
{    
    uint32_t i;
    
    /* Set the DC output for command first */
    gpio_set_pin_level(EINK_DC, 0); /* Command */
    
    /* Send the command */
    eink_send_raw_byte(command, 0);
    /* Set the DC output for data since is the default output anyway */
    gpio_set_pin_level(EINK_DC, 1); /* Data */
    
    /* Loop through array of data and send each byte individually */
    if (data_bytes != 0) {
        for (i=0;i<data_bytes;i++) {
            eink_send_raw_byte(*data, 0);
            data++;
        }
    }
}

/**
 * \brief Writes a command and read returned data byte from the Eink display driver 
 *
 * Sends data over the SPI interface, initially with a command followed by
 * a read of the returned data. Ensures the SPI interface is correctly
 * set before sending raw data. Switches the DATA line to an input ready for
 * receive operation. Returns the DATA line to an output once complete.
 *
 * \param command Command byte to write.
 * \param *data Pointer to data byte to read into.
 */
void eink_read_data(
        uint8_t command,
        uint8_t *data)
{    
    uint16_t i;
    
    *data = 0;
    
    /* Set the DC output for command first */
    gpio_set_pin_level(EINK_DC, 0); /* Command */
    
    /* Send the command */
    eink_send_raw_byte(command, 0);
    /* Set the DC output for data */
    gpio_set_pin_level(EINK_DC, 1); /* Data */
    
    /* Switch SDA pin to input */
    gpio_set_pin_direction(EINK_SPI_SDA, GPIO_DIRECTION_IN);
    gpio_set_pin_pull_mode(EINK_SPI_SDA, GPIO_PULL_UP);
    
    /* Set CSB LOW to enable communication to the display */
    gpio_set_pin_level(EINK_CSB, 0);
    /* Loop through to read each bit of our returned data byte */
    for (i=0;i<8;i++) {
        gpio_set_pin_level(EINK_SPI_SCL, 1);
        if (gpio_get_pin_level(EINK_SPI_SDA)) {
            *data |= 1;
        }
        gpio_set_pin_level(EINK_SPI_SCL, 0);
        if(i!=7) *data = *data << 1;
    }
    /* Set CSB HIGH to end communication to the display */
    gpio_set_pin_level(EINK_CSB, 1);
    
    /* Switch SDA pin back to output */
    gpio_set_pin_direction(EINK_SPI_SDA, GPIO_DIRECTION_OUT);
}


/**
 * \brief Writes raw data to the Eink display driver 
 *
 * Writes raw SPI data bits to the selected Eink display driver along  
 * with a clock. Also sets CSB pin to enable data transmission. Note 
 * that no other SPI pins are changed. For correct SPI transmission, use
 * eink_write_data() or eink_read_data().
 *
 * \param send_byte Data byte to write.
 */
void eink_send_raw_byte(
        uint8_t send_byte,
        uint8_t f_send)
{    
    uint8_t i;
    
    if(!f_send)
        /* Set CSB LOW to enable communication to the display */
        gpio_set_pin_level(EINK_CSB, 0);
    /* Loop through each bit of our byte to send */
    for (i=0;i<8;i++) {
        if (send_byte & 0x80) {
            gpio_set_pin_level(EINK_SPI_SDA, 1);
        } else {
            gpio_set_pin_level(EINK_SPI_SDA, 0);
        }
        gpio_set_pin_level(EINK_SPI_SCL, 1);
        send_byte=send_byte << 1;
        gpio_set_pin_level(EINK_SPI_SCL, 0);
    }
    if(!f_send)
        /* Set CSB HIGH to end communication to the display */
        gpio_set_pin_level(EINK_CSB, 1);
}

/**
 * \brief Sets a specified bit within a byte
 *
 * \param *byte Pointer to the byte to change.
 * \param bit_number Which bit to modifiy.
 * \param bit_value Bit value.
 */
void eink_set_bit(uint8_t *byte, uint8_t bit_number, bool bit_value)
{
    /* Update the byte to either set or clear the selected bit */   
    if (bit_value == 1) {
        *byte |= 1 << bit_number;
    } else {
        *byte &= ~(1 << bit_number);
    }
}