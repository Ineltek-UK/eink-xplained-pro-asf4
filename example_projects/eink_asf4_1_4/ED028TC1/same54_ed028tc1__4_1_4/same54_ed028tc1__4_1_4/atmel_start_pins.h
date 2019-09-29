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

// SAME54 has 14 pin functions

#define GPIO_PIN_FUNCTION_A 0
#define GPIO_PIN_FUNCTION_B 1
#define GPIO_PIN_FUNCTION_C 2
#define GPIO_PIN_FUNCTION_D 3
#define GPIO_PIN_FUNCTION_E 4
#define GPIO_PIN_FUNCTION_F 5
#define GPIO_PIN_FUNCTION_G 6
#define GPIO_PIN_FUNCTION_H 7
#define GPIO_PIN_FUNCTION_I 8
#define GPIO_PIN_FUNCTION_J 9
#define GPIO_PIN_FUNCTION_K 10
#define GPIO_PIN_FUNCTION_L 11
#define GPIO_PIN_FUNCTION_M 12
#define GPIO_PIN_FUNCTION_N 13

#define EINK_BUSY GPIO(GPIO_PORTA, 7)
#define EINK_SPI_SCL GPIO(GPIO_PORTB, 26)
#define EINK_RST GPIO(GPIO_PORTB, 27)
#define EINK_CSB GPIO(GPIO_PORTB, 28)
#define EINK_SPI_SDA GPIO(GPIO_PORTB, 29)
#define EINK_X_BUTTON_0_PIN GPIO(GPIO_PORTB, 31)
#define EINK_X_LED_0_PIN GPIO(GPIO_PORTC, 18)

#endif // ATMEL_START_PINS_H_INCLUDED
