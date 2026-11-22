
#include "timer_callback.h"

/* Simulated ADC1 handle */
ADC_HandleTypeDef hadc1 = {ADC1};

/* Diagnostic counter */
volatile uint32_t ulADCStartErrors = 0;

/* TIM2 callback */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    {
        if (HAL_ADC_Start_IT(&hadc1) != HAL_OK)
        {
            ulADCStartErrors++;
        }
    }
}
