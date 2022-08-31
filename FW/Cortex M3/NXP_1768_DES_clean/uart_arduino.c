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


#ifdef ARDUINO_MCUZONE_NXP_1768
uint8_t charter = 0;
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
	Nucleo_UsartHard();
#endif

#ifdef ARDUINO_MCUZONE_NXP_1768
	Nxp1768_UsartHard();
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

#ifdef ARDUINO_MCUZONE_NXP_1768
	Nxp1768_get_char_uart(&charter);
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

#ifdef ARDUINO_MCUZONE_NXP_1768
Nxp1768_send_char_uart(data);
#endif

}
#ifdef ARDUINO_XNUCLEO_F0
void Nucleo_UsartHard ()
{
  USART_InitTypeDef USART_InitStructure;

  Nucleo_Initiate_Usart();
  GPIO_Configuration ();

	USART_InitTypeDef tUSART;
	//NVIC_InitTypeDef NVIC_InitStructure;

	/* config USART2 clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/* USART2 mode config */
	tUSART.USART_BaudRate = 38400;
	tUSART.USART_WordLength = USART_WordLength_8b;
	tUSART.USART_StopBits = USART_StopBits_1;
	tUSART.USART_Parity = USART_Parity_No;
	tUSART.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	tUSART.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

	USART_Init(USART2, &tUSART);
	USART_ClearFlag(USART2,USART_FLAG_RXNE);
	USART_ClearFlag(USART2,USART_FLAG_TC);

	USART_Cmd(USART2, ENABLE);
};

void Nucleo_Initiate_Usart (void)
{
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA , ENABLE);
  RCC_APB1PeriphClockCmd (USARTy_CLK, ENABLE);
};

void GPIO_Configuration(void)
{

	GPIO_InitTypeDef tGPIO;
  	//USART
  	tGPIO.GPIO_Pin = GPIO_Pin_2;
    tGPIO.GPIO_Mode = GPIO_Mode_AF;
    tGPIO.GPIO_OType = GPIO_OType_PP;
    tGPIO.GPIO_PuPd = GPIO_PuPd_UP;
    tGPIO.GPIO_Speed = GPIO_Speed_10MHz;
  	GPIO_Init(GPIOA, &tGPIO);

  	tGPIO.GPIO_Pin = GPIO_Pin_3;
    tGPIO.GPIO_Mode = GPIO_Mode_AF;
    tGPIO.GPIO_OType = GPIO_OType_PP;
    tGPIO.GPIO_PuPd = GPIO_PuPd_NOPULL;
    tGPIO.GPIO_Speed = GPIO_Speed_10MHz;
  	GPIO_Init(GPIOA, &tGPIO);

  	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);
  	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);

};

void get_char_uart(uint8_t *ch) {
	 while(USART_GetFlagStatus(USART2,USART_FLAG_RXNE) == RESET){}
	 *ch = (uint8_t) USART_ReceiveData(USART2);
}
//send_char: send a byte via uart
void send_char_uart(uint8_t ch) {
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
	 USART_SendData(USART2,(uint8_t)ch);
	 return ch;
}
#endif

#ifdef ARDUINO_MCUZONE_NXP_1768
void Nxp1768_UsartHard (){
	Nxp1768_GPIO_Configuration();
	Nxp1768_Initiate_Usart();

}

void Nxp1768_Initiate_Usart (void){
	UART_CFG_Type UARTConfigStruct;
	UART_FIFO_CFG_Type UARTFIFOConfigStruct;
	UART_ConfigStructInit(&UARTConfigStruct);
	UARTConfigStruct.Baud_rate = 38400;
	UART_Init(LPC_UART0, &UARTConfigStruct);
	UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);
	UART_FIFOConfig(LPC_UART0, &UARTFIFOConfigStruct);
	UART_TxCmd(LPC_UART0, ENABLE);

}

void Nxp1768_GPIO_Configuration(void){
	PINSEL_CFG_Type PinCfg;
	PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Pinnum = 2;
	PinCfg.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 3;
	PINSEL_ConfigPin(&PinCfg);

}

void Nxp1768_get_char_uart(uint8_t *ch){
	UART_Receive(LPC_UART0,ch,1,BLOCKING);
}

void Nxp1768_send_char_uart(uint8_t ch){
	UART_Send(LPC_UART0,&ch,1,BLOCKING);
}
#endif









