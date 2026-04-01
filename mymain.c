#include "main.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lcd.h"

/* 任务函数：LED 闪烁 */
void LED_Task(void *pvParameters)
{
  	LED_Init();
    while(1)
    {
        LED0=0;			     	//LED0亮
        vTaskDelay(500);                       // 500ms 延时
        LED0=1;					//LED0灭
        vTaskDelay(500);                       // 500ms 延时
    }
}

void LCD_Task(void *pvParameters)
{
    LCD_Init();
    while(1)
    {
		// LCD_Clear(0,0,0);		//清屏
        LCD_Clear(0x03f,0x00,0x00);		//清屏
        vTaskDelay(1000);
        LCD_Clear(0x00,0x03f,0x00);		//清屏
        vTaskDelay(1000);
        LCD_Clear(0x00,0x00,0x3f);		//清屏
        vTaskDelay(1000);


        // HAL_GPIO_WritePin(GPIOA,
        //     GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4,
        //     GPIO_PIN_SET
        // );
        // vTaskDelay(1000);
        // HAL_GPIO_WritePin(GPIOA,
        //     GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4,
        //     GPIO_PIN_RESET
        // );
        // vTaskDelay(1000);
    }
}

int mymain(void)
{
	// ------------- 创建任务 -------------
    xTaskCreate(
        LED_Task,       // 任务函数
        "LED_Task",     // 任务名
        256,            // 栈大小
        NULL,           // 参数
        1,              // 优先级
        NULL            // 任务句柄
    );

    xTaskCreate(
        LCD_Task,       // 任务函数
        "LCD_Task",     // 任务名
        2048,            // 栈大小
        NULL,           // 参数
        1,              // 优先级
        NULL            // 任务句柄
    );

    // ------------- 启动 FreeRTOS 调度器 -------------
    vTaskStartScheduler();
	return -1; // 正常情况下不会执行到这里
}

void vApplicationMallocFailedHook(void)
{
  __disable_irq();
  while (1)
  {
  }
}
