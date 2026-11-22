
#include "unity.h"
#include "timer_callback.h"
#include "mock_stm32_hal_mock.h"

/* Access the error counter */
extern volatile uint32_t ulADCStartErrors;

/* Runs before each test */
void setUp(void)
{
    ulADCStartErrors = 0;
}

/* Runs after each test */
void tearDown(void)
{
}

/* Test: TIM2 should start ADC1 */
void test_TIM2_starts_ADC1(void)
{
    TIM_HandleTypeDef htim2 = {TIM2};

    HAL_ADC_Start_IT_ExpectAndReturn(&hadc1, HAL_OK);

    HAL_TIM_PeriodElapsedCallback(&htim2);

    TEST_ASSERT_EQUAL_UINT32(0, ulADCStartErrors);
}
