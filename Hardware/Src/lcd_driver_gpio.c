#include "lcd_driver.h"
#include "sys.h"

//-----------------LCD端口定义---------------- 
#define	LCD_BL    PBout(1)   //LCD背光    		 PB1

#define	LCD_RESET PAout(0)	 //LCD复位    		 PA0
#define	LCD_CS	PAout(1)  //片选端口  	     PA1
#define	LCD_RS	PAout(2)  //数据/命令        PA2	   
#define	LCD_SCL	PAout(3)  //时钟			 PA3
#define	LCD_SDA	PAout(4)  //写数据			 PA4
//#define LCD_SDO PCin(0)   //PC0    SDO

void LCD_Driver_Init(void){
    __HAL_RCC_GPIOA_CLK_ENABLE();           	//开启GPIOA时钟
	__HAL_RCC_GPIOB_CLK_ENABLE();           	//开启GPIOB时钟

    GPIO_InitTypeDef GPIO_Initure;
	
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4; 				//PA0-4
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//推挽输出
    GPIO_Initure.Pull=GPIO_NOPULL;          	//
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;    //高速
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);

	GPIO_Initure.Pin=GPIO_PIN_1; 				//PB1
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//推挽输出
    GPIO_Initure.Pull=GPIO_NOPULL;          	//
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;    //高速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
}

void LCD_Reset(u8 state){
    LCD_RESET = state;
}

// 简单的微秒级延时，根据你的主频调整，这里给通用版
void SPI_Delay(void)
{
    // __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
	// __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
	// __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
	// __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
	// __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
	// __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
	// __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
	// __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
}

//写寄存器函数
//regval:寄存器值
void LCD_WR_REG(u8 REG)
{ 
	u8 i;
	LCD_CS = 0;
	
	LCD_RS = 0;
	
	for(i=0; i<8; i++)
	{
		if (REG & 0x80)
		 LCD_SDA = 1;
		else
		 LCD_SDA = 0;
		
		REG <<= 1;		 
		LCD_SCL = 0;
		SPI_Delay();		  
		LCD_SCL = 1;
		SPI_Delay();
	}//送低8位
	LCD_CS=1;
}
//写LCD数据
//data:要写入的值
void LCD_WR_DATA(u16 DATA)
{										    	   
  u8 i;
	LCD_CS = 0;
	
	LCD_RS = 1;
	
	for(i=0; i<8; i++)
	{
		if (DATA & 0x80)
		 LCD_SDA = 1;
		else
		 LCD_SDA = 0;
		
		DATA <<= 1;		 
		LCD_SCL = 0;
		SPI_Delay();		  
		LCD_SCL = 1;
		SPI_Delay();
	}//送低8位
	LCD_CS=1;
}		


//LCD写GRAM
//RGB_Code:颜色值
void LCD_WriteRAM(u16 DAT)
{							    
  u8 i;
	LCD_CS = 0;
	
	LCD_RS = 1;
	
	for(i=0; i<16; i++)
	{
		if (DAT & 0x8000)
		 LCD_SDA = 1;
		else
		 LCD_SDA = 0;
		
		DAT <<= 1;		 
		LCD_SCL = 0;		  
		LCD_SCL = 1;
	} 
	LCD_CS=1;
}

