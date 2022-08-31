/*
 * pinata_protocol.h
 *
 * Created: 03.08.2021 9:18:56
 *  Author: harde
 */ 


#ifndef PINATA_H_
#define PINATA_H_
#include <stdint.h>
#include <stdio.h>
#include "uart_arduino.h"

enum PINATA_CMD{
	PINATA_CMD_SW_DES_ENCRYPT = 0x44,
	PINATA_CMD_SW_DES_DECRYPT = 0x45,
	PINATA_CMD_SW_AES_ENCRYPT = 0xAE,
	PINATA_CMD_SW_AES_DECRYPT = 0xEA,
	PINATA_CMD_GET_CODE_REV	  = 0xF1,
	PINATA_CMD_CHECK_PIN	  = 0xF2,
};

void pinata_comannd_init(void);
void pinata_comannd_loop(void);
void des_encypt(uint8_t* buffer);
void simple_pincode_check(int8_t* buffer);
void des_decrypt(uint8_t* buffer);
void aes_encypt(uint8_t* buffer);
void aes_decrypt(uint8_t* buffer);
void get_core_version(uint8_t* buffer);
void get_bytes(uint8_t* buffer, size_t buffer_len);
void send_bytes(uint8_t* buffer, size_t buffer_len);





#endif /* INCFILE1_H_ */