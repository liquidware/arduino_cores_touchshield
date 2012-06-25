
#include "wiring_private.h"

/* A structure to describe the pins */
typedef struct {
    uint8_t  *PortReg;      //The port of the pin ex. PORTA, PORTB,...
    uint8_t   PinNum;       //The pin number in the port, usually PA1, PA2,...
    uint8_t  *PinReg;       //The pin input register
    uint8_t  *DirReg;       //The data direction register
    uint8_t  *PCMSK;        //The PCMSK register number
    uint8_t   PCINT;        //The PCINT bit number
} PIN_DESC_T;

/* Maximum number of pins, for error checking */
#define NUM_PINS sizeof(pinTable)/sizeof(PIN_DESC_T)

/*==============================================================================
 * CONSTANTS
 *============================================================================*/

/* The user pin lookup table.
   This table provides a mapping to the hardware pins. */
PIN_DESC_T const pinTable[] = {
    /* 0 */  { &PORTE, 0, &PINE, &DDRE },
    /* 1 */  { &PORTE, 1, &PINE, &DDRE },
    /* 2 */  { &PORTD, 2, &PIND, &DDRD },
    /* 3 */  { &PORTD, 3, &PIND, &DDRD },
    /* 4 */  { &PORTH, 0, &PINH, &DDRH },
    /* 5 */  { &PORTH, 1, &PINH, &DDRH },
    /* 6 */  { &PORTJ, 0, &PINJ, &DDRJ },
    /* 7 */  { &PORTJ, 1, &PINJ, &DDRJ },
    /* 8 */  { &PORTB, 7, &PINB, &DDRB },
    /* 9 */  { &PORTB, 6, &PINB, &DDRB },
    /* 10 */  { &PORTH, 6, &PINH, &DDRH },
    /* 11 */  { &PORTE, 3, &PINE, &DDRE },
    /* 12 */  { &PORTH, 3, &PINH, &DDRH },
    /* 13 */  { &PORTL, 3, &PINL, &DDRL },
};

/*==============================================================================
 * FUNCTIONS
 *============================================================================*/

/* ===========================================================================
*  FUNCTION: pinMode
*
*  DESIGN DESCRIPTION:
*     Changes the pin direction
*
*  PARAMETER LIST:
*     pin - The pin number to be changed.
*     val - INPUT or OUTPUT value to be written to the pin
*
*  RETURNED:
*     none
*
*  DESIGN NOTES/CONSTRAINTS:
*
*
*===========================================================================*/
void pinMode(uint8_t pin, uint8_t mode)
{
    PIN_DESC_T *p = &pinTable[pin];

    /* Check desired mode */
    if (mode == INPUT)
    {
        /* Look up the DDR reg and set the bit */
        CLRBIT(*p->DirReg,
               p->PinNum);
    }
    else
    {
        /* Look up the DDR reg and set the bit */
        SETBIT(*p->DirReg,
                p->PinNum);
    }
}

/* ===========================================================================
*  FUNCTION: digitalWrite
*
*  DESIGN DESCRIPTION:
*     Write a value to a pin
*
*  PARAMETER LIST:
*     pin - The pin number to be changed.
*     val - HIGH or LOW value to write to the pin
*
*  RETURNED:
*     none
*
*  DESIGN NOTES/CONSTRAINTS:
*
*
*===========================================================================*/
void digitalWrite(uint8_t pin, uint8_t val)
{
    PIN_DESC_T *p = &pinTable[pin];

    /* Check desired state */
    if (val == HIGH)
    {
        /* Look up the port reg and set the bit */
        SETBIT(*p->PortReg,
                p->PinNum);
    }
    else
    {
        /* Look up the port reg and set the bit */
        CLRBIT(*p->PortReg,
                p->PinNum);
    }
}

/* ===========================================================================
*  FUNCTION: digitalRead
*
*  DESIGN DESCRIPTION:
*     Reads a digital pin
*
*  PARAMETER LIST:
*     pin - The pin to be read
*
*  RETURNED:
*     returns 0 or the masked pin register value
*
*  DESIGN NOTES/CONSTRAINTS:
*
*
*===========================================================================*/
int digitalRead(uint8_t pin)
{
    PIN_DESC_T *p = &pinTable[pin];

    /* Return the pin register value at the bit location */
    if (CHECKBIT(*p->PinReg, p->PinNum) >> p->PinNum) {
        return HIGH;
    } else{
        return LOW;
    }
}

void delayMicroseconds(unsigned int us){ ; }
