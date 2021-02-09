/**
 * \file
 * 
 * \brief AC040TC1 Eink Display Service
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
#include <eink/displays/ac040tc1.h>

/**
 * \brief AC040TC1 GFX Library Configuration.
 * Used to load the GFX config structure with AC040TC1 information and map
 * the display specific functions.
 */
//eink_ac040tc1_set_pixel, eink_ac040tc1_graphics_load_mono_image
struct eink_gfx_config eink_gfx_ac040tc1_config = { GFX_AC040TC1, GFX_AC040TC1_MAX_WIDTH, GFX_AC040TC1_MAX_HEIGHT, GFX_AC040TC1_DISPLAY_BUFFER_SIZE, ROTATE_0, 0, 0, eink_ac040tc1_refresh_display_buffer, 0, 0, eink_ac040tc1_put_display_buffer, 0, eink_ac040tc1_set_pixel, 0, eink_ac040tc1_graphics_load_mono_image, 0 };
    
/**
 * \brief Initialize UC8159 controller and Eink display.
 * It will also write the display buffers to all zeros.
 *
 * \note This function will clear the contents of the display if clear_display is set to true
 *
 * \param *config The configuration to write to the display driver.
 * \param clear_display If enabled, the display with be cleared.
 */
void eink_ac040tc1_init(struct uc8159_config *const config, bool clear_display)
{
    uint32_t buffer_index;

    /* Allocated memory for display buffers */
    uint32_t malloc_size = GFX_AC040TC1_DISPLAY_BUFFER_SIZE * sizeof(uint8_t);
    ac040tc1_dtm1_display_buffer = (uint8_t*) malloc (malloc_size);
    eink_gfx_ac040tc1_config.display_buffer_1_ptr = ac040tc1_dtm1_display_buffer;

    /* Initialize the low-level display controller. */
    uc8159_init();
    
    /* Ensure the config is set to 7-colours */
    config->panel_settings.display_colours = UC8159_7C;
    /* Set the correct resolution for the AC040TC1 */
    config->panel_settings.display_resolution = RES_640x448;

    /* Configure the UC8159 controller. */
    uc8159_set_config(config);
    
    /* Set the GFX display rotation */
    eink_gfx_ac040tc1_config.display_rotation = config->panel_settings.display_rotation;
    
    /* Set the GFX Library Configuration to AC040TC1 */
    ptr_eink_gfx_config = (struct eink_gfx_config*) &eink_gfx_ac040tc1_config;

    /* Set both display buffers to all WHITE. */
    for (buffer_index = 0; buffer_index < GFX_AC040TC1_DISPLAY_BUFFER_SIZE; buffer_index++) {
        /* Note that if the memory size is too small, a hard fault will occur here */
        ptr_eink_gfx_config->display_buffer_1_ptr[buffer_index] = 0x11;
    }
    
    /* Send both display buffers to the display if requested. */
    if(clear_display) eink_ac040tc1_put_display_buffer(true);
}

/**
 * \brief Sends the display buffer to the Display and can be 
 * refreshed if requested.
 * 
 * Note that 3 bits are required per pixel with a single byte of the data buffer
 * holding 2 pixels in the following format:
 * - 1 1 1 - 2 2 2
 * Bits 3 and 7 are ignored by the controller.
 * 
 * The pixels are configured as follows:
 * - 0b000 : Black
 * - 0b001 : White
 * - 0b010 : Green
 * - 0b011 : Blue
 * - 0b100 : Red
 * - 0b101 : Yellow
 * - 0b110 : Orange
 * - 0b111 : Clean
 *
 * \param refresh_display Refresh the display after uploading the display buffers.
 */
void eink_ac040tc1_put_display_buffer(bool refresh_display)
{
    uint32_t i;
        
    /* Update the DTM1 register with the display buffer */
    eink_write_data(UC8159_DTM1, ptr_eink_gfx_config->display_buffer_1_ptr, GFX_AC040TC1_DISPLAY_BUFFER_SIZE);
    uc8159_wait_for_busy();
    
    if(refresh_display) {
        /* Enable power to the display */
        eink_write_data(UC8159_PON, 0, 0);
        uc8159_wait_for_busy();
        
        /* Send the Refresh Display command */
        eink_write_data(UC8159_DRF, 0, 0);
        uc8159_wait_for_busy();
        
        /* Disable power to the display */
        eink_write_data(UC8159_POF, 0, 0);
        uc8159_wait_for_busy();
    }
}

/**
 * \brief Set a specified pixel in the display buffer based an x and a y coordinate.
 *
 * \param x_set X coordinate of the pixel to set.
 * \param y_set Y coordinate of the pixel to set.
 * \param pixel_colour Color to set the pixel to.
 */
void eink_ac040tc1_set_pixel(eink_coordinate x_set, eink_coordinate y_set, enum eink_pixel_colour pixel_colour)
{
    eink_coordinate x1_set, y1_set, odd_even_bit;
    uint32_t byte_set;
    
    /* Ignore any pixels being set outside of the display window */
    if ( (x_set >= 0) && (x_set < uc8159_global_instance.display_width) && (y_set >= 0) && (y_set < uc8159_global_instance.display_height) ) {
        
        if ( (uc8159_global_instance.panel_settings.display_rotation == ROTATE_90) || (uc8159_global_instance.panel_settings.display_rotation == ROTATE_270) ) {
           /* Calculate which byte the pixel in question is contained in */
           y1_set = (y_set - (y_set % 2)) / 2;
           byte_set = y1_set + ((GFX_AC040TC1_MAX_WIDTH / 2) * x_set);
           odd_even_bit = y_set % 2;
        } else {
            /* Calculate which byte the pixel in question is contained in */
            x1_set = (x_set - (x_set % 2)) / 2;
            byte_set = x1_set + ((GFX_AC040TC1_MAX_WIDTH / 2) * y_set);
            odd_even_bit = x_set % 2;
        }
        if (odd_even_bit) {
            //Set bits 2:0
            switch(pixel_colour) {
                case PIXEL_BLACK:
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 2, 0);
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 1, 0);
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 0, 0);
                    break;
                case PIXEL_WHITE:
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 2, 0);
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 1, 0);
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 0, 1);
                    break;
                case PIXEL_GREEN:
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 2, 0);
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 1, 1);
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 0, 0);
                    break;
                case PIXEL_BLUE:
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 2, 0);
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 1, 1);
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 0, 1);
                    break;
                case PIXEL_RED:
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 2, 1);
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 1, 0);
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 0, 0);
                    break;
                case PIXEL_YELLOW:
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 2, 1);
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 1, 0);
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 0, 1);
                    break;
                case PIXEL_ORANGE:
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 2, 1);
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 1, 1);
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 0, 0);
                    break;
                case PIXEL_CLEAN:
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 2, 1);
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 1, 1);
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 0, 1);
                    break;
            }
        } else {
            //Set bits 6:4
            switch(pixel_colour) {
                case PIXEL_BLACK:
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 6, 0);
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 5, 0);
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 4, 0);
                    break;
                case PIXEL_WHITE:
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 6, 0);
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 5, 0);
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 4, 1);
                    break;
                case PIXEL_GREEN:
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 6, 0);
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 5, 1);
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 4, 0);
                    break;
                case PIXEL_BLUE:
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 6, 0);
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 5, 1);
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 4, 1);
                    break;
                case PIXEL_RED:
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 6, 1);
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 5, 0);
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 4, 0);
                    break;
                case PIXEL_YELLOW:
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 6, 1);
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 5, 0);
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 4, 1);
                    break;
                case PIXEL_ORANGE:
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 6, 1);
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 5, 1);
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 4, 0);
                    break;  
                case PIXEL_CLEAN:
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 6, 1);
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 5, 1);
                    eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 4, 1);
                    break; 
            }
        }
        /**
          * Note that 3 bits are required per pixel with a single byte of the data buffer
          * holding 2 pixels in the following format:
          * - 1 1 1 - 2 2 2
          * Bits 3 and 7 are ignored by the controller.
          *
          * The pixels are configured as follows:
          * - 0b000 : Black
          * - 0b001 : White
          * - 0b010 : Green
          * - 0b011 : Blue
          * - 0b100 : Red
          * - 0b101 : Yellow
          * - 0b110 : Orange
          * - 0b111 : Clean
          */
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
void eink_ac040tc1_graphics_load_mono_image(uint8_t *img_array, uint16_t array_size, eink_coordinate image_width_px, eink_coordinate image_height_bytes, eink_coordinate x_place, eink_coordinate y_place, enum eink_pixel_colour foreground_colour, enum eink_pixel_colour background_colour)
{
    uint32_t buffer_index, i, j;
    uint16_t k, l, m, bit_value;
    
    if(foreground_colour != background_colour) {
        /* Scan pixels by column */
        for (i = 0; i < image_width_px; i++) {
            m = (image_height_bytes-1);
            for (j = 0; j < image_height_bytes; j++) {
                /* Process each pixel in byte  */
                l = 7;
                for (k = 0; k < 8; k++) {
                    
                    bit_value = ((img_array[(i*image_height_bytes)+j] >> k) & 0x1);
                    if(bit_value) {
                        if(foreground_colour != PIXEL_NONE) gfx_eink_set_pixel((x_place+i), (y_place+(m*8)+k), foreground_colour);
                    } else {
                        if(background_colour != PIXEL_NONE) gfx_eink_set_pixel((x_place+i), (y_place+(m*8)+k), background_colour);
                    }
                    l--;
                }
                m--;        
            }
        }
    }
}