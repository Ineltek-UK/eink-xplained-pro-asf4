/**
 * \file
 * 
 * \brief Eink Text Service
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
#include <eink/eink_gfx/eink_gfx_text.h>

/**
 * \brief Writes a character at a specified coordinate.
 *
 * \param character Character to write.
 * \param start_x X coordinate to write character.
 * \param start_y Y coordinate to write character.
 * \param text_colour Color of character.
 * \param eink_font Font structure to use. 
 */
void gfx_eink_text_write_character(char character, eink_x_coordinate start_x, eink_y_coordinate start_y, enum eink_pixel_colour text_colour, struct gfx_eink_font eink_font)
{
    volatile uint16_t k, bit_value;
    volatile uint32_t current_char_index;
    volatile uint32_t start_font_index, end_font_index, i, j;
    volatile eink_x_coordinate current_x, send_x;
    volatile eink_y_coordinate current_y, send_y;
    
    volatile uint8_t *font_character_size_map_ptr;
    volatile uint8_t *font_array_ptr;
    
    current_x = start_x;
    current_y = start_y;
    
    /* Get the index of the start of the character in the font array */
    start_font_index = 0;
    font_character_size_map_ptr = eink_font.font_character_size_map_ptr;
    current_char_index = character - eink_font.font_starting_ascii_char;
    for(i = 0; i < current_char_index; i++) {
        start_font_index += (*font_character_size_map_ptr * eink_font.font_height_bytes);
        font_character_size_map_ptr++;
    }
    
    /* Ensure the chosen character is still in the font array */
    if( (start_font_index + (*font_character_size_map_ptr * eink_font.font_height_bytes) ) < eink_font.font_array_size) {
        
        /* Move the pointer to the character starting byte */
        font_array_ptr = (uint8_t*) eink_font.font_array_ptr;
        
        if (eink_font.font_endianness == FONT_BIG_ENDIAN) font_array_ptr += start_font_index;
        else font_array_ptr += ((start_font_index - 1) + (sizeof(uint8_t) * (eink_font.font_height_bytes)));
    
       /**
        * Loop horizontally through the columns of the font first,
        * then loop vertically through each row of the font (bit by bit)
        */
        j = 0;
        end_font_index = start_font_index + (*font_character_size_map_ptr * eink_font.font_height_bytes) - 1;
        for (i = start_font_index; i <= end_font_index; i++) {
            if (eink_font.font_endianness == FONT_BIG_ENDIAN) {
                for (k = 0; k < 8; k++) {
                    bit_value = ((*font_array_ptr >> k) & 0x1);
                    send_x = current_x;
                    send_y = (current_y + k);
                    if ( ( (bit_value == 1) && (eink_font.font_inverted == FONT_NORMAL) ) || ( (bit_value == 0) && (eink_font.font_inverted == FONT_INVERTED) ) ) {
                        gfx_eink_set_pixel(send_x, send_y, text_colour);
                    }
                }
            } else {
                for (k = 8; k > 0; k--) {
                    bit_value = ((*font_array_ptr >> (k-1)) & 0x1);
                    send_x = current_x;
                    send_y = (current_y + (k-1));
                    if ( ( (bit_value == 1) && (eink_font.font_inverted == FONT_NORMAL) ) || ( (bit_value == 0) && (eink_font.font_inverted == FONT_INVERTED) ) ) {
                        gfx_eink_set_pixel(send_x, send_y, text_colour);
                    }
                }
                /*for (k = 0; k < 8; k++) {
                    bit_value = ((*font_array_ptr >> k) & 0x1);
                    send_x = current_x;
                    send_y = (current_y + k);
                    if ( ( (bit_value == 1) && (eink_font.font_inverted == FONT_NORMAL) ) || ( (bit_value == 0) && (eink_font.font_inverted == FONT_INVERTED) ) ) {
                        gfx_ed029tc1_set_pixel(send_x, send_y, text_colour);
                    }
                }*/
            }

            
            j++;
            current_y += 8;
            if (eink_font.font_endianness == FONT_BIG_ENDIAN) font_array_ptr += sizeof(uint8_t);
            else font_array_ptr -= sizeof(uint8_t);
            
            if(j == eink_font.font_height_bytes) {
                j = 0;
                if (eink_font.font_endianness == FONT_BIG_ENDIAN) current_y = start_y;
                else current_y = start_y;
                
                if (eink_font.font_endianness == FONT_LITTLE_ENDIAN) font_array_ptr += (sizeof(uint8_t) * 2 * eink_font.font_height_bytes);
                
                current_x++;
            }
        }
    }
}

/**
 * \brief Write a string of data starting at (x,y)
 *
 * \param *char_array Pointer to array of charaters - end with a null char.
 * \param start_x X coordinate to start writing.
 * \param start_y Y coordinate to start writing.
 * \param text_colour Color of character.
 * \param eink_font Font structure to use.
 */
void gfx_eink_text_write_string(char *char_array, eink_x_coordinate start_x, eink_y_coordinate start_y, enum eink_pixel_colour text_colour, struct gfx_eink_font eink_font)
{
    uint8_t i = 0;
    eink_x_coordinate x_send = start_x;
    
    while(char_array[i] != '\0')
    {
        gfx_eink_text_write_character(char_array[i], x_send, start_y, text_colour, eink_font);
            
        x_send += eink_font.font_character_size_map_ptr[char_array[i]-eink_font.font_starting_ascii_char];
        i++;        
    }
}