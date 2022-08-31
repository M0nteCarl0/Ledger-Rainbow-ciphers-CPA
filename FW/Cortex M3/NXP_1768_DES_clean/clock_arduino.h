/*
 * clock_arduino.h
 *
 * Created: 19.07.2021 11:01:26
 *  Author: harde
 */ 


#include "arduino_config.h"

#ifdef ARDUINO_LEO || ARDUINO_UNO
#include "avr/io.h"
#endif

#ifdef ARDUINO_XNUCLEO_F0
#include <stm32f0xx_rcc.h>
#include <stm32f0xx_gpio.h>
#endif

#ifndef CLOCK_ARDUINO_H_
#define CLOCK_ARDUINO_H_

void clock_arduino_16Mhz(void);
void clock_arduino_8Mhz(void);
void clock_arduino_4Mhz(void);


#endif /* CLOCK_ARDUINO_H_ */
