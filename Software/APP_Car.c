#include "APP_Car.h"

extern float angle;		//卡尔曼滤波后的预测角度

void APP_Car_GetAngle()
{
	//读取MPU6050的数据
	Hw_MPU6050_Init();
	MPU6050_ArgValue args = Hw_MPU6050_GetData();
	//atan2得出弧度，2*PI/360 = 弧度/角度，角度为最终需要得出的
	double Acc_Angle = atan2(args.Ax, args.Az) * 180 / PI;
	//1° ≈ 16.4 的比例对应关系, 量程为±2000°/S，65536/4000≈16.4
	double Gyro_Y = -args.Gy / 16.4;		//符号一致
	Cm_Filter_Kalman(Acc_Angle, Gyro_Y);
}
