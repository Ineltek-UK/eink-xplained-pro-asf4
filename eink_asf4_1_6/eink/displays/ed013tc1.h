/**
 * \file
 * 
 * \brief ED013TC1 Eink Display Service
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
#ifndef ED013TC1_H_
#define ED013TC1_H_

#include <compiler.h>

#include <eink/drivers/eink_driver.h>
#include <eink/drivers/uc8173/uc8173.h>

#include <eink/eink_gfx/eink_gfx.h>
#include <eink/eink_gfx/eink_gfx_graphics.h>

#ifdef __cplusplus
extern "C" {
#endif

//! \name Fundamental Display defines
//@{
#define GFX_ED013TC1_MAX_WIDTH              256
#define GFX_ED013TC1_MAX_HEIGHT             128
#define GFX_ED013TC1_PIXELS_PER_BYTE        8
#define GFX_ED013TC1_DISPLAY_BUFFER_SIZE    ((GFX_ED013TC1_MAX_WIDTH * GFX_ED013TC1_MAX_HEIGHT) / GFX_ED013TC1_PIXELS_PER_BYTE)
//@}

//! \name ED013TC1 display buffers 
//@{
volatile uint8_t *ed013tc1_dtm1_display_buffer, *ed013tc1_dtm2_display_buffer;
//@}

//! \name ED013TC1 display initialization function
//@{
void eink_ed013tc1_init(
        struct uc8173_config *const config,
        bool clear_display);
//@}

//! \name ED013TC1 display functions
//@{
    
/**
 * \brief Send the refresh command to the display driver.
 */
static inline void eink_ed013tc1_refresh_display_buffer(void)
{
    uint8_t eink_data[7];
    
    /* Enable power to the display */   
    eink_write_data(UC8173_PON, 0, 0);
    uc8173_wait_for_busy_low();
    
    uc8173_send_gu2_lut();
	
    /* Display Refresh */
    eink_data[0] = 0x01; /* GU2 Update Mode */
    eink_data[1] = 0x00; /* X = 0 */
    eink_data[2] = 0x00;
    eink_data[3] = 0x00; /* Y = 0 */
    eink_data[4] = 0x7F; /* W = 127 */
    eink_data[5] = 0x00;
    eink_data[6] = 0xFF; /* H = 255 */
    eink_write_data(UC8173_DRF, eink_data, 7);
    
    /* Disable power to the display */
    eink_write_data(UC8173_POF, 0, 0);
    uc8173_wait_for_busy_low();
}
    
void eink_ed013tc1_put_display_buffer(
        bool refresh_display);

void eink_ed013tc1_set_pixel(
        eink_coordinate x_set,
        eink_coordinate y_set,
        enum eink_pixel_colour pixel_colour);

void eink_ed013tc1_graphics_load_mono_image(
        uint8_t *img_array,
        uint16_t array_size,
        eink_coordinate image_width_px,
        eink_coordinate image_height_bytes,
        eink_coordinate x_place,
        eink_coordinate y_place,
        enum eink_pixel_colour foreground_colour,
        enum eink_pixel_colour background_colour);
/*       
void eink_ed028tc1_graphics_load_4bgrey_image(
        uint8_t *img_array,
        uint16_t array_size,
        eink_coordinate image_width_px,
        eink_coordinate image_height_bytes,
        eink_coordinate x_place,
        eink_coordinate y_place);*/
//@}

#ifdef __cplusplus
}
#endif

#endif /* ED013TC1_H_ */