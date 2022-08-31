/*
 * trigger_arduino.h
 *
 * Created: 19.07.2021 10:18:28
 *  Author: harde
 */ 
#ifndef TRIGGER_ARDUINO_H_
#define TRIGGER_ARDUINO_H_

#include "arduino_config.h"
#ifdef ARDUINO_LEO || ARDUINO_UNO
#include "avr/io.h"
#endif

#ifdef ARDUINO_XNUCLEO_F0
#include <stm32f0xx_rcc.h>
#include <stm32f0xx_gpio.h>
#endif

#ifdef ARDUINO_MCUZONE_NXP_1768
#include <lpc17xx_lib/include/lpc17xx_clkpwr.h>
#include <lpc17xx_lib/include/lpc17xx_gpio.h>
#endif
/*
trigger output Arduino UNO
IO3 -> PD3(PORTD Pin3)

trigger output NXP 1768 P0.25

*/

void trigger_setup();
void trigger_high();
void trigger_low();

#endif /* TRIGGER_ARDUINO_H_ */
