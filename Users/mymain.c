#include "main.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lcd.h"
#include "lvgl.h"
#include "demos/lv_demos.h"
#include <string.h>
#include "lv_port_disp.h"
#include "delay.h"
#include "stdio.h"
#include "gt911.h"
#include "lv_port_indev.h"
#include "demos/widgets/lv_demo_widgets.h"

/* 任务函数：LED 闪烁 */
void LED_Task(void *pvParameters)
{
  	LED_Init();
    while(1)
    {
        LED0=0;			     	//LED0亮
        // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET); // PA1 置0，LED亮
        vTaskDelay(500);                       // 500ms 延时
        LED0=1;					//LED0灭
        // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET); // PA1 置1，LED灭
        vTaskDelay(500);                       // 500ms 延时
    }
}

void lvgl_draw(void *pvParameters){
    lv_init();
	lv_port_disp_init();
    lv_port_indev_init();

    lv_demo_widgets();

	while(1){
        /* 单任务串行调用 LVGL，避免并发访问导致随机异常 */
        lv_timer_handler();

        vTaskDelay(pdMS_TO_TICKS(5));
	}
}

void test(void *pvParameters){

    vTaskDelete(NULL);
}

int mymain(void)
{
    HAL_Delay(1000); // 等待系统稳定
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
        lvgl_draw, // 任务函数
        "LVGL_Draw",       // 任务名
        1024*3,                // 栈大小
        NULL,               // 参数
        1,                  // 优先级
        NULL                // 任务句柄
    );

    // xTaskCreate(
    //     test,       // 任务函数
    //     "test",     // 任务名
    //     256,            // 栈大小
    //     NULL,           // 参数
    //     1,              // 优先级
    //     NULL            // 任务句柄
    // );

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

extern UART_HandleTypeDef huart1;

int __io_putchar(int ch)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
  return ch;
}
