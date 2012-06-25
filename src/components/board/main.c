#include "wiring.h"
#include "platform.h"

//*******************************************************************************
int main()
{

  board->init();

  //sketchEarlyInits();

  runSketch();

  return 0; /* NOT REACHED */
}
