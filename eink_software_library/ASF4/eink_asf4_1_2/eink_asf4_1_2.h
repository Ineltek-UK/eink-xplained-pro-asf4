/**
 * \file
 * 
 * \brief Eink Software Library
 * Microchip ASF4 Variant - release 1.2 - January 2019
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

/** \mainpage
 * \section intro Introduction
 * This is Ineltek's Eink Software Library (version 1.2) for use with Microchip SAM MCUs and ASF4.
 *
 * It is designed to work with the following displays and driver ICs:
 * - <strong>ED029TC1</strong> : UC8151
 * - <strong>ED057TC2</strong> : UC8159
 * - <em><strong>HINK-E0154A05</strong> : SSD1608 (not yet implemented)</em>
 * 
 * It also includes several open source fonts in varying sizes and weights.
 * \note The library only allows a single display to be connected to the MCU.
 * 
 * \note At present only SAM devices are compatible with the ASF4 library. Mega, XMega and Tiny devices
 * are not currently supported. 
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
 * must use the Eink ASF3 Software Library.
 * 
 * \subsection setup_pins Pinmux Configuration
 * 
 * Next we need to define the pins for the display and ensure they are labelled correctly within
 * Atmel START. Pin directions are automatically configured by the library, so any direction can be set
 * within START. The following pins are required with the following labels, note the EXT1 pins have been
 * included if using the Eink Xplained Pro or Eink XL Xplained Pro:
 * - <strong>EINK_DC</strong>: Data/Command pin - <em>EXT1 Pin 3</em>
 * - <strong>EINK_BS</strong>: BS pin - <em>EXT1 Pin 5</em>
 * - <strong>EINK_BUSY</strong> : Busy pin - <em>EXT1 Pin 6</em>
 * - <strong>EINK_CSB</strong> : Chip Select pin - <em>EXT1 Pin 15</em>
 * - <strong>EINK_RST</strong> : Reset pin - <em>EXT1 Pin 16</em>

 * - <strong>EINK_SPI_SDA</strong> : Data pin - <em>EXT1 Pin 17</em>
 * - <strong>EINK_SPI_SCL</strong> : Clock pin - <em>EXT1 Pin 18</em>
 * 
 * If using the ED057TC2 (Eink XL Xplained Pro), we also need to define the following:
 * 
 * - <strong>EINK_FMSDO</strong> : Flash/EEPROM data output pin - <em>EXT1 Pin 11</em>
 * - <strong>EINK_MFCSB</strong> : MCU to Flash/EEPROM select pin - <em>EXT1 Pin 12</em>
 * 
 * \note Note that if using the library with an Xplained MCU board, pin assignments on EXT1 can vary
 * between different MCU boards.
 * 
 * We can also add the LEDs and Buttons on the Eink Xplained Pro and Eink Xplained XL Pro boards:
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
 * - <BASE_DIR>/eink_asf4_1_2
 * - <BASE_DIR>/eink_asf4_1_2/fonts
 *
 * Finally the following line must be added directly below the Atmel START include statement:
 * \code
 * #include <atmel_start.h>
 * #include <eink_asf4_1_2.h>
 * \endcode
 * 
 * \subsection setup_initialization Initialization Code
 *
 * We now need to add the initialization code to our main.c file to get the example up and running.
 *
 * The definition:
 * \code
 * #define eink_project_excluded
 * \endcode
 * must be add above the Atmel START include if the Eink library is not included in the Solution Explorer:
 * \code
 * #define eink_project_excluded
 * #include <atmel_start.h>
 * #include <eink_asf4_1_2.h>
 * \endcode
 *
 * To initialize the display, we have to first select which display we are using as the Eink Software
 * Library supports multiple Eink displays.
 *
 * \subsubsection setup_initialization_ed029tc1 ED029TC1 Initialization
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
 * \subsubsection setup_initialization_ED057TC2 ED057TC2 Initialization
 * To initialize the Eink library for use with the ED057TC2 (UC8159), the eink_ed057tc2_init() function
 * must be called. \ref uc8159_config must be used as the configuration struct. Note that
 * uc8159_get_config_defaults() should be called before making any required changes and passing them to
 * the initialization function.
 *
 * Example Code:
 *
 * \code
 * struct uc8159_config eink_conf;
 *
 * uc8159_get_config_defaults(&eink_conf);
 * eink_conf.temperature_sensor_settings.sensor_select = EXTERNAL_TEMPERATURE_SENSOR;
 * eink_conf.panel_settings.display_rotation = ROTATE_0;
 * eink_ed057tc2_init(&eink_conf, false);
 * \endcode
 *
 * \subsubsection setup_initialization_hink_e0154a05 HINK-E0154A05 Initialization
 * \note Note yet implemented
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
 * gfx_eink_refresh_display_buffer()
 *
 * \section compinfo Compilation Information
 * This software was written for the GNU GCC compiler using Atmel Studio 7.0 and requires ASF version
 * 4 (originally compiled from Atmel START version 1.4.1810). Other compilers may or may not work.
 *
 * \section contactinfo Contact Information
 * For further information, visit
 * <A href="http://www.ineltek.com">Ineltek</A>.\n
 */
#ifndef EINK_ASF4_H_INCLUDED
#define EINK_ASF4_H_INCLUDED

#include <stdlib.h>

#ifdef eink_project_excluded
    #include <eink/drivers/eink_driver.c>

    #include <eink/drivers/uc8151/uc8151.c>
    #include <eink/displays/ed029tc1.c>
    #include <eink/drivers/ssd1608/ssd1608.c>
    #include <eink/displays/hink_e0154a05.c>
    #include <eink/drivers/uc8159/uc8159.c>
    #include <eink/displays/ed057tc2.c>

    #include <eink/eink_gfx/eink_gfx.h>
    #include <eink/eink_gfx/eink_gfx_graphics.c>
    #include <eink/eink_gfx/eink_gfx_text.c>
#else
    #include <eink/drivers/eink_driver.h>

    #include <eink/drivers/uc8151/uc8151.h>
    #include <eink/displays/ed029tc1.h>
    #include <eink/drivers/ssd1608/ssd1608.h>
    #include <eink/displays/hink_e0154a05.h>
    #include <eink/drivers/uc8159/uc8159.h>
    #include <eink/displays/ed057tc2.h>

    #include <eink/eink_gfx/eink_gfx.h>
    #include <eink/eink_gfx/eink_gfx_graphics.h>
    #include <eink/eink_gfx/eink_gfx_text.h>
#endif

#endif /* EINK_ASF4_H_INCLUDED */