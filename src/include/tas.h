#ifndef TAS_H
#define TAS_H

#include "utils.h"
#include <stdlib.h>

typedef struct TasMove {
  u8 click_type;
  s32 x, y;
} TasMove;

TasMove* read_tas_file(const WCHAR* filename);

#endif // TAS_H