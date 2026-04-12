#include "gt911.h"

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GT911_INT_pin)
    {
        // GT911_Callback();
    }
}