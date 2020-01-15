/**
 * \file
 * 
 * \brief Eink Xplained Pro Demo - SAMD21 - ED029TC1
 *
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

/** 
 * \mainpage
 * \section intro Introduction
 * This example code demonstrates the use of the Eink Xplained Pro (ED029TC1) with the SAMD21 Xplained Pro
 * board, making use of Ineltek's Eink ASF4 Software Library (version 1.5).<br>
 * It uses the following hardware:
 * - the SAMD21 Xplained Pro.
 * - the Eink Xplained Extension (ED029TC1) on EXT1 and EXT2.
 *
 * \section setup_steps Setup Steps
 *
 * \subsection setup_prerequisites Prerequisites
 * 
 * In order to use the Eink ASF4 Software Library, several software components must be added from the
 * Atmel START. These are:
 * - Delay (driver)
 * - PORT (driver)
 * 
 * \note Note that this library is not compatible with the ASF Wizard in Atmel Studio - for this you
 * must use the Eink ASF3 Software Library - no longer in development.
 * 
 * \subsection setup_pins Pinmux Configuration
 * 
 * Next we need to define the pins for the display and ensure they are labeled correctly within
 * Atmel START. Pin directions are automatically configured by the library, so any direction can be set
 * within START. The following pins are required with the following labels:
 * - <strong>EINK_DC</strong> : Data/Command pin - <em>EXT1 Pin 3</em>
 * - <strong>EINK_BS</strong> : BS pin - <em>EXT1 Pin 5</em>
 * - <strong>EINK_BUSY</strong> : Busy pin - <em>EXT1 Pin 6</em>
 * - <strong>EINK_CSB</strong> : Chip Select pin - <em>EXT1 Pin 15</em>
 * - <strong>EINK_RST</strong> : Reset pin - <em>EXT1 Pin 16</em>

 * - <strong>EINK_SPI_SDA</strong> : Data pin - <em>EXT1 Pin 17</em>
 * - <strong>EINK_SPI_SCL</strong> : Clock pin - <em>EXT1 Pin 18</em>
 * 
 * We can also add the LEDs and Buttons on the Eink Xplained Pro board:
 * - <strong>EINK_X_LED_0_PIN</strong> : LED 1 - <em>EXT1 Pin 8</em>
 * - <strong>EINK_X_LED_1_PIN</strong> : LED 2 - <em>EXT1 Pin 10</em>
 * - <strong>EINK_X_LED_2_PIN</strong> : LED 3 - <em>EXT1 Pin 14</em>
 *
 * - <strong>EINK_X_BUTTON_0_PIN</strong> : Button 1 - <em>EXT1 Pin 7</em>
 * - <strong>EINK_X_BUTTON_1_PIN</strong> : Button 2 - <em>EXT1 Pin 9</em>
 * - <strong>EINK_X_BUTTON_2_PIN</strong> : Button 3 - <em>EXT1 Pin 13</em>
 * 
 * \subsection setup_project Atmel Studio Project Configuration
 *
 * Once your Atmel Studio project has been generated from Atmel START, we now need to add the library
 * to the compiler include paths:<br>
 * See Project Properties > Toolchain > ARM/GNU C Compiler > Directories:
 * - <BASE_DIR>/eink_asf4_1_5
 * - <BASE_DIR>/eink_asf4_1_5/fonts
 *
 * Finally the following line must be added directly below the Atmel START include statement:
 * \code
 * #define eink_project_excluded
 * #include <atmel_start.h>
 * #include <eink_asf4_1_5.h>
 * \endcode
 *
 * In this example project, the library is located elsewhere and not in the Solution
 * Explorer, hence why this has been included.
 *
 * To initialize the Eink library for use with the ED029TC1 (UC8151), the eink_ed029tc1_init() function
 * must be called. \ref uc8151_config must be used as the configuration struct. Note that
 * uc8151_get_config_defaults() should be called before making any required changes and passing them to
 * the initialization function.
 *
 * Example Code:
 *
 * \code
 * struct uc8151_config eink_conf;
 *
 * uc8151_get_config_defaults(&eink_conf);
 * eink_conf.temperature_sensor_settings.sensor_select = EXTERNAL_TEMPERATURE_SENSOR;
 * eink_conf.panel_settings.display_colours = B_W_PIXELS;
 * eink_conf.panel_settings.display_rotation = ROTATE_180;
 * eink_ed029tc1_init(&eink_conf, false);
 * \endcode
 *
 * \section gfx_library GFX Library
 *
 * Once initialized, the library stores a buffer in RAM in order keep track of the canvas - initially
 * it is all white. There are several primitive function that allow for some data to be written to the canvas:
 * - gfx_eink_graphics_fill_screen()
 * - gfx_eink_load_mono_image()
 * - gfx_eink_graphics_draw_line()
 * - gfx_eink_graphics_draw_rect()
 * - gfx_eink_text_write_string()
 * - gfx_eink_set_pixel()
 *
 * The canvas buffer can be uploaded to the display controller with the use of gfx_eink_put_display_buffer()
 *
 * If you decide to load the buffer but refresh the display later on, this can be achieved using the
 * gfx_eink_refresh_display()
 *
 * \section files Main Files
 * - main.c : Initialize the Eink display and displays some demo screens.
 *
 * \section usage Usage
 * -# Build the program and download it into the board.
 * -# Wait for the RED LED to turn off - indicates initialization is complete.
 * -# Press one of the 3 buttons on the Eink Xplained Extension to change the display.
 *
 * \section compinfo Compilation Information
 * This software was written for the GNU GCC compiler using Atmel Studio 7.0 and requires ASF version
 * 4 (originally compiled from Atmel START version 1.7.279). Other compilers may or may not work.
 *
 * \section contactinfo Contact Information
 * For further information, visit
 * <A href="http://www.ineltek.com">Ineltek</A>.\n
 */
#define eink_project_excluded

#include <atmel_start.h>

//Add the Eink library
#include <eink_asf4_1_5.h>

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
	eink_conf.panel_settings.display_colours = B_W_PIXELS;
	eink_conf.panel_settings.display_rotation = ROTATE_180;
	eink_ed029tc1_init(&eink_conf, false);
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
			gfx_eink_load_mono_image(&eink_ArduinoBarcode, sizeof(eink_ArduinoBarcode), eink_ArduinoBarcode_width_px, eink_ArduinoBarcode_height_bytes, 182, 75, PIXEL_WHITE, PIXEL_BLACK);
			gfx_eink_text_write_string("$22.20", 7, 0, PIXEL_BLACK, ArvoBold_32pt);
			gfx_eink_text_write_string("Arduino Uno", 7, 48, PIXEL_BLACK, ArvoBold_18pt);
			gfx_eink_text_write_string("R3 ATMEGA328", 7, 78, PIXEL_BLACK, Arvo_14pt);
			gfx_eink_text_write_string("MCU Board", 7, 100, PIXEL_BLACK, Arvo_14pt);
			gfx_eink_text_write_string("7640152110092", 190, 107, PIXEL_BLACK, Arvo_8pt);
			gfx_eink_graphics_draw_rect((296-130), 5, 125, 46, PIXEL_BLACK, PIXEL_BLACK);
			gfx_eink_text_write_string("SALE", (296-125), 0, PIXEL_WHITE, ArvoBold_32pt);
			break;
		case 1:
			gfx_eink_graphics_fill_screen(FILL_BLACK);
			gfx_eink_load_mono_image(&eink_IneltekLogo, sizeof(eink_IneltekLogo), eink_IneltekLogo_width_px, eink_IneltekLogo_height_bytes, 0, 0, PIXEL_WHITE, PIXEL_BLACK);
			gfx_eink_graphics_draw_rect(6, 54, 282, 68, PIXEL_WHITE, FILL_WHITE);
			gfx_eink_text_write_string("Meeting Room 2", 12, 49, PIXEL_BLACK, ArvoBold_22pt);
			gfx_eink_text_write_string("Reserved 14:00 - 16:00", 12, 86, PIXEL_BLACK, Arvo_18pt);
			break;
		case 2:
			gfx_eink_text_write_string("Anton 16pt", 6, 1, PIXEL_BLACK, Anton_16pt);
			gfx_eink_text_write_string("Arvo Bold 18pt", 105, 0, PIXEL_BLACK, ArvoBold_18pt);
			gfx_eink_text_write_string("IBM Plex Sans 18pt", 5, 25, PIXEL_BLACK, IBMPlexSans_18pt);
			gfx_eink_text_write_string("Roboto 8pt", 224, 36, PIXEL_BLACK, Roboto_8pt);
			gfx_eink_text_write_string("Roboto Bold 18pt", 5, 48, PIXEL_BLACK, RobotoBold_18pt);
			gfx_eink_text_write_string("Arvo 10pt", 203, 55, PIXEL_BLACK, Arvo_14pt);
			gfx_eink_text_write_string("Roboto Light 24pt", 4, 71, PIXEL_BLACK, RobotoLight_24pt);
			gfx_eink_text_write_string("Roboto Medium 14pt", 7, 104, PIXEL_BLACK, RobotoMedium_14pt);
			gfx_eink_text_write_string("Arvo 8pt", 198, 105, PIXEL_BLACK, Arvo_14pt);
			break;
	}
	
	/* Send both display buffers to the display. */
	gfx_eink_put_display_buffer(true);
	
	gpio_set_pin_level(EINK_X_LED_0_PIN, 1);
}