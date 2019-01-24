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
#ifndef EINK_GFX_TEXT_H_
#define EINK_GFX_TEXT_H_

#include <compiler.h>
#include <eink/eink_gfx/eink_gfx.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  \brief Font Endianneess enum.
 *
 *  Describes the endianness of the character bytes.
 */
enum gfx_eink_font_endianness {
    FONT_BIG_ENDIAN,
    FONT_LITTLE_ENDIAN
};

/**
 *  \brief Font Inverted enum.
 */
enum gfx_eink_font_inverted {
    FONT_NORMAL,
    FONT_INVERTED
};

/**
 *  \brief Font Configuration structure.
 *
 *  Stores data about the font.
 * 
 * \note Each character has a set height in pixels but has a variable width
 *       dependent on the letter. This is stored in a separate array that must
 *       be included in the structure.
 *
 * \note The font height is set in pixels, but due to how each character is stored
         the height must be in bytes. Note any blank space in character bytes is ignored.
 */
struct gfx_eink_font {
    uint8_t *font_array_ptr;                       /**< Pointer to the font array */
    uint16_t font_array_size;                      /**< Total size of the font array */
    uint8_t *font_character_size_map_ptr;          /**< Pointer to font width array */
    uint8_t font_starting_ascii_char;              /**< Starting character ASCII value */
    uint8_t font_height_bytes;                     /**< Height of the font in bytes */
    enum gfx_eink_font_endianness font_endianness;
    enum gfx_eink_font_inverted font_inverted;
};

//! \name Eink text functions
//@{
void gfx_eink_text_write_character(
        char character,
        eink_x_coordinate start_x,
        eink_y_coordinate start_y,
        enum eink_pixel_colour text_colour,
        struct gfx_eink_font eink_font);
        
void gfx_eink_text_write_string(
        char *char_array,
        eink_x_coordinate start_x,
        eink_y_coordinate start_y,
        enum eink_pixel_colour text_colour,
        struct gfx_eink_font eink_font);
//@}

#ifdef __cplusplus
}
#endif

#endif /* EINK_GFX_TEXT_H_ */