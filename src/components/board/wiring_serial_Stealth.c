/*
  wiring_serial.c - serial functions.
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis //hacked by Chris Ladden

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

  $Id: wiring.c 248 2007-02-03 15:36:30Z mellis $
*/
#ifdef _TOUCH_STEALTH_

#include "wiring_private.h"
#include "usart.h"

#define RX_PIN                                  PE5
#define TX_PIN                                  PE6
#define RXTX_PIN                                PINE
#define RXTX_DDR                                DDRE
#define RXTX_PORT                               PORTE

void beginSerial(unsigned long baud)
{
	//disable hardware UART
	CLRBIT(UCSR0B,RXEN0);
	CLRBIT(UCSR0B,TXEN0);


	SETBIT(RXTX_PORT, TX_PIN);  //idle state
	SETBIT(RXTX_DDR, TX_PIN);	//output
	CLRBIT(RXTX_DDR, RX_PIN);	//input
	sei();
}

void serialWrite(unsigned char c)
{


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

}

int serialAvailable()
{
    //return(RX_BUFFER_SIZE + rx_buffer_head - rx_buffer_tail) % RX_BUFFER_SIZE;

    return 1; //always return 1, serialAvailable is usually followed by a serialRead(),
              //and since because we don't support interrupt based software serial.
              //This forces the application into a blocking read.
}

int serialRead()
{
volatile unsigned int x;
volatile unsigned char bits=0;
volatile unsigned char rx_msg=0;

cli();

while(CHECKBIT(RXTX_PIN, RX_PIN) >>  RX_PIN) {;}




for(x=0; x<45; x++) {;}


//if ( (CHECKBIT(RXTX_PIN, RX_PIN) >>  RX_PIN) != 0) //noise on the line?
//	return 0;

for (bits = 0; bits<8; bits++)
	{

	for(x=0; x<90; x++) {;}

	rx_msg >>= 1; //shift over one bit
	if (CHECKBIT(RXTX_PIN, RX_PIN))
		rx_msg |= B10000000;	//bit is a '1'
	}

for(x=0; x<45; x++) {;}

sei(); //enable interrupts

return rx_msg;
}
#endif
