#include "utils.h"

DWORD64 get_module_base(const wchar_t* module_name, const DWORD PID) {
  // this function is basically just black magic
  MODULEENTRY32 module_entry = { 0 };
  HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, PID);

  if (!snapshot) {
    error("Error while getting the snapshot!");
    return 0;
  }

  module_entry.dwSize = sizeof(module_entry);

  // getting the first module in the process
  if (!Module32First(snapshot, &module_entry)) {
    error("Error while getting the first module!");
    return 0;
  }

  do {
    if (wcscmp(module_entry.szModule, module_name)) {
      CloseHandle(snapshot);
      return (DWORD64)module_entry.modBaseAddr;
    }
  } while (Module32Next(snapshot, &module_entry));

  CloseHandle(snapshot);
  error("Couldn't find the specified module!");
  return 0;
}

void set_console_cursor_visibility(BOOL show) {
  HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

  CONSOLE_CURSOR_INFO cursor_info;

  GetConsoleCursorInfo(out, &cursor_info);
  cursor_info.bVisible = show;
  SetConsoleCursorInfo(out, &cursor_info);
}

RECT get_window_coordinates(const HWND window_handle) {
  RECT rect;
  SecureZeroMemory(&rect, sizeof(rect));
  GetWindowRect(window_handle, &rect);

  return rect;
}

POINT get_relative_cursor_position(const HWND window_handle) {
  POINT p;
  SecureZeroMemory(&p, sizeof(p));

  GetCursorPos(&p);
  ScreenToClient(window_handle, &p);

  return p;
}