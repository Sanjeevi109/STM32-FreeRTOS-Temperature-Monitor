# STM32F407VG Hardware and FreeRTOS Configuration

## 1. Microcontroller and Development Environment

- Microcontroller: STM32F407VG
- IDE: STM32CubeIDE
- Peripheral library: STM32 HAL
- Real-time operating system: Native FreeRTOS
- Programming language: Embedded C

Configuration is maintained in `RTOS_temp.ioc` and the project source files.

## 2. System Clock Configuration

| Parameter | Configured value |
|---|---|
| Oscillator source | HSI — 16 MHz |
| PLLM | 8 |
| PLLN | 50 |
| PLLP | 4 |
| SYSCLK | 25 MHz |
| HCLK | 25 MHz |
| APB1 peripheral clock | 6.25 MHz |
| APB1 timer clock | 12.5 MHz |
| APB2 peripheral clock | 12.5 MHz |

**Purpose:** The clock system supplies timing for the CPU, timers, ADC, UART, and FreeRTOS.

The PLL derives the 25 MHz system clock from the internal HSI oscillator.

## 3. TIM2 Configuration

| Parameter | Value |
|---|---|
| Timer | TIM2 |
| Clock source | Internal |
| Timer input clock | 12.5 MHz |
| Prescaler | 12499 |
| Auto-reload register | 999 |
| Counter direction | Up |
| Interrupt | Enabled |
| NVIC preemption priority | 6 |

### Timer Frequency Calculation

Timer update frequency:

\[
f = \frac{12,500,000}{(12499+1)(999+1)} = 1\text{ Hz}
\]

Therefore, TIM2 generates an update event approximately once per second.

**Purpose:** Initiate periodic ADC measurements without making the sampling task continuously poll the ADC.

### Timer Callback

`HAL_TIM_PeriodElapsedCallback()` checks the TIM2 instance and starts ADC conversion using `HAL_ADC_Start_IT()`.

## 4. ADC1 Configuration

| Parameter | Value |
|---|---|
| ADC peripheral | ADC1 |
| Input pin | PA1 |
| Channel | IN1 |
| Resolution | 12-bit |
| Data alignment | Right |
| Sampling time | 84 ADC clock cycles |
| Conversion mode | Single conversion |
| Continuous conversion | Disabled |
| Trigger | Software start |
| End-of-conversion selection | Single conversion |
| DMA | Disabled |
| ADC NVIC preemption priority | 5 |

**Purpose:** Convert the TMP36 analog voltage into a digital value between 0 and 4095.

Although the ADC uses software triggering, the firmware starts each conversion from the TIM2 callback, providing timer-driven sampling at the application level.

The ADC completion callback reads the result and notifies the FreeRTOS acquisition task.

## 5. USART2 Configuration

| Parameter | Value |
|---|---|
| Peripheral | USART2 |
| Transmit pin | PA2 |
| Receive pin | PA3 |
| Baud rate | 115200 |
| Data bits | 8 |
| Stop bits | 1 |
| Parity | None |
| Hardware flow control | None |
| Mode | Transmit and receive |
| Oversampling | 16 |

**Purpose:** Transmit temperature readings and warning messages to a serial terminal.

The processing task uses HAL UART transmission functions to send formatted text.

## 6. NVIC Configuration

| Setting | Value |
|---|---|
| Priority grouping | NVIC_PRIORITYGROUP_4 |
| ADC interrupt priority | 5 |
| TIM2 interrupt priority | 6 |

On the STM32F407, lower numerical NVIC priority values represent higher urgency.

ADC1 is therefore assigned a higher interrupt priority than TIM2.

The ADC interrupt communicates with FreeRTOS using `xTaskNotifyFromISR()`, so its priority must comply with the FreeRTOS interrupt API restrictions.

**Verification required:** The uploaded FreeRTOS configuration still defines `configKERNEL_INTERRUPT_PRIORITY` and `configMAX_SYSCALL_INTERRUPT_PRIORITY` as zero. These values must be reviewed and corrected as appropriate for the Cortex-M4F port before hardware runtime testing.

## 7. FreeRTOS Configuration

Values from the uploaded `FreeRTOSConfig.h`:

| Parameter | Value |
|---|---|
| CPU clock | 25 MHz |
| Tick rate | 100 Hz |
| Maximum task priorities | 5 |
| Configured heap size | 16 KB |
| Stack overflow checking | Disabled |

The project creates three application tasks:

| Task | Priority | Stack depth argument |
|---|---|---|
| StartTime | Idle + 3 | 200 |
| ReadTemp | Idle + 2 | 256 |
| ProcessTemp | Idle + 1 | 512 |

Task stack-depth arguments are expressed in stack units defined by the FreeRTOS port, not necessarily bytes.

A queue with capacity for ten `float` values transfers computed temperatures from ReadTemp to ProcessTemp.

## 8. Project Validation Status

- STM32CubeIDE compilation: previously reported successful
- Temperature conversion unit tests: passed
- TIM2 successful ADC-start callback test: passed
- Physical ADC and TIM2 interrupt execution: not yet verified
- Complete FreeRTOS runtime behavior: not yet verified

These configuration values describe the uploaded firmware. They do not establish that hardware operation has been validated.