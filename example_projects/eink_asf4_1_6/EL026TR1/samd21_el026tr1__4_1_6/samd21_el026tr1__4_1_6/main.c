/**
 * \file
 * 
 * \brief Eink Xplained Pro Demo - SAMD21 - EL026TR1
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
#include <arduino_barcode.h>
#include <ineltek_logo.h>

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
	struct uc8151_config eink_conf;
	
	uc8151_get_config_defaults(&eink_conf);
	eink_conf.temperature_sensor_settings.sensor_select = EXTERNAL_TEMPERATURE_SENSOR;
	eink_conf.panel_settings.display_colours = B_W_R_PIXELS;
	eink_conf.panel_settings.display_rotation = ROTATE_180;
	eink_el026tr1_init(&eink_conf, false);
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
			gfx_eink_load_mono_image(&eink_ArduinoBarcode, sizeof(eink_ArduinoBarcode), eink_ArduinoBarcode_width_px, eink_ArduinoBarcode_height_bytes, 182, 95, PIXEL_WHITE, PIXEL_BLACK);
			gfx_eink_text_write_string("$22.20", 7, 0, PIXEL_BLACK, ArvoBold_32pt);
			gfx_eink_text_write_string("Arduino Uno", 7, 48, PIXEL_BLACK, ArvoBold_22pt);
			gfx_eink_text_write_string("R3 ATMEGA328", 7, 98, PIXEL_BLACK, Arvo_14pt);
			gfx_eink_text_write_string("MCU Board", 7, 120, PIXEL_BLACK, Arvo_14pt);
			gfx_eink_text_write_string("7640152110092", 190, 127, PIXEL_BLACK, Arvo_8pt);
			gfx_eink_graphics_draw_rect((296-130), 5, 125, 46, PIXEL_RED, FILL_RED);
			gfx_eink_text_write_string("SALE", (296-125), 0, PIXEL_WHITE, ArvoBold_32pt);
			break;
		case 1:
			gfx_eink_graphics_fill_screen(FILL_RED);
			gfx_eink_load_mono_image(&eink_IneltekLogo, sizeof(eink_IneltekLogo), eink_IneltekLogo_width_px, eink_IneltekLogo_height_bytes, 0, 5, PIXEL_WHITE, PIXEL_RED);
			gfx_eink_graphics_draw_rect(6, 65, 284, 81, PIXEL_WHITE, FILL_WHITE);
			gfx_eink_text_write_string("Meeting Room 2", 15, 65, PIXEL_BLACK, ArvoBold_22pt);
			gfx_eink_text_write_string("Reserved 14:00 - 16:00", 15, 108, PIXEL_BLACK, Arvo_18pt);
			break;
		case 2:
			gfx_eink_text_write_string("Anton 16pt", 6, 1, PIXEL_BLACK, Anton_16pt);
			gfx_eink_text_write_string("Arvo Bold 18pt", 105, 0, PIXEL_RED, ArvoBold_18pt);
			gfx_eink_text_write_string("IBM Plex Sans 18pt", 5, 31, PIXEL_RED, IBMPlexSans_18pt);
			gfx_eink_text_write_string("Roboto 8pt", 224, 42, PIXEL_BLACK, Roboto_8pt);
			gfx_eink_text_write_string("Roboto Bold 18pt", 5, 60, PIXEL_BLACK, RobotoBold_18pt);
			gfx_eink_text_write_string("Arvo 10pt", 203, 67, PIXEL_RED, Arvo_14pt);
			gfx_eink_text_write_string("Roboto Light 24pt", 4, 87, PIXEL_RED, RobotoLight_24pt);
			gfx_eink_text_write_string("Roboto Medium 14pt", 7, 125, PIXEL_BLACK, RobotoMedium_14pt);
			gfx_eink_text_write_string("Arvo 8pt", 198, 125, PIXEL_RED, Arvo_14pt);
			break;
	}
	
	/* Send both display buffers to the display. */
	gfx_eink_put_display_buffer(true);
	
	gpio_set_pin_level(EINK_X_LED_0_PIN, 1);
}