// #include "spi.h"
#include "lcd_driver.h"
#include "stm32_target_hal.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F103开发板
//SPI驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2017/5/30
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

// SPI_HandleTypeDef SPI1_Handler;  //SPI1句柄
extern SPI_HandleTypeDef hspi1;

//以下是SPI模块的初始化代码，配置成主机模式 						  
//SPI口初始化
//这里针是对SPI1的初始化
// void SPI1_Init(void)
// {
//     SPI1_Handler.Instance=SPI1;                         //SPI1
//     SPI1_Handler.Init.Mode=SPI_MODE_MASTER;             //设置SPI工作模式，设置为主模式
//     SPI1_Handler.Init.Direction=SPI_DIRECTION_2LINES;   //设置SPI单向或者双向的数据模式:SPI设置为双线模式
//     SPI1_Handler.Init.DataSize=SPI_DATASIZE_8BIT;       //设置SPI的数据大小:SPI发送接收8位帧结构
//     SPI1_Handler.Init.CLKPolarity=SPI_POLARITY_HIGH;    //串行同步时钟的空闲状态为高电平
//     // SPI1_Handler.Init.CLKPolarity = SPI_POLARITY_LOW;     // 空闲低电平（正确）
//     // SPI1_Handler.Init.CLKPhase=SPI_PHASE_2EDGE;         //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
//     SPI1_Handler.Init.CLKPhase = SPI_PHASE_1EDGE;         // 第一个边沿采样（正确）
//     SPI1_Handler.Init.NSS=SPI_NSS_SOFT;                 //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
//     // SPI1_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_256;//定义波特率预分频的值:波特率预分频值为256
//     SPI1_Handler.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;  // 2分频最快
//     SPI1_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;        //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
//     SPI1_Handler.Init.TIMode=SPI_TIMODE_DISABLE;        //关闭TI模式
//     SPI1_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//关闭硬件CRC校验
//     SPI1_Handler.Init.CRCPolynomial=7;                  //CRC值计算的多项式
//     HAL_SPI_Init(&SPI1_Handler);//初始化
    
//     __HAL_SPI_ENABLE(&SPI1_Handler);                    //使能SPI1
	
//     // SPI1_ReadWriteByte(0Xff);                           //启动传输
// }

//SPI1底层驱动，时钟使能，引脚配置
//此函数会被HAL_SPI_Init()调用
//hspi:SPI句柄
// void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
// {
//     GPIO_InitTypeDef GPIO_Initure;
    
//     __HAL_RCC_GPIOA_CLK_ENABLE();       //使能GPIOA时钟
//     __HAL_RCC_SPI1_CLK_ENABLE();        //使能SPI1时钟
    
//     //PA5,6,7
//     GPIO_Initure.Pin=GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
//     GPIO_Initure.Mode=GPIO_MODE_AF_PP;              //复用推挽输出
//     GPIO_Initure.Pull=GPIO_PULLUP;                  //上拉
//     GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;        //快速            
//     HAL_GPIO_Init(GPIOA,&GPIO_Initure);
// }

//SPI速度设置函数
//SPI速度=fAPB1/分频系数
//@ref SPI_BaudRate_Prescaler:SPI_BAUDRATEPRESCALER_2~SPI_BAUDRATEPRESCALER_2 256
//fAPB1时钟一般为42Mhz：
// void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
// {
//     assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//判断有效性
//     __HAL_SPI_DISABLE(&SPI1_Handler);            //关闭SPI
//     SPI1_Handler.Instance->CR1&=0XFFC7;          //位3-5清零，用来设置波特率
//     SPI1_Handler.Instance->CR1|=SPI_BaudRatePrescaler;//设置SPI速度
//     __HAL_SPI_ENABLE(&SPI1_Handler);             //使能SPI
    
// }

//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI1_ReadWriteByte(u8 TxData)
{
    u8 Rxdata;
    HAL_SPI_TransmitReceive(&hspi1,&TxData,&Rxdata,1, 1000);       
 	return Rxdata;          		    //返回收到的数据		
}

#define	LCD_RESET PAout(0)	 //LCD复位    		 PA0
#define	LCD_CS	PAout(1)  //片选端口  	     PA1
#define	LCD_RS	PAout(2)  //数据/命令端口   PA2


void LCD_Driver_Init(void){
    __HAL_RCC_GPIOA_CLK_ENABLE();           	//开启GPIOA时钟
	// __HAL_RCC_GPIOB_CLK_ENABLE();           	//开启GPIOB时钟

    GPIO_InitTypeDef GPIO_Initure;
	
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3; 				//PA0-2
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//推挽输出
    GPIO_Initure.Pull=GPIO_PULLDOWN;          	//
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;    //高速
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);

    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,GPIO_PIN_SET);

    // SPI1_Init();                         //初始化SPI1
}

void LCD_Reset(u8 state){
    LCD_RESET = state;
}


//写寄存器函数
//regval:寄存器值
void LCD_WR_REG(u8 REG)
{ 
	LCD_CS = 0;
	
	LCD_RS = 0;
	HAL_SPI_Transmit(&hspi1, &REG, 1, 100);
	
	LCD_CS=1;
}
//写LCD数据
//data:要写入的值
void LCD_WR_DATA(u16 DATA)
{	
    u8 tx = (u8)DATA;

	LCD_CS = 0;
	
	LCD_RS = 1;
	
    HAL_SPI_Transmit(&hspi1, &tx, 1, 100);
	LCD_CS=1;
}

// void LCD_WR_REG_buffer(u8 * pDATA, uint32_t size)
// {	
// 	LCD_RS = 0;
//     HAL_SPI_Transmit(&hspi1, pDATA, size, 100);
// }

// void LCD_WR_DATA_buffer(u8 * pDATA, uint32_t size)
// {	
//     LCD_RS = 1;
//     HAL_SPI_Transmit(&hspi1, pDATA, size, 100);
// }


// ====================== DMA 版本 ======================
static void LCD_SPI_Wait_DMA(void)
{
    // 等待 SPI 状态空闲
    while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY);
    
    // 等待 DMA TX 空闲
    while (HAL_DMA_GetState(hspi1.hdmatx) != HAL_DMA_STATE_READY);
}

void LCD_WR_REG_buffer(u8 *pDATA, uint32_t size)
{	
    // 1. 等待上一次传输结束（保证不冲突）
    LCD_SPI_Wait_DMA();
    
    // 2. 配置指令模式
    LCD_RS = 0;
    
    // 3. DMA 非阻塞发送
    HAL_SPI_Transmit_DMA(&hspi1, pDATA, size);
}

void LCD_WR_DATA_buffer(u8 *pDATA, uint32_t size)
{	
    // 1. 等待上一次传输结束（保证不冲突）
    LCD_SPI_Wait_DMA();
    
    // 2. 配置数据模式
    LCD_RS = 1;
    
    // 3. DMA 非阻塞发送
    HAL_SPI_Transmit_DMA(&hspi1, pDATA, size);
}


//LCD写GRAM
//RGB_Code:颜色值
void LCD_WriteRAM(u16 DAT)
{		
    u8 tx = (u8)DAT;

	LCD_CS = 0;
	
	LCD_RS = 1;
	
    HAL_SPI_Transmit(&hspi1, &tx, 1, 100);
	LCD_CS=1;
}