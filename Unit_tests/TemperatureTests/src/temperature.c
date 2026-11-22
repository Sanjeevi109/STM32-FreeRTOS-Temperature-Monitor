
#include "temperature.h"

float ADC_To_Temperature(uint32_t rawADC)
{
    float voltage;

    voltage = ((float)rawADC / 4095.0f) * 3.3f;

    return (voltage - 0.5f) * 100.0f;
}
