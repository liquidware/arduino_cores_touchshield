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
#ifdef _TOUCH_STEALTH_
#include "oled_stealth.h"
#endif
#include	"bitops.h"
#include	"calibrate.h"
#include	"usart.h"
#include	"font.h"

#ifdef __cplusplus
extern "C" {
#endif

#if 0
volatile unsigned char press;
volatile unsigned char x_loc;
volatile unsigned char y_loc;
unsigned char yy_loc, xx_loc;
volatile unsigned char x_loc_max, y_loc_max;
volatile unsigned int sample_count;

MATRIX matrix;

#define ee_isCalibrated 28
#define ee_matrix 0
//EEMEM unsigned char ee_isCalibrated;
//EEMEM MATRIX ee_matrix;
#endif

static unsigned int
touchscreen_stealth_process_x(unsigned int adc_value)
{
  unsigned int x_loc = ADCH; //save the ADC result
#if 0

  if (x_loc > 15) //The natural idle value of the touch screens is about 2 for the x and 2 for the y
  //when there as been a press greater than 15 it is on the region of the screen (assuming the y also has a press greater than 15)
  {
    sample_count++; //this keeps track of how many samples the processor took durring a press
    if (x_loc > x_loc_max)//here we look for the max x value durring a press (based on the max of all the sample counts)
    { //save the new maximums
      x_loc_max = x_loc;//save the new maximum x
      y_loc_max = y_loc;//save the previous y
    }
    if (sample_count > 10000) //40,000 samples
    {
      eeprom_write_byte ((unsigned char*) ee_isCalibrated, 23);
      //eeprom_write_byte(50, 23);

      //lcd_clear();
      COLOR c =
      {
        255,
        0,
        0};
      COLOR blk =
      {
        0,
        0,
        0};
      lcd_clearScreen (blk);
      char msg[40];
      strncpy_P (msg, PSTR ("Touchscreen Reset."), 40);
      dispPutS (msg, 0, 50, c, blk);

      strncpy_P (msg, PSTR ("Please powercycle"), 40);
      dispPutS (msg, 0, 60, c, blk);
      while (1) {
        ;
      }

    }

  } else // there wasn't a press because the screen was still in an idle state
  {
    //idle
    if (x_loc_max <= 15)//not just pressed because x_loc_max would have a value of16 or greater from above
    {
    } else //this sample is the first sample after a touch took place
    {
      if (sample_count > 30) //makes sure the press is greater than 80 samples long (shorter press's are usually triggered by noise or other errors)
      {
        xx_loc = x_loc_max; //save the most recent touch screen x sample
        yy_loc = y_loc_max;//save the most recent touch screen y sample
        x_loc_max = 0;//reinitialize the program to be ready to sample again
        y_loc_max = 0;//reinitialize the program to be ready to sample again
        sample_count = 0;//reinitialize the program to be ready to sample again
        press = yes;//flag the pressed status from   no -> yes

      }
    }
  }
#endif
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

struct touchscreen_device touchscreen_slide =
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