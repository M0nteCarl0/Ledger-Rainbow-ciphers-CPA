/*
 * led_arduino.c
 *
 * Created: 21.07.2021 14:30:47
 *  Author: harde
 */ 

#include "led_arduino.h"

void led_onboard_init(void)
{
#ifdef ARDUINO_UNO
	DDRD|= (1<<3);
#endif

#ifdef ARDUINO_LEO
	DDRC|= (1<<7);
#endif
	
#ifdef ARDUINO_XNUCLEO_F0
	 RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	 GPIO_InitTypeDef GPIO_InitStructure;
     GPIO_StructInit(&GPIO_InitStructure);
	 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;
	 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	 GPIO_Init (GPIOC, &GPIO_InitStructure);
#endif

#ifdef ARDUINO_MCUZONE_NXP_1768
	  CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCGPIO, ENABLE);
#endif

}

void led_onboard_on(void)
{
#ifdef ARDUINO_UNO
	PORTD|= (1<<3);
#endif

#ifdef ARDUINO_LEO
	PORTC|= (1<<7);
#endif	

#ifdef ARDUINO_XNUCLEO_F0
	GPIO_WriteBit(GPIOC,GPIO_Pin_8,  Bit_SET);
#endif

#ifdef ARDUINO_MCUZONE_NXP_1768

#endif
	
}

void led_onboard_off(void)
{
#ifdef ARDUINO_UNO
	PORTD&=~(1<<3);
#endif

#ifdef ARDUINO_LEO
	PORTC&=~(1<<7);
#endif	

#ifdef ARDUINO_XNUCLEO_F0
	GPIO_WriteBit(GPIOC,GPIO_Pin_8,  Bit_RESET);
#endif

#ifdef ARDUINO_MCUZONE_NXP_1768

#endif
	
}
