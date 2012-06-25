/* 

 ` wiring.c - Partial implementation of the Wiring API for the ATmega645.
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

 **Adapted to the TouchShield and ported to the ATmega645 - Chris Ladden

 */

#include "wiring_private.h"

#include "bitops.h"
#include "touchscreen.h"
#include "adc_sampling.h"

#define ARDUINO_TX_ENABLE_PORT  PORTE
#define ARDUINO_TX_ENABLE_DDR   DDRE
#define ARDUINO_TX_ENABLE_PIN   PE7

#define RX_PIN                                  PE5
#define TX_PIN                                  PE6
#define RXTX_PIN                                PINE
#define RXTX_DDR                                DDRE
#define RXTX_PORT                               PORTE

#define PCMSK_REGISTER                  PCMSK0          //RX pin change interrupt mask register
#define RX_PCINT                                PCINT5          //RX pin change interrupt bit position
#define BITTIME                                 200                     //ticks of the counter 9600 = 200
#define BITTIME_HALF                    100                     //ticks of the counter 9600 = 100


void
bitbang_init()
{
  /*
   //disable hardware UART
   CLRBIT(UCSR0B,RXEN0);
   CLRBIT(UCSR0B,TXEN0);


   SETBIT(RXTX_PORT, TX_PIN);  //idle state
   SETBIT(RXTX_DDR, TX_PIN);	//output
   CLRBIT(RXTX_DDR, RX_PIN);	//input
   sei(); */
}

void
bitbang_putc(unsigned char c)
{

  /*
   volatile unsigned int x;
   volatile unsigned char bits=0;

   cli();

   CLRBIT(RXTX_PORT, TX_PIN);

   for(x=0; x<90; x++) {;}

   for (bits = 0; bits<8; bits++)
   {


   if ( CHECKBIT(c,bits) )
   {
   SETBIT(RXTX_PORT,TX_PIN); //bit is a '1'
   }
   else
   {
   CLRBIT(RXTX_PORT,TX_PIN); //bit is a '0'
   }


   for(x=0; x<90; x++) {;}

   }

   SETBIT(RXTX_PORT, TX_PIN);
   for(x=0; x<90; x++) {;}

   sei(); //enable interrupts
   */
}

unsigned char
bitbang_receive()
{
  volatile unsigned int x;
  volatile unsigned char bits = 0;
  volatile unsigned char rx_msg = 0;

  cli ();

  while (CHECKBIT(RXTX_PIN, RX_PIN) >> RX_PIN) {
    ;
  }

  for (x = 0; x < 45; x++) {
    ;
  }

//if ( (CHECKBIT(RXTX_PIN, RX_PIN) >>  RX_PIN) != 0) //noise on the line?
//	return 0;

  for (bits = 0; bits < 8; bits++) {

    for (x = 0; x < 90; x++) {
      ;
    }

    rx_msg >>= 1; //shift over one bit
    if (CHECKBIT(RXTX_PIN, RX_PIN))
      rx_msg |= B10000000; //bit is a '1'
  }

  for (x = 0; x < 45; x++) {
    ;
  }

  sei (); //enable interrupts

  return rx_msg;
}

void
stealth_arduino_run()
{
  SETBIT(ARDUINO_TX_ENABLE_DDR, ARDUINO_TX_ENABLE_PIN);
  //DDR for TX buffer
  SETBIT(ARDUINO_RESET_PORT, ARDUINO_RESET_PIN);
  //Run arduino
  SETBIT(ARDUINO_TX_ENABLE_PORT, ARDUINO_TX_ENABLE_PIN);
  //Don't Drive TX buffer
}

void
stealth_arduino_reset()
{
  CLRBIT(ARDUINO_RESET_PORT, ARDUINO_RESET_PIN);
  //arduino in reset
  CLRBIT(ARDUINO_TX_ENABLE_PORT, ARDUINO_TX_ENABLE_PIN);
  //Drive TX buffer
}

void
stealth_init()
{
  stealth_arduino_run ();
  display->init ();
  dataflash_init ();
  touchscreen->init ();
  touchscreen_service_init ();
  bmp_init ();
}

struct platform_board stealth_board =
  {
      .init = stealth_init,
      .arduino_reset = stealth_arduino_reset,
      .arduino_run = stealth_arduino_run, };

