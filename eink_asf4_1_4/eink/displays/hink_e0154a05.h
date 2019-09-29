/**
 * \file
 * 
 * \brief HINK-E0154A05 Eink Display Service
 * 
 * Eink Software Library
 * Microchip ASF4 Variant - release 1.4 - October 2019
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
#ifndef HINK_E0154A05_H_
#define HINK_E0154A05_H_

#include <compiler.h>

#include <eink/drivers/eink_driver.h>
#include <eink/drivers/ssd1608/ssd1608.h>

#include <eink/eink_gfx/eink_gfx.h>
#include <eink/eink_gfx/eink_gfx_graphics.h>

#ifdef __cplusplus
extern "C" {
#endif

//! \name Fundamental Display defines
//@{
#define GFX_HINK_E0154A05_MAX_WIDTH              200
#define GFX_HINK_E0154A05_MAX_HEIGHT             200
#define GFX_HINK_E0154A05_PIXELS_PER_BYTE        8
#define GFX_HINK_E0154A05_DISPLAY_BUFFER_SIZE    ((GFX_HINK_E0154A05_MAX_WIDTH * GFX_HINK_E0154A05_MAX_HEIGHT) / GFX_HINK_E0154A05_PIXELS_PER_BYTE)
//@}

//! \name HINK-E0154A05 display buffers 
//@{
volatile uint8_t *hink_e0154a05_display_buffer;
//@}

//! \name HINK-E0154A05 display initialization function
//@{
void eink_hink_e0154a05_init(
        struct ssd1608_config *const config,
        bool clear_display);
//@}

//! \name HINK-E0154A45 display initialization function
//@{
void eink_hink_e0154a45_init(
        struct ssd1608_config *const config,
        bool clear_display);
//@}

//! \name ED0291TC1 display functions
//@{
    
/**
 * \brief Send the refresh command to the display driver.
 */
static inline void eink_hink_e0154a05_refresh_display_buffer(void)
{
    uint8_t eink_data[1];
    uint8_t temperature_value = 20;
    
    if(ssd1608_global_instance.display_config.lut_selection == SSD1608_LUT_REGISTER)
    {
        if(ssd1608_global_instance.display_config.display_part_no == SSD1608_HINK_E0154A05)
            eink_data[0] = 0xC7; /* Register LUT Update - HINK-E0154A05 */
        else
            eink_data[0] = 0xFF; /* Register LUT Update - HINK-E0154A45 */
        
    } else {
        eink_data[0] = 0xF7; /* OTP LUT Update */
    }

    /* Display Update Control 2 */
    eink_write_data(SSD1608_DUC2, eink_data, 1);
    ssd1608_wait_for_busy();
    
    /* Master Activation */
    eink_write_data(SSD1608_MA, 0, 0);
    ssd1608_wait_for_busy();
}

void eink_hink_e0154a05_put_display_buffer(
        bool refresh_display);

void eink_hink_e0154a05_set_pixel(
        eink_x_coordinate x_set,
        eink_y_coordinate y_set,
        enum eink_pixel_colour pixel_colour);

void eink_hink_e0154a05_graphics_load_mono_image(
        uint8_t *img_array,
        uint16_t array_size,
        eink_x_coordinate image_width_px,
        eink_y_coordinate image_height_bytes,
        eink_x_coordinate x_place,
        eink_y_coordinate y_place,
        enum eink_pixel_colour foreground_colour,
        enum eink_pixel_colour background_colour);
        
        
//@}

#ifdef __cplusplus
}
#endif

#endif /* HINK_E0154A05_H_ */