/**
 * \file
 * 
 * \brief Eink XL Xplained Pro Demo - SAMG55 - AC057TC1
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
#include <ArvoBold/ArvoBold_40pt.h>
#include <RobotoBold/RobotoBold_18pt.h>
#include <RobotoBold/RobotoBold_76pt.h>
#include <RobotoMedium/RobotoMedium_24pt.h>
#include <RobotoMedium/RobotoMedium_30pt.h>
//And any bitmaps
#include <ineltek_splash_colour.h>
#include <apples_barcode.h>
#include <apples_colour.h>

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
	struct uc8159_config eink_conf;
	
	uc8159_get_config_defaults(&eink_conf);
	eink_conf.panel_settings.display_rotation = ROTATE_0;
	eink_ac057tc1_init(&eink_conf, false);
}

void eink_draw_display(uint8_t display_no)
{
	uint16_t i;
	char writeText[16];
	
	/* Clean the display first - always required to reduce ghosting */
	gpio_set_pin_level(EINK_X_LED_0_PIN, 0);
	gfx_eink_graphics_fill_screen(FILL_CLEAN);
	gfx_eink_put_display_buffer(true);
	gpio_set_pin_level(EINK_X_LED_0_PIN, 1);
	
	/* Now update the display */
	gpio_set_pin_level(EINK_X_LED_1_PIN, 0);
	gfx_eink_graphics_fill_screen(FILL_WHITE);
	switch(display_no)
	{
		case 0:
			gfx_eink_load_mono_image(&eink_IneltekSplashColour_Bla, sizeof(eink_IneltekSplashColour_Bla), eink_IneltekSplashColour_width_px, eink_IneltekSplashColour_height_bytes, 0, 0, PIXEL_BLACK, PIXEL_NONE);
			gfx_eink_load_mono_image(&eink_IneltekSplashColour_Blu, sizeof(eink_IneltekSplashColour_Blu), eink_IneltekSplashColour_width_px, eink_IneltekSplashColour_height_bytes, 0, 0, PIXEL_BLUE, PIXEL_NONE);
			gfx_eink_load_mono_image(&eink_IneltekSplashColour_Ora, sizeof(eink_IneltekSplashColour_Ora), eink_IneltekSplashColour_width_px, eink_IneltekSplashColour_height_bytes, 0, 0, PIXEL_ORANGE, PIXEL_NONE);
			gfx_eink_load_mono_image(&eink_IneltekSplashColour_Red, sizeof(eink_IneltekSplashColour_Red), eink_IneltekSplashColour_width_px, eink_IneltekSplashColour_height_bytes, 0, 0, PIXEL_RED, PIXEL_NONE);
			gfx_eink_text_write_string("github.com/Ineltek-UK", 195, 396, PIXEL_WHITE, RobotoBold_18pt);
			break;
		case 1:
			gfx_eink_text_write_string("Granny Smith Apples", 5, 0, PIXEL_BLACK, ArvoBold_40pt);
			gfx_eink_text_write_string("France", 7, 62, PIXEL_BLUE, RobotoMedium_30pt);
			gfx_eink_text_write_string("Loose, single", 8, 107, PIXEL_BLACK, RobotoMedium_24pt);
			/* French flag */
			gfx_eink_graphics_draw_rect(432, 77, 150, 100, PIXEL_BLACK, FILL_WHITE);
			gfx_eink_graphics_draw_rect(432, 77, 51, 100, PIXEL_NONE, FILL_BLUE);
			gfx_eink_graphics_draw_rect(531, 77, 51, 100, PIXEL_NONE, FILL_RED);
			
			gfx_eink_text_write_string("25p", 51, 167, PIXEL_ORANGE, RobotoBold_76pt);
			gfx_eink_text_write_string("/ea", 234, 236, PIXEL_BLACK, RobotoMedium_24pt);
			/* Apples Barcode */
			gfx_eink_load_mono_image(&eink_ApplesBarcode, sizeof(eink_ApplesBarcode), eink_ApplesBarcode_width_px, eink_ApplesBarcode_height_bytes, 33, 337, PIXEL_GREEN, PIXEL_NONE);
			gfx_eink_text_write_string("0000", 60, 415, PIXEL_GREEN, RobotoBold_18pt);
			gfx_eink_text_write_string("9539", 160, 415, PIXEL_GREEN, RobotoBold_18pt);
			/* Apples Bitmap */
			gfx_eink_load_mono_image(&eink_ApplesColour_Bla, sizeof(eink_ApplesColour_Bla), eink_ApplesColour_width_px, eink_ApplesColour_height_bytes, 275, 208, PIXEL_BLACK, PIXEL_NONE);
			gfx_eink_load_mono_image(&eink_ApplesColour_Gre, sizeof(eink_ApplesColour_Gre), eink_ApplesColour_width_px, eink_ApplesColour_height_bytes, 275, 208, PIXEL_GREEN, PIXEL_NONE);
			gfx_eink_load_mono_image(&eink_ApplesColour_Ora, sizeof(eink_ApplesColour_Ora), eink_ApplesColour_width_px, eink_ApplesColour_height_bytes, 275, 208, PIXEL_ORANGE, PIXEL_NONE);
			gfx_eink_load_mono_image(&eink_ApplesColour_Red, sizeof(eink_ApplesColour_Red), eink_ApplesColour_width_px, eink_ApplesColour_height_bytes, 275, 208, PIXEL_RED, PIXEL_NONE);
			break;
		case 2:
			gfx_eink_graphics_draw_rect(20, 20, 80, 408, PIXEL_BLACK, FILL_BLACK);
			gfx_eink_graphics_draw_rect(100, 20, 80, 408, PIXEL_BLACK, FILL_WHITE);
			gfx_eink_graphics_draw_rect(180, 20, 80, 408, PIXEL_BLACK, FILL_RED);
			gfx_eink_graphics_draw_rect(260, 20, 80, 408, PIXEL_BLACK, FILL_GREEN);
			gfx_eink_graphics_draw_rect(340, 20, 80, 408, PIXEL_BLACK, FILL_BLUE);
			gfx_eink_graphics_draw_rect(420, 20, 80, 408, PIXEL_BLACK, FILL_YELLOW);
			gfx_eink_graphics_draw_rect(500, 20, 80, 408, PIXEL_BLACK, FILL_ORANGE);
			break;
	}
	gfx_eink_put_display_buffer(true);
	gpio_set_pin_level(EINK_X_LED_1_PIN, 1);
}