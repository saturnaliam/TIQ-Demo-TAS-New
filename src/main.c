#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include "include/utils.h"

void tas_info();

int main() {
  printf("\x1b[2J\x1b[H"); // clearing the screen
  printf("[\x1b[38;5;5mT\x1b[0m] Open TAS tools\n[\x1b[38;5;11mR\x1b[0m] Run TAS\n[\x1b[38;5;9mQ\x1b[0m] Quit\n");

  while (1) {
    if (GetKeyState('Q') & 0x8000) break;
    if (GetKeyState('T') & 0x8000) tas_info();
    if (GetKeyState('R') & 0x8000) UNIMPLEMENTED;
  }

  return 0;
}

void tas_info() {
  printf("\x1b[2J\x1b[H"); // clearing the screen

  DWORD PID;
  HWND window_handle = FindWindowW(NULL, L"Adobe Flash Player 32");
  GetWindowThreadProcessId(window_handle, &PID);
  HANDLE process_handle = OpenProcess(PROCESS_VM_READ, FALSE, PID);

  if (process_handle == NULL) {
    error("Process handle not found!");
    exit(1);
  }

  DWORD base_address = get_module_base(L"Adobe Flash Player 32", PID);

  if (base_address == 0) {
    error("Error while getting base address!");
    exit(1);
  }

  printf("\x1b[1m\x1b[38;5;5m=== MEMORY ===\x1b[0m\n");
  printf("Base Address: %#x", base_address);
}