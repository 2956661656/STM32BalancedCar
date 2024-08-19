#include "IIC.h"


void Pl_IIC_Init()
{
	//IIC复用功能已经被占用——PB{6:7}|{10:11}可用
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef GPIO_InitType;
	GPIO_InitType.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitType.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;		//IIC2
	GPIO_InitType.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitType);

	I2C_InitTypeDef I2C_InitType;
	I2C_InitType.I2C_Ack = I2C_Ack_Enable;
	I2C_InitType.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitType.I2C_ClockSpeed = 100000;			//100kHz
	I2C_InitType.I2C_DutyCycle = I2C_DutyCycle_2;			//时钟占空比
	I2C_InitType.I2C_Mode = I2C_Mode_I2C;
	I2C_InitType.I2C_OwnAddress1 = 0x00;
	I2C_Init(I2C2, &I2C_InitType);
	
	I2C_Cmd(I2C2, ENABLE);

}
