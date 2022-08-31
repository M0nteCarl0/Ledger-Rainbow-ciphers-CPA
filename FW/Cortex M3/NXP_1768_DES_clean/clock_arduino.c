/*
 * clock_arduino.c
 *
 * Created: 19.07.2021 11:01:54
 *  Author: harde
 */ 

#include "clock_arduino.h"
#ifdef ARDUINO_XNUCLEO_F0
#include "stm32f0xx_flash.h"
#endif

#ifdef ARDUINO_XNUCLEO_F0
static void system_clock_set(void)
{
	__IO uint32_t StartUpCounter = 0, HSEStatus = 0;

	/* Set HSION bit */
	RCC->CR |= (uint32_t)0x00000001;

#ifdef STM32F0XX_ MD
	/* Reset SW[1:0], HPRE[3:0], PPRE[2:0], ADCPRE and MCOSEL[2:0] bits */
	RCC->CFGR &= (uint32_t)0xF8FFB80C;
#else
	/* Reset SW[1:0], HPRE[3:0], PPRE[2:0], ADCPRE, MCOSEL[2:0], MCOPRE[2:0] and PLLNODIV bits */
	RCC->CFGR &= (uint32_t)0x08FFB80C;
#endif /* STM32F0XX_MD */

	/* Reset HSEON, CSSON and PLLON bits */
	RCC->CR &= (uint32_t)0xFEF6FFFF;

	/* Reset HSEBYP bit */
	RCC->CR &= (uint32_t)0xFFFBFFFF;

	/* Reset PLLSRC, PLLXTPRE and PLLMUL[3:0] bits */
	RCC->CFGR &= (uint32_t)0xFFC0FFFF;

	/* Reset PREDIV1[3:0] bits */
	RCC->CFGR2 &= (uint32_t)0xFFFFFFF0;

	/* Reset USARTSW[1:0], I2CSW, CECSW and ADCSW bits */
	RCC->CFGR3 &= (uint32_t)0xFFFFFEAC;

	/* Reset HSI14 bit */
	RCC->CR2 &= (uint32_t)0xFFFFFFFE;

	/* Disable all interrupts */
	RCC->CIR = 0x00000000;


	/* SYSCLK, HCLK, PCLK configuration ----------------------------------------*/
	/* At this stage the HSI is already enabled */

	/* Enable Prefetch Buffer and set Flash Latency */
	FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY;

	/* HCLK = SYSCLK */
	RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;

	/* PCLK = HCLK */
	RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE_DIV1;

#if defined (PLL_SOURCE_HSI)
	/* PLL configuration = HSI/2 * 12 = 48 MHz */
	RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL));
	RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSI_Div2 | RCC_CFGR_PLLMULL12);
#else  /* PLL_SOURCE_HSE_BYPASS or PLL_SOURCE_HSE */
	/* Enable HSE */
	RCC->CR |= ((uint32_t)RCC_CR_HSEON);
#if defined (PLL_SOURCE_HSE_BYPASS)
	/* HSE oscillator bypassed with external clock */
	RCC->CR |= (uint32_t)(RCC_CR_HSEBYP);
#endif /* PLL_SOURCE_HSE_BYPASS or PLL_SOURCE_HSE */

	/* Wait till HSE is ready and if Time out is reached exit */
	do {
		HSEStatus = RCC->CR & RCC_CR_HSERDY;
		StartUpCounter++;
	} while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

	if ((RCC->CR & RCC_CR_HSERDY) != RESET) {
		HSEStatus = (uint32_t)0x01;
	} else {
		HSEStatus = (uint32_t)0x00;
	}

	if (HSEStatus == (uint32_t)0x01) {
		/* PLL configuration = HSE * 6 = 48 MHz */
		RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL));
		RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_PREDIV1 | RCC_CFGR_PLLXTPRE_PREDIV1 | RCC_CFGR_PLLMULL6);
	} else { /* If HSE fails to start-up, the application will have wrong clock
	     configuration. User can add here some code to deal with this error */
	}
#endif /*PLL_SOURCE_HSI*/

    /* Enable PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait till PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0) {

    }
    /* Select PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;

    /* Wait till PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)RCC_CFGR_SWS_PLL) {

    }

	SystemCoreClockUpdate();
}
#endif

void clock_arduino_16Mhz(void)
{
#ifdef ARDUINO_XNUCLEO_F0
	__IO uint32_t StartUpCounter = 0, HSEStatus = 0;

		/* Set HSION bit */
		RCC->CR |= (uint32_t)0x00000001;

	#ifdef STM32F0XX_MD
		/* Reset SW[1:0], HPRE[3:0], PPRE[2:0], ADCPRE and MCOSEL[2:0] bits */
		RCC->CFGR &= (uint32_t)0xF8FFB80C;
	#else
		/* Reset SW[1:0], HPRE[3:0], PPRE[2:0], ADCPRE, MCOSEL[2:0], MCOPRE[2:0] and PLLNODIV bits */
		RCC->CFGR &= (uint32_t)0x08FFB80C;
	#endif /* STM32F0XX_MD */

		/* Reset HSEON, CSSON and PLLON bits */
		RCC->CR &= (uint32_t)0xFEF6FFFF;

		/* Reset HSEBYP bit */
		RCC->CR &= (uint32_t)0xFFFBFFFF;

		/* Reset PLLSRC, PLLXTPRE and PLLMUL[3:0] bits */
		RCC->CFGR &= (uint32_t)0xFFC0FFFF;

		/* Reset PREDIV1[3:0] bits */
		RCC->CFGR2 &= (uint32_t)0xFFFFFFF0;

		/* Reset USARTSW[1:0], I2CSW, CECSW and ADCSW bits */
		RCC->CFGR3 &= (uint32_t)0xFFFFFEAC;

		/* Reset HSI14 bit */
		RCC->CR2 &= (uint32_t)0xFFFFFFFE;

		/* Disable all interrupts */
		RCC->CIR = 0x00000000;


		/* SYSCLK, HCLK, PCLK configuration ----------------------------------------*/
		/* At this stage the HSI is already enabled */

		/* Enable Prefetch Buffer and set Flash Latency */
		FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY;

		/* HCLK = SYSCLK */
		RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;

		/* PCLK = HCLK */
		RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE_DIV1;

	#if defined (PLL_SOURCE_HSI)
		/* PLL configuration = HSI/2 * 12 = 48 MHz */
		RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL));
		RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSI_Div2 | RCC_CFGR_PLLMULL12);
	#else  /* PLL_SOURCE_HSE_BYPASS or PLL_SOURCE_HSE */
		/* Enable HSE */
		RCC->CR |= ((uint32_t)RCC_CR_HSEON);
	#if defined (PLL_SOURCE_HSE_BYPASS)
		/* HSE oscillator bypassed with external clock */
		RCC->CR |= (uint32_t)(RCC_CR_HSEBYP);
	#endif /* PLL_SOURCE_HSE_BYPASS or PLL_SOURCE_HSE */

		/* Wait till HSE is ready and if Time out is reached exit */
		do {
			HSEStatus = RCC->CR & RCC_CR_HSERDY;
			StartUpCounter++;
		} while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

		if ((RCC->CR & RCC_CR_HSERDY) != RESET) {
			HSEStatus = (uint32_t)0x01;
		} else {
			HSEStatus = (uint32_t)0x00;
		}

		if (HSEStatus == (uint32_t)0x01) {
			/* PLL configuration = HSE * 6 = 48 MHz */
			RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL));
			RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_PREDIV1 | RCC_CFGR_PLLXTPRE_PREDIV1 | RCC_CFGR_PLLMULL2);
		} else { /* If HSE fails to start-up, the application will have wrong clock
		     configuration. User can add here some code to deal with this error */
		}
	#endif /*PLL_SOURCE_HSI*/

	    /* Enable PLL */
	    RCC->CR |= RCC_CR_PLLON;

	    /* Wait till PLL is ready */
	    while((RCC->CR & RCC_CR_PLLRDY) == 0) {

	    }
	    /* Select PLL as system clock source */
	    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
	    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;

	    /* Wait till PLL is used as system clock source */
	    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)RCC_CFGR_SWS_PLL) {

	    }
		SystemCoreClockUpdate();

#endif

#ifdef ARDUINO_MCUZONE_NXP_1768

#endif

}

void clock_arduino_8Mhz(void)
{

}

void clock_arduino_4Mhz(void)
{


}
