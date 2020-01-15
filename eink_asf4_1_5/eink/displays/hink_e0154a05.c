/**
 * \file
 * 
 * \brief HINK-E0154A05 Eink Display Service
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
#include <eink/displays/hink_e0154a05.h>

/**
 * \brief HINK-E0154A05 GFX Library Configuration.
 * Used to load the GFX config structure with HINK-E0154A05 information and map
 * the display specific functions.
 */
struct eink_gfx_config eink_gfx_hink_e0154a05_config = { GFX_HINK_E0154A05, GFX_HINK_E0154A05_MAX_WIDTH, GFX_HINK_E0154A05_MAX_HEIGHT, GFX_HINK_E0154A05_DISPLAY_BUFFER_SIZE, ROTATE_0, 0, 0, eink_hink_e0154a05_refresh_display_buffer, 0, 0, eink_hink_e0154a05_put_display_buffer, 0, eink_hink_e0154a05_set_pixel, 0, eink_hink_e0154a05_graphics_load_mono_image, 0 };
    
/**
 * \brief Initialize SSD1608 controller and Eink display.
 * It will also write the display buffer to all zeros.
 *
 * \note This function will clear the contents of the display if clear_display is set to true
 *
 * \param *config The configuration to write to the display driver.
 * \param clear_display If enabled, the display with be cleared.
 */
void eink_hink_e0154a05_init(struct ssd1608_config *const config, bool clear_display)
{
    uint16_t buffer_index;

    /* Allocated memory for display buffers */
    uint32_t malloc_size = GFX_HINK_E0154A05_DISPLAY_BUFFER_SIZE * sizeof(uint8_t);
    hink_e0154a05_display_buffer = (uint8_t*) malloc (malloc_size);
    eink_gfx_hink_e0154a05_config.display_buffer_1_ptr = hink_e0154a05_display_buffer;
    
    /* Set the display part number */
    config->display_part_no = SSD1608_HINK_E0154A05;

    /* Initialize the low-level display controller. */
    ssd1608_init();

    /* Configure the SSD1608 controller. */
    ssd1608_set_config(config);
    
    /* Set the GFX display rotation */
    eink_gfx_hink_e0154a05_config.display_rotation = config->display_rotation;
        
    /* Set the GFX Library Configuration to HINK_E0154A05 */
    ptr_eink_gfx_config = (struct eink_gfx_config*) &eink_gfx_hink_e0154a05_config;

    /* Set display buffers to all WHITE. */
    for (buffer_index = 0; buffer_index < GFX_HINK_E0154A05_DISPLAY_BUFFER_SIZE; buffer_index++) {
        /* Note that if the memory size is too small, a hard fault will occur here */
        ptr_eink_gfx_config->display_buffer_1_ptr[buffer_index] = 0xFF;
    }
    
    /* Send both display buffers to the display if requested. */
    if(clear_display) eink_hink_e0154a05_put_display_buffer(true);
}

/**
 * \brief Sends the display buffer to the Display and can be
 * refreshed if requested.
 * 
 * Note that only a single bit is required per pixel, where:
 * - 0 : Black
 * - 1 : White
 *
 * \param refresh_display Refresh the display after uploading the display buffers.
 */
void eink_hink_e0154a05_put_display_buffer(bool refresh_display)
{
    uint16_t i;
    uint8_t eink_data[1];
    
    /* Write VCOM value */
    eink_data[0] = 0x7D;
    eink_write_data(SSD1608_WVCOM, eink_data, 1);
    
    /* Set RAM X Start Position */
    eink_data[0] = 0x18;
    eink_data[1] = 0x00;
    eink_write_data(SSD1608_RXAS, eink_data, 2);
    
    /* Set RAM X Address Counter */
    eink_data[0] = 0x18;
    eink_write_data(SSD1608_RXC, eink_data, 1);
    
    /* Set RAM Y Address Counter */
    eink_data[0] = 0xC7;
    eink_data[1] = 0x00;
    eink_write_data(SSD1608_RYC, eink_data, 2);
    
    /* Update the RAM with the display buffer */
    eink_write_data(SSD1608_WR, ptr_eink_gfx_config->display_buffer_1_ptr, GFX_HINK_E0154A05_DISPLAY_BUFFER_SIZE);
    ssd1608_wait_for_busy();
    
    if(refresh_display) {
        /* Send the Refresh Display command */
        eink_hink_e0154a05_refresh_display_buffer();
    }
}

/**
 * \brief Set a specified pixel in the display buffer based an x and a y coordinate.
 *
 * \param x_set X coordinate of the pixel to set.
 * \param y_set Y coordinate of the pixel to set.
 * \param pixel_colour Color to set the pixel to.
 */
void eink_hink_e0154a05_set_pixel(eink_coordinate x, eink_coordinate y, enum eink_pixel_colour pixel_colour)
{    
    eink_coordinate x_set, y_set, max_x, max_y;
    eink_coordinate y1_set, x1_set, byte_set;
    uint8_t pixel_set = 1;
    uint16_t bit_set;
    
    if (pixel_colour == PIXEL_BLACK) pixel_set = 0;
    else if (pixel_colour == PIXEL_WHITE) pixel_set = 1;
    
    switch(ssd1608_global_instance.display_config.display_rotation)
    {
        case ROTATE_0:
            y_set = x;
            x_set = ssd1608_global_instance.display_height - y;
            
            max_x = ssd1608_global_instance.display_height;
            max_y = ssd1608_global_instance.display_width;
            break;
        case ROTATE_90:
            x_set = ssd1608_global_instance.display_width - x;
            y_set = ssd1608_global_instance.display_height - y;
            
            max_x = ssd1608_global_instance.display_width;
            max_y = ssd1608_global_instance.display_height;
            break;
        case ROTATE_180:
            y_set = ssd1608_global_instance.display_width - x;
            x_set = y;
            
            max_x = ssd1608_global_instance.display_height;
            max_y = ssd1608_global_instance.display_width;
            break;
        case ROTATE_270:
            x_set = x;
            y_set = y;
            
            max_x = ssd1608_global_instance.display_width;
            max_y = ssd1608_global_instance.display_height;
            break;
    }
    /* Ignore any pixels being set outside of the display window */
    if ( (x_set >= 0) && (x_set < max_x) && (y_set >= 0) && (y_set < max_y) ) {
        /* Calculate which byte the pixel in question is contained in */
        y1_set = ((y_set - (y_set % 8)) / 8);
        byte_set = (((x_set + 1) * 25) - 1 - y1_set);
        /* Calculate which bit in that byte the pixel in question is */
        bit_set = (y_set % 8);
    
        /* Set the NEW display buffer */
        eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], bit_set, pixel_set);
    }
}

/**
 * \brief Draws a bitmap onto the display canvas.
 *
 * This function draws a bitmap onto the canvas, specified by a width (in px) and height (in bytes),
 * starting at coordinates x and y.
 * 
 * \note The data bytes in the bitmap are translated vertically (non-paged) - bytes should be 8 pixels
 * per byte, big endian.
 * 
 * \param *img_array Pointer to bitmap array.
 * \param array_size Bitmap array size.
 * \param image_width_px Width of the bitmap in px.
 * \param image_height_bytes Height of the bitmap in bytes - therefore bitmap array must have a height
 *                           as a multiple of 8.
 * \param x_place X location to place with bitmap.
 * \param y_place Y location to place with bitmap.
 * \param foreground_colour Color of the foreground - where bit is equal to 1 in the array.
 * \param eink_pixel_colour Color of the background - where bit is equal to 0 in the array.
 */
void eink_hink_e0154a05_graphics_load_mono_image(uint8_t *img_array, uint16_t array_size, eink_coordinate image_width_px, eink_coordinate image_height_bytes, eink_coordinate x_place, eink_coordinate y_place, enum eink_pixel_colour foreground_colour, enum eink_pixel_colour background_colour)
{
    uint16_t buffer_index, i, j;
    uint16_t k, l, m, bit_value;
    
    if(foreground_colour != background_colour) {
        /* Scan pixels by column */
        for (i = 0; i < image_width_px; i++) {
            m = 0;
            for (j = (image_height_bytes); j > 0; j--) {
                /* Process each pixel in byte  */
                l = 7;
                for (k = 0; k < 8; k++) {
                    
                    bit_value = ((img_array[(i*image_height_bytes)+j-1] >> k) & 0x1);
                    if(bit_value) {
                        if(foreground_colour != PIXEL_NONE) gfx_eink_set_pixel((x_place+i), (y_place+(m*8)+k), foreground_colour);
                        } else {
                        if(background_colour != PIXEL_NONE) gfx_eink_set_pixel((x_place+i), (y_place+(m*8)+k), background_colour);
                    }
                    l--;
                }
                m++;
            }
        }
    }
    return;
}