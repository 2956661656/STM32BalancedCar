#pragma once
#include "stm32f10x.h"                  // Device header

void Pl_Encoder_Init();
uint16_t Pl_Encoder_Get();
int16_t Pl_Encoder_GetCCR_TIM2();
int16_t Pl_Encoder_GetCCR_TIM3();
