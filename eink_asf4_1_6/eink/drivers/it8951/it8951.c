/**
 * \file
 * 
 * \brief IT8951 Eink Driver Component
 * 
 * Eink Software Library
 * Microchip ASF4 Variant - release 1.6 - July 2020
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
#include <eink/drivers/it8951/it8951.h>

/**
 * \brief IT8951 GFX Library Configuration.
 * Used to load the GFX config structure with IT8951 information and map
 * the display specific functions.
 */
struct eink_gfx_config eink_gfx_it8951_config = { GFX_IT8951, 0, 0, 0, ROTATE_0, 0, 0, eink_it8951_refresh_display, eink_it8951_refresh_display_mode, eink_it8951_refresh_partial_display, eink_it8951_put_display_buffer, eink_it8951_put_partial_display_buffer, 0, eink_it8951_set_pixel_raw, 0, eink_it8951_graphics_load_4bgrey_image };

/**
 * \brief IT8951 Display Driver Configuration.
 * Used to load the Display Driver config structure with IT8951 information and map
 * the display specific functions.
 */
struct eink_ite_config eink_ite_it8951_config = {it8951_wait_for_hrdy, 0};

/**
 * \brief Initialize IT8951 controller and Eink display.
 * It will also write the display buffers to all zeros.
 *
 * \note This function will clear the contents of the display if clear_display is set to true
 *
 * \param *config The configuration to write to the display driver.
 * \param clear_display If enabled, the display with be cleared.
 */
void eink_it8951_init(enum eink_display_rotation d_rotation, bool clear_display)
{    
    uint32_t buffer_index;
    
    /* Initialize delay routine */
    delay_init(SysTick);

    /* Initialize the interface */
    eink_ite_interface_init();

    /* Reset display */
    it8951_hard_reset();
    
    /* Wait for BUSY output to return HIGH */
    it8951_wait_for_hrdy();
    
    /* Set the Driver Configuration to IT8951 */
    ptr_eink_ite_config = (struct eink_ite_config*) &eink_ite_it8951_config;

    /* Set the GFX Library Configuration to ED043WC3 */
    ptr_eink_gfx_config = (struct eink_ite_gfx_config*) &eink_gfx_it8951_config;
    
    /* Enable SPI interface */
#ifdef EINK_HOST_RDY
    spi_m_sync_get_io_descriptor(&SPI_0, &ptr_eink_ite_config->spi_io);
    spi_m_sync_enable(&SPI_0);
#endif

    /* Get the IT8951 configuration. */
    eink_ite_read_data(IT8951_I80_USR_GET_DEV_INFO, 0, 0, &it8951_global_instance.display_config, sizeof(struct it8951_config)/2);
    
    /* Set the global instance variables */
    it8951_global_instance.display_width = it8951_global_instance.display_config.display_height;
    it8951_global_instance.display_height = it8951_global_instance.display_config.display_width;
    it8951_global_instance.display_rotation = d_rotation;
    it8951_global_instance.img_buffer_addr = it8951_global_instance.display_config.img_buffer_addr_l | (it8951_global_instance.display_config.img_buffer_addr_h << 16);
    it8951_global_instance.display_buffer_length = ((it8951_global_instance.display_width * it8951_global_instance.display_height) / GFX_IT8951_PIXELS_PER_BYTE);

    /* Allocated memory for display buffers */
    uint32_t malloc_size = it8951_global_instance.display_buffer_length * sizeof(uint8_t);
    it8951_dtm_display_buffer = (uint8_t*) malloc (malloc_size);
    eink_gfx_it8951_config.display_buffer_1_ptr = it8951_dtm_display_buffer;
    eink_gfx_it8951_config.display_buffer_size = it8951_global_instance.display_buffer_length;

    /* Set the GFX display rotation */
    eink_gfx_it8951_config.display_rotation = d_rotation;
    
    /* Ensure the GFX library has the correct width and height based on rotation */
    if((d_rotation == ROTATE_0) || (d_rotation == ROTATE_180))
    {
        ptr_eink_gfx_config->display_width = it8951_global_instance.display_width;
        ptr_eink_gfx_config->display_height = it8951_global_instance.display_height;
    } else {
        ptr_eink_gfx_config->display_height = it8951_global_instance.display_width;
        ptr_eink_gfx_config->display_width = it8951_global_instance.display_height;
    }

    /* Set both display buffers to all WHITE. */
    for (buffer_index = 0; buffer_index < it8951_global_instance.display_buffer_length; buffer_index++) {
        /* Note that if the memory size is too small, a hard fault will occur here */
        ptr_eink_gfx_config->display_buffer_1_ptr[buffer_index] = 0xFF;
    }
    
    /* Send both display buffers to the display if requested. */
    if(clear_display) eink_it8951_put_display_buffer(true);
}

/**
 * \brief Sends the display buffer to the Display and can be 
 * refreshed if requested.
 *
 * \param refresh_display if true, the entire display will be refreshed
 */
void eink_it8951_put_display_buffer(bool refresh_display)
{
    uint16_t eink_data, rotation = 0;
    uint16_t* display_buffer;
    eink_coordinate j;

    /* Set Image Buffer Base address (IT8951) */
    uint16_t usWordH = (unsigned short)((it8951_global_instance.img_buffer_addr >> 16) & 0x0000FFFF);
    uint16_t usWordL = (unsigned short)( it8951_global_instance.img_buffer_addr & 0x0000FFFF);
    it8951_write_reg(LISAR + 2, usWordH);
    it8951_write_reg(LISAR, usWordL);
    
    /* Set arguments for Load Image Start */
    switch(it8951_global_instance.display_rotation)
    {
        case ROTATE_0: rotation = 0; break;
        case ROTATE_90: rotation = 1; break;
        case ROTATE_180: rotation = 2; break;
        case ROTATE_270: rotation = 3; break;
    }
    eink_data = (0 << 8) | (2 << 4) | (rotation);
    eink_ite_write_data(IT8951_I80_LD_IMG, &eink_data, 1);
    
    /* Get pointer to Display Buffer */
    display_buffer = (uint16_t*) it8951_dtm_display_buffer;
    
    /* Write pixels to Frame Buffer */
    eink_ite_write_data_fast(0, display_buffer, (it8951_global_instance.display_buffer_length/2));
    
    /* Send Load Img End Command */
    eink_ite_write_data(IT8951_I80_LD_IMG_END, 0, 0);

    if(refresh_display) eink_it8951_refresh_display();
}

/**
 * \brief Sends a partial area of the display buffer to the Display and can
 * be refreshed if requested.
 *
 * \param start_x starting X coordinate start of the display buffer to send
 * \param start_y starting Y coordinate start of the display buffer to send
 * \param window_w width of the display buffer to send
 * \param window_h height of the display buffer to send
 */
void eink_it8951_put_partial_display_buffer(eink_coordinate start_x, eink_coordinate start_y, eink_coordinate window_w, eink_coordinate window_h)
{
    uint16_t eink_data[5], rotation = 0;
    uint16_t* display_buffer;
    uint32_t i, j, k;

    /* Get pointer to Display Buffer */
    display_buffer = (uint16_t*) it8951_dtm_display_buffer;
        
    /* Set Image Buffer Base address (IT8951) */
    uint16_t usWordH = (unsigned short)((it8951_global_instance.img_buffer_addr >> 16) & 0x0000FFFF);
    uint16_t usWordL = (unsigned short)( it8951_global_instance.img_buffer_addr & 0x0000FFFF);
    it8951_write_reg(LISAR + 2, usWordH);
    it8951_write_reg(LISAR, usWordL);
    
    /* Set arguments for Load Image Start */
    switch(it8951_global_instance.display_rotation)
    {
        case ROTATE_0: rotation = 0; break;
        case ROTATE_90: rotation = 1; break;
        case ROTATE_180: rotation = 2; break;
        case ROTATE_270: rotation = 3; break;
    }
    eink_data[0] = (0 << 8) | (2 << 4) | (rotation);
    eink_data[1] = start_y;
    eink_data[2] = start_x;
    eink_data[3] = window_h;
    eink_data[4] = window_w;
    eink_ite_write_data(IT8951_I80_LD_IMG_AREA, eink_data, 5);
    
    /* Move to start of the window within the display buffer */
    if((it8951_global_instance.display_rotation == ROTATE_0) || (it8951_global_instance.display_rotation == ROTATE_180))
        display_buffer += (it8951_global_instance.display_height / 4) * (start_x);
    else
        display_buffer += (it8951_global_instance.display_width / 4) * (start_x);
    display_buffer += (start_y / 4);
    
    /* Loop through window sending the display buffer */
    for(j = 0; j < window_w; j++)
    {
        eink_ite_write_data(0, display_buffer, (window_h / 4));
        display_buffer += (window_h / 4);
        
        if((it8951_global_instance.display_rotation == ROTATE_0) || (it8951_global_instance.display_rotation == ROTATE_180))
            display_buffer += (it8951_global_instance.display_height / 4) - (window_h / 4);
        else
            display_buffer += (it8951_global_instance.display_width / 4) - (window_h / 4);
    }
    
    /* Send Load Img End Command */
    eink_ite_write_data(IT8951_I80_LD_IMG_END, 0, 0);
}

/**
 * \brief Set a specified pixel in the display buffer based an x and a y coordinate.
 *
 * \param x_set X coordinate of the pixel to set.
 * \param y_set Y coordinate of the pixel to set.
 * \param pixel_value Greyscale value to set the pixel to.
 */
void eink_it8951_set_pixel_raw(eink_coordinate x_set, eink_coordinate y_set, uint8_t pixel_value)
{
    eink_coordinate x1_set, y1_set, odd_even_bit;
    uint32_t byte_set;
    
    /* Ignore any pixels being set outside of the display window */
    if ( (x_set >= 0) && (x_set < ptr_eink_gfx_config->display_width) && (y_set >= 0) && (y_set < ptr_eink_gfx_config->display_height) ) {
        
        if ( (it8951_global_instance.display_rotation == ROTATE_0) || (it8951_global_instance.display_rotation == ROTATE_180) ) {
            /* Calculate which byte the pixel in question is contained in */
            y1_set = (y_set - (y_set % 2)) / 2;
            byte_set = y1_set + ((it8951_global_instance.display_config.display_width / 2) * x_set);
            odd_even_bit = y_set % 2;
        } else {
            /* Calculate which byte the pixel in question is contained in */
            y1_set = (y_set - (y_set % 2)) / 2;
            byte_set = y1_set + ((it8951_global_instance.display_config.display_height / 2) * x_set);
            odd_even_bit = y_set % 2;
        }
        
        if (!odd_even_bit) {
            //Set bits 2:0
            eink_ite_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 3, ((pixel_value >> 3) & 0x1));
            eink_ite_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 2, ((pixel_value >> 2) & 0x1));
            eink_ite_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 1, ((pixel_value >> 1) & 0x1));
            eink_ite_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 0, (pixel_value & 0x1));
        } else {
           eink_ite_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 7, ((pixel_value >> 3) & 0x1));
           eink_ite_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 6, ((pixel_value >> 2) & 0x1));
           eink_ite_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 5, ((pixel_value >> 1) & 0x1));
           eink_ite_set_bit(&ptr_eink_gfx_config->display_buffer_1_ptr[byte_set], 4, (pixel_value & 0x1));
        }
    }
}

/**
 * \brief Load a bitmap into the display buffer.
 *
 * This function simply copies an array of data directly into the appropriate display buffers
 * (depending on color) as specified by a width (in px) and height (in bytes), starting at
 * coordinates x and y.
 * 
 * \note The data bytes in the buffer are translated vertically (non-paged) with byte 0 being 
 * equal to the first 8 pixels in the top left corner of the display - bytes should be Big Endian.
 * 
 * \param *img_array Pointer to bitmap array.
 * \param array_size Bitmap array size.
 * \param image_width_px Width of the bitmap in px.
 * \param image_height_bytes Height of the bitmap in bytes - therefore bitmap array must have a height
 *                           as a multiple of 8.
 * \param x_place X location to place with bitmap.
 * \param y_place Y location to place with bitmap.
 */
void eink_it8951_graphics_load_4bgrey_image(uint8_t *img_array, uint16_t array_size, eink_coordinate image_width_px, eink_coordinate image_height_bytes, eink_coordinate x_place, eink_coordinate y_place)
{
    volatile uint16_t buffer_index, i, j;
    volatile uint16_t k, l, m, bit_value_1, bit_value_2;
    eink_coordinate x_set, y_set;
    
    /* Scan pixels by column */
    for (i = 0; i < image_width_px; i++) {
        m = 0;
        for (j = 0; j < image_height_bytes; j++) {
            x_set = x_place+i;
            y_set = y_place+m;
            
            if ( (x_set >= 0) && (x_set < ptr_eink_gfx_config->display_width) && (y_set >= 0) && (y_set < ptr_eink_gfx_config->display_height) ) {
                /* Process each pixel in byte  */
                bit_value_1 = ((img_array[(i*image_height_bytes)+j]) & 0xF);
                bit_value_2 = ((img_array[(i*image_height_bytes)+j] >> 4) & 0xF);
                /* Set the pixel in the display buffer */
                eink_it8951_set_pixel_raw(x_set, (y_set+1), (~bit_value_1));
                eink_it8951_set_pixel_raw(x_set, (y_set), (~bit_value_2));
            }

        m += 2;
        }
    }
}