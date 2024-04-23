#ifndef TAS_H
#define TAS_H

#include "utils.h"
#include <stdlib.h>

typedef struct TasMove {
  u8 click_type;
  s32 x, y;
} TasMove;

/**
 * @note the caller takes control of the memory and should free it
 * @brief reads a tas file and returns an array of the moves
 * @param filename the filename
 * @return an array of the moves
 */
TasMove* read_tas_file(const WCHAR* filename);

#endif // TAS_H