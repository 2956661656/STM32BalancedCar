#pragma once

#include "stm32f10x.h"                  // Device header

void Pl_SPI_Init(); 
void Pl_SPI_Start();
void Pl_SPI_Stop();
uint8_t Pl_SPI_SwapByte(uint8_t byteData);