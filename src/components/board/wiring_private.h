/*
  wiring_private.h - Internal header file.
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  $Id: wiring.h 239 2007-01-12 17:58:39Z mellis $
*/
//*******************************************************************************
//*	Jan  3,	2009	<MLS> Modified to be compatible with stealh and slide
//*******************************************************************************

#ifndef WiringPrivate_h
#define WiringPrivate_h

#ifndef _HARDWARE_DEF_H_
	#include	"HardwareDef.h"
#endif

#include <avr/io.h>
#include <inttypes.h>
#include <stdlib.h>
#include <inttypes.h>
#include <avr/interrupt.h>
//#include <avr/pgmspace.h>
//#include <avr/eeprom.h>

#include "wiring.h"


#ifndef ARDUINO_RESET_PORT
	#define ARDUINO_RESET_PORT	PORTE
#endif
#ifndef ARDUINO_RESET_PIN
	#define ARDUINO_RESET_PIN	PE4
#endif

#ifdef _TOUCH_STEALTH_

#endif




#ifdef __cplusplus
extern "C"{
#endif

// Define constants and variables for buffering incoming serial data.  We're
// using a ring buffer (I think), in which rx_buffer_head is the index of the
// location to which to write the next incoming character and rx_buffer_tail
// is the index of the location from which to read.
#define RX_BUFFER_SIZE 64

extern unsigned char rx_buffer[RX_BUFFER_SIZE];

extern int rx_buffer_head;
extern int rx_buffer_tail;

typedef void (*voidFuncPtr)(void);

#ifdef __cplusplus
} // extern "C"
#endif


#endif

