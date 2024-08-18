#include "stm32f10x.h"
#include "TB6612.h"
#include "Encoder.h"
#include "IIC.h"

int main()
{
	Hw_TB6612_Init();
	Pl_Encoder_Init();
	
	
	while(1){
		
	}
}
