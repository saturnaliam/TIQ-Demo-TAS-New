#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include "include/utils.h"
#include "include/tas.h"

static const f64 SLEEP_TIME = 41.6; // how long to sleep between stat refreshes
static const u32 LEVEL_OFFSETS[8] = { 0xc95b64, 0x24, 0xa8c, 0x4, 0x2c, 0x50, 0x264, 0x4c };
BOOL debug;

void tas_info();

int main(int argc, char** argv) {
  if (argc != 2) {
    debug = FALSE;
  } else if (strcmp(argv[1], "-d") == 0) {
    debug = TRUE;
  }

  printf("\x1b[2J\x1b[H"); // clearing the screen

  TasMove move = { .click_type = LEFT_CLICK | LEFT_RELEASE, .x = 30000, .y = 30000 };
  TasMove moves[1];
  moves[0] = move;
  Tas tas = { .moves = moves, .moves_length = 1 };
  run_tas(tas);

 if (debug == TRUE) {
    printf("\x1b[3m\x1b[91mDEBUG MODE ENABLED\x1b[0m\n");
  }

  printf("=== LUCIA'S TIQ DEMO TASING TOOLS ===\n");
  printf("[\x1b[38;5;5mT\x1b[0m] Open TAS tools\n[\x1b[38;5;11mR\x1b[0m] Run TAS\n[\x1b[38;5;9mQ\x1b[0m] Quit\n");

  while (TRUE) {
    if (GetKeyState('Q') & 0x8000) break;
    if (GetKeyState('T') & 0x8000) tas_info();
    if (GetKeyState('R') & 0x8000) UNIMPLEMENTED;
  }

  return 0;
}

void tas_info() {
  set_console_cursor_visibility(FALSE);
  printf("\x1b[2J\x1b[H"); // clearing the screen

  // getting the pid and other required stuff
  DWORD PID;
  HWND window_handle = FindWindowW(NULL, L"Adobe Flash Player 32");
  GetWindowThreadProcessId(window_handle, &PID);
  HANDLE process_handle = OpenProcess(PROCESS_VM_READ, FALSE, PID);

  if (process_handle == NULL) {
    error("Process handle not found!");
    exit(1);
  } else if (debug == TRUE) {
    okay("Process handle found!");
    message("Process handle: %d", process_handle);
    message("Window handle: %d", window_handle);
    message("PID: %d", PID);
  }

  DWORD64 base_address = get_module_base(L"Adobe Flash Player 32", PID);

  if (base_address == 0) {
    error("Error while getting base address!");
    exit(1);
  } else if (debug == TRUE) {
    okay("Base address found!");
    message("Base address: %#x", base_address);
  }

  printf("\x1b[1m\x1b[38;5;139m=== MEMORY ===\x1b[0m\n");
  printf("Base Address: \x1b[93m%#x\x1b[0m\n", base_address);

  s8 array_length = length(LEVEL_OFFSETS);
  DWORD64 level_address = base_address;
  int scene;

  SIZE_T bytes_read; // i do not need to use this, but the function requires it anyways

  for (int i = 0; i < array_length; i++) {
    level_address += LEVEL_OFFSETS[i];

    if (i + 1 == array_length) {
      ReadProcessMemory(process_handle, (LPVOID)level_address, &scene, sizeof(scene), &bytes_read);
    } else {
      ReadProcessMemory(process_handle, (LPVOID)level_address, &level_address, sizeof(scene), &bytes_read);
    }

    if (debug == TRUE) { 
      message("Bytes read: %d", bytes_read);
    }

    printf("Pointer %d: \x1b[38;5;140m%#x\x1b[0m [\x1b[38;5;218m%#x\x1b[0m]\n", i + 1, level_address, LEVEL_OFFSETS[i]);
  }

  printf("\n=== GAME DATA ===\n");

  if (debug == TRUE) {
    message("BEGIN REFRESH SECTION. REFRESHED EVERY %fms", SLEEP_TIME);
  }

  printf("\x1b[s");

  while (1) {
    ReadProcessMemory(process_handle, (LPVOID)level_address, &scene, sizeof(scene), &bytes_read);
    reprint("Scene: \x1b[38;5;105m%d\x1b[0m", scene);

    RECT window_coordinates = get_window_coordinates(window_handle);
    reprint("Window Coordinates: \x1b[38;5;175m(%d, %d)\x1b[0m", window_coordinates.right, window_coordinates.bottom);
    
    POINT cursor_pos = get_relative_cursor_position(window_handle);
    reprint("Relative Cursor Position: \x1b[38;5;187m(%d, %d)\x1b[0m", cursor_pos.x, cursor_pos.y);

    if (debug == TRUE) {
      message("Window right: %d   ", window_coordinates.right);
      message("Window left: %d   ", window_coordinates.left);
      message("Window top: %d   ", window_coordinates.top);
      message("Window bottom: %d   ", window_coordinates.bottom);
    }

    printf("\x1b[u");

    Sleep(SLEEP_TIME);
  }
}