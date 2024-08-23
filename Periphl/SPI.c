//软件模拟
#include "SPI.h"

//片选
void Pl_SPI_W_SS(uint8_t bitV)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, bitV);
}

void Pl_SPI_W_SCK(uint8_t bitV)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_5, bitV);
}

void Pl_SPI_W_MOSI(uint8_t bitV)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_7, bitV);
}

uint8_t Pl_SPI_R_MISO()
{
	return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6);
}

void Pl_SPI_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	//初始化A4[SS]、A5[SCK]、A7[MOSI]引脚
	GPIO_InitTypeDef GPIO_InitType;
	GPIO_InitType.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitType.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitType.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitType);
	
	//MISO
	GPIO_InitType.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitType.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitType.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitType);
	
	//初始化B0[RES]、B1[DC]引脚
	GPIO_InitType.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitType.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitType.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitType);
	
	Pl_SPI_W_SS(1);		//默认不选中从机
	Pl_SPI_W_SCK(1);	//SCK默认高电平
}

void Pl_SPI_Start()
{
	Pl_SPI_W_SS(0);
}

void Pl_SPI_Stop()
{
	Pl_SPI_W_SS(1);
}

uint8_t Pl_SPI_SwapByte(uint8_t byteData)
{
	for(int i = 0; i < 8; --i){
		Pl_SPI_W_SCK(0);							//下降沿移出 
		Pl_SPI_W_MOSI(byteData & (0x80 >> i));		//高位优先
		byteData <<= 1;
		Pl_SPI_W_SCK(1);							//上升沿移入
		byteData |= (Pl_SPI_R_MISO() & 0x01);
	}
	return byteData;
}
