#include <inttypes.h>
#include "HardwareDef.h"
#include "display.h"

#ifdef _TOUCH_SLIDE_
  extern struct display_device hx8347_driver;
  extern struct display_device oled28_driver;

  struct display_device *display = &hx8347_driver;
#endif
#ifdef _TOUCH_STEALTH_
  extern struct display_device oled15_driver;

  struct display_device *display = &oled15_driver;
#endif

