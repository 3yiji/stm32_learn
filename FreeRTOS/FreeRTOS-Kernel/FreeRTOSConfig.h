#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include "stm32f1xx_hal.h"

#define configUSE_PREEMPTION                        1                           // 1=抢占式调度，优先级高的任务可抢占低优先级任务
#define configUSE_PORT_OPTIMISED_TASK_SELECTION    0                           // 任务选择优化开关，Cortex-M3通常关闭即可
#define configUSE_TIME_SLICING                      1                           // 同优先级任务按时钟节拍轮转
#define configUSE_16_BIT_TICKS                      0                           // 0=使用32位tick计数，避免tick溢出过快
#define configIDLE_SHOULD_YIELD                     1                           // 空闲任务在同优先级有就绪任务时主动让出CPU

#define configCPU_CLOCK_HZ                          72000000UL                  // CPU主频72MHz（HSE 8MHz * PLL x9）
#define configTICK_RATE_HZ                          1000U                       // 系统节拍1kHz，即1ms一次tick中断
#define configMAX_PRIORITIES                        8                           // 可用任务优先级数量（0~7）
#define configMINIMAL_STACK_SIZE                    128U                        // 空闲任务最小栈深度（单位：word，不是字节）
#define configTOTAL_HEAP_SIZE                       (15U * 1024U)               // heap_4可用堆大小，单位字节
#define configMAX_TASK_NAME_LEN                     16                          // 任务名最大长度（含结尾\0）
#define configTIMER_TASK_PRIORITY                   (configMAX_PRIORITIES - 1)  // 软件定时器服务任务优先级
#define configTIMER_QUEUE_LENGTH                    10                          // 软件定时器命令队列长度
#define configTIMER_TASK_STACK_DEPTH                256U                        // 软件定时器服务任务栈深度（单位：word）

#define configUSE_MUTEXES                           1                           // 使能互斥量
#define configUSE_RECURSIVE_MUTEXES                 1                           // 使能递归互斥量
#define configUSE_COUNTING_SEMAPHORES               1                           // 使能计数信号量
#define configUSE_QUEUE_SETS                        0                           // 队列集合功能开关
#define configUSE_TASK_NOTIFICATIONS                1                           // 使能任务通知机制（轻量级同步）
#define configUSE_TRACE_FACILITY                    0                           // 运行轨迹功能开关
#define configUSE_STATS_FORMATTING_FUNCTIONS        0                           // 运行状态格式化输出函数开关
#define configGENERATE_RUN_TIME_STATS               0                           // 运行时统计开关（需额外高精度计时器支持）

#define configUSE_IDLE_HOOK                         0                           // 空闲钩子函数开关
#define configUSE_TICK_HOOK                         0                           // Tick钩子函数开关
#define configCHECK_FOR_STACK_OVERFLOW              0                           // 栈溢出检测等级，2更严格
#define configUSE_MALLOC_FAILED_HOOK                1                           // 内存分配失败钩子开关

#define configSUPPORT_STATIC_ALLOCATION             0                           // 是否支持静态内存分配
#define configSUPPORT_DYNAMIC_ALLOCATION            1                           // 是否支持动态内存分配（heap_x）

#define configPRIO_BITS                             4                           // F103 NVIC有效优先级位数（通常为4位）
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY     15                          // 库接口可配置的最低中断优先级值
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 5                          // 可调用FreeRTOS API的最高中断优先级值
#define configKERNEL_INTERRUPT_PRIORITY             (configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS))      // 内核中断优先级（移位后写入寄存器）
#define configMAX_SYSCALL_INTERRUPT_PRIORITY        (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS)) // 系统调用中断优先级上限（移位后写入寄存器）

#define configENABLE_FPU                            0                           // Cortex-M3无FPU，固定关闭
#define configENABLE_MPU                            0                           // Cortex-M3无MPU端口需求，固定关闭

#define INCLUDE_vTaskPrioritySet                    1                           // 使能vTaskPrioritySet接口
#define INCLUDE_uxTaskPriorityGet                   1                           // 使能uxTaskPriorityGet接口
#define INCLUDE_vTaskDelete                         1                           // 使能vTaskDelete接口
#define INCLUDE_vTaskSuspend                        1                           // 使能vTaskSuspend接口
#define INCLUDE_vTaskDelayUntil                     1                           // 使能vTaskDelayUntil接口
#define INCLUDE_vTaskDelay                          1                           // 使能vTaskDelay接口
#define INCLUDE_xTaskGetSchedulerState              1                           // 使能xTaskGetSchedulerState接口
#define INCLUDE_xTaskGetCurrentTaskHandle           1                           // 使能xTaskGetCurrentTaskHandle接口
#define INCLUDE_uxTaskGetStackHighWaterMark         1                           // 使能栈剩余水位查询接口

#define vPortSVCHandler                             SVC_Handler                 // FreeRTOS SVC中断服务函数映射到CMSIS名称
#define xPortPendSVHandler                          PendSV_Handler              // FreeRTOS PendSV中断服务函数映射到CMSIS名称
#define xPortSysTickHandler                         SysTick_Handler             // FreeRTOS SysTick中断服务函数映射到CMSIS名称

#endif