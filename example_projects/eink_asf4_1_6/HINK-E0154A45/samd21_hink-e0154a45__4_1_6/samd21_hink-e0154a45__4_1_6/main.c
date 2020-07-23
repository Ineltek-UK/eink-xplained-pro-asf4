/**
 * \file
 * 
 * \brief Eink Xplained Pro Demo - SAMD21 - HINK-E0154A45
 *
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
#define eink_project_excluded

#include <atmel_start.h>

//Add the Eink library
#include <eink_asf4_1_6.h>

//Add any additional fonts needed
#include <RobotoBold/RobotoBold_18pt.h>
#include <RobotoBold/RobotoBold_30pt.h>
#include <RobotoBold/RobotoBold_76pt.h>
#include <RobotoMedium/RobotoMedium_24pt.h>
//And any bitmaps
#include <ineltek_logo_small.h>

//Function prototypes
void eink_init_display();
void eink_draw_display(uint8_t display_no);

int main (void)
{
	system_init();

	eink_init_display();
	
	gpio_set_pin_level(EINK_X_LED_0_PIN, 1);
	while (1)
	{
		if(!gpio_get_pin_level(EINK_X_BUTTON_0_PIN)) {
			while(!gpio_get_pin_level(EINK_X_BUTTON_0_PIN));
			eink_draw_display(0);
		}
		if(!gpio_get_pin_level(EINK_X_BUTTON_1_PIN)) {
			while(!gpio_get_pin_level(EINK_X_BUTTON_1_PIN));
			eink_draw_display(1);
		}
		if(!gpio_get_pin_level(EINK_X_BUTTON_2_PIN)) {
			while(!gpio_get_pin_level(EINK_X_BUTTON_2_PIN));
			eink_draw_display(2);
		}
	}
}

void eink_init_display(void)
{
	struct ssd1608_config eink_conf;
	
	ssd1608_get_config_defaults(&eink_conf);
	eink_conf.display_rotation = ROTATE_0;
	eink_hink_e0154a45_init(&eink_conf, false);
}

void eink_draw_display(uint8_t display_no)
{
	uint16_t i;
	char writeText[16];
	struct ssd1608_config eink_conf;
	ssd1608_get_config_defaults(&eink_conf);
	eink_conf.display_rotation = ROTATE_0;	
	
	gpio_set_pin_level(EINK_X_LED_0_PIN, 0);
	
	gfx_eink_graphics_fill_screen(FILL_WHITE);
	
	switch(display_no)
	{
		case 0:
			/* Borders */
			gfx_eink_graphics_draw_line(0, 22, 199, 22, PIXEL_BLACK);
			/* Battery Icon */
			gfx_eink_graphics_draw_line(3, 4, 23, 4, PIXEL_BLACK);
			gfx_eink_graphics_draw_line(3, 16, 23, 16, PIXEL_BLACK);
			gfx_eink_graphics_draw_line(3, 4, 3, 16, PIXEL_BLACK);
			gfx_eink_graphics_draw_line(23, 4, 23, 7, PIXEL_BLACK);
			gfx_eink_graphics_draw_line(23, 7, 26, 7, PIXEL_BLACK);
			gfx_eink_graphics_draw_line(23, 13, 23, 16, PIXEL_BLACK);
			gfx_eink_graphics_draw_line(23, 13, 26, 13, PIXEL_BLACK);
			gfx_eink_graphics_draw_line(26, 7, 26, 13, PIXEL_BLACK);
			/* Battery Gauge */
			gfx_eink_graphics_draw_rect(5, 6, 5, 9, PIXEL_BLACK, FILL_BLACK);
			gfx_eink_graphics_draw_rect(11, 6, 5, 9, PIXEL_BLACK, FILL_BLACK);
			gfx_eink_graphics_draw_rect(17, 6, 5, 9, PIXEL_BLACK, FILL_BLACK);
			/* Antenna Icon */
			gfx_eink_graphics_draw_line(171, 5, 171, 17, PIXEL_BLACK);
			gfx_eink_graphics_draw_line(167, 5, 170, 8, PIXEL_BLACK);
			gfx_eink_graphics_draw_line(175, 5, 172, 8, PIXEL_BLACK);
			/* Antenna Gauge */
			gfx_eink_graphics_draw_rect(177, 11, 4, 7, PIXEL_BLACK, FILL_BLACK);
			gfx_eink_graphics_draw_rect(182, 8, 4, 10, PIXEL_BLACK, FILL_BLACK);
			gfx_eink_graphics_draw_rect(187, 5, 4, 13, PIXEL_BLACK, FILL_WHITE);
			gfx_eink_graphics_draw_rect(192, 2, 4, 16, PIXEL_BLACK, FILL_WHITE);
			/* Current Temperature Text */
			gfx_eink_text_write_string("13", 23, 23, PIXEL_BLACK, RobotoBold_76pt);
			gfx_eink_text_write_string("o", 140, 37, PIXEL_BLACK, RobotoBold_18pt);
			gfx_eink_text_write_string("C", 155, 42, PIXEL_BLACK, RobotoBold_30pt);
			/* Room Name Text */
			gfx_eink_text_write_string("Bedroom 2", 22, 145, PIXEL_BLACK, RobotoMedium_24pt);
			break;
		case 1:
			gfx_eink_load_mono_image(&eink_IneltekLogoSmall, sizeof(eink_IneltekLogoSmall), eink_IneltekLogoSmall_width_px, eink_IneltekLogoSmall_height_bytes, 10, 10, PIXEL_WHITE, PIXEL_BLACK);
			
			gfx_eink_text_write_string("Seat", 68, 54, PIXEL_BLACK, RobotoMedium_24pt);
			gfx_eink_text_write_string("6H", 33, 77, PIXEL_BLACK, RobotoBold_76pt);
			break;
		case 2:			
			gfx_eink_graphics_fill_screen(FILL_WHITE);
			gfx_eink_put_display_buffer(true);
			
			eink_conf.update_mode = UPDATE_FAST;
			ssd1608_set_config(&eink_conf);
			gfx_eink_put_display_buffer(true);
			
			gfx_eink_text_write_string("Counter", 7, 7, PIXEL_BLACK, RobotoMedium_24pt);
			for(uint8_t i = 15; i < 26; i++)
			{
				gfx_eink_graphics_draw_rect(30, 65, 140, 80, PIXEL_WHITE, FILL_WHITE);
				sprintf(writeText, "%d", i);
				gfx_eink_text_write_string(writeText, 40, 40, PIXEL_BLACK, RobotoBold_76pt);
				gfx_eink_put_display_buffer(true);
			}
			eink_conf.update_mode = UPDATE_SLOW;
			ssd1608_set_config(&eink_conf);
			break;
	}
	
	/* Send both display buffers to the display. */
	gfx_eink_put_display_buffer(true);
	
	gpio_set_pin_level(EINK_X_LED_0_PIN, 1);
}