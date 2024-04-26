#ifndef TAS_H
#define TAS_H

#include "utils.h"
#include <stdlib.h>

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
 * @note the caller takes control of the memory and should free it
 * @brief reads a tas file and returns an array of the moves
 * @param filename the filename
 * @return an array of the moves
 */
Tas parse_tas_file(const WCHAR* filename);

/**
 * @brief runs the tas
 * @param tas the full tas
 */
void run_tas(Tas tas);

#endif // TAS_H