/**
 * \file
 * 
 * \brief Eink Xplained Pro Demo - SAMD21 - ED013TC1
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

//And any bitmaps
#include <arduino_barcode.h>
#include <ineltek_logo.h>

//Function prototypes
void eink_init_display();
void eink_draw_display(uint8_t display_no);

int main (void)
{
	uint8_t eink_data[7];
	
	system_init();

	uc8173_init();
	
	eink_data[0] = 0x17;
	eink_data[1] = 0x97;
	eink_data[2] = 0x20;
	eink_write_data(UC8173_BTST, eink_data, 3);
	
	eink_data[0] = 0x03;
	eink_data[1] = 0x01;
	eink_data[2] = 0x2B;
	eink_data[3] = 0x2B;
	eink_data[4] = 0x00;
	eink_write_data(UC8173_PWR, eink_data, 5);
	   
	
	eink_write_data(UC8173_PON, 0, 0);
	uc8173_wait_for_busy_low();
	
	
	eink_data[0] = 0x0B;
	eink_data[1] = 0x86;
	eink_write_data(UC8173_PSR, eink_data, 2);


	eink_data[0] = 0x00;
	eink_write_data(UC8173_PFS, eink_data, 1);
	
	
	eink_data[0] = 0x25;
	eink_write_data(UC8173_LPRD, eink_data, 1);

	eink_data[0] = 0x00;
	eink_write_data(UC8173_TSE, eink_data, 1);


	eink_data[0] = 0xE1;
	eink_data[1] = 0x20;
	eink_data[2] = 0x10;
	eink_write_data(UC8173_CDI, eink_data, 3);

	eink_data[0] = 0x7F;
	eink_data[1] = 0x00;
	eink_data[2] = 0xFF;
	eink_write_data(UC8173_TRES, eink_data, 3);
	
	eink_data[0] = 0xA9;
	eink_data[1] = 0xA9;
	eink_data[2] = 0xEB;
	eink_data[3] = 0xEB;
	eink_data[4] = 0x02;
	eink_write_data(UC8173_GDS, eink_data, 5);
  
	/*spi_9b_send_9b(AMV);
	spi_9b_send(1,0x11);
	SPI_COMMAND(VV);
	unsigned char vcom_temp = spi_9b_get();
	vcom_temp = vcom_temp + 12;
	AutoVCOM = vcom_temp;
	   
	spi_9b_send_9b(VDCS);
	spi_9b_send(1,AutoVCOM);  //0x12
	   
	spi_9b_send_9b(VBDS);
	spi_9b_send(1,AutoVCOM);*/

	eink_data[0] = 0x02;
	eink_write_data(UC8173_LVSEL, eink_data, 1);
	
	eink_data[0] = 0x02;
	eink_data[1] = 0x02;
	eink_write_data(UC8173_GBS, eink_data, 2);
	
	eink_data[0] = 0x02;
	eink_data[1] = 0x02;
	eink_write_data(UC8173_GSS, eink_data, 2);	
	
	eink_write_data(UC8173_POF, 0, 0);
	uc8173_wait_for_busy_high();
	
	gpio_set_pin_level(EINK_X_LED_0_PIN, 1);
	
	while(1);
	
	eink_data[0] = 0x00;
	eink_data[1] = 0x00;
	eink_data[2] = 0x00;
	eink_data[3] = 0x7F;
	eink_data[4] = 0x00;
	eink_data[5] = 0xFF;
	eink_write_data(UC8173_DTMW, eink_data, 6);

	#define GFX_ED013TC1_MAX_WIDTH              128
	#define GFX_ED013TC1_MAX_HEIGHT             256
	#define GFX_ED013TC1_PIXELS_PER_BYTE        4
	#define GFX_ED013TC1_DISPLAY_BUFFER_SIZE    ((GFX_ED013TC1_MAX_WIDTH * GFX_ED013TC1_MAX_HEIGHT) / GFX_ED013TC1_PIXELS_PER_BYTE)

	volatile uint8_t *ed013tc1_dtm2_display_buffer;
	uint32_t malloc_size = GFX_ED013TC1_DISPLAY_BUFFER_SIZE * sizeof(uint8_t);
	ed013tc1_dtm2_display_buffer = (uint8_t*) malloc (malloc_size);
	
	uint16_t buffer_index;
	
	/* Set both display buffers to all WHITE. */
	for (buffer_index = 0; buffer_index < GFX_ED013TC1_DISPLAY_BUFFER_SIZE; buffer_index++) {
		/* Note that if the memory size is too small, a hard fault will occur here */
		ed013tc1_dtm2_display_buffer[buffer_index] = 0xFF;
	}
	
	eink_write_data(UC8173_DTM2, ed013tc1_dtm2_display_buffer, GFX_ED013TC1_DISPLAY_BUFFER_SIZE);

	
	eink_data[0] = 0x00;
	eink_data[1] = 0x00;
	eink_data[2] = 0x00;
	eink_data[3] = 0x00;
	eink_data[4] = 0x7F;
	eink_data[5] = 0x00;
	eink_data[6] = 0xFF;
	eink_write_data(UC8173_DRF, eink_data, 7);
	uc8173_wait_for_busy_low();
	
}

void eink_init_display(void)
{
	uc8173_init();
}

void eink_draw_display(uint8_t display_no)
{
	uint16_t i;
	char writeText[16];
	
	gpio_set_pin_level(EINK_X_LED_0_PIN, 0);
	
	gfx_eink_graphics_fill_screen(FILL_WHITE);
	

	
	/* Send both display buffers to the display. */
	gfx_eink_put_display_buffer(true);
	
	gpio_set_pin_level(EINK_X_LED_0_PIN, 1);
}