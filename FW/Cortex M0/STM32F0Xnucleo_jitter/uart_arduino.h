/*
 * uart_arduino.h
 *
 * Created: 19.07.2021 10:08:22
 *  Author: harde
 */ 

#ifndef UART_ARDUINO_H_
#define UART_ARDUINO_H_



#include "arduino_config.h"
#ifdef ARDUINO_LEO || ARDUINO_UNO
#include "avr/io.h"

#define BAUD_RATE0 USART_SPEED
#define CPU_CLK_SPEED F_CPU

#ifdef ARDUINO_UNO
#define NUM_OF_BAUDREGS	2
#define BAUD0H_REG		
#define NUM_OF_UARTS    1
#define RXTXEN0_REG		UCSR0B
#define STAT0RXTX_REG 	UCSR0A
#define UDRM			UDR0

#endif

#ifdef ARDUINO_LEO
#define NUM_OF_BAUDREGS	2
#define BAUD0H_REG
#define NUM_OF_UARTS    1
#define RXTXEN0_REG		UCSR1B
#define STAT0RXTX_REG 	UCSR1A
#define UDRM			UDR1

#endif

#endif


//GPIO USART XNUCLEO-F08 PA2 and PA3 - USART2
#ifdef ARDUINO_XNUCLEO_F0
#include <stm32f0xx_rcc.h>
#include <stm32f0xx_gpio.h>
#include <stm32f0xx_usart.h>
void Nucleo_UsartHard ();
void Nucleo_Initiate_Usart (void);
void GPIO_Configuration(void);
void get_char_uart(uint8_t *ch);
void send_char_uart(uint8_t ch);
#endif










void uart_arduino_init0(void);
char uart_arduino_input_ch_0(void);
void uart_arduino_output_ch_0(char data);

#endif /* UART_ARDUINO._H_ */
