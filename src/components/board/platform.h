#ifndef PLATFORM_H
#define PLATFORM_H

struct platform_board {
  void          (*init)(void);
  void          (*arduino_reset)(void);
  void          (*arduino_run)(void);
};

extern struct platform_board *board;

#endif
