/*
 * arduino_config.h
 *
 * Created: 19.07.2021 12:09:43
 *  Author: harde
 */ 

#ifndef ARDUINO_CONFIG_H_
#define ARDUINO_CONFIG_H_



//#define ARDUINO_UNO 1
//#define ARDUINO_LEO 1
//#define ARDUINO_XNUCLEO_F0 1
#define ARDUINO_MCUZONE_NXP_1768 1

#define DES_CRYPTO
#define AES_CRYPTO

#define F_CPU 16000000
#define USART_SPEED 38400
#define SS_VER SS_VER_1_1

#endif /* ARDUINO_CONFIG_H_ */
