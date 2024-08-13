#include "TB6612.h"

void Hw_TB6612_Init()
{
	//tim初始化
	Pl_TIM_Init();
	//GPIO
	GPIO_InitTypeDef GPIO_InitType;
	GPIO_InitType.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitType.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;		//给ABIN
	GPIO_InitType.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitType);
}

//电机A
void Hw_TB6612_SetMotorADir(uint8_t dir)
{
	if(dir == FOREWARD){	//前进
		GPIO_ResetBits(GPIOB, GPIO_Pin_14);		//AIN1
		GPIO_SetBits(GPIOB, GPIO_Pin_15);		//AIN2
	}else if(dir == BACKWARD){
		GPIO_ResetBits(GPIOB, GPIO_Pin_14);		//AIN1
		GPIO_SetBits(GPIOB, GPIO_Pin_15);		//AIN2
	}else{		//刹车
		GPIO_SetBits(GPIOB, GPIO_Pin_14);		//AIN1
	    GPIO_SetBits(GPIOB, GPIO_Pin_15);		//AIN2
	}
}
//电机B
void Hw_TB6612_SetMotorBDir(uint8_t dir)
{
	if(dir == FOREWARD){	//前进
		GPIO_ResetBits(GPIOB, GPIO_Pin_13);		//BIN1
		GPIO_SetBits(GPIOB, GPIO_Pin_12);		//BIN2
	}else if(dir == BACKWARD){
		GPIO_ResetBits(GPIOB, GPIO_Pin_13);		//BIN1
		GPIO_SetBits(GPIOB, GPIO_Pin_12);		//BIN2
	}else{		//刹车
		GPIO_SetBits(GPIOB, GPIO_Pin_13);		//BIN1
	    GPIO_SetBits(GPIOB, GPIO_Pin_12);		//BIN2
	}
}

//符号代表正负，比率代表速度（占空比）
void Hw_TB6612_SetPWM(int8_t signedRateA, int8_t signedRateB)
{
	if(signedRateA > 0){
		Hw_TB6612_SetMotorADir(FOREWARD);
	}else if(signedRateA < 0){
		Hw_TB6612_SetMotorADir(BACKWARD);
		signedRateA = -signedRateA;
	}else{		//刹车
		Hw_TB6612_SetMotorADir(BREAKING);
	}
	TIM_SetCompare4(TIM4, signedRateA);
	
	if(signedRateB > 0){
		Hw_TB6612_SetMotorBDir(FOREWARD);
	}else if(signedRateB < 0){
		Hw_TB6612_SetMotorBDir(BACKWARD);
		signedRateB = -signedRateB;
	}else{		//刹车
		Hw_TB6612_SetMotorBDir(BREAKING);
	}
	TIM_SetCompare3(TIM4, signedRateB);
}
