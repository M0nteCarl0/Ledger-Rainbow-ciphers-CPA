/*
 * clock_arduino.c
 *
 * Created: 19.07.2021 11:01:54
 *  Author: harde
 */ 

#include "clock_arduino.h"

void clock_arduino_16Mhz(void)
{
#ifdef ARDUINO_XNUCLEO_F0
	 // Set up 16 MHz Core Clock using HSI (8Mhz) with PLL x 2
RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_2);
RCC_PLLCmd(ENABLE);

// Wait for PLLRDY after enabling PLL.
while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) != SET)
{ }
RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);  // Select the PLL as clock source.
SystemCoreClockUpdate();
#endif

}

void clock_arduino_8Mhz(void)
{

}

void clock_arduino_4Mhz(void)
{


}
