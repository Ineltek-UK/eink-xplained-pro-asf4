/**
 * \file
 * 
 * \brief IT8951 Eink Driver Component
 * 
 * Eink Software Library
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
#ifndef IT8951_H_INCLUDED
#define IT8951_H_INCLUDED

#include <compiler.h>
#include <hal_gpio.h>
#include <hal_delay.h>

#include <eink/drivers/eink_ite_driver.h>
#include <eink/drivers/it8951/it8951.h>

#include <eink/eink_gfx/eink_gfx.h>
#include <eink/eink_gfx/eink_gfx_graphics.h>

// Controller and Eink configuration file
#include <atmel_start_pins.h>

#ifdef __cplusplus
extern "C" {
#endif

//! \name IT8951 I80 Command defines.
//@{
#define IT8951_I80_SYS_RUN              0x0001
#define IT8951_I80_STANDBY              0x0002
#define IT8951_I80_SLEEP                0x0003
#define IT8951_I80_REG_RD               0x0010
#define IT8951_I80_REG_WR               0x0011
#define IT8951_I80_MEM_BST_RD_T         0x0012
#define IT8951_I80_MEM_BST_RD_S         0x0013
#define IT8951_I80_MEM_BST_WR           0x0014
#define IT8951_I80_MEM_BST_END          0x0015
#define IT8951_I80_LD_IMG               0x0020
#define IT8951_I80_LD_IMG_AREA          0x0021
#define IT8951_I80_LD_IMG_END           0x0022
#define IT8951_I80_SET_VCOM_VALUE		0x0039

#define IT8951_I80_USR_DPY_AREA         0x0034
#define IT8951_I80_USR_GET_DEV_INFO     0x0302
//@}

//Register Base Address
#define DISPLAY_REG_BASE				0x1000

//Update Parameter Setting Register
#define LUTAFSR							(DISPLAY_REG_BASE + 0x224)

//-------System Registers----------------
#define SYS_REG_BASE					0x0000

//Address of System Registers
#define I80CPCR							(SYS_REG_BASE + 0x04)
//-------Memory Converter Registers----------------
#define MCSR_BASE_ADDR					0x0200
#define MCSR							(MCSR_BASE_ADDR  + 0x0000)
#define LISAR							(MCSR_BASE_ADDR + 0x0008)

//! \name Fundamental Display defines
//@{
#define GFX_IT8951_PIXELS_PER_BYTE		2
//@}

//! \name ED043WC3 display buffers
//@{
volatile uint8_t *it8951_dtm_display_buffer;
//@}

/**
 * \brief IT8951 Device config structure.
 *
 * Retrieved from IT8951 during initialization, corresponds to firmware
 * loaded in IT8951. 
 */
struct it8951_config {
	uint16_t display_width;
	uint16_t display_height;
	uint16_t img_buffer_addr_l;
	uint16_t img_buffer_addr_h;
	uint16_t it8951_fwversion[8];
	uint16_t it8951_lutversion[8];
};

/**
 * \brief IT8951 Driver component software device instance structure.
 *
 * Used to retain software state information of an associated display.
 *
 * \note The fields of this structure should not be altered by the user
 *       application; they are reserved for module-internal use only.
 */
struct it8951_module {
    /** Display settings */
    struct it8951_config						display_config;						
    /** Display width */
    eink_coordinate								display_width;
    /** Display height */
    eink_coordinate								display_height;
	/** Display buffer size */
	uint32_t									display_buffer_length;
	/** Display rotation */
	enum eink_display_rotation					display_rotation;
	/** Image Buffer Base Address */
	uint32_t									img_buffer_addr;
};
volatile struct it8951_module it8951_global_instance;

//! \name UC8151 reset
//@{  
/**
 * \brief Perform a hard reset of the display driver.
 *
 * This functions will reset the IT8951 by setting the reset pin low for 10ms.
 */
static inline void it8951_hard_reset(void)
{
    gpio_set_pin_level(EINK_RST, 0);
    delay_ms(10);
    gpio_set_pin_level(EINK_RST, 1);
    delay_ms(50);
}
//@}


//! \name IT8951 busy check
//@{

/**
 * \brief Checks if the display driver is busy.
 *
 * This functions will read the IT8951's busy status by reading the HRDY pin and will
 * hold the MCU until it returns HIGH again.
 */
static inline void it8951_wait_for_hrdy(void)
{
#ifdef EINK_HOST_RDY
    while(!gpio_get_pin_level(EINK_HOST_RDY));
#endif
}
//@}

static inline void it8951_write_reg(uint16_t reg_addr, uint16_t reg_value)
{
	uint16_t data_send[2] = {reg_addr, reg_value};
	eink_ite_write_data(IT8951_I80_REG_WR, data_send, 2);
}

static inline uint16_t it8951_read_reg(uint16_t reg_addr)
{
	uint16_t reg_value;
	eink_ite_read_data(IT8951_I80_REG_RD, &reg_addr, 1, &reg_value, 1);	
	return reg_value;
}

static inline void it8951_write_vcom(uint16_t vcom_value)
{
	uint16_t data_send[2] = {2, vcom_value};
	eink_ite_write_data(IT8951_I80_SET_VCOM_VALUE, data_send, 2);
}

//! \name ED043WC3 display initialization function
//@{
void eink_it8951_init(
		enum eink_display_rotation display_rotation,
        bool clear_display);
//@}

//! \name ED043WC3 display functions
//@{
/**
 * \brief Send the refresh command to the display driver.
 *
 * \param update_mode what mode to refresh the entire display with
 */
 static inline void eink_it8951_refresh_display_mode(uint8_t update_mode)
{
	 uint16_t eink_data[5];
	 eink_data[0] = (uint16_t) 0;
	 eink_data[1] = (uint16_t) 0;
	 eink_data[2] = (uint16_t) it8951_global_instance.display_height;
	 eink_data[3] = (uint16_t) it8951_global_instance.display_width;
	 eink_data[4] = (uint16_t) update_mode;
	 /* Send I80 Display Command */
	 eink_ite_write_data(IT8951_I80_USR_DPY_AREA, &eink_data[0], 1);
	 eink_ite_write_data(0, &eink_data[1], 1);
	 eink_ite_write_data(0, &eink_data[2], 1);
	 eink_ite_write_data(0, &eink_data[3], 1);
	 eink_ite_write_data(0, &eink_data[4], 1);
 }
    
/**
 * \brief Send the refresh command to the display driver.
 */
 static inline void eink_it8951_refresh_display(void)
{
	eink_it8951_refresh_display_mode(2);
}

/**
 * \brief Send the refresh command to the display driver to update 
 * a partial window of the display.
 *
 * \param show_x starting x coordinate of the update window
 * \param show_y starting y coordinate of the update window
 * \param show_w width of the update window
 * \param show_h height of the update window
 * \param update_mode what mode to refresh the display with
 */
static inline void eink_it8951_refresh_partial_display(
	eink_coordinate show_x,
	eink_coordinate show_y,
	eink_coordinate show_w,
	eink_coordinate show_h,
	uint8_t update_mode)
{
	uint16_t eink_data[5];
	switch(it8951_global_instance.display_rotation)
	{
		case ROTATE_0:
			eink_data[0] = (uint16_t) show_y;
			eink_data[1] = (uint16_t) show_x;
			eink_data[2] = (uint16_t) show_h;
			eink_data[3] = (uint16_t) show_w;
			break;
		case ROTATE_90:
			eink_data[0] = (uint16_t) show_x;
			eink_data[1] = (uint16_t) it8951_global_instance.display_width-show_y-show_h;
			eink_data[2] = (uint16_t) show_w;
			eink_data[3] = (uint16_t) show_h;
			break;
		case ROTATE_180:
			eink_data[0] = (uint16_t) it8951_global_instance.display_height-show_y-show_h;
			eink_data[1] = (uint16_t) it8951_global_instance.display_width-show_x-show_w;
			eink_data[2] = (uint16_t) show_h;
			eink_data[3] = (uint16_t) show_w;
			break;
		case ROTATE_270:
			eink_data[0] = (uint16_t) it8951_global_instance.display_height-show_x-show_w;
			eink_data[1] = (uint16_t) show_y;
			eink_data[2] = (uint16_t) show_w;
			eink_data[3] = (uint16_t) show_h;
			break;
	}
	eink_data[4] = (uint16_t) update_mode;
	/* Send I80 Display Command */
	eink_ite_write_data(IT8951_I80_USR_DPY_AREA, &eink_data[0], 1);
	eink_ite_write_data(0, &eink_data[1], 1);
	eink_ite_write_data(0, &eink_data[2], 1);
	eink_ite_write_data(0, &eink_data[3], 1);
	eink_ite_write_data(0, &eink_data[4], 1);
}

void eink_it8951_put_display_buffer(
		bool refresh_display);

void eink_it8951_put_partial_display_buffer(
		eink_coordinate start_x, 
		eink_coordinate start_y, 
		eink_coordinate window_w,
		eink_coordinate window_h);

void eink_it8951_set_pixel_raw(
        eink_coordinate x_set,
        eink_coordinate y_set,
        uint8_t pixel_value);

void eink_it8951_graphics_load_4bgrey_image(
        uint8_t *img_array,
        uint16_t array_size,
        eink_coordinate image_width_px,
        eink_coordinate image_height_bytes,
        eink_coordinate x_place,
        eink_coordinate y_place);
//@}

#ifdef __cplusplus
}
#endif

#endif /* IT8951_H_INCLUDED */