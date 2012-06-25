//*******************************************************************************
/*
  wiring_serial.c - serial functions.
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

  $Id: wiring.c 248 2007-02-03 15:36:30Z mellis $
*/
#ifdef _TOUCH_SLIDE_

#include "wiring_private.h"
#include "usart.h"

//*******************************************************************************
SIGNAL(SIG_USART1_RECV)
{
        unsigned char c =       UDR1;

        int i = (rx_buffer_head + 1) % RX_BUFFER_SIZE;

        // if we should be storing the received character into the location
        // just before the tail (meaning that the head would advance to the
        // current location of the tail), we're about to overflow the buffer
        // and so we don't write the character or advance the head.
        if (i != rx_buffer_tail)
        {
                rx_buffer[rx_buffer_head]       =       c;
                rx_buffer_head  =       i;
        }
}

//*******************************************************************************
void beginSerial(unsigned long baud)
{
	unsigned long UB;

	UB	=	F_OSC/(8*baud)-1;

	// set baud rate
	UBRR1H	=	(unsigned char)(UB>>8);
	UBRR1L	=	(unsigned char)UB;
	
	// Enable transmitter and receiver
	UCSR1B	=	(1<<RXEN1) | (1<<TXEN1) | (1<< RXCIE1);

 	UCSR1A |= (1<<U2X1);  //double speed

	sei(); //enable interrupts
}

//*******************************************************************************
void serialWrite(unsigned char c)
{
	while (!(UCSR1A & (1 << UDRE1)));
	UDR1	=	c;
}

//*******************************************************************************
int serialAvailable()
{
	return (RX_BUFFER_SIZE + rx_buffer_head - rx_buffer_tail) % RX_BUFFER_SIZE;
}

//*******************************************************************************
int serialRead()
{
	// if the head isn't ahead of the tail, we don't have any characters
	if (rx_buffer_head == rx_buffer_tail)
	{
		return -1;
	}
	else
	{
		unsigned char c	=	rx_buffer[rx_buffer_tail];
		rx_buffer_tail	=	(rx_buffer_tail + 1) % RX_BUFFER_SIZE;
		return c;
	}
}

#endif
