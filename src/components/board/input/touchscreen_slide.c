//*******************************************************************************
//*		
//*	
//*	
//*******************************************************************************
//*	Detailed Edit History
//*		PLEASE put comments here every time you work on this code
//*******************************************************************************
//*	Jan  2,	2009	<MLS> Started working on converging touchStealth with touchSlide cores
//*	Jan  4,	2009	<MLS> Changed touchShield touchscreen_process_x args to int to be compatible with slide
//*******************************************************************************

#include	<avr/io.h>
#include	<inttypes.h>
#include	<stdlib.h>
#include	<inttypes.h>
#include	<avr/interrupt.h>
#include	<avr/pgmspace.h>
#include	<avr/eeprom.h>

#include	"touchscreen.h"
#include	"graphics.h"
#include	"bitops.h"
#include	"calibrate.h"
#include	"usart.h"
#include	"font.h"

//*******************************************************************************
static unsigned int
touchscreen_slide_process_x(unsigned int adc_value)
{

  /* Store Value */
  unsigned int x_samples = 1023 - adc_value;

  /* Remove Excitation */
  DDRF = 0x00;

  /* Discharge Signal */
  PORTF = 0x00; //Ground
  DDRF = 0xFF; //Output
  DDRF = 0x00; //Floating

  return x_samples;
}

//*******************************************************************************
static unsigned int
touchscreen_slide_process_y(unsigned int adc_value)
{
  /* Store Value */
  unsigned int y_samples = 1023 - adc_value;

  /* Remove Excitation */
  DDRF = 0x00;

  /* Discharge Signal */
  PORTF = 0x00; //Ground
  DDRF = 0xFF; //Output
  DDRF = 0x00; //Floating

  return y_samples;
}

static unsigned int
touchscreen_slide_process_z1(unsigned int adc_value)
{

  /* Store Value */
  unsigned int z1_samples = adc_value;

  /* Remove Excitation */
  DDRF = 0x00;

  /* Discharge Signal */
  PORTF = 0x00; //Ground
  DDRF = 0xFF; //Output
  DDRF = 0x00; //Floating

  return z1_samples;
}

//*******************************************************************************
static unsigned int
touchscreen_slide_process_z2(unsigned int adc_value)
{

  /* Store Value */
  unsigned int z2_samples = adc_value;

  /* Remove Excitation */
  DDRF = 0x00;

  /* Discharge Signal */
  PORTF = 0x00; //Ground
  DDRF = 0xFF; //Output
  DDRF = 0x00; //Floating

  return z2_samples;
}

//*******************************************************************************
static void
touchscreen_slide_init(void)
{
  ADCSRA = (1 << ADEN) | (0 << ADIE) | (0 << ADPS2) | (1 << ADPS1)
      | (1 << ADPS0); //clock freq fosc

  TCCR0B = (0 << CS02) | (1 << CS01) | (0 << CS00); //timer 0 setup to overflow every 128us
  TIMSK0 = (1 << TOIE0);

  sei (); //enable global interupts
}

//*******************************************************************************
static void
touchscreen_slide_setup_x()
{

  /* Setup MUX */
  ADMUX = YPLUS;

  /* Setup Excitation */
  CLRBIT(PORTF, YPLUS_PIN);
  CLRBIT(PORTF, YMINUS_PIN);
  //Y
  SETBIT(PORTF, XPLUS_PIN);
  CLRBIT(PORTF, XMINUS_PIN);
  //X

  /* Drive Excitation */
  CLRBIT(DDRF, YPLUS_PIN);
  CLRBIT(DDRF, YMINUS_PIN);
  //Y
  SETBIT(DDRF, XPLUS_PIN);
  SETBIT(DDRF, XMINUS_PIN);
  //X drive

} //end setup_y

//*******************************************************************************
static void
touchscreen_slide_setup_y()
{

  /* Setup MUX */
  ADMUX = XPLUS;

  /* Setup Excitation */
  SETBIT(PORTF, YPLUS_PIN);
  CLRBIT(PORTF, YMINUS_PIN);
  //Y
  CLRBIT(PORTF, XPLUS_PIN);
  CLRBIT(PORTF, XMINUS_PIN);
  //X

  /* Drive Excitation */
  SETBIT(DDRF, YPLUS_PIN);
  SETBIT(DDRF, YMINUS_PIN);
  //Y drive
  CLRBIT(DDRF, XPLUS_PIN);
  CLRBIT(DDRF, XMINUS_PIN);
  //X
} //end setup_x

//*******************************************************************************
static void
touchscreen_slide_setup_z1()
{

  /* Setup MUX */
  ADMUX = XPLUS;

  /* Setup Excitation */
  SETBIT(PORTF, YPLUS_PIN);
  CLRBIT(PORTF, YMINUS_PIN);
  //Y
  CLRBIT(PORTF, XPLUS_PIN);
  CLRBIT(PORTF, XMINUS_PIN);
  //X

  /* Drive Excitation */
  SETBIT(DDRF, YPLUS_PIN);
  CLRBIT(DDRF, YMINUS_PIN);
  //Y
  CLRBIT(DDRF, XPLUS_PIN);
  SETBIT(DDRF, XMINUS_PIN);
  //X

}

//*******************************************************************************
static void
touchscreen_slide_setup_z2()
{

  /* Setup MUX */
  ADMUX = YMINUS;

  /* Setup Excitation */
  SETBIT(PORTF, YPLUS_PIN);
  CLRBIT(PORTF, YMINUS_PIN);
  //Y
  CLRBIT(PORTF, XPLUS_PIN);
  CLRBIT(PORTF, XMINUS_PIN);
  //X

  /* Drive Excitation */
  SETBIT(DDRF, YPLUS_PIN);
  CLRBIT(DDRF, YMINUS_PIN);
  //Y
  CLRBIT(DDRF, XPLUS_PIN);
  SETBIT(DDRF, XMINUS_PIN);
  //X

}

static void
touchscreen_slide_resume(void)
{

}

static void
touchscreen_slide_suspend(void)
{

}

static void
touchscreen_slide_enable(void)
{

}

static void
touchscreen_slide_disable(void)
{

}

struct touchscreen_device touchscreen_slide_driver =
  {
      .init = touchscreen_slide_init,

      .setup_x = touchscreen_slide_setup_x,
      .setup_y = touchscreen_slide_setup_y,
      .setup_z1 = touchscreen_slide_setup_z1,
      .setup_z2 = touchscreen_slide_setup_z2,

      .process_x = touchscreen_slide_process_x,
      .process_y = touchscreen_slide_process_y,
      .process_z1 = touchscreen_slide_process_z1,
      .process_z2 = touchscreen_slide_process_z2,

      .resume = touchscreen_slide_resume,
      .suspend = touchscreen_slide_suspend,
      .enable = touchscreen_slide_enable,
      .disable = touchscreen_slide_disable,
  };

