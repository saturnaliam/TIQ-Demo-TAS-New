#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include "include/utils.h"

int main() {
  printf("[\x1b[38;5;5mT\x1b[0m] Open TAS tools\n[\x1b[38;5;11mR\x1b[0m] Run TAS\n[\x1b[38;5;9mQ\x1b[0m] Quit\n");

  while (1) {
    if (GetKeyState('Q') & 0x8000) break;
    if (GetKeyState('T') & 0x8000) UNIMPLEMENTED;
    if (GetKeyState('R') & 0x8000) UNIMPLEMENTED;
  }

  return 0;
}