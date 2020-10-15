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
#include <eink/drivers/eink_ite_driver.h>

/**
 * \brief Writes a command and associated data to the ITE display driver 
 *
 * Sends data over the SPI interface, initially with a command followed by
 * a specified number of data bytes. Ensures the SPI interface is correctly
 * set before sending raw data.
 *
 * \param command Command byte to write.
 * \param *data Pointer to data array to write.
 * \param data_bytes Length of data array
 * \param fast_write Whether or not to do a fast write.
 */
void _eink_ite_write_data(
        uint16_t command,
        uint16_t *data,
        uint32_t data_bytes,
        uint8_t fast_write)
{   
    uint32_t i, j, sendWords_m, sendWords_r;
    
    eink_ite_wait_for_hrdy();
    if (command != 0) {
        /* Set CSB LOW to enable communication to the display */
#ifdef EINK_SPI_CSB
        gpio_set_pin_level(EINK_SPI_CSB, 0);
#endif

        /* Send Preamble for write */
        eink_ite_send_raw_hword(0x6000);

        /* Send Command */
        eink_ite_wait_for_hrdy();
        eink_ite_send_raw_hword(command);
#ifdef EINK_SPI_CSB
        /* Set CSB LOW to enable communication to the display */
        gpio_set_pin_level(EINK_SPI_CSB, 1);
#endif
    }
    
    eink_ite_wait_for_hrdy();
    if (data_bytes != 0) {
        /* Data burst must be sent 1024 words at a time */
        sendWords_r = data_bytes % 2048;
        sendWords_m = (data_bytes - sendWords_r) / 2048;
        
        /* If we have more than 1024 words, start sending them in bursts */
        for(j=0; j<sendWords_m; j++)
        {
            /* Set CSB LOW to enable communication to the display */
#ifdef EINK_SPI_CSB
            gpio_set_pin_level(EINK_SPI_CSB, 0);
#endif
            eink_ite_send_raw_hword(0x0000);
            for (i=0; i<2048; i++)
            {
                if(fast_write) eink_ite_send_raw_hword(*data);
                else eink_ite_send_raw_hword(*data);
                data++;
            }           
            /* Set CSB LOW to enable communication to the display */
#ifdef EINK_SPI_CSB
            gpio_set_pin_level(EINK_SPI_CSB, 1);
#endif
            eink_ite_wait_for_hrdy();
        }

        /* Set CSB LOW to enable communication to the display */
#ifdef EINK_SPI_CSB
        gpio_set_pin_level(EINK_SPI_CSB, 0);
#endif
        eink_ite_send_raw_hword(0x0000);
        /* Send the remaining data (or all the data if there was less than 1024 words */
        for (i=0; i<sendWords_r; i++) {
            if(fast_write) eink_ite_send_raw_hword_fast(*data);
            else eink_ite_send_raw_hword_fast(*data);
            data++;
        }
        /* Set CSB LOW to enable communication to the display */
#ifdef EINK_SPI_CSB
        gpio_set_pin_level(EINK_SPI_CSB, 1);
#endif
        eink_ite_wait_for_hrdy();
    }
}

/**
 * \brief Writes a command and associated data to the ITE display driver 
 *
 * Sends data over the SPI interface, initially with a command followed by
 * a specified number of data bytes. Ensures the SPI interface is correctly
 * set before sending raw data.
 *
 * \param command Command byte to write.
 * \param *data Pointer to data array to write.
 * \param data_bytes Length of data array
 */
void eink_ite_write_data(
        uint16_t command,
        uint16_t *data,
        uint32_t data_bytes)
{
    _eink_ite_write_data(command, data, data_bytes, 0);
}

/**
 * \brief Writes a command and associated data to the ITE display driver, avoids
 * some checks and pauses to ensure data is sent quickly - not currently implemented.
 *
 * Sends data over the SPI interface, initially with a command followed by
 * a specified number of data bytes. Ensures the SPI interface is correctly
 * set before sending raw data.
 *
 * \param command Command byte to write.
 * \param *data Pointer to data array to write.
 * \param data_bytes Length of data array
 */
void eink_ite_write_data_fast(
        uint16_t command,
        uint16_t *data,
        uint32_t data_bytes)
{
    _eink_ite_write_data(command, data, data_bytes, 1);
}

/**
 * \brief Writes a command and read returned data byte from the ITE display driver 
 *
 * Sends data over the SPI interface, initially with a command followed by
 * a read of the returned data. Ensures the SPI interface is correctly
 * set before sending raw data. Switches the DATA line to an input ready for
 * receive operation. Returns the DATA line to an output once complete.
 *
 * \param command Command byte to write.
 * \param *data Pointer to data byte to read into.
 */
void eink_ite_read_data(
        uint16_t command,
        uint16_t *data_send,
        uint32_t data_send_bytes,
        uint16_t *data_get,
        uint32_t data_get_bytes)
{
    uint16_t w_preamble, read_hword;
    uint32_t i, j;
    
    /* Set CSB LOW to enable communication to the display */
#ifdef EINK_SPI_CSB
    gpio_set_pin_level(EINK_SPI_CSB, 0);
#endif

    /* Send Preamble for write */
    eink_ite_wait_for_hrdy();
    eink_ite_send_raw_hword(0x6000);

    /* Send Command */
    eink_ite_wait_for_hrdy();
    eink_ite_send_raw_hword(command);
    
    /* Set CSB LOW to enable communication to the display */
#ifdef EINK_SPI_CSB
    gpio_set_pin_level(EINK_SPI_CSB, 1);
#endif
    
    if (data_send_bytes != 0) {
        eink_ite_wait_for_hrdy();
        /* Set CSB LOW to enable communication to the display */
#ifdef EINK_SPI_CSB
        gpio_set_pin_level(EINK_SPI_CSB, 0);
#endif
        
        eink_ite_send_raw_hword(0x0000);
        
        for (i=0;i<data_send_bytes;i++) {
            eink_ite_send_raw_hword(*data_send);
            data_send++;
        }
        /* Set CSB LOW to enable communication to the display */
#ifdef EINK_SPI_CSB
        gpio_set_pin_level(EINK_SPI_CSB, 1);
#endif
    }
    
    eink_ite_wait_for_hrdy();
    /* Set CSB LOW to enable communication to the display */
#ifdef EINK_SPI_CSB
    gpio_set_pin_level(EINK_SPI_CSB, 0);
#endif
    
    /* Send Preamble for read */
    eink_ite_wait_for_hrdy();
    eink_ite_send_raw_hword(0x1000);
    
    /* Read Dummy (under IT8951 SPI to I80 spec) */
    eink_ite_wait_for_hrdy();
    eink_ite_read_raw_hword(&read_hword);
    
    /* Read Data */
    for (j=0;j<data_get_bytes;j++) {
        eink_ite_wait_for_hrdy();
        eink_ite_read_raw_hword((uint16_t *)data_get);
        data_get++;
    }

    /* Set CSB HIGH to end communication to the display */
#ifdef EINK_SPI_CSB
    gpio_set_pin_level(EINK_SPI_CSB, 1);
#endif
}

/**
 * \brief Writes raw data to the ITE display driver
 *
 * \note Function doesn't control CSB, for correct SPI transmission, use
 * eink_write_data() or eink_read_data().
 *
 * \param send_byte Data word to write.
 */
void eink_ite_send_raw_hword(
        uint16_t send_hword)
{   
#ifdef EINK_HOST_RDY    
    uint8_t send_data[2] = { ((send_hword >> 8) & 0xFF), (send_hword & 0xFF) };
    io_write(ptr_eink_ite_config->spi_io, send_data, 2);
#endif
}

/**
 * \brief Writes raw data to the ITE display driver using a modified HAL function
 * in order to improve speed of transmission
 *
 * \note Function doesn't control CSB, for correct SPI transmission, use
 * eink_write_data() or eink_read_data().
 *
 * \param send_byte Data word to write.
 */
void eink_ite_send_raw_hword_fast(
        uint16_t send_hword)
{   
#ifdef EINK_HOST_RDY    
    uint8_t send_data[2] = { ((send_hword >> 8) & 0xFF), (send_hword & 0xFF) };
    io_write(ptr_eink_ite_config->spi_io, send_data, 2);
#endif
}

/**
 * \brief Reads raw data from the ITE display driver
 *
 * \note Function doesn't control CSB, for correct SPI transmission, use
 * eink_write_data() or eink_read_data().
 *
 * \param read_byte Data word to read to.
 */
void eink_ite_read_raw_hword(
        uint16_t *read_hword)
{
    uint8_t read_data[2];
    
#ifdef EINK_HOST_RDY
    io_read(ptr_eink_ite_config->spi_io, read_data, 2);
    *read_hword = (((uint16_t) read_data[0] << 8) & 0xFF00) | (read_data[1] & 0xFF);
#endif
}

/**
 * \brief Sets a specified bit within a byte
 *
 * \param *byte Pointer to the byte to change.
 * \param bit_number Which bit to modifiy.
 * \param bit_value Bit value.
 */
void eink_ite_set_bit(uint8_t *byte, uint8_t bit_number, bool bit_value)
{
    /* Update the byte to either set or clear the selected bit */   
    if (bit_value == 1) {
        *byte |= 1 << bit_number;
    } else {
        *byte &= ~(1 << bit_number);
    }
}