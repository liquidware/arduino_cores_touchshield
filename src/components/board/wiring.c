#include <avr/io.h>
#include <avr/interrupt.h>
#include "platform.h"
#include "display.h"

#define MILLISECOND_CNT_MAX     7       //8*128uS = 1.024 mS
#define MAX_CALLBACKS           4

volatile unsigned char millisecond_cnt;
volatile unsigned long millis_var = 0;
volatile unsigned char millis_var_counter = 0;

volatile unsigned char mSecCnt = 0;
unsigned long mSec;

volatile unsigned char callback_count = 0;

void (*callbacks[MAX_CALLBACKS + 1])(void);

////////////////////////////////////////////////
//This is our 128uS time base from timer 0
//
////////////////////////////////////////////////
//*******************************************************************************
SIGNAL( SIG_OVERFLOW0)
{
  unsigned char i;

  /* Millisecond Counter */
  if (millis_var_counter == MILLISECOND_CNT_MAX) {
    millis_var++;
    millis_var_counter = 0;
  } else {
    millis_var_counter++;
  }

  if (millisecond_cnt < MILLISECOND_CNT_MAX) {
    millisecond_cnt++;
  }

  for (i = 0; i < callback_count; i++)
    (*callbacks[i]) ();

} //end interrupt

/**
 * Schedule a function to be run
 * every 128 microseconds
 */
void
schedule(void(*func)(void))
{
  cli ();

  if (callback_count >= MAX_CALLBACKS)
    return;

  callbacks[callback_count] = func;
  callback_count++;

  sei ();
}

void
unschedule(void(*func)(void))
{
  unsigned char i;
  unsigned char j;

  cli ();
  for (i = 0; i < callback_count; i++)
    if (callbacks[i] == func) {
      for (j = i; j < callback_count; j++)
        callbacks[j] = callbacks[j + 1];
      callback_count--;
      break;
    }

  sei ();
}

unsigned long
millis()
{
    if (display->screen_rev() == 3)
        return millis_var;
    else
        return millis_var * 2;
}

void
delay(unsigned long ms)
{

  while (ms--) {
    millisecond_cnt = 0; //reset the counter
    while (millisecond_cnt != MILLISECOND_CNT_MAX) {
      ;
    } //wait for 1 mS
  }

}

void arduinoRun()
{
  board->arduino_run();
}

void arduinoReset()
{
  board->arduino_reset();
}

void init()
{
  board->init();
}
