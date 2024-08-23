#include "OLED.h"

static uint8_t OLED_Buffer[8][128];

void Hw_OLED_Flush()
{
	for(uint8_t i = 0; i < 8; ++i){
		Hw_OLED_SetCursor(0, i);		//换页
		
		Hw_OLED_WriteData(OLED_Buffer[i], 128);
	}
}

void Hw_OLED_WriteCmd(uint8_t cmd)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_1);		//发送命令
	Pl_SPI_Start();
	Pl_SPI_SwapByte(cmd);
	Pl_SPI_Stop();
}

void Hw_OLED_WriteData(uint8_t *data, uint8_t count)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_1);		//发送D数据
	Pl_SPI_Start();
	for(uint8_t i = 0; i < count; ++i){
//		Pl_SPI_SwapByte(data);
	
	}
	Pl_SPI_Stop();
}

void Hw_OLED_Init()
{
	//初始化SPI
	Pl_SPI_Init();
	
	//延迟
	uint32_t i, j;
	for (i = 0; i < 1000; i++)			//上电延时
	{
		for (j = 0; j < 1000; j++);
	}
	
	//标准初始化
	Hw_OLED_WriteCmd(0xAE);	//关闭显示
	
	Hw_OLED_WriteCmd(0xD5);	//设置显示时钟分频比/振荡器频率
	Hw_OLED_WriteCmd(0x80);
	
	Hw_OLED_WriteCmd(0xA8);	//设置多路复用率
	Hw_OLED_WriteCmd(0x3F);
	
	Hw_OLED_WriteCmd(0xD3);	//设置显示偏移
	Hw_OLED_WriteCmd(0x00);
	
	Hw_OLED_WriteCmd(0x40);	//设置显示开始行
	
	Hw_OLED_WriteCmd(0xA1);	//设置左右方向，0xA1正常 0xA0左右反置
	
	Hw_OLED_WriteCmd(0xC8);	//设置上下方向，0xC8正常 0xC0上下反置

	Hw_OLED_WriteCmd(0xDA);	//设置COM引脚硬件配置
	Hw_OLED_WriteCmd(0x12);
	
	Hw_OLED_WriteCmd(0x81);	//设置对比度控制
	Hw_OLED_WriteCmd(0xCF);

	Hw_OLED_WriteCmd(0xD9);	//设置预充电周期
	Hw_OLED_WriteCmd(0xF1);

	Hw_OLED_WriteCmd(0xDB);	//设置VCOMH取消选择级别
	Hw_OLED_WriteCmd(0x30);

	Hw_OLED_WriteCmd(0xA4);	//设置整个显示打开/关闭

	Hw_OLED_WriteCmd(0xA6);	//设置正常/倒转显示

	Hw_OLED_WriteCmd(0x8D);	//设置充电泵
	Hw_OLED_WriteCmd(0x14);

	Hw_OLED_WriteCmd(0xAF);	//开启显示
		
	//OLED_Clear();				//OLED清屏
}

void Hw_OLED_SetCursor(uint8_t X, uint8_t page)
{
	Hw_OLED_WriteCmd(0x00 | (X & 0x0F));
	Hw_OLED_WriteCmd(0x10 | ((X & 0xF0) >> 4));
	
	Hw_OLED_WriteCmd(0xB0 | page);
}

void Hw_OLED_Clear()
{
	for(uint8_t i = 0; i < 8; ++i){
		for(uint8_t j = 0; j < 128; ++j){
//			Hw_OLED_WriteData(0x00);
			OLED_Buffer[i][j] = 0x00;
		}
	}
	Hw_OLED_Flush();
}

void Hw_OLED_ShowChar(uint8_t X, uint8_t page, uint8_t ascii)
{
	//Hw_OLED_SetCursor(X, page);
	for(int i = 0; i < 8; ++i){
		//Hw_OLED_WriteData(OLED_F8x16[ascii][i]);
		OLED_Buffer[page][X] = OLED_F8x16[ascii - ' '][i];
	}
	assert_param(page < 8);
	//Hw_OLED_SetCursor(X, page+1);
	for(int i = 8; i < 16; ++i){
		//Hw_OLED_WriteData(OLED_F8x16[ascii][i]);
		OLED_Buffer[page + 1][X] = OLED_F8x16[ascii - ' '][i];
	}
}

void Hw_OLED_ShowString(uint8_t X, uint8_t page, char *string)
{
	//assert_param(X + stringSize < 128);
	while(*string != '\0'){
		Hw_OLED_ShowChar(X+=8, page, *string);
	}
}

void Hw_OLED_Show_Image(uint8_t X, uint8_t Y, uint8_t H, uint8_t W, uint8_t *Image)
{
	for(uint8_t j = 0; j < (H-1) / 8 + 1; j++){		//向上取整
		for(uint8_t i = 0; i < W; ++i){
			OLED_Buffer[Y/8 + j][X+i] |= Image[j + W + i] << (Y % 8);
			OLED_Buffer[Y/8 + j+1][X+i] |= Image[j + W + i] >> (8-Y % 8);
		}
	}
}
