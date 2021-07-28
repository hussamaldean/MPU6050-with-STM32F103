#include "MPU6050.h"
#define MPU6050_I2C_Address (0x68<<1)
#define rate 300
uint64_t previous;

int main(void)
{
rcc_HSE_config();
SystemCoreClockUpdate();
rcc_systick_config(72000);
uart1_gpio_init();
uart1_config();
i2c_I2C1_gpioConfig();
i2c_I2C1_config();
	if(i2c_I2C1_isSlaveAddressExist(MPU6050_I2C_Address,500))
		{
		printf("MPU6050 is  exist\r\n");
		
		}
		
		else {printf("MPU6050 is  not exist\r\n"); while(1);}
		
//Read Who am i register
    uint8_t data;
    I2C_Read(117, &data, 1);
    printf("WHO AM I register - 0x%02X\n", data);

    //Read accelerometer
    MPU_ConfigTypeDef myConfig;
    myConfig.Accel_Full_Scale = AFS_SEL_4g;
    myConfig.ClockSource = Internal_8MHz;
    myConfig.CONFIG_DLPF = DLPF_184A_188G_Hz;
    myConfig.Sleep_Mode_Bit = 0;
    myConfig.Gyro_Full_Scale = FS_SEL_500;
    MPU6050_Config(&myConfig);
    ScaledData_Def meAccel;		
		
		
while (1)
	{
			MPU6050_Get_Accel_Scale(&meAccel);
			if(millis()-previous >rate){
    	printf("Accel: X = %.2f Y = %.2f Z = %.2f\r\n", meAccel.x, meAccel.y, meAccel.z);
    	printf("==============================\r\n");
			previous=millis();
			}
	}
}