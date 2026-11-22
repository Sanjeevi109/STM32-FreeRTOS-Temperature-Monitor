# STM32F407VG FreeRTOS Temperature Monitoring System

## Project Overview

An embedded firmware project developed using STM32CubeIDE, STM32 HAL, and FreeRTOS.

The application is designed to acquire analog temperature measurements, process them using FreeRTOS tasks, and transmit temperature information through UART.

The project also includes host-based unit testing using Ceedling, Unity, and CMock.

## Hardware and Software

| Component | Description |
|---|---|
| Microcontroller | STM32F407VG |
| Development environment | STM32CubeIDE |
| RTOS | FreeRTOS |
| Peripheral drivers | STM32 HAL |
| Temperature sensor model | TMP36 |
| Timer | TIM2 |
| ADC | ADC1, 12-bit |
| Communication | USART2 |
| Programming language | Embedded C |
| Unit-testing framework | Ceedling |
| Testing libraries | Unity and CMock |

## Firmware Architecture

The intended execution sequence is:

1. TIM2 generates a periodic update interrupt.
2. The timer callback starts ADC conversion.
3. The ADC conversion-complete callback reads the ADC value.
4. A FreeRTOS task receives the ADC value through a task notification.
5. The raw ADC value is converted into temperature.
6. The temperature is transferred through a FreeRTOS queue.
7. A processing task transmits the result through UART.
8. Temperatures above 30°C trigger a warning message.

## Temperature Conversion

The TMP36 conversion formula used is:

Temperature (°C) = ((ADC / 4095) × 3.3 − 0.5) × 100

Where:

- ADC is the 12-bit conversion result.
- 4095 is the maximum 12-bit ADC value.
- 3.3 V is the assumed ADC reference voltage.
- 0.5 V is the TMP36 offset.

## Unit Testing

Unit tests are implemented using Ceedling, Unity, and CMock.

Confirmed tests:

| Test | Result |
|---|---|
| Temperature conversion at approximately 25°C | PASS |
| Temperature conversion across all 4,096 ADC values | PASS |
| TIM2 callback starts ADC1 when HAL returns HAL_OK | PASS |

The timer callback test uses CMock to simulate the HAL ADC-start function. These host-based tests do not verify physical interrupt timing, sensor accuracy, or the complete hardware execution path.

### Run Unit Tests

Navigate to:

`unit_tests/TemperatureTests`

Run:

`ceedling test:all`

## Repository Structure

- `firmware/RTOS_temp/` — STM32CubeIDE firmware project
- `unit_tests/TemperatureTests/` — Ceedling unit-testing project
- `docs/` — Firmware architecture and configuration documentation
- `evidence/` — Test evidence and supporting screenshots
- `.github/workflows/` — Planned CI automation

## Build and Validation Status

- STM32CubeIDE firmware compilation: previously reported successful
- Temperature conversion unit tests: passed
- TIM2 successful ADC-start callback test: passed
- Physical hardware and end-to-end FreeRTOS execution: not yet verified

## Development Objective

Develop practical embedded firmware engineering skills covering peripheral configuration, interrupt handling, FreeRTOS task communication, debugging, unit testing, documentation, and automated verification.
