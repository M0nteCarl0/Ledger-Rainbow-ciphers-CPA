/*
 * trigger_arduino.c
 *
 * Created: 19.07.2021 10:18:03
 *  Author: harde
 */ 

#include "trigger_arduino.h"

void trigger_setup()
{

#ifdef ARDUINO_UNO
	DDRD|= (1<<3);
#endif

#ifdef ARDUINO_LEO
	DDRD|= (1<<0);
#endif
	
#ifdef ARDUINO_XNUCLEO_F0
	 RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	 GPIO_InitTypeDef GPIO_InitStructure;
     GPIO_StructInit(&GPIO_InitStructure);
	 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11;
	 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	 GPIO_Init (GPIOA, &GPIO_InitStructure);
#endif

}

void trigger_high()
{
#ifdef ARDUINO_XNUCLEO_F0
	GPIO_WriteBit(GPIOA,GPIO_Pin_11,  Bit_SET);
#endif

#ifdef ARDUINO_UNO
	PORTD|=(1<<3);
#endif
#ifdef ARDUINO_LEO
	PORTD|= (1<<0);
#endif  	
}

void trigger_low()
{

#ifdef ARDUINO_XNUCLEO_F0
	GPIO_WriteBit(GPIOA,GPIO_Pin_11,  Bit_RESET);
#endif

#ifdef ARDUINO_UNO	
	PORTD&=~(1<<3);
#endif
#ifdef ARDUINO_LEO
	PORTD&=~(1<<0);
#endif
   	
}
