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
#include <hpl_gclk_base.h>
#include <hpl_pm_base.h>

void delay_driver_init(void)
{
	delay_init(SysTick);
}

void system_init(void)
{
	init_mcu();

	// GPIO on PA04

	gpio_set_pin_level(EINK_SPI_SDA,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(EINK_SPI_SDA, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(EINK_SPI_SDA, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA05

	gpio_set_pin_level(EINK_CSB,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(EINK_CSB, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(EINK_CSB, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA06

	gpio_set_pin_level(EINK_RST,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(EINK_RST, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(EINK_RST, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA07

	gpio_set_pin_level(EINK_SPI_SCL,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(EINK_SPI_SCL, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(EINK_SPI_SCL, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB00

	gpio_set_pin_level(EINK_DC,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(EINK_DC, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(EINK_DC, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB02

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

	// GPIO on PB03

	gpio_set_pin_level(EINK_X_LED_0_PIN,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(EINK_X_LED_0_PIN, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(EINK_X_LED_0_PIN, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB04

	// Set pin direction to input
	gpio_set_pin_direction(EINK_X_BUTTON_1_PIN, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(EINK_X_BUTTON_1_PIN,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_UP);

	gpio_set_pin_function(EINK_X_BUTTON_1_PIN, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB05

	gpio_set_pin_level(EINK_X_LED_1_PIN,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(EINK_X_LED_1_PIN, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(EINK_X_LED_1_PIN, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB06

	gpio_set_pin_level(EINK_BS,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(EINK_BS, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(EINK_BS, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB07

	gpio_set_pin_level(EINK_BUSY,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(EINK_BUSY, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(EINK_BUSY, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB08

	gpio_set_pin_level(EINK_X_LED_2_PIN,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(EINK_X_LED_2_PIN, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(EINK_X_LED_2_PIN, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB09

	// Set pin direction to input
	gpio_set_pin_direction(EINK_X_BUTTON_2_PIN, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(EINK_X_BUTTON_2_PIN,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_UP);

	gpio_set_pin_function(EINK_X_BUTTON_2_PIN, GPIO_PIN_FUNCTION_OFF);

	delay_driver_init();
}
