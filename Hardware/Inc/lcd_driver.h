#ifndef _LCD_DRIVER_H
#define _LCD_DRIVER_H

#include "sys.h"


void LCD_Driver_Init(void);

void LCD_WR_REG(u8 REG);

void LCD_WR_DATA(u16 DATA);

void LCD_Reset(u8 state);

void LCD_WR_DATA_buffer(u8 * pDATA, uint32_t size);
#endif
