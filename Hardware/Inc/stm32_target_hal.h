#ifndef __STM32_TARGET_HAL_H
#define __STM32_TARGET_HAL_H

#if defined(STM32F405xx) || defined(STM32F415xx) || defined(STM32F407xx) || defined(STM32F417xx) || \
    defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx) || \
    defined(STM32F469xx) || defined(STM32F479xx) || defined(STM32F412Zx) || defined(STM32F412Vx)
#define STM32_TARGET_FAMILY_F4 1
#include "stm32f4xx_hal.h"

#elif defined(STM32F100xB) || defined(STM32F100xE) || defined(STM32F101x6) || defined(STM32F101xB) || \
      defined(STM32F101xE) || defined(STM32F101xG) || defined(STM32F103x6) || defined(STM32F103xB) || \
      defined(STM32F103xE) || defined(STM32F103xG) || defined(STM32F105xC) || defined(STM32F107xC)
#define STM32_TARGET_FAMILY_F1 1
#include "stm32f1xx_hal.h"

#else
#error "Unsupported STM32 target family. Update stm32_target_hal.h for the new MCU."

#endif

#endif /* __STM32_TARGET_HAL_H */