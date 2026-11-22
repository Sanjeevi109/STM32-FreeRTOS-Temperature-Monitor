# Firmware Unit Testing and Verification

## 1. Testing Objective

This project uses host-based unit testing to verify the temperature-conversion algorithm and selected interrupt callback behavior without requiring an STM32 development board.

The testing environment consists of:

| Tool | Purpose |
|---|---|
| Ceedling | Automates test discovery, compilation, execution, and reporting |
| Unity | Provides assertions to verify expected results |
| CMock | Generates mock implementations of hardware-dependent functions |
| GCC | Compiles and links unit tests on the development computer |

## 2. Testing Environment

- Operating system: Windows
- Ceedling version: 1.2.2
- Programming language: C
- Unit-test project: `TemperatureTests`
- Configuration file: `project.yml`

The unit tests execute on the development computer, not on the STM32F407VG microcontroller.

## 3. Test Case TC-01 — Temperature Conversion at 25°C

**Objective:** Verify that the ADC-to-temperature function calculates approximately 25°C for a known ADC input.

Function under test:

`ADC_To_Temperature()`

Input:

`ADC = 931`

Expected output:

Approximately 25°C.

Unity assertion:

```c
TEST_ASSERT_FLOAT_WITHIN(0.1f, 25.0f, actual);
```

**Recorded result: PASS**

## 4. Test Case TC-02 — Exhaustive 12-bit ADC Input Testing

**Objective:** Verify temperature-conversion calculations across the entire 12-bit ADC input range.

Input range:

`0–4095`

Total inputs:

**4,096**

Test approach:

1. Iterate through every ADC value.
2. Calculate the expected mathematical output.
3. Execute `ADC_To_Temperature()`.
4. Compare the result with the expected value.
5. Report a failure when the difference exceeds the specified tolerance.

Unity assertion:

```c
TEST_ASSERT_FLOAT_WITHIN(0.001f, expected, actual);
```

**Recorded result: PASS**

This test checks consistency with the specified conversion model across the full ADC range. It does not independently validate sensor accuracy or the physical ADC.

## 5. Test Case TC-03 — TIM2 Callback Starts ADC1

**Objective:** Verify that the TIM2 callback requests an ADC conversion when invoked with the TIM2 peripheral handle.

Function under test:

`HAL_TIM_PeriodElapsedCallback()`

Mocked dependency:

`HAL_ADC_Start_IT()`

CMock expectation:

```c
HAL_ADC_Start_IT_ExpectAndReturn(&hadc1, HAL_OK);
```

Callback invocation:

```c
HAL_TIM_PeriodElapsedCallback(&htim2);
```

Expected behavior:

- The callback recognizes TIM2.
- `HAL_ADC_Start_IT()` is called exactly once with the expected ADC handle.
- The mocked HAL function returns `HAL_OK`.
- The ADC-start error counter remains zero.

**Recorded result: PASS**

This test validates callback logic through a manually invoked function and a mock dependency. It does not generate a physical timer interrupt.

## 6. Test Cases Requiring Additional Verification

| Test case | Purpose | Status |
|---|---|---|
| TC-04 | Confirm TIM3 does not start ADC1 | Not yet confirmed |
| TC-05 | Confirm ADC-start error counter increments when HAL returns HAL_ERROR | Not yet confirmed |
| TC-06 | Confirm ADC completion notifies the acquisition task | Not yet confirmed |
| TC-07 | Confirm queue and processing-task integration | Not yet confirmed |
| TC-08 | Verify real TIM2 interrupts on hardware | Not yet confirmed |

## 7. Running the Tests

Open a terminal in:

`unit_tests/TemperatureTests`

To execute every test:

```powershell
ceedling test:all
```

To execute only the timer callback tests:

```powershell
ceedling test:test_timer_callback
```

Ceedling automatically compiles and executes the selected tests and reports their results.

## 8. Evidence

The project documentation should include captured terminal output demonstrating successful test execution.

Recommended evidence files:

- Temperature conversion test output
- Exhaustive 4,096-input test output
- TIM2 callback unit-test output
- Complete Ceedling test summary

Evidence should be obtained from actual executions, not reconstructed or fabricated.

## 9. Verification Limitations

The completed host-based unit tests do not prove:

- Physical TIM2 interrupt frequency
- Actual ADC conversion accuracy
- STM32 NVIC interrupt behavior
- Complete FreeRTOS task scheduling
- Physical UART communication
- Temperature sensor performance

These require further integration testing, hardware testing, or suitable simulation.

## 10. Conclusion

The recorded unit tests verify the temperature-conversion calculations and the TIM2 callback's successful ADC-start behavior under controlled host-based conditions.

Additional callback, integration, and hardware tests are required before the complete embedded application can be considered validated.