#include <avr/io.h>
#include <inttypes.h>
#include <avr/eeprom.h>

#include "HardwareDef.h"
#include "touchscreen.h"
#include "display.h"
#include "calibrate.h"
#include "bitops.h"

#ifdef _TOUCH_SLIDE_
extern struct touchscreen_device touchscreen_slide_driver;
struct touchscreen_device *touchscreen = &touchscreen_slide_driver;
#endif
#ifdef _TOUCH_STEALTH_
extern struct touchscreen_device touchscreen_stealth;
struct touchscreen_device *touchscreen = &touchscreen_stealth;
#endif

#define SAMPLE_CNT_MAX  1
#define NO      0
#define YES 1
#define PRESSURE_TOUCHED        20

typedef enum
{
  RTI_TASK0,
  RTI_TASK1,
  RTI_TASK2,
  RTI_TASK3,
  RTI_TASK4,
  RTI_TASK5,
  RTI_TASK_MAX = 40,
} RTI_TASK;

RTI_TASK curr_task = RTI_TASK0;

volatile unsigned char touching = NO;
volatile unsigned char touched = NO;
volatile unsigned int x_loc = 0;
volatile unsigned int y_loc = 0;
volatile unsigned int z1_loc = 0;
volatile unsigned int z2_loc = 0;
volatile unsigned int x_loc_touched = 0;
volatile unsigned int y_loc_touched = 0;

unsigned int x_samples = 0;
unsigned int y_samples = 0;
unsigned int z1_samples = 0;
unsigned int z2_samples = 0;
unsigned char sample_count = 0;
float pressure = 0;

MATRIX matrix;

#define ee_isCalibrated 28
#define ee_matrix 0

#define PT1_X 32
#define PT1_Y 24
#define PT2_X 160
#define PT2_Y 216
#define PT3_X 288
#define PT3_Y 120

/**
 * Fires every 128 microseconds
 *
 * Handles the touchscreen
 * sampling algorithm tasks
 */
void
touchscreen_isr(void)
{
  /* RTI Tasks */
  if (curr_task == RTI_TASK0) {
    touchscreen->setup_x ();
    SETBIT(ADCSRA, ADSC);
    //start ADC conversion!
  } else if (curr_task == RTI_TASK1) {
    x_samples = touchscreen->process_x (ADC);
  } else if (curr_task == RTI_TASK2) {
    touchscreen->setup_y ();
    SETBIT(ADCSRA, ADSC);
    //start ADC conversion!
  } else if (curr_task == RTI_TASK3) {
    y_samples = touchscreen->process_y (ADC);
    touchscreen->setup_z1 ();
    SETBIT(ADCSRA, ADSC);
    //start ADC conversion!
  } else if (curr_task == RTI_TASK4) {
    z1_samples = touchscreen->process_z1 (ADC);
    touchscreen->setup_z2 ();
    SETBIT(ADCSRA, ADSC);
    //start ADC conversion!
  } else if (curr_task == RTI_TASK5) {
    z2_samples = touchscreen->process_z2 (ADC);
    touchscreen_service ();
  }

  /* Next task */
  curr_task++;
  if (curr_task == RTI_TASK_MAX) {
    /* Reset the Task Number */
    curr_task = RTI_TASK0;
  }
}

/**
 * this is a really random place to
 * put this function but we needed it for touch calibrate
 */
void
delay_ms(unsigned char x)
{
  unsigned char w, y, g;

  for (w = 0; w < x; w++) {
    for (y = 0; y < 13; y++) {
      for (g = 0; g < 255; g++) {
        asm("nop");
      }
    }
  }
}

//*******************************************************************************
void
touchscreen_service()
{
  unsigned int pressure;

  z1_loc = z1_samples;
  z2_loc = z2_samples;

  if (z1_loc != 0) {
    pressure = z2_loc / z1_loc;
  } else {
    pressure = PRESSURE_TOUCHED;
  }

  /* Test pressure */
  if (pressure < PRESSURE_TOUCHED) {
    touching = YES;

    x_loc = y_samples;
    y_loc = x_samples;

  } else {
    /* Check for previously touching */
    if (touching == YES) {
      x_loc_touched = x_loc;
      y_loc_touched = y_loc;
      touched = YES;
    }

    /* Reset touch data */
    touching = NO;
    x_loc = 0;
    y_loc = 0;
  }
}

/**
 * Check EEPROM for existing calibration data.
 * If none is found, the calibration screen is shown
 */
void
touchscreen_service_init(void)
{

  if (eeprom_read_byte (
      (unsigned char*) ee_isCalibrated) != TOUCHSCR_IS_CALIBRATED)
      {
    touch_calibrate ();
  } else {
    // read the previous saved calibration values in from memory
    eeprom_read_block (&matrix, (unsigned char*) ee_matrix, sizeof(MATRIX));
  }

  schedule (touchscreen_isr);
}

/****************************************************/
/*                                                                                                      */
/* Touch Calibrate                                                                      */
/*                                                                                                      */
/****************************************************/

/* calibrating the touch screen compensates for 4 major issues
 relating a touch location to a pixel location.
 1)      An offset in the x-aligenment
 2)      An offset in the y-aligenment
 3)      A rotational offset between the rows of pixels and the touch screen
 4)      Compensates the offset of each press location on the touch screen to a pixel location on the LCD screen
 */
//*******************************************************************************
char
touch_calibrate(void)
{
  COLOR blk =
    {
        0,
        0,
        0 };
  COLOR c1 =
    {
        255,
        0,
        0 }; //circle COLOR
  POINT p;

  //array of input points
  POINT32 screenSample[3];

  //array of expected correct answers
  POINT32 displaySample[3] =
    {
          {
              PT1_X,
              PT1_Y },
          {
              PT2_X,
              PT2_Y },
          {
              PT3_X,
              PT3_Y } };

  /* An array of perfect input screen points used to obtain a first pass  */
  /*      calibration matrix good enough to collect calibration samples.          */
  POINT32 perfectScreenSample[3] =
    {
          {
              100,
              100 },
          {
              900,
              500 },
          {
              500,
              900 } };

  /* An array of perfect display points used to obtain a first pass               */
  /*      calibration matrix good enough to collect calibration samples.          */
  POINT32 perfectDisplaySample[3] =
    {
          {
              100,
              100 },
          {
              900,
              500 },
          {
              500,
              900 } };

  //initial pass
  setCalibrationMatrix (&perfectDisplaySample[0], &perfectScreenSample[0],
      &matrix);

  dispPutS ("Touch circle to calibrate", 30, 5, c1, blk);

  touched = NO;
  dispColor (c1);
  dispRectangle (PT1_X, PT1_Y, 6, 6); //draw

  while (!touch_getTouched (&p)) {
    ;
  }

  screenSample[0].x = p.x;
  screenSample[0].y = p.y;

  dispColor (blk);
  dispRectangle (PT1_X, PT1_Y, 6, 6); //erase
  delay_ms (250);

  dispColor (c1);
  dispRectangle (PT2_X, PT2_Y, 6, 6); //draw

  while (!touch_getTouched (&p)) {
    ;
  }

  screenSample[1].x = p.x;
  screenSample[1].y = p.y;

  dispColor (blk);
  dispRectangle (PT2_X, PT2_Y, 6, 6); //erase
  delay_ms (250);

  dispColor (c1);
  dispRectangle (PT3_X, PT3_Y, 6, 6); //draw

  while (!touch_getTouched (&p)) {
    ;
  }

  screenSample[2].x = p.x;
  screenSample[2].y = p.y;

  dispColor (blk);
  dispRectangle (PT3_X, PT3_Y, 6, 6); //erase
  delay_ms (250);

  /* Clean up */
  dispColor (blk);
  dispClearScreen ();
  dispPutS ("Calibrating the", 15, 15, c1, blk);
  dispPutS ("Touch Screen", 20, 25, c1, blk);

  //now, the real calibration pass
  setCalibrationMatrix (&displaySample[0], &screenSample[0], &matrix);

  eeprom_write_byte ((unsigned char*) ee_isCalibrated, TOUCHSCR_IS_CALIBRATED);
  eeprom_write_block (&matrix, (unsigned char*) ee_matrix, sizeof(MATRIX));

  delay_ms (250);
  delay_ms (250);
  delay_ms (250);

  return 0;
}

/****************************************************/
/*                                                                                                      */
/* touch_getTouched                                                                     */
/*                                                                                                      */
/****************************************************/
char
touch_getTouched(POINT * p)
{

  if (touched) {
    POINT32 my_point, calibrated_point;
    my_point.x = (long) x_loc_touched;
    my_point.y = (long) y_loc_touched;

    /* Calibrate the coordinates */
    getDisplayPoint (&calibrated_point, &my_point, &matrix);

    /* Store the values */
    (p->x) = (unsigned int) calibrated_point.x;
    (p->y) = (unsigned int) calibrated_point.y;

    touched = FALSE;

    return 1;
  }

  return 0;

}

/****************************************************/
/*                                                                                                      */
/* touch_getCursor                                                                      */
/*                                                                                                      */
/****************************************************/
char
touch_getCursor(POINT * p)
{

  if (touching) {
    POINT32 my_point, calibrated_point;
    my_point.x = (long) x_loc;
    my_point.y = (long) y_loc;

    /* Calibrate the coordinates */
    getDisplayPoint (&calibrated_point, &my_point, &matrix);

    /* Store the values */
    (p->x) = (unsigned int) calibrated_point.x;
    (p->y) = (unsigned int) calibrated_point.y;

    return 1;
  }

  return 0;

} //end touch_get_cursor
