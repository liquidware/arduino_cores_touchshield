#include "HardwareDef.h"
#include "wiring_private.h"

#ifdef _TOUCH_SLIDE_
    #include "wiring_serial_Slide.c"
#endif
#ifdef _TOUCH_STEALTH_
    #include "wiring_serial_Stealth.c"
#endif

unsigned char rx_buffer[RX_BUFFER_SIZE];

int rx_buffer_head = 0;
int rx_buffer_tail = 0;

//*******************************************************************************
void serialBegin(unsigned long baud)
{
        beginSerial(baud);
}

void serialFlush()
{
        // don't reverse this or there may be problems if the RX interrupt
        // occurs after reading the value of rx_buffer_head but before writing
        // the value to rx_buffer_tail; the previous value of rx_buffer_head
        // may be written to rx_buffer_tail, making it appear as if the buffer
        // were full, not empty.
        rx_buffer_head  =       rx_buffer_tail;
}

//*******************************************************************************
void printMode(int mode)
{
        // do nothing, we only support serial printing, not lcd.
}

//*******************************************************************************
void printByte(unsigned char c)
{
        serialWrite(c);
}

//*******************************************************************************
void printNewline()
{
        printByte('\n');
}

//*******************************************************************************
void printString(const char *s)
{
        while (*s)
                printByte(*s++);
}

//*******************************************************************************
void printIntegerInBase(unsigned long n, unsigned long base)
{
        unsigned char buf[8 * sizeof(long)]; // Assumes 8-bit chars.
        unsigned long i =       0;

        if (n == 0)
        {
                printByte('0');
                return;
        }

        while (n > 0)
        {
                buf[i++]        =       n % base;
                n /= base;
        }

        for (; i > 0; i--)
                printByte(buf[i - 1] < 10 ?
                        '0' + buf[i - 1] :
                        'A' + buf[i - 1] - 10);
}

//*******************************************************************************
void printInteger(long n)
{
        if (n < 0)
        {
                printByte('-');
                n       =       -n;
        }

        printIntegerInBase(n, 10);
}

//*******************************************************************************
void printHex(unsigned long n)
{
        printIntegerInBase(n, 16);
}

//*******************************************************************************
void printOctal(unsigned long n)
{
        printIntegerInBase(n, 8);
}

//*******************************************************************************
void printBinary(unsigned long n)
{
        printIntegerInBase(n, 2);
}
