/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@bug.st>
 * SPI Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */
 
/*
 * Modified by (February 2014): AESS Estudiants - <aess@aess.upc.es>
 * Function changes:
 *	SPI.begin(mode) --> mode: SPI_MASTER or SPI_SLAVE
 *	SPI.attachInterrupt(function) --> function is the SPI ISR. When a interrupt occurs it will be called. 
 *	This function must take no parameters and return nothing.
 *
 *	Other functions act as the standard library
 */

#ifndef _SPI_H_INCLUDED
#define _SPI_H_INCLUDED

#include <stdio.h>
#include <Arduino.h>
#include <avr/pgmspace.h>

#define SPI_CLOCK_DIV4 0x00
#define SPI_CLOCK_DIV16 0x01
#define SPI_CLOCK_DIV64 0x02
#define SPI_CLOCK_DIV128 0x03
#define SPI_CLOCK_DIV2 0x04
#define SPI_CLOCK_DIV8 0x05
#define SPI_CLOCK_DIV32 0x06
//#define SPI_CLOCK_DIV64 0x07

#define SPI_MODE0 0x00
#define SPI_MODE1 0x04
#define SPI_MODE2 0x08
#define SPI_MODE3 0x0C

#define SPI_MODE_MASK 0x0C  // CPOL = bit 3, CPHA = bit 2 on SPCR
#define SPI_CLOCK_MASK 0x03  // SPR1 = bit 1, SPR0 = bit 0 on SPCR
#define SPI_2XCLOCK_MASK 0x01  // SPI2X = bit 0 on SPSR

#define SPI_MASTER 0x00
#define SPI_SLAVE  0x01

typedef void (*voidFuncPtr)(void);

class SPIClass {
public:
  inline static byte transfer(byte _data);
  inline static void transfer(void *buf, size_t count);

  // SPI Configuration methods

  static void attachInterrupt(void (*userFunc)(void));
  static void detachInterrupt(); // Default

  static void begin(byte _mode); // Default
  static void end();

  static void setBitOrder(uint8_t);
  static void setDataMode(uint8_t);
  static void setClockDivider(uint8_t);
};

extern SPIClass SPI;

byte SPIClass::transfer(byte _data) {
byte receive;
 
 /* Modification */
if(SPCR & _BV(MSTR)) //Check if Arduino is configured as Master or Slave
 {
   SPDR = _data;
   while (!(SPSR & _BV(SPIF)));
   receive = SPDR;
 }
 else
 {
   while(!(SPSR & _BV(SPIF)));
   receive = SPDR;
   SPDR = _data;
   
 }
 return receive;
}

void SPIClass::transfer(void *rx_buff, void* tx_buff, size_t count)
{
	if (count <= 0)
		return;


	if (SPCR & _BV(MSTR))
	{	// if master, data reg = first val of buf
		SPDR = (uint8_t) *tx_buff++;
		while (--count > 0)
		{
			while (!(SPSR & _BV(SPIF)));
			*rx_buff++ = SPDR;
			SPDR = (uint8_t) *tx_buff++;
		}
		while (!(SPSR & _BV(SPIF)));
		*rx_buff = SPDR;
	}
	else
	{
		while (--count > 0)
		{
			while (!(SPSR & _BV(SPIF)));
			*rx_buff++ = SPDR;
			SPDR = (uint8_t) *tx_buff++;
		}
		while(!(SPSR & _BV(SPIF)));
   		*rx_buff = SPDR;
		SPDR = (uint8_t) *tx_buff;
	}
}

#endif
