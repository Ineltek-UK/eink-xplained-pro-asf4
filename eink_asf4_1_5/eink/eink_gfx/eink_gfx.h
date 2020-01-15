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
#ifndef EINK_GFX_H_
#define EINK_GFX_H_

#include <compiler.h>
#include <stdlib.h>
#include <eink/drivers/eink_driver.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Eink GFX Libary Display Selector enum.
 *
 * \note The Eink GFX Graphics services is compatible with multiple Eink displays.
 * This needs enumeration is used with the eink_gfx_init function to select which
 * display is currently in use. 
 */
enum eink_gfx_display_select {
    GFX_EL026TR1,
    GFX_ED028TC1,
    GFX_ED029TC1,
    GFX_HINK_E0154A05,
    GFX_ED057TC2,
    GFX_IT8951
};

/**
 * \brief Pixel Color enum.
 */
enum eink_pixel_colour {
     PIXEL_BLACK,
     PIXEL_WHITE,
     PIXEL_RED,
     PIXEL_YELLOW,
     PIXEL_NONE
};

 /**
 * \brief Fill Color enum.
 */
enum eink_fill_colour {
     FILL_BLACK,
     FILL_WHITE,
     FILL_RED,
     FILL_YELLOW,
     FILL_NONE
};

/**
 * \brief Fill Colour enum.
 */
enum eink_fill_type {
     FILL_FILLED,
     FILL_NO_FILL
};

/**
 * \brief Eink GFX Library Configuration structure.
 *
 * Used in Eink Graphics Service to ensure the correct functionality is used
 * based on the display being used.
 */
struct eink_gfx_config {
    /** GFX Library selected display. */
    enum eink_gfx_display_select                gfx_display;
    /** Display width */
    eink_coordinate                             display_width;
    /** Display height */
    eink_coordinate                             display_height;
    /** Display buffer size */
    uint32_t                                    display_buffer_size;
    /** Display rotation */
    enum eink_display_rotation                  display_rotation;
    
    /** 
     * Pointer to display buffer 1.
     *
     * - For UC8151 in B/W mode, used as the OLD display buffer.
     * - For UC8151 in B/W/R mode, used as the Black/White display buffer.
     * - For UC8159, used as the display buffer.
     * - For UC8177, used as the display buffer.
     * - For SSD1608, used as the display buffer.
     * - For IT8951, used as the display buffer.
     */
    uint8_t                                     *display_buffer_1_ptr;
    /** 
     * Pointer to display buffer 2.
     *
     * - For UC8151 in B/W mode, used as the NEW display buffer.
     * - For UC8151 in B/W/R mode, used as the RED display buffer.
     * - For UC8159, left unused.
     * - For UC8177, left unused.
     * - For SSD1608, left unused.
     * - For IT8951, left unused.
     */
    uint8_t                                     *display_buffer_2_ptr;
    uint32_t (*refresh_display)(void);                                                                                                                          /**< Pointer to gfx refresh display function. */
    uint32_t (*refresh_display_mode)(uint8_t update_mode);                                                                                                      /**< Pointer to gfx refresh display with update mode function. */
    uint32_t (*refresh_partial_display)(eink_coordinate show_x, eink_coordinate show_y, eink_coordinate show_w, eink_coordinate show_h, uint8_t update_mode);   /**< Pointer to gfx refresh partial display function. */
    uint32_t (*put_display_buffer)(bool refresh_display);                                                                                                       /**< Pointer to gfx put display buffer function. */
    uint32_t (*put_partial_display_buffer)(eink_coordinate start_x, eink_coordinate start_y, eink_coordinate window_w, eink_coordinate window_h);               /**< Pointer to gfx put partial display buffer function. */
    uint32_t (*set_pixel)(eink_coordinate x_set, eink_coordinate y_set, enum eink_pixel_colour pixel_colour);                                                   /**< Pointer to gfx set pixel function. */
    uint32_t (*set_pixel_raw)(eink_coordinate x_set, eink_coordinate y_set, uint8_t pixel_value);                                                               /**< Pointer to gfx set pixel raw function. */
    uint32_t (*load_mono_image)(uint8_t *img_array, uint16_t array_size, eink_coordinate image_width_px, eink_coordinate image_height_bytes, eink_coordinate x_place, eink_coordinate y_place, enum eink_pixel_colour foreground_colour, enum eink_pixel_colour background_colour); /**< Pointer to gfx load mono image function. */
    uint32_t (*load_4bgrey_image)(uint8_t *img_array, uint16_t array_size, eink_coordinate image_width_px, eink_coordinate image_height_bytes, eink_coordinate x_place, eink_coordinate y_place); /**< Pointer to gfx load 4-bit greyscale image function. */
};

/**
 * \brief Pointer to the Eink GFX Libary Configuration structure.
 */
struct eink_gfx_config *ptr_eink_gfx_config;

/**
 * \name Global GFX functions
 * 
 * Global functions linked to the gfx_config structure. Ensures correct
 * display is addressed (after being initialized) and gives a nice function
 * to use. 
 */
/**
 * \brief Refresh Display function.
 */
static void gfx_eink_refresh_display(void) {
    ptr_eink_gfx_config->refresh_display();
}
/**
 * \brief Refresh Display with mode function.
 */
static void gfx_eink_refresh_display_mode(uint8_t update_mode) {
    ptr_eink_gfx_config->refresh_display_mode(update_mode);
}

/**
 * \brief Refresh Partial Display function.
 */
static void gfx_eink_refresh_partial_display(eink_coordinate show_x, eink_coordinate show_y, eink_coordinate show_w, eink_coordinate show_h, uint8_t update_mode) {
    ptr_eink_gfx_config->refresh_partial_display(show_x, show_y, show_w, show_h, update_mode);
}

/**
 * \brief Put Display Buffer function.
 */
static void gfx_eink_put_display_buffer(bool refresh_display) {
    ptr_eink_gfx_config->put_display_buffer(refresh_display);
}

/**
 * \brief Put Partial Display Buffer function.
 */
static void gfx_eink_put_partial_display_buffer(eink_coordinate start_x, eink_coordinate start_y, eink_coordinate window_w, eink_coordinate window_h) {
    ptr_eink_gfx_config->put_partial_display_buffer(start_x, start_y, window_w, window_h);
}

/**
 * \brief Set Pixel function.
 */
static void gfx_eink_set_pixel(eink_coordinate x_set, eink_coordinate y_set, enum eink_pixel_colour pixel_colour) {
    ptr_eink_gfx_config->set_pixel(x_set, y_set, pixel_colour);
}

/**
 * \brief Set Pixel with raw value function.
 */
static void gfx_eink_set_pixel_raw(eink_coordinate x_set, eink_coordinate y_set, uint8_t pixel_value) {
    ptr_eink_gfx_config->set_pixel_raw(x_set, y_set, pixel_value);
}

/**
 * \brief Load Mono Image function.
 */
static void gfx_eink_load_mono_image(uint8_t *img_array, uint16_t array_size, eink_coordinate image_width_px, eink_coordinate image_height_bytes, eink_coordinate x_place, eink_coordinate y_place, enum eink_pixel_colour foreground_colour, enum eink_pixel_colour background_colour) {
    ptr_eink_gfx_config->load_mono_image(img_array, array_size, image_width_px, image_height_bytes, x_place, y_place, foreground_colour, background_colour);
}
//@}

/**
 * \brief Load 4-bit Greyscale Image function.
 */							
static void gfx_eink_load_4bgrey_image(uint8_t *img_array, uint16_t array_size, eink_coordinate image_width_px, eink_coordinate image_height_bytes, eink_coordinate x_place, eink_coordinate y_place) {
    ptr_eink_gfx_config->load_4bgrey_image(img_array, array_size, image_width_px, image_height_bytes, x_place, y_place);
}
//@}

#ifdef __cplusplus
}
#endif

#endif /* EINK_GFX_H_ */