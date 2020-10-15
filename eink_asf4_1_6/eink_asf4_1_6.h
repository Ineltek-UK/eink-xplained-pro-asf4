/**
 * \file
 * 
 * \brief Eink Software Library
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

/** \mainpage
 * \section intro Introduction
 * This is Ineltek's Eink Software Library (version 1.6) for use with Microchip SAM MCUs and ASF4.
 *
 * It is designed to work with the following displays and driver ICs libraries:
 * - <strong>ED013TC1</strong> : UC8173
 * - <strong>EL026TR1</strong> : UC8151
 * - <strong>ED028TC1</strong> : UC8177
 * - <strong>ED029TC1</strong> : UC8151
 * - <strong>ED037TC1</strong> : SSD1677
 * - <strong>AC057TC1</strong> : UC8159
 * - <strong>ED057TC2</strong> : UC8159
 * - <strong>HINK-E0154A45</strong> : SSD1608
 * - IT8951 <em>(example project uses the ED043WC3)</em>
 *
 * \note Displays may use alternative, but compatible driver ICs.
 * 
 * The following displays will also work correctly by using the equivalent display functions:
 * - <strong>EL026TR2</strong> : uses the EL026TR1 display functions
 * - <strong>EL029TR1</strong> : uses the ED029TC1 display functions (minor configuration change)
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
 * If using the IT8951, you will also require the following software component from Atmel START:
 * - SPI (driver) - SPI_Master_Sync mode
 * 
 * \note Note that this library is not compatible with the ASF Wizard in Atmel Studio - for this you
 * must use the Eink ASF3 Software Library - no longer in development.
 * 
 * \subsection setup_pins Pinmux Configuration
 * 
 * Next we need to define the pins for the display and ensure they are labelled correctly within
 * Atmel START. Pin directions are automatically configured by the library, so any direction can be set
 * within START. The following pins are required with the following labels, note the EXT1 pins have been
 * included if using the Eink Xplained Pro or Eink XL Xplained Pro:
 * - <strong>EINK_DC</strong> : Data/Command pin - <em>EXT1 Pin 3</em> (Not required for ED028TC1)
 * - <strong>EINK_BS</strong> : BS pin - <em>EXT1 Pin 5</em> (Not required for ED028TC1)
 * - <strong>EINK_BUSY</strong> : Busy pin - <em>EXT1 Pin 6</em>
 * - <strong>EINK_CSB</strong> : Chip Select pin - <em>EXT1 Pin 15</em>
 * - <strong>EINK_RST</strong> : Reset pin - <em>EXT1 Pin 16</em>

 * - <strong>EINK_SPI_SDA</strong> : Data pin - <em>EXT1 Pin 17</em>
 * - <strong>EINK_SPI_SCL</strong> : Clock pin - <em>EXT1 Pin 18</em>
 * 
 * If using the ED057TC2 or AC057TC1 (Eink XL Xplained Pro), we also need to define the following:
 * - <strong>EINK_FMSDO</strong> : Flash/EEPROM data output pin - <em>EXT1 Pin 11</em>
 * - <strong>EINK_MFCSB</strong> : MCU to Flash/EEPROM select pin - <em>EXT1 Pin 12</em>
 * 
 * If using the IT8951 (Eink ITE Xplained Pro), the SPI module must be setup in SPI_Master_Sync
 * mode with the name <strong>SPI_0</strong>. We must also define the following pins instead of the pins
 * mentioned above:
 * - <strong>EINK_RST</strong> : Reset pin - <em>EXT1 Pin 3</em>
 * - <strong>EINK_HOST_RDY</strong> : Host ready pin - <em>EXT1 Pin 6</em>
 * - <strong>EINK_SPI_CSB</strong> : SPI chip select pin - <em>EXT1 Pin 15</em>
 * - <strong>EINK_SPI_SCK</strong> : SPI clock pin - <em>EXT1 Pin 18</em>
 * - <strong>EINK_SPI_MOSI</strong> : SPI Data In pin - <em>EXT1 Pin 16</em>
 * - <strong>EINK_SPI_MISO</strong> : SPI Data Out pin - <em>EXT1 Pin 17</em>
 * 
 * \note Note that if using the library with an Xplained MCU board, pin assignments on EXT1 can vary
 * between different MCU boards.
 * 
 * We can also add the LEDs and Buttons on the Eink Xplained Pro, Eink Xplained XL Pro and Eink ITE Xplained
 * Pro boards:
 * - <strong>EINK_X_LED_0_PIN</strong> : LED 1 - <em>EXT1 Pin 8</em>
 * - <strong>EINK_X_LED_1_PIN</strong> : LED 2 - <em>EXT1 Pin 10 (*see note)</em>
 * - <strong>EINK_X_LED_2_PIN</strong> : LED 3 - <em>EXT1 Pin 14</em>
 *
 * - <strong>EINK_X_BUTTON_0_PIN</strong> : Button 1 - <em>EXT1 Pin 7</em>
 * - <strong>EINK_X_BUTTON_1_PIN</strong> : Button 2 - <em>EXT1 Pin 9</em>
 * - <strong>EINK_X_BUTTON_2_PIN</strong> : Button 3 - <em>EXT1 Pin 13</em>
 * 
 * \note slight variation on the Eink ITE Xplained Pro board:
 * - <strong>EINK_X_LED_1_PIN</strong> : LED 3 - <em>EXT1 Pin 14</em>
 * 
 * \subsection setup_project Atmel Studio Project Configuration
 *
 * Once your Atmel Studio project has been generated from Atmel START, we now need to add the library
 * to the compiler include paths:<br>
 * See Project Properties > Toolchain > ARM/GNU C Compiler > Directories:
 * - <BASE_DIR>/eink_asf4_1_6
 * - <BASE_DIR>/eink_asf4_1_6/fonts
 *
 * Finally the following line must be added directly below the Atmel START include statement:
 * \code
 * #include <atmel_start.h>
 * #include <eink_asf4_1_6.h>
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
 * #include <eink_asf4_1_6.h>
 * \endcode
 *
 * To initialize the display, we have to first select which display we are using as the Eink Software
 * Library supports multiple Eink displays.
 *
 * \subsubsection setup_initialization_ed013tc1 ED013TC1 Initialization
 * To initialize the Eink library for use with the ED013TC1 (UC8173), the eink_ed013tc1_init() function
 * must be called. \ref uc8173_config must be used as the configuration struct. Note that
 * uc8173_get_config_defaults() should be called before making any required changes and passing them to
 * the initialization function.
 *
 * Example Code:
 *
 * \code
 * struct uc8173_config eink_conf;
 *
 * uc8173_get_config_defaults(&eink_conf);
 * eink_conf.display_rotation = ROTATE_0;
 * eink_ed013tc1_init(&eink_conf, false);
 * \endcode
 *
 *
 * \subsubsection setup_initialization_el026tr1 EL026TR1 Initialization
 * To initialize the Eink library for use with the EL026TR1 (UC8151), the eink_el026tr1_init() function
 * must be called. \ref uc8151_config must be used as the configuration struct. Note that
 * uc8151_get_config_defaults() should be called before making any required changes and passing them to
 * the initialization function.
 * 
 * \note No configuration changes are required if using EL026TR2
 *
 * Example Code:
 *
 * \code
 * struct uc8151_config eink_conf;
 *
 * uc8151_get_config_defaults(&eink_conf);
 * eink_conf.temperature_sensor_settings.sensor_select = EXTERNAL_TEMPERATURE_SENSOR;
 * eink_conf.panel_settings.display_colours = B_W_R_PIXELS;
 * eink_conf.panel_settings.display_rotation = ROTATE_180;
 * eink_el026tr1_init(&eink_conf, false);
 * \endcode
 *
 * \subsubsection setup_initialization_ed028tc1 ED028TC1 Initialization
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
 * \subsubsection setup_initialization_ed029tc1 ED029TC1 Initialization
 * To initialize the Eink library for use with the ED029TC1 (UC8151), the eink_ed029tc1_init() function
 * must be called. \ref uc8151_config must be used as the configuration struct. Note that
 * uc8151_get_config_defaults() should be called before making any required changes and passing them to
 * the initialization function.
 * 
 * \note If using the EL029TR1, the config struct must specify that the display has 3 colors:
 * \code
 * eink_conf.panel_settings.display_colours = B_W_R_PIXELS;
 * \endcode
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
 * \subsubsection setup_initialization_ed037tc1 ED037TC1 Initialization
 * To initialize the Eink library for use with the ED037TC1 (SSD1677), the eink_ed037tc1_init() function
 * must be called. \ref ssd1677_config must be used as the configuration struct. Note that
 * ssd1677_get_config_defaults() should be called before making any required changes and passing them to
 * the initialization function.
 * 
 * Example Code:
 *
 * \code
 * struct ssd1677_config eink_conf;
 *
 * ssd1677_get_config_defaults(&eink_conf);
 * eink_conf.temperature_sensor_settings.sensor_select = EXTERNAL_TEMPERATURE_SENSOR;
 * eink_conf.panel_settings.display_rotation = ROTATE_0;
 * eink_ed037tc1_init(&eink_conf, false);
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
 * \subsubsection setup_initialization_hink_e0154a45 HINK-E0154A45 Initialization
 * To initialize the Eink library for use with the HINK-E0154A45 (SSD1608), the eink_hink_e0154a45_init() function
 * must be called. \ref ssd1608_config must be used as the configuration struct. Note that
 * ssd1608_get_config_defaults() should be called before making any required changes and passing them to
 * the initialization function.
 *
 * Example Code:
 *
 * \code
 * struct ssd1608_config eink_conf;
 *
 * ssd1608_get_config_defaults(&eink_conf);
 * eink_conf.display_rotation = ROTATE_0;
 * eink_hink_e0154a45_init(&eink_conf, false);
 * \endcode
 *
 * \subsubsection setup_initialization_it8951 IT8951 Initialization
 * To initialize the Eink library for use with the IT8951, the eink_it8951_init() function
 * must be called. This function only requires two parameters to be passed with it, the display rotation and
 * whether or not to update the display after initialisation. The IT8951 uses the firmware file in its external
 * flash to retrieve the majority of its settings.
 *
 * Example Code:
 *
 * \code
 * eink_it8951_init(ROTATE_270, false);
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
 * If using a greyscale display/driver (ED028TC1/IT8951), there are a few extra functions that can be passed a raw
 * value denoting the shade of gray:
 * - gfx_eink_graphics_fill_screen_raw()
 * - gfx_eink_load_4bgrey_image()
 * - gfx_eink_graphics_draw_line_raw()
 * - gfx_eink_graphics_draw_rect_raw()
 * - gfx_eink_text_write_string_raw()
 * - gfx_eink_set_pixel_raw()
 * 
 * The canvas buffer can be uploaded to the display controller with the use of gfx_eink_put_display_buffer()
 *
 * If you decide to load the buffer but refresh the display later on, this can be achieved using the 
 * gfx_eink_refresh_display()
 *
 * If using the IT8951, there are several more options available to do partial uploads of data, partial updates of the display
 * and updates using several modes. The following functions can be used:
 * - gfx_eink_put_display_buffer()
 * - gfx_eink_put_partial_display_buffer()
 * - gfx_eink_refresh_display()
 * - gfx_eink_refresh_display_mode()
 * - gfx_eink_refresh_partial_display()
 *
 * \section compinfo Compilation Information
 * This software was written for the GNU GCC compiler using Atmel Studio 7.0 and requires ASF version
 * 4 (originally compiled from Atmel START version 1.8.449). Other compilers may or may not work.
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
    #include <eink/drivers/eink_ite_driver.c>

	#include <eink/drivers/uc8173/uc8173.c>
	#include <eink/displays/ed013tc1.c>
    #include <eink/drivers/uc8151/uc8151.c>
    #include <eink/displays/ed029tc1.c>
    #include <eink/displays/el026tr1.c>
    #include <eink/drivers/ssd1608/ssd1608.c>
    #include <eink/displays/hink_e0154a05.c>
    #include <eink/displays/hink_e0154a45.c>
    #include <eink/drivers/uc8159/uc8159.c>
    #include <eink/displays/ac057tc1.c>
    #include <eink/displays/ed057tc2.c>
    #include <eink/drivers/uc8177/uc8177.c>
    #include <eink/displays/ed028tc1.c>
    #include <eink/drivers/ssd1677/ssd1677.c>
    #include <eink/displays/ed037tc1.c>
    #include <eink/drivers/it8951/it8951.c>

    #include <eink/eink_gfx/eink_gfx.h>
    #include <eink/eink_gfx/eink_gfx_graphics.c>
    #include <eink/eink_gfx/eink_gfx_text.c>
#else
    #include <eink/drivers/eink_driver.h>
    #include <eink/drivers/eink_ite_driver.h>

	#include <eink/drivers/uc8173/uc8173.h>
	#include <eink/displays/ed013tc1.h>
    #include <eink/drivers/uc8151/uc8151.h>
    #include <eink/displays/ed029tc1.h>
    #include <eink/displays/el026tr1.h>
    #include <eink/drivers/ssd1608/ssd1608.h>
    #include <eink/displays/hink_e0154a05.h>
    #include <eink/drivers/uc8159/uc8159.h>
    #include <eink/displays/ac057tc1.h>
    #include <eink/displays/ed057tc2.h>
    #include <eink/drivers/uc8177/uc8177.h>
    #include <eink/displays/ed028tc1.h>
    #include <eink/drivers/ssd1677/ssd1677.h>
    #include <eink/displays/ed037tc1.h>
    #include <eink/drivers/it8951/it8951.h>

    #include <eink/eink_gfx/eink_gfx.h>
    #include <eink/eink_gfx/eink_gfx_graphics.h>
    #include <eink/eink_gfx/eink_gfx_text.h>
#endif

#endif /* EINK_ASF4_H_INCLUDED */