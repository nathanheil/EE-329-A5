/* --------------------------------------------------------------------------
 * dac.c
 * EE 329 A5: SPI DAC Interface
 *
 * Description:
 * Initializes GPIOA and SPI1 for communication with MCP4921 DAC.
 * Converts millivolt values into DAC command words and transmits
 * them using SPI.
 *
 * Author: Nathan Heil
 * Date: May 10, 2025
 * -------------------------------------------------------------------------- */

#include "stm32l4xx.h"
#include "dac.h"
#include "spi.h"
#include <stdint.h>

/* --------------------------------------------------------------------------
 * Function: DAC_Init
 * Initializes SPI1 and GPIOA pins for MCP4921 interface.
 * CS  = PA4 (manual control)
 * SCK = PA5 (SPI1_SCK)
 * MOSI = PA7 (SPI1_MOSI)
 * -------------------------------------------------------------------------- */
void DAC_Init(void) {
   RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;       // Enable GPIOA clock
   RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;        // Enable SPI1 clock

   GPIOA->MODER &= ~(GPIO_MODER_MODE4);       // Clear PA4 mode
   GPIOA->MODER |=  (GPIO_MODER_MODE4_0);     // Set PA4 to output (CS)

   // Configure PA5, PA7 to AF5 (SPI1)
   GPIOA->MODER &= ~(GPIO_MODER_MODE5 | GPIO_MODER_MODE7);
   GPIOA->MODER |=  (GPIO_MODER_MODE5_1 | GPIO_MODER_MODE7_1);
   GPIOA->AFR[0] &= ~((0xF << GPIO_AFRL_AFSEL5_Pos) | (0xF << GPIO_AFRL_AFSEL7_Pos));
   GPIOA->AFR[0] |=  ((0x5 << GPIO_AFRL_AFSEL5_Pos) | (0x5 << GPIO_AFRL_AFSEL7_Pos));

   GPIOA->OTYPER  &= ~(GPIO_OTYPER_OT5 | GPIO_OTYPER_OT7);
   GPIOA->PUPDR   &= ~(GPIO_PUPDR_PUPD5 | GPIO_PUPDR_PUPD7);
   GPIOA->OSPEEDR |=  (GPIO_OSPEEDR_OSPEED5 | GPIO_OSPEEDR_OSPEED7);

   GPIOA->BSRR = GPIO_PIN_4;                  // Set CS high

   SPI_init();                                // Call SPI configuration
}

/* --------------------------------------------------------------------------
 * Function: DAC_Write
 * Sends 16-bit data word to MCP4921 over SPI.
 * Handles manual CS toggling.
 * -------------------------------------------------------------------------- */
void DAC_Write(uint16_t data_word) {
   GPIOA->BRR = GPIO_PIN_4;                   // CS low to start transfer

   while (!(SPI1->SR & SPI_SR_TXE));          // Wait for TX buffer empty
   *((volatile uint16_t*)&SPI1->DR) = data_word; // Write 16-bit command

   while (SPI1->SR & SPI_SR_BSY);             // Wait until SPI not busy
   GPIOA->BSRR = GPIO_PIN_4;                  // CS high to end transfer
}

/* --------------------------------------------------------------------------
 * Function: DAC_Convert
 * Converts a millivolt value to a 16-bit MCP4921 DAC command.
 * Automatically selects gain = 2 for values <= 2.048V, gain = 1 otherwise.
 * -------------------------------------------------------------------------- */
uint16_t DAC_Convert(uint16_t mV) {
   if (mV > 4096) mV = 4096;                  // Clamp input to max Vout

   uint16_t gain_bit;
   uint16_t data;

   if (mV <= 2048) {
      gain_bit = 1;                           // Use 2x gain
      data = ((uint32_t)mV * 4096 + 1024) / 2048;
   } else {
      gain_bit = 0;                           // Use 1x gain
      data = ((uint32_t)mV * 4096 + 2048) / 4096;
   }

   if (data > 4095) data = 4095;              // Limit to 12-bit max

   uint16_t cmd = 0;
   cmd |= (gain_bit << 13);                   // GAIN bit
   cmd |= (1 << 12);                          // SHDN = 1 (active mode)
   cmd |= (data & 0x0FFF);                    // 12-bit DAC data

   return cmd;
}





