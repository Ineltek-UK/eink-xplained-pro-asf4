/**
 * \file
 * 
 * \brief ED037TC1 Eink Display Service
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
#include <eink/displays/ed037tc1.h>

/**
 * \brief ED037TC1 GFX Library Configuration.
 * Used to load the GFX config structure with ED037TC1 information and map
 * the display specific functions.
 */
struct eink_gfx_config eink_gfx_ed037tc1_config = { GFX_ED037TC1, GFX_ED037TC1_MAX_WIDTH, GFX_ED037TC1_MAX_HEIGHT, GFX_ED037TC1_DISPLAY_BUFFER_SIZE, ROTATE_0, 0, 0, eink_ed037tc1_refresh_display_buffer, 0, 0, eink_ed037tc1_put_display_buffer, eink_ed037tc1_put_partial_display_buffer, eink_ed037tc1_set_pixel, 0, eink_ed037tc1_graphics_load_mono_image, 0 };
    
/**
 * \brief Initialize UC8159 controller and Eink display.
 * It will also write the display buffers to all zeros.
 *
 * \note This function will clear the contents of the display if clear_display is set to true
 *
 * \param *config The configuration to write to the display driver.
 * \param clear_display If enabled, the display with be cleared.
 */
void eink_ed037tc1_init(struct ssd1677_config *const config, bool clear_display)
{
    uint32_t buffer_index;

    /* Allocated memory for display buffers */
    uint32_t malloc_size = GFX_ED037TC1_DISPLAY_BUFFER_SIZE * sizeof(uint8_t);
    ed037tc1_dtm1_display_buffer = (uint8_t*) malloc (malloc_size);
    eink_gfx_ed037tc1_config.display_buffer_1_ptr = ed037tc1_dtm1_display_buffer;

    /* Initialize the low-level display controller. */
    ssd1677_init();

    /* Configure the UC8159 controller. */
    ssd1677_set_config(config);
    
    /* Set the GFX display rotation */
    eink_gfx_ed037tc1_config.display_rotation = config->display_rotation;
    
    /* Set the GFX Library Configuration to ED037TC1 */
    ptr_eink_gfx_config = (struct eink_gfx_config*) &eink_gfx_ed037tc1_config;

    /* Set both display buffers to all WHITE. */
    for (buffer_index = 0; buffer_index < GFX_ED037TC1_DISPLAY_BUFFER_SIZE; buffer_index++) {
        /* Note that if the memory size is too small, a hard fault will occur here */
        ptr_eink_gfx_config->display_buffer_1_ptr[buffer_index] = 0xFF;
    }
    
    /* Send both display buffers to the display if requested. */
    if(clear_display) eink_ed037tc1_put_display_buffer(true);
}

/**
 * \brief Sends the display buffer to the Display and can be 
 * refreshed if requested.
 * 
 * Note that 2 bits are required per pixel with a single byte of the data buffer
 * holding 4 pixels in the following format:
 * 1 1 2 2 3 3 4 4
 * 
 * The pixels are configured as follows:
 * - 0b00 : Black
 * - 0b01 : Grey 1
 * - 0b10 : Grey 2
 * - 0b11 : White
 *
 * \param refresh_display Refresh the display after uploading the display buffers.
 */
void eink_ed037tc1_put_display_buffer(bool refresh_display)
{
    uint32_t i;
    uint8_t eink_data[2];
    
    // setting initial counter of X direction in RAM
    eink_data[0] = 0x00;
    eink_data[1] = 0x00;
    eink_write_data(SSD1677_X_ADDRC, eink_data, 2);

    // setting initial counter of Y direction in RAM
    eink_data[0] = 0x00;
    eink_data[1] = 0x00;
    eink_write_data(SSD1677_Y_ADDRC, eink_data, 2);
    
    /* Update the B/W RAM register with the display buffer */
    eink_write_data(SSD1677_RAM_BW, ptr_eink_gfx_config->display_buffer_1_ptr, GFX_ED037TC1_DISPLAY_BUFFER_SIZE);
    
    /* Refresh */
    if(refresh_display) {
        /* Upload update waveform - Note for demo purposes, only 25C waveforms are used */
        if(ssd1677_global_instance.panel_settings.update_mode == UPDATE_SLOW)
        {
            eink_write_data(SSD1677_LUT_REG, ED037TC1_A2_LUT, 105); /* Slow Update */
        } else {
            eink_write_data(SSD1677_LUT_REG, ED037TC1_DU_LUT, 105); /* Fast Update */
        }
        /* Display Update */
        eink_write_data(SSD1677_DSP_ACT, 0, 0);
#ifdef EINK_BUSY
        while(gpio_get_pin_level(EINK_BUSY));
#endif
    }
}

/*
    Update window 
    SSD1677_X_ADDR (0x44) - Specifies start and end address of the update window in the X direction 
    SSD1677_Y_ADDR (0x45) - Specifies start and end address of the update window in the Y direction
        Data[4] = Start[7:0], Start[9:8], End[7:0], End[9:8]
    
    RAM Buffer Address
    SSD1677_X_ADDRC (0x4E) - Specifies location in RAM for starting address counter in the X direction
    SSD1677_Y_ADDRC (0x4F) - Specifies location in RAM for starting address counter in the Y direction
        Data[4] = Address[7:0], Address[9:8]
    */
void eink_ed037tc1_put_partial_display_buffer(eink_coordinate start_x, eink_coordinate start_y, eink_coordinate window_w, eink_coordinate window_h)
{
	uint8_t eink_data[4], mod_calc_x, mod_calc_w;
    uint16_t i, temp_y;
    uint32_t x1_set, y1_set, byte_set;
	eink_coordinate window_set_x, window_set_y, window_set_w, window_set_h;
    
    /* Calculate which byte the pixel in question is contained in */
	if (ssd1677_global_instance.panel_settings.display_rotation == ROTATE_90) {
		y1_set = ((GFX_ED037TC1_MAX_WIDTH / 8) - 1) - (((start_y + window_h) - ((start_y + window_h) % 8)) / 8);
		byte_set = y1_set + ((GFX_ED037TC1_MAX_WIDTH/8) * (start_x));
		mod_calc_x = start_y;		
		
		//(start_y - (start_y % 8))
		//(start_y + (8 - (start_y % 8)))
		
		window_set_x = (GFX_ED037TC1_MAX_WIDTH - start_y - window_h);
		window_set_y = start_x;
		
		window_set_w = window_h / 8;
		window_set_h = window_w;
    } else if (ssd1677_global_instance.panel_settings.display_rotation == ROTATE_180) {
        x1_set = (GFX_ED037TC1_MAX_WIDTH - 1) - ((start_x - (start_x % 8)) / 8);
        byte_set = ( ((GFX_ED037TC1_MAX_HEIGHT - 1) * (GFX_ED037TC1_MAX_WIDTH/8)) - (start_y * (GFX_ED037TC1_MAX_WIDTH/8)) + x1_set );
        mod_calc_x = start_x;
    } else if (ssd1677_global_instance.panel_settings.display_rotation == ROTATE_270) {
        y1_set = (start_y - (start_y % 8)) / 8;
        byte_set = ( ((GFX_ED037TC1_MAX_HEIGHT - 1) * (GFX_ED037TC1_MAX_WIDTH/8)) - (start_y * (GFX_ED037TC1_MAX_WIDTH/8)) + y1_set );
        mod_calc_x = start_y;
    } else{
        x1_set = (start_x - (start_x % 8)) / 8;
        byte_set = x1_set + ((GFX_ED037TC1_MAX_WIDTH/8) * start_y);
        mod_calc_x = (start_x - (start_x % 8));
        mod_calc_w = 8 - (window_w % 8);
		
		window_set_x = start_x;
		window_set_y = start_y;
		
		window_set_w = (window_w + mod_calc_x + mod_calc_w) / 8;
		window_set_h = window_h;
    }
    
    uint8_t* display_buffer;
    display_buffer = (uint8_t*) ptr_eink_gfx_config->display_buffer_1_ptr;
    display_buffer += byte_set;
        
    for(i=0; i<window_set_h; i++)
    {
	    // setting initial counter of X direction in RAM
	    eink_data[0] = (window_set_x & 0xFF);
	    eink_data[1] = (window_set_x >> 8) & 0x3;
	    eink_write_data(SSD1677_X_ADDRC, eink_data, 2);

	    // setting initial counter of Y direction in RAM
	    temp_y = window_set_y + i;
	    eink_data[0] = (temp_y & 0xFF);
	    eink_data[1] = (temp_y >> 8) & 0x3;
	    eink_write_data(SSD1677_Y_ADDRC, eink_data, 2);
	    
	    display_buffer += (GFX_ED037TC1_MAX_WIDTH / 8);
	    
	    /* Update the B/W RAM register with the display buffer */
	    eink_write_data(SSD1677_RAM_BW, display_buffer, window_set_w);
    }
}

/**
 * \brief Set a specified pixel in the display buffer based an x and a y coordinate.
 *
 * \param x_set X coordinate of the pixel to set.
 * \param y_set Y coordinate of the pixel to set.
 * \param pixel_colour Color to set the pixel to.
 */
void eink_ed037tc1_set_pixel(eink_coordinate x_set, eink_coordinate y_set, enum eink_pixel_colour pixel_colour)
{
    uint32_t x1_set, y1_set, bit_set;
    uint32_t byte_set;
    
    /* Ignore any pixels being set outside of the display window */
    if ( (x_set >= 0) && (x_set < ssd1677_global_instance.display_width) && (y_set >= 0) && (y_set < ssd1677_global_instance.display_height) ) {
        
        if (ssd1677_global_instance.panel_settings.display_rotation == ROTATE_90) {
            y1_set = ((GFX_ED037TC1_MAX_WIDTH / 8) - 1) - ((y_set - (y_set % 8)) / 8);
            byte_set = y1_set + ((GFX_ED037TC1_MAX_WIDTH/8) * (x_set));
            bit_set = y_set % 8;
        } else if (ssd1677_global_instance.panel_settings.display_rotation == ROTATE_180) {
            x1_set = (GFX_ED037TC1_MAX_WIDTH - 1) - ((x_set - (x_set % 8)) / 8);
            byte_set = ( ((GFX_ED037TC1_MAX_HEIGHT - 1) * (GFX_ED037TC1_MAX_WIDTH/8)) - (y_set * (GFX_ED037TC1_MAX_WIDTH/8)) + x1_set );
            bit_set = x_set % 8;
        } else if (ssd1677_global_instance.panel_settings.display_rotation == ROTATE_270) {
            y1_set = (y_set - (y_set % 8)) / 8;
            byte_set = ( ((GFX_ED037TC1_MAX_HEIGHT - 1) * (GFX_ED037TC1_MAX_WIDTH/8)) - (x_set * (GFX_ED037TC1_MAX_WIDTH/8)) + y1_set );
            bit_set = 7 - (y_set % 8);
        } else{
            x1_set = (x_set - (x_set % 8)) / 8;
            byte_set = x1_set + ((GFX_ED037TC1_MAX_WIDTH/8) * y_set);
            bit_set = 7 - (x_set % 8);
        }

        switch(pixel_colour) {
            case PIXEL_BLACK:
                eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], bit_set, 0);
                break;
            case PIXEL_WHITE:
                eink_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], bit_set, 1);
                break;
        }
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
void eink_ed037tc1_graphics_load_mono_image(uint8_t *img_array, uint16_t array_size, eink_coordinate image_width_px, eink_coordinate image_height_bytes, eink_coordinate x_place, eink_coordinate y_place, enum eink_pixel_colour foreground_colour, enum eink_pixel_colour background_colour)
{
    uint16_t buffer_index, i, j;
    uint16_t k, l, m, bit_value;
    
    if(foreground_colour != background_colour) {
        /* Scan pixels by column */
        for (i = 0; i < image_width_px; i++) {
            m = 0;
            for (j = image_height_bytes; j > 0; j--) {
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