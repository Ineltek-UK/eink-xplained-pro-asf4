/**
 * \file
 * 
 * \brief Eink Graphics Service
 * 
 * Eink Software Library
 * Microchip ASF4 Variant - release 1.5 - January 2020
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
#ifndef EINK_GFX_GRAPHICS_H_
#define EINK_GFX_GRAPHICS_H_

#include <compiler.h>
#include <eink/eink_gfx/eink_gfx.h>

#ifdef __cplusplus
extern "C" {
#endif

//! \name Eink drawing functions
//@{
void gfx_eink_graphics_fill_screen_raw(
        uint8_t fill_value);
        
void gfx_eink_graphics_fill_screen(
        enum eink_fill_colour fill_type);


void _gfx_eink_graphics_draw_line(
        eink_coordinate x0,
        eink_coordinate y0,
        eink_coordinate x1,
        eink_coordinate y1,
        uint8_t line_raw,
        enum eink_pixel_colour line_colour,
        uint8_t line_value);

void gfx_eink_graphics_draw_line(
        eink_coordinate x1,
        eink_coordinate y1,
        eink_coordinate x2,
        eink_coordinate y2,
        enum eink_pixel_colour line_colour);

void gfx_eink_graphics_draw_line_raw(
        eink_coordinate x0,
        eink_coordinate y0,
        eink_coordinate x1,
        eink_coordinate y1,
        uint8_t line_value);

void gfx_eink_graphics_draw_line(
        eink_coordinate x1,
        eink_coordinate y1,
        eink_coordinate x2,
        eink_coordinate y2,
        enum eink_pixel_colour line_colour);

void gfx_eink_graphics_draw_rect(
        eink_coordinate x0,
        eink_coordinate y0,
        eink_coordinate width,
        eink_coordinate height,
        enum eink_pixel_colour line_colour,
        enum eink_fill_colour fill_colour);

void gfx_eink_graphics_draw_rect_raw(
        eink_coordinate x0,
        eink_coordinate y0,
        eink_coordinate width,
        eink_coordinate height,
        uint8_t line_value,
        enum eink_fill_type fill_type,
        uint8_t fill_value);
//@}


#ifdef __cplusplus
}
#endif

#endif /* EINK_GFX_GRAPHICS_H_ */