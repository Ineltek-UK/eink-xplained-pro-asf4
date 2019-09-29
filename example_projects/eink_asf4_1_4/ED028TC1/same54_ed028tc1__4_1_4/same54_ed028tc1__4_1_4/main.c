/**
 * \file
 * 
 * \brief Eink Xplained Pro Demo - SAMD21 - ED028TC1
 *
 * Microchip ASF4 Variant - release 1.4 - October 2019
 * 
 * \author George Sephton
 * 
 * \copyright Copyright (c) 2019 Ineltek Ltd. All rights reserved.
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
 * This example code demonstrates the use of the Eink Xplained Pro (ED028TC1) with the SAMD21 Xplained Pro
 * board, making use of Ineltek's Eink ASF4 Software Library (version 1.4).<br>
 * It uses the following hardware:
 * - the SAMD21 Xplained Pro.
 * - the Eink HULK board with ED028TC1 connected with jumper wires to EXT1.
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
 * - <strong>EINK_BUSY</strong> - <em>HULK Pin 4</em> : Busy pin - <em>EXT1 Pin 6</em>
 * - <strong>EINK_CSB</strong> - <em>HULK Pin 5</em> : Chip Select pin - <em>EXT1 Pin 15</em>
 * - <strong>EINK_RST</strong> - <em>HULK Pin 3</em> : Reset pin - <em>EXT1 Pin 16</em>

 * - <strong>EINK_SPI_SDA</strong> - <em>HULK Pin 14</em> : Data pin - <em>EXT1 Pin 17</em>
 * - <strong>EINK_SPI_SCL</strong> - <em>HULK Pin 12</em> : Clock pin - <em>EXT1 Pin 18</em>

 * We also need to connect the power pins to the HULK board - obviously no configuration is
 * required in Atmel START for these:
 * - <strong>3.3V</strong> - <em>HULK Pin 1</em> : <em>PWR Pin 4</em>
 * - <strong>GND</strong> - <em>HULK Pin 16</em> : <em>PWR Pin 2</em>
 * 
 * \subsection setup_project Atmel Studio Project Configuration
 *
 * Once your Atmel Studio project has been generated from Atmel START, we now need to add the library
 * to the compiler include paths:<br>
 * See Project Properties > Toolchain > ARM/GNU C Compiler > Directories:
 * - <BASE_DIR>/eink_asf4_1_4
 * - <BASE_DIR>/eink_asf4_1_4/fonts
 *
 * Finally the following line must be added directly below the Atmel START include statement:
 * \code
 * #define eink_project_excluded
 * #include <atmel_start.h>
 * #include <eink_asf4_1_4.h>
 * \endcode
 *
 * In this example project, the library is located elsewhere and not in the Solution
 * Explorer, hence why this has been included.
 *
 * To initialize the Eink library for use with the ED028TC1 (UC8177), the eink_ed028tc1_init() function
 * must be called. \ref uc8177_config must be used as the configuration struct. Note that
 * uc8177_get_config_defaults() should be called before making any required changes and passing them to
 * the initialization function.
 *
 * Example Code:
 *
 * \code
 * struct uc8177_config eink_conf;
 *
 * uc8177_get_config_defaults(&eink_conf);
 * eink_conf.display_rotation = ROTATE_0;
 * eink_ed028tc1_init(&eink_conf, false);
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
 * \note Note that the standard GFX functions only allow graphics to be drawn in black and white when using the
 * ED028TC1. 
 *
 * Since the ED028TC1 is a 4-bit greyscale display, there are several functions that are also available:
 * - eink_ed028tc1_graphics_load_4bgrey_image()
 * - eink_ed028tc1_set_pixel_raw()
 *
 * The canvas buffer can be uploaded to the display controller with the use of gfx_eink_put_display_buffer()
 *
 * If you decide to load the buffer but refresh the display later on, this can be achieved using the
 * gfx_eink_refresh_display_buffer(). Since the ED028TC1 supports multiple updates modes and can also specify
 * a windowed area to update, the function eink_ed028tc1_refresh_display_buffer_custom() is also available.
 *
 * \section files Main Files
 * - main.c : Initialize the Eink display and displays some demo screens.
 *
 * \section usage Usage
 * -# Build the program and download it into the board.
 * -# Wait for the LED0 on the SAM E54 Xplained Pro to turn off - indicates initialization is complete.
 * -# Press the SW0 on the SAM E54 Xplained Pro to display the demo.
 *
 * \section compinfo Compilation Information
 * This software was written for the GNU GCC compiler using Atmel Studio 7.0 and requires ASF version
 * 4 (originally compiled from Atmel START version 1.6.1881). Other compilers may or may not work.
 *
 * \section contactinfo Contact Information
 * For further information, visit
 * <A href="http://www.ineltek.com">Ineltek</A>.\n
 */
#define eink_project_excluded

#include <atmel_start.h>

//Add the Eink library
#include <eink_asf4_1_4.h>

//Add any additional fonts needed
#include <ArvoBold/ArvoBold_40pt.h>
#include <Arvo/Arvo_28pt.h>
//And any bitmaps
#include <ineltek_logo_4bGrey.h>
#include <ineltek_logo.h>

//Function prototypes
void eink_init_display();
void eink_draw_thick_line(eink_x_coordinate x1, eink_y_coordinate y1, eink_x_coordinate x2, eink_y_coordinate y2, enum eink_pixel_colour line_colour);

int main (void)
{
	struct uc8177_config eink_conf;
	uc8177_get_config_defaults(&eink_conf);
	
	system_init();

	eink_init_display();
	
	gfx_eink_graphics_fill_screen(FILL_WHITE);
	
	eink_ed028tc1_graphics_load_4bgrey_image(&eink_IneltekLogo_4bGrey, sizeof(eink_IneltekLogo_4bGrey), eink_IneltekLogo_4bGrey_width_px, eink_IneltekLogo_4bGrey_height_bytes, 20, 20);
	gfx_eink_text_write_string("Meeting Room 2", 20, 126, PIXEL_BLACK, ArvoBold_40pt);
	eink_draw_thick_line(20, 205, 570, 205, PIXEL_BLACK);
	
	gfx_eink_text_write_string("10:00", 20, 220, PIXEL_BLACK, Arvo_28pt);
	gfx_eink_text_write_string("-", 134, 220, PIXEL_BLACK, Arvo_28pt);
	gfx_eink_text_write_string("10:45", 160, 220, PIXEL_BLACK, Arvo_28pt);
	gfx_eink_text_write_string("John D", 290, 220, PIXEL_BLACK, Arvo_28pt);
	gfx_eink_text_write_string("11:00", 20, 270, PIXEL_BLACK, Arvo_28pt);
	gfx_eink_text_write_string("-", 134, 270, PIXEL_BLACK, Arvo_28pt);
	gfx_eink_text_write_string("11:30", 160, 270, PIXEL_BLACK, Arvo_28pt);
	gfx_eink_text_write_string("Team A PDR", 290, 270, PIXEL_BLACK, Arvo_28pt);
	gfx_eink_text_write_string("13:00", 20, 320, PIXEL_BLACK, Arvo_28pt);
	gfx_eink_text_write_string("-", 134, 320, PIXEL_BLACK, Arvo_28pt);
	gfx_eink_text_write_string("14:00", 160, 320, PIXEL_BLACK, Arvo_28pt);
	gfx_eink_text_write_string("Steve H", 290, 320, PIXEL_BLACK, Arvo_28pt);
	gfx_eink_text_write_string("14:30", 20, 370, PIXEL_BLACK, Arvo_28pt);
	gfx_eink_text_write_string("-", 134, 370, PIXEL_BLACK, Arvo_28pt);
	gfx_eink_text_write_string("15:30", 160, 370, PIXEL_BLACK, Arvo_28pt);
	gfx_eink_text_write_string("Paul T", 290, 370, PIXEL_BLACK, Arvo_28pt);
	gfx_eink_text_write_string("16:15", 20, 420, PIXEL_BLACK, Arvo_28pt);
	gfx_eink_text_write_string("-", 134, 420, PIXEL_BLACK, Arvo_28pt);
	gfx_eink_text_write_string("17:00", 160, 420, PIXEL_BLACK, Arvo_28pt);
	gfx_eink_text_write_string("Group Review", 290, 420, PIXEL_BLACK, Arvo_28pt);
	
	gfx_eink_put_display_buffer(true);
	
	gpio_set_pin_level(EINK_X_LED_0_PIN, 1);
	
	while(1)
	{
		if(!gpio_get_pin_level(EINK_X_BUTTON_0_PIN)) {
			while(!gpio_get_pin_level(EINK_X_BUTTON_0_PIN));
			break;
		}
	}
	
	while (1)
	{
		gpio_set_pin_level(EINK_X_LED_0_PIN, 0);
		gfx_eink_graphics_draw_rect(280, 223, 280, 45, PIXEL_WHITE, FILL_WHITE);
		gfx_eink_text_write_string(" - In Progress -", 290, 220, PIXEL_BLACK, Arvo_28pt);
		gfx_eink_put_display_buffer(false);
		eink_ed028tc1_refresh_display_buffer_custom(3, 280, 223, 280, 45);
		gpio_set_pin_level(EINK_X_LED_0_PIN, 1);
		
		delay_ms(500);
		
		gpio_set_pin_level(EINK_X_LED_0_PIN, 0);
		gfx_eink_graphics_draw_rect(280, 223, 280, 45, PIXEL_WHITE, FILL_WHITE);
		gfx_eink_text_write_string("10:00", 20, 220, PIXEL_BLACK, Arvo_28pt);
		gfx_eink_text_write_string("-", 134, 220, PIXEL_BLACK, Arvo_28pt);
		gfx_eink_text_write_string("10:45", 160, 220, PIXEL_BLACK, Arvo_28pt);
		gfx_eink_text_write_string("John D", 290, 220, PIXEL_BLACK, Arvo_28pt);
		gfx_eink_put_display_buffer(false);
		eink_ed028tc1_refresh_display_buffer_custom(3, 280, 223, 280, 45);
		gpio_set_pin_level(EINK_X_LED_0_PIN, 1);
		
		delay_ms(500);
	}
}

void eink_init_display(void)
{
	struct uc8177_config eink_conf;
	
	uc8177_get_config_defaults(&eink_conf);
	eink_conf.display_rotation = ROTATE_90;
	eink_ed028tc1_init(&eink_conf, false);
}

void eink_draw_thick_line(eink_x_coordinate x1, eink_y_coordinate y1, eink_x_coordinate x2, eink_y_coordinate y2, enum eink_pixel_colour line_colour)
{
	gfx_eink_graphics_draw_line((x1-1), (y1-1), (x2-1), (y2-1), PIXEL_BLACK);
	gfx_eink_graphics_draw_line(x1, y1, x2, y2, PIXEL_BLACK);
	gfx_eink_graphics_draw_line((x1+1), (y1+1), (x2+1), (y2+1), PIXEL_BLACK);
}