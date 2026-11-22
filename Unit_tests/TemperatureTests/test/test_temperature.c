
#include "unity.h"
#include "temperature.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_ADC_To_Temperature_25C(void)
{
    float actual = ADC_To_Temperature(931);

    TEST_ASSERT_FLOAT_WITHIN(0.1f, 25.0f, actual);
}

void test_all_4096_ADC_values(void)
{
    for (uint32_t adc = 0; adc <= 4095; adc++)
    {
        float expected = ((float)adc / 4095.0f) * 330.0f - 50.0f;

        float actual = ADC_To_Temperature(adc);

        TEST_ASSERT_FLOAT_WITHIN(0.001f, expected, actual);
    }
}
