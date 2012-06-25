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

#include	"HardwareDef.h"

#include	"touchscreen.h"

#include	"bitops.h"
#include	"calibrate.h"
#include	"usart.h"
#include	"font.h"

#ifdef __cplusplus
extern "C" {
#endif

static unsigned int
touchscreen_stealth_process_x(unsigned int adc_value)
{
  unsigned int x_loc = ADCH; //save the ADC result

  return x_loc;
} //end

static unsigned int
touchscreen_stealth_process_y(unsigned int adc_value)
{

  unsigned int y_loc = ADCH; //save the ADC result

  return y_loc;
} //end ADC Interrupt

static void
touchscreen_stealth_init() // initialize the touch algorithm
{

  ADCSRA = (1 << ADEN) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1)
      | (1 << ADPS0); //clock freq fosc/128

  TCCR0A = (0 << CS02) | (1 << CS01) | (0 << CS00); //timer 0 setup to overflow  every 128us
  TIMSK0 = (1 << TOIE0);

  sei (); //enable global interupts	because this is an inturrupt based saple method

} //end touch_init

//initial sampling direction
static void
touchscreen_stealth_setup_x() //set up a sample in the x direction
{

  unsigned char x;

  /*           basically you
   - ground the y plus pin
   - set +5V to the y minus pin
   - ground the x plus pin
   - and read the voltage at the x minus pin

   */

  //swithing channels,
  //drive the x+ high, x- low
  SETBIT(DDRF, XPLUS_PIN);
  SETBIT(DDRF, XMINUS_PIN);

  SETBIT(PORTF, XPLUS_PIN);
  CLRBIT(PORTF, XMINUS_PIN);

  CLRBIT(PORTF, YPLUS_PIN);
  //grounding out screen
  SETBIT(DDRF, YPLUS_PIN);
  //set to output

  for (x = 0; x < 50; x++) //delay before internal avr caoaciters can discharge
      {
    asm("nop");
  }

  //Y+ input
  CLRBIT(DDRF, YPLUS_PIN);
  //Y- floating
  CLRBIT(DDRF, YMINUS_PIN);
  ADMUX = YPLUS;

  for (x = 0; x < 200; x++) //delay before internal avr caoaciters can discharge
      {
    asm("nop");
  }

  for (x = 0; x < 200; x++) //delay before internal avr caoaciters can discharge
      {
    asm("nop");
  }

  for (x = 0; x < 200; x++) //delay before internal avr caoaciters can discharge
      {
    asm("nop");
  }

  for (x = 0; x < 200; x++) //delay before internal avr caoaciters can discharge
      {
    asm("nop");
  }
  for (x = 0; x < 200; x++) //delay before internal avr caoaciters can discharge
      {
    asm("nop");
  }

  SETBIT(ADCSRA, ADSC);
  //start conversion!

} //end setup_y

static void
touchscreen_stealth_setup_y() // set up a sample in the y-direction
{

  unsigned char x;

  /*basically you
   - ground the y plus pin
   - set +5V to the y minus pin
   - ground the x plus pin
   - and read the voltage at the x minus pin

   */

  //swithing channels,
  //drive the y+ high, y- low
  SETBIT(DDRF, YPLUS_PIN);
  SETBIT(DDRF, YMINUS_PIN);

  SETBIT(PORTF, YPLUS_PIN);
  CLRBIT(PORTF, YMINUS_PIN);

  CLRBIT(PORTF, XPLUS_PIN);
  //grounding out touch screen
  SETBIT(DDRF, XPLUS_PIN);
  //set to output

  for (x = 0; x < 50; x++) //delay before internal avr caoaciters can discharge
      {
    asm("nop");
  }

  //X+ input
  CLRBIT(DDRF, XPLUS_PIN);
  //X- floating
  CLRBIT(DDRF, XMINUS_PIN);
  ADMUX = XPLUS;

  for (x = 0; x < 200; x++) //delay before internal avr caoaciters can discharge
      {
    asm("nop");
  }

  SETBIT(ADCSRA, ADSC);

} //end setup_x

static void
touchscreen_stealth_setup_z1()
{
}

static void
touchscreen_stealth_setup_z2()
{
}

static void
touchscreen_stealth_process_z1(unsigned int adc_value)
{
}

static void
touchscreen_stealth_process_z2(unsigned int adc_value)
{
}

/****************************************************/
/*													*/
/* Touch Calibrate									*/
/*													*/
/****************************************************/

/* calibrating the touch screen compensates for 4 major issues 
 relating a touch location to a pixel location.
 1)	An offset in the x-aligenment
 2)	An offset in the y-aligenment
 3)	A rotational offset between the rows of pixels and the touch screen
 4)	Compensates the offset of each press location on the touch screen to a pixel location on the LCD screen
 */
char
touchscreen_stealth_calibrate(void)
{
#if 0
  //array of input points
  POINT32 screenSample[3];

  //array of expected correct answers
  POINT32 displaySample[3] =
  {
    {
      20,
      60},
    {
      60,
      20},
    {
      90,
      115}};

  /* An array of perfect input screen points used to obtain a first pass	*/
  /*	calibration matrix good enough to collect calibration samples.		*/

  POINT32 perfectScreenSample[3] =
  {
    {
      100,
      100},
    {
      900,
      500},
    {
      500,
      900}};

  /* An array of perfect display points used to obtain a first pass		*/
  /*	calibration matrix good enough to collect calibration samples.		*/

  POINT32 perfectDisplaySample[3] =
  {
    {
      100,
      100},
    {
      900,
      500},
    {
      500,
      900}};

  //initial pass
  setCalibrationMatrix (&perfectDisplaySample[0], &perfectScreenSample[0],
      &matrix);

//gui_puts("",0,120,0);//print the idle state of the touch screen 

  COLOR c1 =
  {
    255,
    0,
    0}; //circle color
  COLOR c2 =
  {
    0x00,
    0x00,
    0x00}; //fill

  dispPutS ("Touch to calibrate", 18, 5, c1, c2);

  dispColor (c1);
  dispRectangle (20, 60, 6, 6);//draw
  press = no;
  while (!press) {
    ;
  }

//got a release
  screenSample[0].x = xx_loc;
  screenSample[0].y = yy_loc;

  dispColor (c2);
  dispRectangle (20, 60, 6, 6);//draw
  delay_ms (250);

  dispColor (c1);
  dispRectangle (60, 20, 6, 6);//draw
  press = no;
  while (!press) {
    ;
  }

//got a release
  screenSample[1].x = xx_loc;
  screenSample[1].y = yy_loc;

  dispColor (c2);
  dispRectangle (60, 20, 6, 6);//draw
  delay_ms (250);

  dispColor (c1);
  dispRectangle (90, 115, 6, 6);//draw
  press = no;
  while (!press) {
    ;
  }

//got a release
  screenSample[2].x = xx_loc;
  screenSample[2].y = yy_loc;

  dispColor (c2);
  dispRectangle (90, 115, 6, 6);//draw

  COLOR c =
  {
    200,
    200,
    200};
  COLOR blk =
  {
    0,
    0,
    0};

  lcd_clearScreen (blk);
  dispPutS ("Calibrating the", 15, 15, c, blk);
  dispPutS ("Touch Screen", 20, 25, c, blk);

  //now, the real calibration pass
  setCalibrationMatrix (&displaySample[0], &screenSample[0], &matrix);

  eeprom_write_byte ((unsigned char*) ee_isCalibrated, TOUCHSCR_IS_CALIBRATED);
  eeprom_write_block (&matrix, (unsigned char*) ee_matrix, sizeof(MATRIX));//MATRIX	=	28 bytes

  delay_ms (250);
  delay_ms (250);
  delay_ms (250);
  POINT clear_value;
  touch_get_cursor (&clear_value);
  press = no;
#endif
  return 0;
}

static void
touchscreen_stealth_resume(void)
{
}

static void
touchscreen_stealth_suspend(void)
{
}

static void
touchscreen_stealth_enable(void)
{
}

static void
touchscreen_stealth_disable(void)
{
}

struct touchscreen_device touchscreen_stealth =
  {

      .init = touchscreen_stealth_init,

      .setup_x = touchscreen_stealth_setup_x,
      .setup_y = touchscreen_stealth_setup_y,
      .setup_z1 = touchscreen_stealth_setup_z1,
      .setup_z2 = touchscreen_stealth_setup_z2,

      .process_x = touchscreen_stealth_process_x,
      .process_y = touchscreen_stealth_process_y,
      .process_z1 = touchscreen_stealth_process_z1,
      .process_z2 = touchscreen_stealth_process_z2,

      .resume = touchscreen_stealth_resume,
      .suspend = touchscreen_stealth_suspend,
      .enable = touchscreen_stealth_enable,
      .disable = touchscreen_stealth_disable,
  };

#ifdef __cplusplus
} // extern "C"
#endif
