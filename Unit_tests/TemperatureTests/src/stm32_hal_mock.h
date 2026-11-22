
#ifndef STM32_HAL_MOCK_H
#define STM32_HAL_MOCK_H

#include <stdint.h>

/* Simplified timer handle */
typedef struct
{
    void *Instance;
} TIM_HandleTypeDef;

/* Simplified ADC handle */
typedef struct
{
    void *Instance;
} ADC_HandleTypeDef;

/* HAL return status */
typedef enum
{
    HAL_OK = 0,
    HAL_ERROR = 1
} HAL_StatusTypeDef;

/* Simulated peripheral identifiers */
#define TIM2 ((void *)0x40000000UL)
#define TIM3 ((void *)0x40000400UL)
#define ADC1 ((void *)0x40012000UL)

/* ADC handle defined in our callback module */
extern ADC_HandleTypeDef hadc1;

/* HAL function we will mock using CMock */
HAL_StatusTypeDef HAL_ADC_Start_IT(ADC_HandleTypeDef *hadc);

#endif
