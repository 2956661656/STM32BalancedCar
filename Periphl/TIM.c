#include "TIM.h"                  // Device header

void Pl_TIM_Init()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitType;
	GPIO_InitType.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitType.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;		//CH3/4输出比较
	GPIO_InitType.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitType);
	
	//选择内部时钟作为时钟源
	TIM_InternalClockConfig(TIM4);
	//配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitType;
	TIM_TimeBaseInitType.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitType.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitType.TIM_Period = 7200 - 1;			//ARR	
	TIM_TimeBaseInitType.TIM_Prescaler = 0;
	TIM_TimeBaseInitType.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitType);
	//配置输出比较
	TIM_OCInitTypeDef TIM_OCInitType;
	TIM_OCStructInit(&TIM_OCInitType);
	TIM_OCInitType.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitType.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitType.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitType.TIM_Pulse = 0;		//CCR
	TIM_OC3Init(TIM4, &TIM_OCInitType);
	TIM_OC4Init(TIM4, &TIM_OCInitType);
	
	TIM_Cmd(TIM4, ENABLE);
	
}
