#include "ADC.h"

void Pl_ADC_Init()
{
	//开启RCC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);			//分频时钟 72 !> 14(requir)
	
	//GPIO
	GPIO_InitTypeDef GPIO_InitType;
	GPIO_InitType.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitType.GPIO_Pin = GPIO_Pin_1;		
	GPIO_InitType.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitType);
	
	//选择通道
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);
	
	//AD转换器初始化
	ADC_InitTypeDef ADC_InitType;
	ADC_InitType.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitType.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;				//触发源->软件
	ADC_InitType.ADC_Mode = ADC_Mode_Independent;		//只启用ADC1
	ADC_InitType.ADC_ContinuousConvMode = DISABLE;
	ADC_InitType.ADC_NbrOfChannel = 1;
	ADC_InitType.ADC_ScanConvMode = DISABLE;
	ADC_Init(ADC1, &ADC_InitType);
	
	//开关控制
	ADC_Cmd(ADC1, ENABLE);
	
	//校准
	ADC_ResetCalibration(ADC1);		//复位校准
	while(ADC_GetResetCalibrationStatus(ADC1) == SET);
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1) == SET);
}

double Pl_ADC_GetValue()
{
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);		//执行完，读取后硬件自动清除
	uint16_t cValue = ADC_GetConversionValue(ADC1);
	//测量的电压是分压后的，要放大到12V
	return (cValue * 3.3 / 4095) * 4;
}

