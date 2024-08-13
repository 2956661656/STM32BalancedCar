#pragma once
#include "TIM.h"                  // Device header

#define FOREWARD	0		//前进
#define BACKWARD	1		//倒车
#define BREAKING	2		//刹车

void Hw_TB6612_Init();
void Hw_TB6612_SetMotorADir(uint8_t dir);
void Hw_TB6612_SetMotorBDir(uint8_t dir);
//符号代表正负，比率代表速度（占空比）
void Hw_TB6612_SetPWM(int8_t signedRateA, int8_t signedRateB);

