/**
 * \file
 * 
 * \brief Eink Xplained Pro Demo - SAMG55 - ED037TC1
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
#include <Anton/Anton_16pt.h>
#include <Arvo/Arvo_8pt.h>
#include <Arvo/Arvo_10pt.h>
#include <Arvo/Arvo_14pt.h>
#include <Arvo/Arvo_18pt.h>
#include <Arvo/Arvo_22pt.h>
#include <Arvo/Arvo_24pt.h>
#include <Arvo/Arvo_28pt.h>
#include <ArvoBold/ArvoBold_18pt.h>
#include <ArvoBold/ArvoBold_22pt.h>
#include <ArvoBold/ArvoBold_24pt.h>
#include <ArvoBold/ArvoBold_28pt.h>
#include <ArvoBold/ArvoBold_32pt.h>
#include <ArvoBold/ArvoBold_40pt.h>
#include <IBMPlexSans/IBMPlexSans_18pt.h>
#include <Roboto/Roboto_8pt.h>
#include <RobotoBold/RobotoBold_18pt.h>
#include <RobotoLight/RobotoLight_24pt.h>
#include <RobotoMedium/RobotoMedium_14pt.h>
//And any bitmaps
#include <ineltek_logo.h>
#include <kanban_barcode.h>
#include <shipping_barcode.h>

//Function prototypes
void eink_init_display();
void eink_draw_display(uint8_t display_no);

int main (void)
{
	system_init();
	uint8_t eink_data[4];

	//eink_init_display();
	
	struct ssd1677_config eink_conf;
	
	ssd1677_get_config_defaults(&eink_conf);
	eink_conf.display_rotation = ROTATE_90;
	eink_ed037tc1_init(&eink_conf, false);
		
	gfx_eink_graphics_fill_screen(FILL_WHITE);
	
	
	const uint16_t startx = 16;
	const uint16_t starty = 48;
	
	const uint16_t width = 240;
	const uint16_t height = 80;
	
	gfx_eink_graphics_draw_line(0, starty, 279, starty, PIXEL_BLACK);
	gfx_eink_graphics_draw_line(startx, 0, startx, 479, PIXEL_BLACK);
	
	gfx_eink_graphics_draw_line(0, (height+starty), 279, (height+starty), PIXEL_BLACK);
	gfx_eink_graphics_draw_line((width+startx), 0, (width+startx), 479, PIXEL_BLACK);
	
	gfx_eink_load_mono_image(&eink_IneltekLogo, sizeof(eink_IneltekLogo), eink_IneltekLogo_width_px, eink_IneltekLogo_height_bytes, startx, 380, PIXEL_WHITE, PIXEL_BLACK);
	
	
	eink_ed037tc1_put_display_buffer(false);
	ssd1677_global_instance.panel_settings.update_mode = UPDATE_SLOW;
	eink_ed037tc1_refresh_display_buffer();
	
	eink_ed037tc1_put_display_buffer(false);
	ssd1677_global_instance.panel_settings.update_mode = UPDATE_FAST;
	eink_ed037tc1_refresh_display_buffer();
	
	gfx_eink_graphics_fill_screen(FILL_BLACK);
	gfx_eink_load_mono_image(&eink_IneltekLogo, sizeof(eink_IneltekLogo), eink_IneltekLogo_width_px, eink_IneltekLogo_height_bytes, 16, 48, PIXEL_WHITE, PIXEL_BLACK);
	
	eink_ed037tc1_put_partial_display_buffer(startx, starty, width, height);
	ssd1677_global_instance.panel_settings.update_mode = UPDATE_SLOW;
	eink_ed037tc1_refresh_display_buffer();
	
	while(1);
	
	//eink_draw_display(0);
	
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
	struct ssd1677_config eink_conf;
	
	ssd1677_get_config_defaults(&eink_conf);	
	eink_conf.display_rotation = ROTATE_0;
	eink_ed037tc1_init(&eink_conf, false);
}

void eink_draw_display(uint8_t display_no)
{
	uint16_t i;
	char writeText[16];
	
	gpio_set_pin_level(EINK_X_LED_0_PIN, 0);
	
	gfx_eink_graphics_fill_screen(FILL_WHITE);
	
	switch(display_no)
	{
		case 0:
			gfx_eink_load_mono_image(&eink_IneltekLogo, sizeof(eink_IneltekLogo), eink_IneltekLogo_width_px, eink_IneltekLogo_height_bytes, 0, 0, PIXEL_WHITE, PIXEL_BLACK);
			gfx_eink_put_display_buffer(true);
			delay_ms(2000);
			
			ssd1677_global_instance.panel_settings.update_mode = UPDATE_FAST;
			gfx_eink_text_write_string("HELLO!", 5, 80, PIXEL_BLACK, ArvoBold_28pt);
			gfx_eink_put_display_buffer(true);
			gfx_eink_text_write_string("HELLO!", 5, 120, PIXEL_BLACK, ArvoBold_28pt);
			gfx_eink_put_display_buffer(true);
			gfx_eink_text_write_string("HELLO!", 5, 160, PIXEL_BLACK, ArvoBold_28pt);
			gfx_eink_put_display_buffer(true);
			break;
		case 1:
			
			break;
		case 2:
			
			break;
	}
	
	/* Send both display buffers to the display. */
	gfx_eink_put_display_buffer(true);
	ssd1677_global_instance.panel_settings.update_mode = UPDATE_SLOW;
	
	gpio_set_pin_level(EINK_X_LED_0_PIN, 1);
}