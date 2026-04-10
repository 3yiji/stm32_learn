#ifndef __GT911_H
#define __GT911_H	
// #include "stm32f1xx_hal.h"
#include "delay.h"
#include "stm32f1xx.h"

#define delay_ms(ms) HAL_Delay(ms)

#define GT911_RST_state(state)          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, state)
#define GT911_INT_pin                   GPIO_PIN_5
#define GT911_INT_port                  GPIOB
#define GT911_INT_state(state)          HAL_GPIO_WritePin(GT911_INT_port, GT911_INT_pin, state)

// 硬件接口
#define I2C_SCL_Port        GPIOB
#define I2C_SCL_Pin         GPIO_PIN_6
#define I2C_SDA_Port        GPIOB
#define I2C_SDA_Pin         GPIO_PIN_7

#define GT911_I2C_ADDR      0x14 // GT911的I2C地址，取决于GT911的INT引脚连接方式

#define I2C_DELAY                20 // I2C每个Bit之间的延时时间，延时越小I2C的速率越高
#define I2C_SCL_LOW          HAL_GPIO_WritePin(I2C_SCL_Port,I2C_SCL_Pin, GPIO_PIN_RESET) // I2C SCL脚输出0
#define I2C_SCL_HIGH         HAL_GPIO_WritePin(I2C_SCL_Port,I2C_SCL_Pin, GPIO_PIN_SET) // I2C SCL脚输出1
#define I2C_SDA_LOW          HAL_GPIO_WritePin(I2C_SDA_Port,I2C_SDA_Pin, GPIO_PIN_RESET) // I2C SDA脚输出0
#define I2C_SDA_HIGH         HAL_GPIO_WritePin(I2C_SDA_Port,I2C_SDA_Pin, GPIO_PIN_SET) // I2C SDA脚输出1
#define I2C_SDA_STATUS       HAL_GPIO_ReadPin(I2C_SDA_Port,I2C_SDA_Pin)  
  
#define IIC_Delay_us(a)            delay_us(a) // 通信延时


//GT911 部分寄存器定义
#define GT_CTRL_REG 	0X8040   	//GT911控制寄存器
#define GT_CFGS_REG 	0X8047   	//GT911配置起始地址寄存器
#define GT_CHECK_REG 	0X80FF   	//GT911校验和寄存器
#define GT_FRESH_REG 	0X8100   	//GT911刷新寄存器
#define GT_PID_REG 		0X8140   	//GT911产品ID寄存器

#define GT_GSTID_REG 	0X814E   	//GT911当前检测到的触摸情况,第7位是触摸标志位，低4位是触摸点数个数

#define GT_TPD_Sta		0X8150		//触摸点起始数据地址
#define GT_TP1_REG 		0X8150  	//第一个触摸点数据地址
#define GT_TP2_REG 		0X8158		//第二个触摸点数据地址
#define GT_TP3_REG 		0X8160		//第三个触摸点数据地址
#define GT_TP4_REG 		0X8168		//第四个触摸点数据地址
#define GT_TP5_REG 		0X8170		//第五个触摸点数据地址


// GT911驱动配置，默认使用第一份配置文件，如果屏幕异常，可尝试使用第二份
extern const uint8_t CTP_CFG_GT911[];

void GT911_Init();
void GT911_Callback();

uint8_t I2C_Scan_Addr();
uint8_t GT911_ReadReg(uint16_t reg_addr, uint8_t *buf, uint8_t len);
uint8_t GT911_WriteReg(uint16_t reg_addr, uint8_t *buf, uint8_t len);
#endif

