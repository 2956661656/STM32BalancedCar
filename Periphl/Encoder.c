#include "Encoder.h"

void Pl_Encoder_Init()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_AFIOEN, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitType;
	GPIO_InitType.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitType.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;		//TIM3CH2/TIM2CH1:2编码器输入
	GPIO_InitType.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitType);
	
	GPIO_InitType.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitType.GPIO_Pin = GPIO_Pin_15;		//TIM3CH1编码器输入
	GPIO_InitType.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitType);			//GPIOA15 ==> B1A
	
	//管脚复用
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);			//关闭JTAG,保留SWJ
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);				//将PA0:1的TIM2CH1:2引脚重映射到PA15和PB3
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);				//将PB4:5映射为TIM3的CH1:2
	
	//配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitType;
	TIM_TimeBaseInitType.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitType.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitType.TIM_Period = 65536 - 1;			//ARR	
	TIM_TimeBaseInitType.TIM_Prescaler = 0;
	TIM_TimeBaseInitType.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitType);
	//==============================================================
	//配置输入捕获
	TIM_ICInitTypeDef TIM_ICInitType;
	TIM_ICStructInit(&TIM_ICInitType);
	TIM_ICInitType.TIM_Channel = TIM_Channel_1;					//CH1
	TIM_ICInitType.TIM_ICPolarity = TIM_ICPolarity_Rising;		//电平极性不反转
	TIM_ICInitType.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM2, &TIM_ICInitType);
	TIM_ICInitType.TIM_Channel = TIM_Channel_2;					//CH2
	TIM_ICInit(TIM2, &TIM_ICInitType);
	
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	//TIM2作为电机A的编码器
	TIM_Cmd(TIM2, ENABLE);
		
	TIM_ICStructInit(&TIM_ICInitType);
	TIM_ICInitType.TIM_Channel = TIM_Channel_1;					//CH1
	TIM_ICInitType.TIM_ICPolarity = TIM_ICPolarity_Rising;		//电平极性不反转
	TIM_ICInitType.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM3, &TIM_ICInitType);
	TIM_ICInitType.TIM_Channel = TIM_Channel_2;					//CH2
	TIM_ICInit(TIM3, &TIM_ICInitType);
	
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	//TIM2作为电机A的编码器
	TIM_Cmd(TIM3, ENABLE);

}

int16_t Pl_Encoder_GetCCR_TIM2()
{
	int16_t value = (int16_t)TIM_GetCounter(TIM2);
	TIM2->CNT = 0;
	return value;
}
int16_t Pl_Encoder_GetCCR_TIM3()
{
	int16_t value = (int16_t)TIM_GetCounter(TIM3);
	TIM3->CNT = 0;
	return value;
}
