/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef ATMEL_START_PINS_H_INCLUDED
#define ATMEL_START_PINS_H_INCLUDED

#include <hal_gpio.h>

// SAMG has 4 pin functions

#define GPIO_PIN_FUNCTION_A 0
#define GPIO_PIN_FUNCTION_B 1
#define GPIO_PIN_FUNCTION_C 2
#define GPIO_PIN_FUNCTION_D 3

#define EINK_X_BUTTON_0_PIN GPIO(GPIO_PORTA, 0)
#define EINK_X_BUTTON_2_PIN GPIO(GPIO_PORTA, 9)
#define EINK_X_LED_2_PIN GPIO(GPIO_PORTA, 10)
#define EINK_CSB GPIO(GPIO_PORTA, 11)
#define EINK_SPI_SDA GPIO(GPIO_PORTA, 12)
#define EINK_RST GPIO(GPIO_PORTA, 13)
#define EINK_SPI_SCL GPIO(GPIO_PORTA, 14)
#define EINK_DC GPIO(GPIO_PORTA, 17)
#define EINK_X_LED_0_PIN GPIO(GPIO_PORTA, 23)
#define EINK_X_BUTTON_1_PIN GPIO(GPIO_PORTA, 24)
#define EINK_BUSY GPIO(GPIO_PORTA, 25)
#define EINK_BS GPIO(GPIO_PORTA, 26)
#define EINK_X_LED_1_PIN GPIO(GPIO_PORTA, 29)

#endif // ATMEL_START_PINS_H_INCLUDED
