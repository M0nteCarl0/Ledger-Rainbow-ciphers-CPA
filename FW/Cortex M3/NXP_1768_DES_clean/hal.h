#ifndef HAL_H_
#define HAL_H_

#include "arduino_config.h"
#include "trigger_arduino.h"
#include "uart_arduino.h"
#include "led_arduino.h"

#define init_uart uart_arduino_init0
#define putch uart_arduino_output_ch_0
#define getch uart_arduino_input_ch_0
void platform_init();

#endif /* HAL_H_ */