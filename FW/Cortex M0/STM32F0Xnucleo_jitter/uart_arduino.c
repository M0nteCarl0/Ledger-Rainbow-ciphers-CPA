/*
 * uart_arduino.c
 *
 * Created: 19.07.2021 10:07:52
 *  Author: harde
 */ 
#include "uart_arduino.h"

#ifdef ARDUINO_UNO || ARDUINO_LEO
#define BAUD_RATE0_REG  (unsigned int)(CPU_CLK_SPEED / (16 * BAUD_RATE0) ) - 1
//Actual baud rate, can be used to calculate error
#define ACTUAL_BAUD0    (unsigned int)(CPU_CLK_SPEED / (16 * BAUD_RATE0_REG + 1)
#endif

#ifdef ARDUINO_XNUCLEO_F0
uint8_t charter = 0;
USART_TypeDef * USARTy = USART2;
#define USARTy_GPIO              GPIOB
#define USARTy_CLK               RCC_APB1Periph_USART2
#define USARTy_GPIO_CLK          RCC_AHBPeriph_GPIOA
#define USARTy_RxPin             GPIO_Pin_3
#define USARTy_TxPin             GPIO_Pin_2
#endif


void uart_arduino_init0(void)
{	
#ifdef ARDUINO_UNO
	RXTXEN0_REG = (1<<RXEN0) | (1<<TXEN0);
	UBRR0H = (unsigned char)(BAUD_RATE0_REG >> 8);
	UBRR0L = (unsigned char)BAUD_RATE0_REG;
#endif	

#ifdef ARDUINO_LEO
	RXTXEN0_REG = (1<<RXEN1) | (1<<TXEN1);
	UBRR1H = (unsigned char)(BAUD_RATE0_REG >> 8);
	UBRR1L = (unsigned char)BAUD_RATE0_REG;
#endif

#ifdef ARDUINO_XNUCLEO_F0


#endif


}

char uart_arduino_input_ch_0(void)
{
#ifdef ARDUINO_UNO
 while ((STAT0RXTX_REG & (1<<RXC0)) != (1<<RXC0)){
	 continue;
 }
 return UDR0;
#endif

#ifdef ARDUINO_LEO
while ((STAT0RXTX_REG & (1<<RXC1)) != (1<<RXC1)){
	continue;
}
return UDR1;
#endif

#ifdef ARDUINO_XNUCLEO_F0
get_char_uart(&charter);
return charter;
#endif
}

void uart_arduino_output_ch_0(char data)
{
#ifdef ARDUINO_UNO
	while ((STAT0RXTX_REG & (1<<UDRE0)) != (1<<UDRE0)){
		continue;
	}
	UDR0 = data;
	return;
#endif	

#ifdef ARDUINO_LEO
while ((STAT0RXTX_REG & (1<<UDRE1)) != (1<<UDRE1)){
	continue;
}
	UDR1 = data;
return;
#endif

#ifdef ARDUINO_XNUCLEO_F0
send_char_uart(data);
#endif

}
#ifdef ARDUINO_XNUCLEO_F0

void Nucleo_UsartHard ()
{
  USART_InitTypeDef USART_InitStructure;

  Nucleo_Initiate_Usart  ();
  GPIO_Configuration ();

  USART_InitStructure.USART_BaudRate    = 38400;
  USART_InitStructure.USART_WordLength  = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits    = USART_StopBits_1;
  USART_InitStructure.USART_Parity      = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode        = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init (USARTy, &USART_InitStructure);
  USART_Cmd (USARTy, ENABLE);
};

void Nucleo_Initiate_Usart (void)
{
  RCC_AHBPeriphClockCmd (USARTy_GPIO_CLK, ENABLE);
  RCC_APB1PeriphClockCmd (USARTy_CLK, ENABLE);
};

void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
                                    /* Configure USARTy Rx as input floating */
  GPIO_InitStructure.GPIO_Pin   = USARTy_RxPin;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
  GPIO_Init (USARTy_GPIO, &GPIO_InitStructure);

                      /* Configure USARTy Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin   = USARTy_TxPin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_Init (USARTy_GPIO, &GPIO_InitStructure);
};

void get_char_uart(uint8_t *ch) {
	 while(!USART_GetFlagStatus(USART2,USART_FLAG_RXNE))
	*ch = (uint8_t) USART_ReceiveData(USART2);
}
//send_char: send a byte via uart
void send_char_uart(uint8_t ch) {
 while(!USART_GetFlagStatus(USART2,USART_FLAG_TXE))
	USART_SendData(USART2, ch);
}
#endif



