/*
 * led_arduino.h
 *
 * Created: 21.07.2021 14:29:54
 *  Author: harde
 */ 


#ifndef LED_ARDUINO_H_
#define LED_ARDUINO_H_

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

void led_onboard_init(void);
void led_onboard_on(void);
void led_onboard_off(void);


#endif /* LED_ARDUINO_H_ */
