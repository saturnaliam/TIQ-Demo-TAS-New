#ifndef TAS_H
#define TAS_H

#include "utils.h"
#include <stdlib.h>
#include <stddef.h>

#define LEFT_CLICK 1
#define LEFT_RELEASE 2

typedef struct TasMove {
  u8 click_type;
  s32 x, y;
} TasMove;

typedef struct Tas {
  TasMove* moves;
  u32 moves_length;
} Tas;

/**
 * @brief starts and runs the tas
 * @param filename the filename
 */
void start_tas(const char* filename);

/**
 * @brief runs the tas
 * @param tas the full tas
 */
void run_tas(Tas tas);

#endif // TAS_H