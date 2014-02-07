/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@bug.st>
 * SPI Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "pins_arduino.h"
#include "SPI.h"

static volatile voidFuncPtr intSPIFunc;

SPIClass SPI;

void SPIClass::begin(byte _mode) {

  // Set SS to high so a connected chip will be "deselected" by default
  digitalWrite(SS, HIGH);

  // When the SS pin is set as OUTPUT, it can be used as
  // a general purpose output port (it doesn't influence
  // SPI operations).
  pinMode(SS, OUTPUT);

  // Warning: if the SS pin ever becomes a LOW INPUT then SPI
  // automatically switches to Slave, so the data direction of
  // the SS pin MUST be kept as OUTPUT.
 if(_mode == SPI_MASTER)
 {
  SPCR |= _BV(MSTR);
  pinMode(SCK, OUTPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(MISO,INPUT);
  }
  else if( _mode == SPI_SLAVE)
  {
  pinMode(MOSI,INPUT);
  pinMode(SCK,INPUT);
  pinMode(MISO,OUTPUT);
  }

  SPCR |= _BV(SPE);
  

  // Set direction register for SCK and MOSI pin.
  // MISO pin automatically overrides to INPUT.
  // By doing this AFTER enabling SPI, we avoid accidentally
  // clocking in a single bit since the lines go directly
  // from "input" to SPI control.  
  // http://code.google.com/p/arduino/issues/detail?id=888
  
}


void SPIClass::end() {
  SPCR &= ~_BV(SPE);
}

void SPIClass::setBitOrder(uint8_t bitOrder)
{
  if(bitOrder == LSBFIRST) {
    SPCR |= _BV(DORD);
  } else {
    SPCR &= ~(_BV(DORD));
  }
}

void SPIClass::setDataMode(uint8_t mode)
{
  SPCR = (SPCR & ~SPI_MODE_MASK) | mode;
}

void SPIClass::setClockDivider(uint8_t rate)
{
  SPCR = (SPCR & ~SPI_CLOCK_MASK) | (rate & SPI_CLOCK_MASK);
  SPSR = (SPSR & ~SPI_2XCLOCK_MASK) | ((rate >> 2) & SPI_2XCLOCK_MASK);
}

void SPIClass::attachInterrupt(void (*userFunc)(void)) {
  intSPIFunc = userFunc;
  SPCR |= _BV(SPIE);
}

void SPIClass::detachInterrupt() {
  SPCR &= ~_BV(SPIE);
}

ISR(SPI_STC_vect)
{
	if(intSPIFunc)
		intSPIFunc();
}