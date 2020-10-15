/**
 * \file
 * 
 * \brief Eink ITE Xplained Pro Demo - SAME54 - ED043WC3
 *
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
#define eink_project_excluded

#include <atmel_start.h>

//Add the Eink library
#include <eink_asf4_1_6.h>

//Add any additional fonts needed
#include <ArvoBold/ArvoBold_18pt.h>
#include <RobotoBold/RobotoBold_30pt.h>
#include <RobotoBold/RobotoBold_76pt.h>
#include <RobotoMedium/RobotoMedium_30pt.h>
//And any bitmaps
#include <ineltek_logo_large_4bGrey.h>
#include <ineltek_logo_small_4bGrey.h>

void eink_draw_display(uint8_t display_no);

uint8_t timer;

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	eink_it8951_init(ROTATE_270, false);
	gpio_set_pin_level(EINK_X_LED_0_PIN, 1);
	
	eink_draw_display(0);
	
	gpio_set_pin_level(EINK_X_LED_1_PIN, 0);
	gfx_eink_put_display_buffer(false);
	gpio_set_pin_level(EINK_X_LED_1_PIN, 1);
	
	while(1)
	{
		timer = 0;
		
		eink_draw_display(0);
		gfx_eink_put_display_buffer(true);
		
		eink_draw_display(8);
		
		for(timer = 0; timer <= 12; timer++)
		{
			eink_draw_display(6);
		}
		timer = 12;
		eink_draw_display(9);
		eink_draw_display(7);
		
		eink_draw_display(1);
		eink_draw_display(2);
		eink_draw_display(3);
		eink_draw_display(4);
		eink_draw_display(5);
		gfx_eink_refresh_display();
		eink_draw_display(8);
		eink_draw_display(9);
	}
}

void eink_draw_display(uint8_t display_no)
{
	uint16_t i;
	char writeText[16];
			
	switch(display_no)
	{
		case 0:
			gfx_eink_graphics_fill_screen_raw(0xFF);
			gfx_eink_load_4bgrey_image(&eink_IneltekLogoLarge_4bGrey, sizeof(eink_IneltekLogoLarge_4bGrey), eink_IneltekLogoLarge_4bGrey_width_px, eink_IneltekLogoLarge_4bGrey_height_bytes, 100, 360);
			gfx_eink_text_write_string_raw("IT8951 Demo", 90, 175, 0x00, RobotoBold_76pt);
			
			gfx_eink_graphics_draw_rect_raw(20, 21, 133, 70, 0x00, FILL_FILLED, 0xFF);
			gfx_eink_text_write_string_raw("Menu", 35, 30, 0x00, RobotoMedium_30pt);
			
			gfx_eink_graphics_draw_rect_raw(650, 18, 133, 70, 0x00, FILL_FILLED, 0xFF);
			sprintf(writeText, "00:%02d", timer);
			gfx_eink_text_write_string_raw(writeText, 665, 28, 0x00, RobotoBold_30pt);
			break;
		case 1:
			gfx_eink_graphics_draw_rect_raw(20, 90, 300, 70, 0x00, FILL_FILLED, 0xFF);
			gfx_eink_text_write_string_raw("Exit", 35, 100, 0x00, RobotoMedium_30pt);
			gfx_eink_graphics_draw_rect_raw(20, 159, 300, 70, 0x00, FILL_FILLED, 0xFF);
			gfx_eink_text_write_string_raw("Settings", 35, 169, 0x00, RobotoMedium_30pt);
			gfx_eink_graphics_draw_rect_raw(20, 228, 300, 70, 0x00, FILL_FILLED, 0xFF);
			gfx_eink_text_write_string_raw("Open", 35, 238, 0x00, RobotoMedium_30pt);
			gfx_eink_graphics_draw_rect_raw(20, 297, 300, 70, 0x00, FILL_FILLED, 0xFF);
			gfx_eink_text_write_string_raw("New", 35, 307, 0x00, RobotoMedium_30pt);
		
			gfx_eink_put_partial_display_buffer(20, 20, 300, 372);
		
			gfx_eink_graphics_draw_rect_raw(20, 21, 133, 70, 0x00, FILL_FILLED, 0x00);
			gfx_eink_text_write_string_raw("Menu", 35, 30, 0xFF, RobotoMedium_30pt);
			
			gfx_eink_put_partial_display_buffer(20, 20, 136, 72);
			gfx_eink_refresh_partial_display(20, 21, 133, 70, 1);
			break;
		case 2:
			gfx_eink_refresh_partial_display(20, 20, 300, 370, 1);
			break;
		case 3:
			gfx_eink_graphics_draw_rect_raw(20, 21, 133, 70, 0x00, FILL_FILLED, 0xFF);
			gfx_eink_text_write_string_raw("Menu", 35, 30, 0x00, RobotoMedium_30pt);
			
			gfx_eink_put_partial_display_buffer(20, 20, 136, 72);
			gfx_eink_refresh_partial_display(20, 21, 133, 70, 1);
			break;
		case 4:
			eink_draw_display(0);
			gfx_eink_put_display_buffer(false);
		
			gfx_eink_graphics_draw_rect_raw(20, 90, 300, 70, 0x00, FILL_FILLED, 0x00);
			gfx_eink_text_write_string_raw("Exit", 35, 100, 0xFF, RobotoMedium_30pt);
			
			gfx_eink_put_partial_display_buffer(20, 88, 304, 72);
			gfx_eink_refresh_partial_display(20, 90, 300, 70, 1);
			break;
		case 5:
			gfx_eink_graphics_draw_rect_raw(20, 90, 300, 70, 0x00, FILL_FILLED, 0xFF);
			gfx_eink_text_write_string_raw("Exit", 35, 100, 0x00, RobotoMedium_30pt);
			
			gfx_eink_put_partial_display_buffer(20, 88, 304, 72);
			gfx_eink_refresh_partial_display(20, 90, 300, 70, 1);
			
			gfx_eink_graphics_draw_rect_raw(20, 90, 300, 70, 0xFF, FILL_FILLED, 0xFF);
			gfx_eink_graphics_draw_line_raw(20, 90, 152, 90, 0x00);
			
			gfx_eink_put_partial_display_buffer(20, 88, 304, 72);
			break;
		case 6:
			gfx_eink_graphics_draw_rect_raw(650, 18, 133, 70, 0x00, FILL_FILLED, 0xFF);
			sprintf(writeText, "00:%02d", timer);
			gfx_eink_text_write_string_raw(writeText, 665, 28, 0x00, RobotoBold_30pt);
			
			gfx_eink_put_partial_display_buffer(648, 16, 136, 72);
			gfx_eink_refresh_partial_display(650, 18, 133, 70, 1);
			break;
		case 7:
			gfx_eink_load_4bgrey_image(&eink_IneltekLogoSmall_4bGrey, sizeof(eink_IneltekLogoSmall_4bGrey), eink_IneltekLogoSmall_4bGrey_width_px, eink_IneltekLogoSmall_4bGrey_height_bytes, 204, 20);
			gfx_eink_put_partial_display_buffer(204, 20, 400, 64);
			gfx_eink_refresh_partial_display(204, 20, 400, 64, 2);
			break;
		case 8:
			gfx_eink_graphics_draw_rect_raw(156, 16, 492, 136, 0xFF, FILL_FILLED, 0xFF);
			gfx_eink_text_write_string_raw("This demo shows the various update", 176, 110, 0x00, ArvoBold_18pt);
			gfx_eink_text_write_string_raw("modes the IT8951 is capable of along", 180, 80, 0x00, ArvoBold_18pt);
			gfx_eink_text_write_string_raw("with its ability to update the entire", 190, 50, 0x00, ArvoBold_18pt);
			gfx_eink_text_write_string_raw("display or just a partial window", 200, 20, 0x00, ArvoBold_18pt);
			gfx_eink_put_partial_display_buffer(156, 16, 492, 136);
			gfx_eink_refresh_partial_display(156, 16, 492, 136, 1);
			break;
		case 9:
			gfx_eink_graphics_draw_rect_raw(156, 16, 492, 136, 0xFF, FILL_FILLED, 0xFF);
			gfx_eink_put_partial_display_buffer(156, 16, 492, 136);
			gfx_eink_refresh_partial_display(156, 16, 492, 136, 1);
			break;
	}
}