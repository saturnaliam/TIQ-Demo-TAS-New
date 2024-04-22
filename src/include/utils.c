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