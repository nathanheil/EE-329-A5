/*
 * spi.c
 *
 *  Created on: May 10, 2025
 *      Author: 16573
 */


#include "stm32l4xx.h"

void SPI_init(void) {
    // Disable SPI1 before configuring
    SPI1->CR1 &= ~(SPI_CR1_SPE);
    SPI1->CR1 &= ~(SPI_CR1_RXONLY);
    SPI1->CR1 &= ~(SPI_CR1_LSBFIRST);
    SPI1->CR1 &= ~(SPI_CR1_CPOL | SPI_CR1_CPHA);
    SPI1->CR1 |=  SPI_CR1_MSTR;

    // CR2 settings
    SPI1->CR2 &= ~(SPI_CR2_TXEIE | SPI_CR2_RXNEIE);
    SPI1->CR2 &= ~(SPI_CR2_FRF);
    SPI1->CR2 |=  SPI_CR2_NSSP;
    SPI1->CR2 |=  (0xF << SPI_CR2_DS_Pos); // 16-bit data
    SPI1->CR2 |=  SPI_CR2_SSOE;

    // Enable SPI1
    SPI1->CR1 |= SPI_CR1_SPE;
}
