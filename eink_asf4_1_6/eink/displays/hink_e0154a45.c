/**
 * \file
 * 
 * \brief HINK-E0154A45 Eink Display Service
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
#include <eink/displays/hink_e0154a05.h>

/**
 * \brief HINK-E0154A45 GFX Library Configuration.
 * Used to load the GFX config structure with HINK-E0154A45 information and map
 * the display specific functions.
 */
struct eink_gfx_config eink_gfx_hink_e0154a45_config = { GFX_HINK_E0154A05, GFX_HINK_E0154A05_MAX_WIDTH, GFX_HINK_E0154A05_MAX_HEIGHT, GFX_HINK_E0154A05_DISPLAY_BUFFER_SIZE, ROTATE_0, 0, 0, eink_hink_e0154a05_refresh_display_buffer, 0, 0, eink_hink_e0154a05_put_display_buffer, 0, eink_hink_e0154a05_set_pixel, 0, eink_hink_e0154a05_graphics_load_mono_image, 0 };

/**
 * \brief Initialize SSD1608 controller and Eink display.
 * It will also write the display buffer to all zeros.
 *
 * \note This function will clear the contents of the display if clear_display is set to true
 *
 * \param *config The configuration to write to the display driver.
 * \param clear_display If enabled, the display with be cleared.
 */
void eink_hink_e0154a45_init(struct ssd1608_config *const config, bool clear_display)
{
    uint16_t buffer_index;

    /* Allocated memory for display buffers */
    uint32_t malloc_size = GFX_HINK_E0154A05_DISPLAY_BUFFER_SIZE * sizeof(uint8_t);
    hink_e0154a05_display_buffer = (uint8_t*) malloc (malloc_size);
    eink_gfx_hink_e0154a45_config.display_buffer_1_ptr = hink_e0154a05_display_buffer;
    
    /* Set the display part number */
    config->display_part_no = SSD1608_HINK_E0154A45;

    /* Initialize the low-level display controller. */
    ssd1608_init();

    /* Configure the SSD1608 controller. */
    ssd1608_set_config(config);
    
    /* Set the GFX display rotation */
    eink_gfx_hink_e0154a45_config.display_rotation = config->display_rotation;
        
    /* Set the GFX Library Configuration to HINK_E0154A05 */
    ptr_eink_gfx_config = (struct eink_gfx_config*) &eink_gfx_hink_e0154a45_config;

    /* Set display buffers to all WHITE. */
    for (buffer_index = 0; buffer_index < GFX_HINK_E0154A05_DISPLAY_BUFFER_SIZE; buffer_index++) {
        /* Note that if the memory size is too small, a hard fault will occur here */
        ptr_eink_gfx_config->display_buffer_1_ptr[buffer_index] = 0xFF;
    }
    
    /* Send both display buffers to the display if requested. */
    if(clear_display) eink_hink_e0154a05_put_display_buffer(true);
}