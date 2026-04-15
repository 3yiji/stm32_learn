#include "lcd_driver.h"
#include "sys.h"


#define LCD_RESET PDout(3)     //LCD复位    		 PD3

// NE1 + A22 作为 RS/DC（STM32F407VGT6 专用）
#define LCD_BASE        ((uint32_t)(0x60000000UL))
#define LCD_CMD_ADDR    (LCD_BASE + 0x00000000UL)  // A22=0 → 写命令
#define LCD_DATA_ADDR   (LCD_BASE + 0x00800000UL)  // A22=1 → 写数据（16bit总线）

#define LCD_CMD     (*(__IO uint16_t *)LCD_CMD_ADDR)
#define LCD_DATA    (*(__IO uint16_t *)LCD_DATA_ADDR)

void LCD_Driver_Init(void){
    
}

void LCD_Reset(u8 state){
    LCD_RESET = state;
}

void LCD_WR_REG_buffer(uint8_t *pDATA, uint32_t size)
{	
    // 命令通常 8bit，按原逻辑兼容
    for(uint32_t i=0; i<size; i++)
    {
        LCD_CMD = (uint16_t)pDATA[i];
    }
}

void LCD_WR_DATA_buffer(uint8_t *pDATA, uint32_t size)
{	
    for(uint32_t i=0; i < size; i++)
    {
        LCD_DATA = (uint16_t)pDATA[i];
    }
}

void LCD_WR_GRAM_buffer(uint8_t *pDATA, uint32_t size)
{	
    // 把 u8 数据流按 16bit 读取 → 一次发 16bit
    uint16_t *p16 = (uint16_t *)pDATA;
    
    // 长度要除以 2（因为两个字节拼成一个16位）
    for(uint32_t i=0; i < size/2; i++)
    {
        LCD_DATA = p16[i];  // ✅ 真正 16bit 并行发送
    }
}

