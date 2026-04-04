#ifndef __GT911_H
#define __GT911_H	
// #include "stm32f1xx_hal.h"
#include "delay.h"
#include "stm32f1xx.h"

#define delay_ms(ms) HAL_Delay(ms)

#define GT911_RST_state(state)          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, state)
#define GT911_INT_pin                   GPIO_PIN_5

// 硬件接口
#define I2C_SCL_Port        GPIOB
#define I2C_SCL_Pin         GPIO_PIN_6
#define I2C_SDA_Port        GPIOB
#define I2C_SDA_Pin         GPIO_PIN_7

#define I2C_DELAY                5 // I2C每个Bit之间的延时时间，延时越小I2C的速率越高
#define I2C_SCL_LOW          HAL_GPIO_WritePin(I2C_SCL_Port,I2C_SCL_Pin, GPIO_PIN_RESET) // I2C SCL脚输出0
#define I2C_SCL_HIGH         HAL_GPIO_WritePin(I2C_SCL_Port,I2C_SCL_Pin, GPIO_PIN_SET) // I2C SCL脚输出1
#define I2C_SDA_LOW          HAL_GPIO_WritePin(I2C_SDA_Port,I2C_SDA_Pin, GPIO_PIN_RESET) // I2C SDA脚输出0
#define I2C_SDA_HIGH         HAL_GPIO_WritePin(I2C_SDA_Port,I2C_SDA_Pin, GPIO_PIN_SET) // I2C SDA脚输出1
#define I2C_SDA_STATUS       HAL_GPIO_ReadPin(I2C_SDA_Port,I2C_SDA_Pin)  
  
#define IIC_Delay_us(a)            delay_us(a) // 通信延时


void GT911_Init();
void GT911_Callback();

uint8_t I2C_Scan_Addr();

#endif

