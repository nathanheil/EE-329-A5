/* --------------------------------------------------------------------------
 * keypad.c
 * EE 329 A5: 4x4 Keypad Interface
 *
 * Description:
 * Configures GPIOD to interface with a 4x4 matrix keypad.
 * Drives columns as outputs, reads rows as inputs.
 * Performs polling and debounced key detection.
 *
 * Author: Nathan Heil, Benjamin Tavares
 * Date: Apr 12, 2025
 * -------------------------------------------------------------------------- */

#include "stm32l4xx_hal.h"
#include "keypad.h"

/* --------------------------------------------------------------------------
 * Function: Keypad_Init
 * Configures GPIOD pins PD0–PD3 as inputs with pulldowns (rows),
 * and PD4–PD7 as outputs (columns) with high-speed push-pull.
 * -------------------------------------------------------------------------- */
void Keypad_Init(void) {
   RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN;       // Enable clock for GPIOD

   // Clear modes for PD0–PD7
   KeypadPort->MODER &= ~(GPIO_MODER_MODE0 | GPIO_MODER_MODE1 |
                          GPIO_MODER_MODE2 | GPIO_MODER_MODE3 |
                          GPIO_MODER_MODE4 | GPIO_MODER_MODE5 |
                          GPIO_MODER_MODE6 | GPIO_MODER_MODE7);

   // Set PD4–PD7 (columns) as outputs
   KeypadPort->MODER |= (GPIO_MODER_MODE4_0 | GPIO_MODER_MODE5_0 |
                         GPIO_MODER_MODE6_0 | GPIO_MODER_MODE7_0);

   // Set output type to push-pull for PD4–PD7
   KeypadPort->OTYPER &= ~(GPIO_OTYPER_OT4 | GPIO_OTYPER_OT5 |
                           GPIO_OTYPER_OT6 | GPIO_OTYPER_OT7);

   // Clear PUPDR settings for PD0–PD3
   KeypadPort->PUPDR &= ~(GPIO_PUPDR_PUPD0 | GPIO_PUPDR_PUPD1 |
                          GPIO_PUPDR_PUPD2 | GPIO_PUPDR_PUPD3);

   // Enable pull-downs on PD0–PD3 (rows)
   KeypadPort->PUPDR |= (2 << GPIO_PUPDR_PUPD0_Pos) |
                        (2 << GPIO_PUPDR_PUPD1_Pos) |
                        (2 << GPIO_PUPDR_PUPD2_Pos) |
                        (2 << GPIO_PUPDR_PUPD3_Pos);

   // Set high-speed output for PD4–PD7
   KeypadPort->OSPEEDR |= ((3 << GPIO_OSPEEDR_OSPEED4_Pos) |
                           (3 << GPIO_OSPEEDR_OSPEED5_Pos) |
                           (3 << GPIO_OSPEEDR_OSPEED6_Pos) |
                           (3 << GPIO_OSPEEDR_OSPEED7_Pos));

   KeypadPort->BRR = KEYPAD_COL_PINS;         // Drive all columns low
}

/* --------------------------------------------------------------------------
 * Key Mapping Table
 * Row/Col → Logical keycode returned from GetKey()
 * -------------------------------------------------------------------------- */
uint8_t key_map[4][4] = {
   {1, 2, 3, 10},   // Row 0: '1' '2' '3' 'A'
   {4, 5, 6, 11},   // Row 1: '4' '5' '6' 'B'
   {7, 8, 9, 12},   // Row 2: '7' '8' '9' 'C'
   {14, 0, 15, 13}  // Row 3: '*' '0' '#' 'D'
};

/* --------------------------------------------------------------------------
 * Function: Keypad_GetKey
 * Polls keypad for any pressed key using active-high logic.
 * Debounces each detection with a small delay.
 * Returns:
 *   -1 if no key pressed
 *   key_map[row][col] if valid key press
 * -------------------------------------------------------------------------- */
int Keypad_GetKey(void) {
   for (int col = 0; col < 4; col++) {
      KeypadPort->BRR = KEYPAD_COL_PINS;       // Clear all columns
      KeypadPort->BSRR = (1 << (col + 4));     // Set current column high

      for (volatile int i = 0; i < 1000; i++);  // Settle time

      for (int row = 0; row < 4; row++) {
         if (KeypadPort->IDR & (1 << row)) {
            for (volatile int d = 0; d < 10000; d++);  // Debounce delay

            if (KeypadPort->IDR & (1 << row)) {        // Still pressed?
               while (KeypadPort->IDR & (1 << row));   // Wait for release
               return key_map[col][row];               // Return key value
            }
         }
      }
   }
   return -1;  // No key pressed
}
