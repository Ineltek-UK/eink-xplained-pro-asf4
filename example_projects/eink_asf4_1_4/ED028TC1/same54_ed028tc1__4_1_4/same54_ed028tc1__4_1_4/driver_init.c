/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_init.h"
#include <peripheral_clk_config.h>
#include <utils.h>
#include <hal_init.h>

void delay_driver_init(void)
{
	delay_init(SysTick);
}

void system_init(void)
{
	init_mcu();

	// GPIO on PA07

	gpio_set_pin_level(EINK_BUSY,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(EINK_BUSY, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(EINK_BUSY, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB26

	gpio_set_pin_level(EINK_SPI_SCL,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(EINK_SPI_SCL, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(EINK_SPI_SCL, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB27

	gpio_set_pin_level(EINK_RST,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(EINK_RST, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(EINK_RST, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB28

	gpio_set_pin_level(EINK_CSB,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(EINK_CSB, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(EINK_CSB, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB29

	gpio_set_pin_level(EINK_SPI_SDA,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(EINK_SPI_SDA, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(EINK_SPI_SDA, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB31

	// Set pin direction to input
	gpio_set_pin_direction(EINK_X_BUTTON_0_PIN, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(EINK_X_BUTTON_0_PIN,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_UP);

	gpio_set_pin_function(EINK_X_BUTTON_0_PIN, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC18

	gpio_set_pin_level(EINK_X_LED_0_PIN,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(EINK_X_LED_0_PIN, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(EINK_X_LED_0_PIN, GPIO_PIN_FUNCTION_OFF);

	delay_driver_init();
}
