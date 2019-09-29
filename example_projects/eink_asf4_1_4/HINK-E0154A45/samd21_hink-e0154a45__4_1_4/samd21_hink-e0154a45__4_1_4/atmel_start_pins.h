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

// SAMD21 has 8 pin functions

#define GPIO_PIN_FUNCTION_A 0
#define GPIO_PIN_FUNCTION_B 1
#define GPIO_PIN_FUNCTION_C 2
#define GPIO_PIN_FUNCTION_D 3
#define GPIO_PIN_FUNCTION_E 4
#define GPIO_PIN_FUNCTION_F 5
#define GPIO_PIN_FUNCTION_G 6
#define GPIO_PIN_FUNCTION_H 7

#define EINK_SPI_SDA GPIO(GPIO_PORTA, 4)
#define EINK_CSB GPIO(GPIO_PORTA, 5)
#define EINK_RST GPIO(GPIO_PORTA, 6)
#define EINK_SPI_SCL GPIO(GPIO_PORTA, 7)
#define EINK_DC GPIO(GPIO_PORTB, 0)
#define EINK_X_BUTTON_0_PIN GPIO(GPIO_PORTB, 2)
#define EINK_X_LED_0_PIN GPIO(GPIO_PORTB, 3)
#define EINK_X_BUTTON_1_PIN GPIO(GPIO_PORTB, 4)
#define EINK_X_LED_1_PIN GPIO(GPIO_PORTB, 5)
#define EINK_BS GPIO(GPIO_PORTB, 6)
#define EINK_BUSY GPIO(GPIO_PORTB, 7)
#define EINK_X_LED_2_PIN GPIO(GPIO_PORTB, 8)
#define EINK_X_BUTTON_2_PIN GPIO(GPIO_PORTB, 9)

#endif // ATMEL_START_PINS_H_INCLUDED
