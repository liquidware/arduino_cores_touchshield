#include "platform.h"

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
