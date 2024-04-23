#include "utils.h"

/**
 * @brief returns the base address of a process (technically of the module, but for our use case its the process)
 * @param module_name the name of the process window
 * @param PID the process id of the process
 * @return the base address of the process
 */
DWORD get_module_base(const wchar_t* module_name, const DWORD PID) {
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
      return (DWORD)module_entry.modBaseAddr;
    }
  } while (Module32Next(snapshot, &module_entry));

  CloseHandle(snapshot);
  error("Couldn't find the specified module!");
  return 0;
}

/**
 * @brief shows / hides the console cursor
 * @param show whether the cursor should be hidden or shown
 */
void set_console_cursor_visibility(BOOL show) {
  HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

  CONSOLE_CURSOR_INFO cursor_info;

  GetConsoleCursorInfo(out, &cursor_info);
  cursor_info.bVisible = show;
  SetConsoleCursorInfo(out, &cursor_info);
}

/**
 * @brief windows fuckery to get the coordinates of the window
 * @param window_handle the windows handle
 * @return a RECT with the coordinates 
 */
RECT get_window_coordinates(const HWND window_handle) {
  RECT rect;
  ZeroMemory(&rect, sizeof(rect));
  GetWindowRect(window_handle, &rect);

  return rect;
}

/**
 * @brief windows fuckery to get the relative cursor position
 * @param window_handle the windows handle
 * @return a POINT with the coordinates
 */
POINT get_relative_cursor_position(const HWND window_handle) {
  POINT p;
  ZeroMemory(&p, sizeof(p));

  GetCursorPos(&p);
  ScreenToClient(window_handle, &p);

  return p;
}