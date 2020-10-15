/**
 * \file
 * 
 * \brief EL026TR1 Eink Display Service
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
#include <eink/displays/el026tr1.h>

/**
 * \brief EL026TR1 GFX Library Configuration.
 * Used to load the GFX config structure with ED029TC1 information and map
 * the display specific functions.
 */
struct eink_gfx_config eink_gfx_el026tr1_config = { GFX_EL026TR1, GFX_EL026TR1_MAX_WIDTH, GFX_EL026TR1_MAX_HEIGHT, GFX_EL026TR1_DISPLAY_BUFFER_SIZE, ROTATE_0, 0, 0, eink_el026tr1_refresh_display_buffer, 0, 0, eink_el026tr1_put_display_buffer, 0, eink_el026tr1_set_pixel, 0, eink_el026tr1_graphics_load_mono_image, 0 };
    
/**
 * \brief Initialize UC8151 controller and Eink display.
 * It will also write the display buffers to all zeros.
 *
 * \note This function will clear the contents of the display if clear_display is set to true
 *
 * \param *config The configuration to write to the display driver.
 * \param clear_display If enabled, the display with be cleared.
 */
void eink_el026tr1_init(struct uc8151_config *const config, bool clear_display)
{    
    uint16_t buffer_index;

    /* Allocated memory for display buffers */
    uint32_t malloc_size = GFX_EL026TR1_DISPLAY_BUFFER_SIZE * sizeof(uint8_t);
    el026tr1_dtm1_display_buffer = (uint8_t*) malloc (malloc_size);
    el026tr1_dtm2_display_buffer = (uint8_t*) malloc (malloc_size);
    eink_gfx_el026tr1_config.display_buffer_1_ptr = el026tr1_dtm1_display_buffer;
    eink_gfx_el026tr1_config.display_buffer_2_ptr = el026tr1_dtm2_display_buffer;

    /* Initialize the low-level display controller. */
    uc8151_init();
    
    /* Automatically set the EL026TR1 variables */
    config->panel_settings.display_colours = B_W_R_PIXELS;

    /* Configure the UC8151 controller. */
    uc8151_set_config(config);
    
    /* Re-configure the display width and height for the EL026TR1 */
    if((config->panel_settings.display_rotation == ROTATE_90) || (config->panel_settings.display_rotation == ROTATE_270)) {
        uc8151_global_instance.display_width   = 152;
        uc8151_global_instance.display_height  = 296;
    } else {
        uc8151_global_instance.display_width   = 296;
        uc8151_global_instance.display_height  = 152;
    }   
    
    /* Set Partial Window settings - not implemented on the EL026TR1 */
    uc8151_global_instance.partial_window_en = PARTIAL_WINDOW_DISABLED;
    uc8151_global_instance.partial_window_width = 0;
    uc8151_global_instance.partial_window_height = 0;
    
    /* Set the GFX display rotation */
    eink_gfx_el026tr1_config.display_rotation = config->panel_settings.display_rotation;
    
    /* Set the GFX Library Configuration to ED029TC1 */
    ptr_eink_gfx_config = (struct eink_gfx_config*) &eink_gfx_el026tr1_config;

    /* Set both display buffers to all WHITE. */
    for (buffer_index = 0; buffer_index < GFX_EL026TR1_DISPLAY_BUFFER_SIZE; buffer_index++) {
        /* Note that if the memory size is too small, a hard fault will occur here */
        ptr_eink_gfx_config->display_buffer_1_ptr[buffer_index] = 0xFF;
        ptr_eink_gfx_config->display_buffer_1_ptr[buffer_index] = 0xFF;
    }
    
    /* Send both display buffers to the display if requested. */
    if(clear_display) eink_el026tr1_put_display_buffer(true);
}

/**
 * \brief Sends the two display buffers to the Display and can be 
 * refreshed if requested.
 * 
 * For the EL026TR1, DTM1 is used as the B/W display buffer and
 * DTM2 is used as the Red display buffer. The controller doesn't take
 * the previous pixel state into account in this mode.
 *
 * \param refresh_display Refresh the display after uploading the display buffers.
 */
void eink_el026tr1_put_display_buffer(bool refresh_display)
{
    uint16_t i;
    
    /* Enable power to the display */
    eink_write_data(UC8151_PON, 0, 0);
        
    /* Update the DTM1 register with the previous display buffer */
    eink_write_data(UC8151_DTM1, ptr_eink_gfx_config->display_buffer_1_ptr, GFX_EL026TR1_DISPLAY_BUFFER_SIZE);
    eink_write_data(UC8151_DSP, 0, 0);
    uc8151_wait_for_busy();
    
    /* Update the DTM2 register with the display buffer */
    eink_write_data(UC8151_DTM2, ptr_eink_gfx_config->display_buffer_2_ptr, GFX_EL026TR1_DISPLAY_BUFFER_SIZE);
    eink_write_data(UC8151_DSP, 0, 0);
    uc8151_wait_for_busy();
    
    if(refresh_display) {
        /* Send the Refresh Display command */
        eink_write_data(UC8151_DRF, 0, 0);
        uc8151_wait_for_busy();
    }
    
    /* Disable power to the display */
    eink_write_data(UC8151_POF, 0, 0);
    uc8151_wait_for_busy();
}

/**
 * \brief Set a specified pixel in the display buffer based an x and a y coordinate.
 *
 * \param x_set X coordinate of the pixel to set.
 * \param y_set Y coordinate of the pixel to set.
 * \param pixel_colour Color to set the pixel to.
 */
void eink_el026tr1_set_pixel(eink_coordinate x_set, eink_coordinate y_set, enum eink_pixel_colour pixel_colour)
{    
    /** Calls the equivalent function but for the ED029TC1 - avoid code repetition */
    eink_ed029tc1_set_pixel(x_set, y_set, pixel_colour);
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
void eink_el026tr1_graphics_load_mono_image(uint8_t *img_array, uint16_t array_size, eink_coordinate image_width_px, eink_coordinate image_height_bytes, eink_coordinate x_place, eink_coordinate y_place, enum eink_pixel_colour foreground_colour, enum eink_pixel_colour background_colour)
{
    /** Calls the equivalent function but for the ED029TC1 - avoid code repetition */
    eink_ed029tc1_graphics_load_mono_image(img_array, array_size, image_width_px, image_height_bytes, x_place, y_place, foreground_colour, background_colour);
}