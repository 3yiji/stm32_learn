#include "main.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lcd.h"

void SystemClock_Config(void);


/* 任务函数：LED 闪烁 */
void LED_Task(void *pvParameters)
{
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

int main(void)
{
	HAL_Init();
	SystemClock_Config();
	LED_Init();
    

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
}

  void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
  {
    (void)xTask;
    (void)pcTaskName;
    taskDISABLE_INTERRUPTS();
    while (1)
    {
    }
  }

  void vApplicationMallocFailedHook(void)
  {
    taskDISABLE_INTERRUPTS();
    while (1)
    {
    }
  }


void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}