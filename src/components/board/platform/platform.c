#include <inttypes.h>

#include "HardwareDef.h"
#include "platform.h"

#ifdef _TOUCH_SLIDE_
        extern struct platform_board slide_board;
        struct platform_board *board = &slide_board;
#endif
#ifdef _TOUCH_STEALTH_
        platform_board *board = stealth_board;
#endif
