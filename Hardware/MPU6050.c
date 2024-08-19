#include "MPU6050.h"

#define MPU6050_ADDRESS 0x68
uint32_t time_out = 10240;

#define WAIT_IIC2_EVENT_TIME_OUT(E) \
	while(I2C_CheckEvent(I2C2, E) == ERROR){ \
		if(--time_out == 0) return; \
	} \
	time_out = 10240
	
void Hw_MPU6050_SendByte(uint8_t RegAddress, uint8_t data)
{
	I2C_GenerateSTART(I2C2, ENABLE);
	WAIT_IIC2_EVENT_TIME_OUT(I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter/*low bit*/);
	WAIT_IIC2_EVENT_TIME_OUT(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	I2C_SendData(I2C2, RegAddress);
	WAIT_IIC2_EVENT_TIME_OUT(I2C_EVENT_MASTER_BYTE_TRANSMITTING);
	
	I2C_SendData(I2C2, data);
	WAIT_IIC2_EVENT_TIME_OUT(I2C_EVENT_MASTER_BYTE_TRANSMITTED);	
		
	I2C_GenerateSTOP(I2C2, ENABLE);
}


void Hw_MPU6050_SendBytes(uint8_t RegAddress, uint8_t *dataAddress, uint32_t size)
{
	I2C_GenerateSTART(I2C2, ENABLE);
	WAIT_IIC2_EVENT_TIME_OUT(I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter/*low bit*/);
	WAIT_IIC2_EVENT_TIME_OUT(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	I2C_SendData(I2C2, RegAddress);
	WAIT_IIC2_EVENT_TIME_OUT(I2C_EVENT_MASTER_BYTE_TRANSMITTING);
	
	while(size--){
		I2C_SendData(I2C2, *dataAddress++);
		WAIT_IIC2_EVENT_TIME_OUT(I2C_EVENT_MASTER_BYTE_TRANSMITTED);
	}
		
	I2C_GenerateSTOP(I2C2, ENABLE);
}

void Hw_MPU6050_ReadBytes(uint8_t RegAddress, uint8_t *dataAddress, uint32_t size)
{
	I2C_GenerateSTART(I2C2, ENABLE);
	WAIT_IIC2_EVENT_TIME_OUT(I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter/*low bit*/);
	WAIT_IIC2_EVENT_TIME_OUT(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	I2C_SendData(I2C2, RegAddress);
	WAIT_IIC2_EVENT_TIME_OUT(I2C_EVENT_MASTER_BYTE_TRANSMITTED);
	
	I2C_GenerateSTART(I2C2, ENABLE);
	WAIT_IIC2_EVENT_TIME_OUT(I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Receiver/*low bit*/);
	WAIT_IIC2_EVENT_TIME_OUT(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);
	
	while(size-- != 0){
		if(size == 1){
			//如果下一个字节接收完成以后不再接收，提前设置非应答和停止位
			I2C_AcknowledgeConfig(I2C2, DISABLE);
			I2C_GenerateSTOP(I2C2, ENABLE);
		}
		//等待字节接收
		WAIT_IIC2_EVENT_TIME_OUT(I2C_EVENT_SLAVE_BYTE_RECEIVED);
		*dataAddress++ = I2C_ReceiveData(I2C2);
	}
		
	I2C_AcknowledgeConfig(I2C2, ENABLE);				//恢复原始响应状态;
}

void Hw_MPU6050_Init()
{
	Pl_IIC_Init();
	
	Hw_MPU6050_SendByte(MPU6050_PWR_MGMT_1 , 	0x01);		//解除睡眠，选择陀螺仪时钟
	Hw_MPU6050_SendByte(MPU6050_PWR_MGMT_2 , 	0x00);		//六个轴均不待机
	
	Hw_MPU6050_SendByte(MPU6050_SMPLRT_DIV , 	0x09);		//10分频
	Hw_MPU6050_SendByte(MPU6050_CONFIG ,	 	0x06);		//最大滤波
	
	Hw_MPU6050_SendByte(MPU6050_GYRO_CONFIG ,	0x18);		//最大量程选择
	Hw_MPU6050_SendByte(MPU6050_ACCEL_CONFIG ,	0x18);
}

MPU6050_ArgValue Hw_MPU6050_GetData()
{
	MPU6050_ArgValue argValueType;
	
	uint8_t args[14] = {0};
	Hw_MPU6050_ReadBytes(MPU6050_ACCEL_XOUT_H, args, 14);
	
	//获取无符号类型的各个轴数据寄存器数据并转为有符号类型
	for(int i = 0; i < 7; ++i){
		*((int16_t*)&argValueType+i) = (int16_t)(args[i] << 8 | args[i+1]);
	}
	return argValueType;
}
