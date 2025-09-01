
/* --------------------------------------------------------------------------
 * dac.h
 * EE 329 A5: DAC Module Header
 *
 * Description:
 * Declares interface functions for SPI DAC control and voltage conversion.
 *
 * Author: Nathan Heil
 * Date: May 10, 2025
 * -------------------------------------------------------------------------- */

#ifndef INC_DAC_H_
#define INC_DAC_H_

#include <stdint.h>

void DAC_Init(void);             // Configure SPI and GPIO for DAC
void DAC_Write(uint16_t word);  // Send 16-bit command to DAC
uint16_t DAC_Convert(uint16_t mV); // Convert millivolts to command word

#endif /* INC_DAC_H_ */
