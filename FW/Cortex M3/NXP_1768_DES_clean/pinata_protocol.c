/*
 * CFile1.c
 *
 * Created: 03.08.2021 9:17:44
 *  Author: harde
 */ 

#include "pinata_protocol.h"
#include "des.h"
#include "trigger_arduino.h"
#include "uart_arduino.h"
#include "aes.h"

uint8_t BufferRx[128];
uint8_t BufferTx[128];
uint8_t Pincode[8] = {'n','e','m','e','m','e','s','i','s','a'};
const uint8_t defaultKeyAES[16] = { 0xca, 0xfe, 0xba, 0xbe, 0xde, 0xad, 0xbe, 0xef, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
const uint8_t defaultKeyDES[8] = { 0xca, 0xfe, 0xba, 0xbe, 0xde, 0xad, 0xbe, 0xef };	
const uint8_t cmdByteIsWrong[] = { 'B','a','d','C','m','d','\n',0x00};
const uint8_t codeVersion[] = { 'V','e','r',' ','2','.','4',0x00};

void pinata_comannd_init(void)
{
   //uart_arduino_init0_in_place(38400);	
}

void pinata_comannd_loop(void)
{
	   uint8_t comand = uart_arduino_input_ch_0(); 
       switch (comand)
       {
		   case PINATA_CMD_SW_DES_ENCRYPT:
		   get_bytes(BufferRx, 8);
		   des_encypt(BufferRx);	   
		   break;
		    
		   case PINATA_CMD_SW_DES_DECRYPT:
		   get_bytes(BufferRx, 8);
		   des_decrypt(BufferRx);
		   break;
		   
		   case PINATA_CMD_SW_AES_DECRYPT:
		   get_bytes(BufferRx, 16);
		   aes_decrypt(BufferRx);
		   break;
		   
		   case PINATA_CMD_SW_AES_ENCRYPT:
		   get_bytes(BufferRx, 16);
		   aes_encypt(BufferRx);
		   break;
			
		   case PINATA_CMD_GET_CODE_REV:
		   get_core_version(BufferRx);
		   break;	
			
		   case PINATA_CMD_CHECK_PIN:
		   simple_pincode_check(BufferRx);
		   break;
			   
		   default:
		   send_bytes(cmdByteIsWrong, 8);  
		   break;
       } 
}

void simple_pincode_check(int8_t* buffer)
{
	trigger_high();
	int i = 0;
	for (i = 0; i < 8; i++)
	{
		if(buffer[i] != Pincode[i])
		{
			break;;
		}
	}
	
	trigger_low();
	send_bytes(buffer, 8);  
}

void get_core_version(uint8_t* buffer)
{
	trigger_high();
	send_bytes(codeVersion, 8);
	trigger_low();
}

void des_encypt(uint8_t* buffer)
{
	trigger_high();
	des(defaultKeyDES, buffer, 0);
	trigger_low();
	send_bytes(buffer, 8);  	  
}

void des_decrypt(uint8_t* buffer)
{
	trigger_high();
	des(defaultKeyDES, buffer, 1);
	send_bytes(buffer, 8);
	trigger_low();
}

void aes_encypt(uint8_t* buffer)
{
	trigger_high();
	AES128_ECB_encrypt(buffer, defaultKeyAES, BufferTx);
	send_bytes(BufferTx, 16);
	trigger_low();
}

void aes_decrypt(uint8_t* buffer)
{
	trigger_high();
	AES128_ECB_decrypt(buffer, defaultKeyAES, BufferTx);
	send_bytes(BufferTx, 16);
	trigger_low();
}

void get_bytes(uint8_t* buffer, size_t buffer_len)
{
	if(buffer == NULL) return;
	int i = 0;
	for (i = 0; i < buffer_len; i++)
	{
		buffer[i] = uart_arduino_input_ch_0();  
	}
}

void send_bytes(uint8_t* buffer, size_t buffer_len)
{
	if(buffer == NULL) return;
	int i = 0;
	for (i = 0; i < buffer_len; i++)
	{
		uart_arduino_output_ch_0(buffer[i]);
	}
	
}
