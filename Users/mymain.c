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
	// // 1. 获取当前活动屏幕（默认屏幕）
	// lv_obj_t * scr = lv_scr_act();

	// // 画个标签：居中显示
	// lv_obj_t * label = lv_label_create(scr);
	// lv_label_set_text(label, "HELLO STM32");
	// lv_obj_set_style_text_font(label, &lv_font_montserrat_14, 0); 
	// lv_obj_center(label); // 居中

	// // 再画个标签：放在屏幕左上角 (50, 100)
	// lv_obj_t * label1 = lv_label_create(scr);
	// lv_label_set_text(label1, "HELLO LVGL");
	// lv_obj_set_style_text_font(label1, &lv_font_montserrat_14, 0);
	// lv_obj_set_pos(label1, 100, 100); // 绝对定位到 (50,100)
	
	// // 画个方块
	// lv_obj_t * test_rect = lv_obj_create(lv_scr_act());
	// lv_obj_set_size(test_rect, 50, 50);
	// lv_obj_set_style_bg_color(test_rect, lv_palette_main(LV_PALETTE_RED), 0);
	// lv_obj_align(test_rect, LV_ALIGN_TOP_LEFT, 0, 0);
	
	// // 再画一个
	// lv_obj_t * test_rect1 = lv_obj_create(lv_scr_act());
	// lv_obj_set_size(test_rect1, 60, 60);
	// lv_obj_set_style_bg_color(test_rect1, lv_palette_main(LV_PALETTE_YELLOW), 0);
	// lv_obj_align(test_rect1, LV_ALIGN_BOTTOM_RIGHT, 0, 0);

    // #if LV_USE_DEMO_STRESS
    //     /* 设为横屏：顺时针 90 度 */
    //     // lv_display_set_rotation(lv_display_get_default(), LV_DISPLAY_ROTATION_90);
    //     lv_demo_stress();
    // #endif
    // #if LV_USE_DEMO_RENDER
    //     lv_demo_render(LV_DEMO_RENDER_SCENE_FILL, LV_OPA_COVER);
    // #endif

	/* 只创建一个对象，后续仅改颜色，避免反复申请释放内存 */
    lv_obj_t * test_rect = lv_obj_create(lv_scr_act());
    lv_obj_set_size(test_rect, 320, 480);
    lv_obj_align(test_rect, LV_ALIGN_TOP_LEFT, 0, 0);

    const lv_palette_t test_colors[] = {
        LV_PALETTE_RED,
        LV_PALETTE_GREEN,
        LV_PALETTE_BLUE,
    };
    uint32_t color_idx = 0;
    TickType_t last_switch_tick = xTaskGetTickCount();
    lv_obj_set_style_bg_color(test_rect, lv_palette_main(test_colors[color_idx]), 0);

	while(1){
        /* 单任务串行调用 LVGL，避免并发访问导致随机异常 */
        lv_timer_handler();

        TickType_t now = xTaskGetTickCount();
        if((now - last_switch_tick) >= pdMS_TO_TICKS(2000)) {
            color_idx = (color_idx + 1U) % (sizeof(test_colors) / sizeof(test_colors[0]));
            lv_obj_set_style_bg_color(test_rect, lv_palette_main(test_colors[color_idx]), 0);
            last_switch_tick = now;
        }

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
