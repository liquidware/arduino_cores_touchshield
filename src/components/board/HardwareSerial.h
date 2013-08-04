/*
  HardwareSerial.h - Hardware serial library for Wiring
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef HardwareSerial_h
#define HardwareSerial_h

#include <inttypes.h>

#include "Stream.h"

#ifdef __cplusplus

struct ring_buffer;

class HardwareSerial : public Stream
{
	private:
		//uint8_t _uart;
		
	public:
		HardwareSerial(uint8_t);
		void begin(long);
		void begin(unsigned long, uint8_t);
		void end();
		virtual int available(void);
    		virtual int peek(void);
		virtual int read(void);
		virtual void flush(void);
    		virtual size_t write(uint8_t);
    		inline size_t write(unsigned long n) { return write((uint8_t)n); }
    		inline size_t write(long n) { return write((uint8_t)n); }
    		inline size_t write(unsigned int n) { return write((uint8_t)n); }
    		inline size_t write(int n) { return write((uint8_t)n); }
		using Print::write; // pull in write(str) and write(buf, size) from Print
		operator bool();

#if 0
		void printNumber(unsigned long, uint8_t);
		void print(char);
		void print(const char[]);
		void print(uint8_t);
		void print(int);
	#ifndef  __MWERKS__
		void print(unsigned int);
	#endif
		void print(long);
		void print(unsigned long);
		void print(long, int);
		void println(void);
		void println(char);
		void println(const char[]);
		void println(uint8_t);
		void println(int);
		void println(long);
		void println(unsigned long);
		void println(long, int);
#endif
};

extern HardwareSerial Serial;
//extern HardwareSerial Serial1;

#endif

#endif

