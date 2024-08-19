#pragma once

#include "stm32f10x.h"                  // Device header
#include "IIC.h"

#define	MPU6050_SMPLRT_DIV		0x19	//采样率分频
#define	MPU6050_CONFIG			0x1A
#define	MPU6050_GYRO_CONFIG		0x1B
#define	MPU6050_ACCEL_CONFIG	0x1C

#define	MPU6050_ACCEL_XOUT_H	0x3B
#define	MPU6050_ACCEL_XOUT_L	0x3C
#define	MPU6050_ACCEL_YOUT_H	0x3D
#define	MPU6050_ACCEL_YOUT_L	0x3E
#define	MPU6050_ACCEL_ZOUT_H	0x3F
#define	MPU6050_ACCEL_ZOUT_L	0x40
#define	MPU6050_TEMP_OUT_H		0x41
#define	MPU6050_TEMP_OUT_L		0x42
#define	MPU6050_GYRO_XOUT_H		0x43
#define	MPU6050_GYRO_XOUT_L		0x44
#define	MPU6050_GYRO_YOUT_H		0x45
#define	MPU6050_GYRO_YOUT_L		0x46
#define	MPU6050_GYRO_ZOUT_H		0x47
#define	MPU6050_GYRO_ZOUT_L		0x48

#define	MPU6050_PWR_MGMT_1		0x6B
#define	MPU6050_PWR_MGMT_2		0x6C
#define	MPU6050_WHO_AM_I		0x75

typedef struct {
	int16_t Ax;
	int16_t Ay;
	int16_t Az;
	int16_t Temp;
	int16_t Gx;
	int16_t Gy;
	int16_t Gz;
} MPU6050_ArgValue;

void Hw_MPU6050_SendByte(uint8_t RegAddress, uint8_t data);
void Hw_MPU6050_SendBytes(uint8_t RegAddress, uint8_t *dataAddress, uint32_t size);
void Hw_MPU6050_ReadBytes(uint8_t RegAddress, uint8_t *dataAddress, uint32_t size);
void Hw_MPU6050_Init();
ArgValue Hw_MPU6050_GetData();

