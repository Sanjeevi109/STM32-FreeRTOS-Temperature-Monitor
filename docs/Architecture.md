# Firmware Architecture

## 1. System Overview

This project implements a temperature-monitoring application using the STM32F407VG microcontroller, STM32 HAL drivers, and FreeRTOS.

The firmware uses TIM2 for periodic sampling, ADC1 for acquiring analog input, FreeRTOS task notifications for passing ADC samples, a queue for transferring temperature values between tasks, and USART2 for transmitting the results.

## 2. System Block Diagram

```text
        TMP36 Temperature Sensor
                  |
                  v
           STM32 ADC1 (PA1)
                  ^
                  |
         TIM2 Periodic Interrupt
                  |
                  v
        HAL_ADC_Start_IT()
                  |
                  v
     ADC Conversion Complete ISR
                  |
                  v
        xTaskNotifyFromISR()
                  |
                  v
       ReadTemp FreeRTOS Task
            Priority: 2
                  |
                  v
       ADC-to-Temperature
             Conversion
                  |
                  v
         FreeRTOS Queue
        Capacity: 10 floats
                  |
                  v
      ProcessTemp FreeRTOS Task
            Priority: 1
                  |
          +-------+-------+
          |               |
          v               v
      USART2 Output    Threshold Check
                          |
                      Temp > 30°C
                          |
                          v
                    UART Warning
```

## 3. FreeRTOS Task Architecture

| Task | Priority relative to idle | Purpose |
|---|---|---|
| StartTime | +3 | Starts TIM2 in interrupt mode, then deletes itself |
| ReadTemp | +2 | Waits for ADC notification and converts ADC data into temperature |
| ProcessTemp | +1 | Receives temperatures from the queue and transmits UART messages |

The application creates these three tasks before starting the FreeRTOS scheduler.

### StartTime Task

Calls `HAL_TIM_Base_Start_IT(&htim2)` to start timer operation, then calls `vTaskDelete(NULL)` to remove itself.

### ReadTemp Task

Waits for an ADC value using `xTaskNotifyWait()`.

After receiving a notification, it converts the raw ADC reading into degrees Celsius and attempts to send the temperature into the FreeRTOS queue.

### ProcessTemp Task

Waits for temperature data using `xQueueReceive()`.

When a temperature is received, it formats and transmits a UART message. If the temperature exceeds 30°C, it also transmits an over-temperature warning.

## 4. Interrupt Architecture

### TIM2 Interrupt

TIM2 generates the periodic sampling event.

The callback:

`HAL_TIM_PeriodElapsedCallback()`

checks whether the interrupt originated from TIM2.

If so, it calls:

`HAL_ADC_Start_IT(&hadc1)`

to request an ADC conversion in interrupt mode.

### ADC Conversion Complete Interrupt

The ADC callback:

`HAL_ADC_ConvCpltCallback()`

verifies that ADC1 generated the completion event.

It then reads the conversion result using `HAL_ADC_GetValue()` and passes the raw ADC value to the ReadTemp task using `xTaskNotifyFromISR()` with `eSetValueWithOverwrite`.

`portYIELD_FROM_ISR()` allows an immediate context switch when required.

## 5. Task Communication

Two communication mechanisms are used.

**Task notification:** Transfers the raw ADC value from the ADC interrupt callback to the ReadTemp task.

**FreeRTOS queue:** Transfers the calculated temperature from ReadTemp to ProcessTemp.

The queue is configured to hold 10 values of type `float`.

## 6. Temperature Calculation

The TMP36 conversion is:

Temperature (°C) = ((ADC / 4095) × 3.3 − 0.5) × 100

The computation assumes a 12-bit ADC, a 3.3 V reference voltage, and the TMP36 voltage-to-temperature relationship.

## 7. Error Monitoring

The firmware defines three diagnostic counters:

| Counter | Purpose |
|---|---|
| `ulADCStartErrors` | Counts ADC-start failures |
| `ulDroppedSamples` | Counts samples that could not be placed in the temperature queue |
| `ulUARTTransmitErrors` | Counts UART transmission or message-formatting failures |

These counters provide diagnostic information about selected firmware failures.

## 8. Validation Scope

The temperature-conversion function has been unit-tested at approximately 25°C and over all 4,096 possible 12-bit ADC values.

The TIM2 callback has passed a host-based unit test for the successful ADC-start path using Unity and CMock.

Hardware timer timing, actual ADC conversion, complete FreeRTOS scheduling behavior, and physical sensor accuracy have not yet been independently verified by these tests.