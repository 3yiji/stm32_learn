#include "main.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lcd.h"
#include "lvgl.h"
#include <string.h>
#include "lv_port_disp.h"
#include "delay.h"

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
    uint8_t color = 0;
    uint8_t *line_buf;
    while(1)
    {

        line_buf = pvPortMalloc(30 * 30 * 3);   // 申请内存
        memset(line_buf, color, 30 * 30 * 3);
        LCD_Flush(0,0,29,29, line_buf); // 刷新指定区域，使用申请的内存填充
        vPortFree(line_buf); // 释放内存
        vTaskDelay(1000);

        line_buf = pvPortMalloc(30 * 30 * 3);   // 申请内存
        memset(line_buf, color, 30 * 30 * 3);
        LCD_Flush(100,100,129,129, line_buf); // 刷新指定区域，使用申请的内存填充
        vPortFree(line_buf); // 释放内存
        vTaskDelay(1000);
        color += 20; // 改变颜色值，形成变化效果
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

int mymain(void)
{
    delay_ms(1000); // 等待系统稳定
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
    //     LCD_Task,       // 任务函数
    //     "LCD_Task",     // 任务名
    //     2048,            // 栈大小
    //     NULL,           // 参数
    //     1,              // 优先级
    //     NULL            // 任务句柄
    // );

    xTaskCreate(
        lvgl_timer_handler, // 任务函数
        "LVGL_Timer",       // 任务名
        2048,                // 栈大小
        NULL,               // 参数
        2,                  // 优先级
        NULL                // 任务句柄
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
