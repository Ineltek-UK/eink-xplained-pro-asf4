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

struct spi_m_sync_descriptor SPI_0;

void SPI_0_PORT_init(void)
{

	gpio_set_pin_level(EINK_SPI_MOSI,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(EINK_SPI_MOSI, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(EINK_SPI_MOSI, PINMUX_PB27D_SERCOM4_PAD0);

	gpio_set_pin_level(EINK_SPI_SCK,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(EINK_SPI_SCK, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(EINK_SPI_SCK, PINMUX_PB26D_SERCOM4_PAD1);

	// Set pin direction to input
	gpio_set_pin_direction(EINK_SPI_MISO, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(EINK_SPI_MISO,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(EINK_SPI_MISO, PINMUX_PB29D_SERCOM4_PAD3);
}

void SPI_0_CLOCK_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM4_GCLK_ID_CORE, CONF_GCLK_SERCOM4_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM4_GCLK_ID_SLOW, CONF_GCLK_SERCOM4_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	hri_mclk_set_APBDMASK_SERCOM4_bit(MCLK);
}

void SPI_0_init(void)
{
	SPI_0_CLOCK_init();
	spi_m_sync_init(&SPI_0, SERCOM4);
	SPI_0_PORT_init();
}

void delay_driver_init(void)
{
	delay_init(SysTick);
}

void system_init(void)
{
	init_mcu();

	// GPIO on PA04

	gpio_set_pin_level(EINK_X_LED_0_PIN,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(EINK_X_LED_0_PIN, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(EINK_X_LED_0_PIN, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA05

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

	// GPIO on PA06

	gpio_set_pin_level(EINK_X_LED_1_PIN,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(EINK_X_LED_1_PIN, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(EINK_X_LED_1_PIN, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA07

	// Set pin direction to input
	gpio_set_pin_direction(EINK_HOST_RDY, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(EINK_HOST_RDY,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_DOWN);

	gpio_set_pin_function(EINK_HOST_RDY, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA27

	gpio_set_pin_level(EINK_SRAM_CSB,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(EINK_SRAM_CSB, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(EINK_SRAM_CSB, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB04

	gpio_set_pin_level(EINK_RST,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(EINK_RST, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(EINK_RST, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB07

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

	// GPIO on PB08

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

	// GPIO on PB09

	gpio_set_pin_level(EINK_X_LED_2_PIN,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(EINK_X_LED_2_PIN, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(EINK_X_LED_2_PIN, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB28

	gpio_set_pin_level(EINK_SPI_CSB,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(EINK_SPI_CSB, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(EINK_SPI_CSB, GPIO_PIN_FUNCTION_OFF);

	SPI_0_init();

	delay_driver_init();
}
