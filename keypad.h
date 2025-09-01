/* --------------------------------------------------------------------------
 * keypad.h
 * EE 329 A5: Keypad Module Header
 *
 * Description:
 * Provides function prototypes and GPIO pin macros for 4x4 matrix keypad
 * interfaced via GPIOD.
 *
 * Author: Nathan Heil
 * Date: Apr 12, 2025
 * -------------------------------------------------------------------------- */

#ifndef INC_KEYPAD_H_
#define INC_KEYPAD_H_

#include "stm32l4xx_hal.h"

#define KeypadPort GPIOD    // Define keypad port as GPIOD
#define KEYPAD_COL_PINS (GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7)

void Keypad_Init(void);    // Configure keypad GPIOs
int Keypad_GetKey(void);   // Poll keypad for keypress

#endif /* INC_KEYPAD_H_ */
