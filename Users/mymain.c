#include "stm32f1xx_hal.h"
#include "main.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lcd.h"
#include "lvgl.h"
#include <string.h>
#include "lv_port_disp.h"
#include "delay.h"
#include "stdio.h"
#include "gt911.h"

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

void lvgl_timer_handler(void *pvParameters){
    lv_init();
	lv_port_disp_init();
	// 1. 获取当前活动屏幕（默认屏幕）
	lv_obj_t * scr = lv_scr_act();

	// 画个标签：居中显示
	lv_obj_t * label = lv_label_create(scr);
	lv_label_set_text(label, "HELLO STM32");
	lv_obj_set_style_text_font(label, &lv_font_montserrat_14, 0); 
	lv_obj_center(label); // 居中

	// 再画个标签：放在屏幕左上角 (50, 100)
	lv_obj_t * label1 = lv_label_create(scr);
	lv_label_set_text(label1, "HELLO LVGL");
	lv_obj_set_style_text_font(label1, &lv_font_montserrat_14, 0);
	lv_obj_set_pos(label1, 100, 100); // 绝对定位到 (50,100)
	
	// 画个方块
	lv_obj_t * test_rect = lv_obj_create(lv_scr_act());
	lv_obj_set_size(test_rect, 50, 50);
	lv_obj_set_style_bg_color(test_rect, lv_palette_main(LV_PALETTE_RED), 0);
	lv_obj_align(test_rect, LV_ALIGN_TOP_LEFT, 0, 0);
	
	// 再画一个
	lv_obj_t * test_rect1 = lv_obj_create(lv_scr_act());
	lv_obj_set_size(test_rect1, 60, 60);
	lv_obj_set_style_bg_color(test_rect1, lv_palette_main(LV_PALETTE_YELLOW), 0);
	lv_obj_align(test_rect1, LV_ALIGN_BOTTOM_RIGHT, 0, 0);

	while(1){
        lv_task_handler(); // 处理 LVGL 任务
		vTaskDelay(10); // 每 10ms 调用一次，确保 LVGL 的动画和事件处理正常
	}
}

void test(void *pvParameters){
    GT911_Init();
    printf("result: %d\n", I2C_Scan_Addr());

    uint8_t * temp_buf = pvPortMalloc(200);
    if (temp_buf == NULL)
    {
        printf("temp_buf alloc failed\n");
        vTaskDelete(NULL);
    }

    GT911_ReadReg(GT_CFGS_REG, temp_buf, 184);
    printf("CFGS:");
    for (int i = 0; i < 184; i++)
    {
        if ((i % 16) == 0)
        {
            printf("\n");
        }
        printf("%02X ", temp_buf[i]);
    }
    printf("\n");
    vPortFree(temp_buf);

    uint8_t temp = 0x00;
    printf("result2: %d\n", GT911_WriteReg(GT_CTRL_REG, &temp, 1));
    while(1){
        vTaskDelay(3000);
        GT911_Callback();
    }


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

    // xTaskCreate(
    //     lvgl_timer_handler, // 任务函数
    //     "LVGL_Timer",       // 任务名
    //     2048,                // 栈大小
    //     NULL,               // 参数
    //     2,                  // 优先级
    //     NULL                // 任务句柄
    // );

    xTaskCreate(
        test,       // 任务函数
        "test",     // 任务名
        256,            // 栈大小
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

extern UART_HandleTypeDef huart3;

int __io_putchar(int ch)
{
  HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
  return ch;
}
