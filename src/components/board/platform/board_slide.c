//*******************************************************************************
/* 

` wiring.c - Partial implementation of the Wiring API for the ATmega2560.
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
	
  **Adapted to the TouchShield and ported to the ATmega2560 - Chris Ladden

*/
//*******************************************************************************

#include "wiring_private.h"

#include <avr/io.h>
#include <stdlib.h>

#include "bitops.h"
#include "touchscreen.h"
#include "adc_sampling.h"

#define ARDUINO_RESET_PORT PORTE
#define ARDUINO_RESET_DDR  DDRE
#define ARDUINO_RESET_PIN  PINE
#define ARDUINO_RESET_PNUM PE4

//*******************************************************************************
void
slide_arduino_run()
{

  /* Disable USART transmitter and receiver */
  UCSR0B = (0 << RXEN0) | (0 << TXEN0) | (0 << RXCIE0);
  SETBIT(ARDUINO_RESET_PORT, ARDUINO_RESET_PNUM);
  //arduino not in reset
  CLRBIT(ARDUINO_RESET_DDR, ARDUINO_RESET_PNUM);
  //remove drive

}

//*******************************************************************************
void
slide_arduino_reset()
{

  SETBIT(ARDUINO_RESET_DDR, ARDUINO_RESET_PNUM);
  CLRBIT(ARDUINO_RESET_PORT, ARDUINO_RESET_PNUM);
  //arduino in reset
}

enum {
  TOUCHSHIELD_SLIDE_REVA = 3,
  TOUCHSHIELD_SLIDE_REVB = 0,
};

unsigned char
touchshield_get_revision(void)
{
  DDRG &= (0 << PG0) | (0 << PG1) | (0 << PG2); //input
  PORTG |= (1 << PG0) | (1 << PG1) | (1 << PG2); //pullups

  delay_ms(5);

  return (PING & 0x03);
}

//*******************************************************************************
void
slide_init()
{
  extern struct display_device hx8347_driver;
  extern struct display_device oled28_driver;

  if (touchshield_get_revision () == TOUCHSHIELD_SLIDE_REVA)
    display = &oled28_driver;
  else
    display = &hx8347_driver;
  display->init ();

  dataflash_init ();
  touchscreen->init ();
  touchscreen_service_init();
  bmp_init ();
}

struct platform_board slide_board = {
    .init = slide_init,
    .arduino_reset = slide_arduino_reset,
    .arduino_run = slide_arduino_run,
};

