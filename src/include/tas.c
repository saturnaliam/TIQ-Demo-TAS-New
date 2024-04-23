#include "tas.h"
#define BUFFER_SIZE 100
#include <strsafe.h>

TasMove* read_tas_file(const WCHAR* filename) {
  DWORD bytes_read;
  WCHAR buffer[BUFFER_SIZE] = { 0 };

  HANDLE file_handle = CreateFileW(filename,
                            GENERIC_READ,
                            FILE_SHARE_READ,
                            NULL,
                            OPEN_EXISTING,
                            FILE_ATTRIBUTE_NORMAL,
                            NULL);

    if (file_handle == INVALID_HANDLE_VALUE) {
      error("Invalid file handle!");
      return NULL;
    }

    if (ReadFile(file_handle, buffer, (BUFFER_SIZE - 1), &bytes_read, NULL) == FALSE) {
      error("Couldn't read file!");
      CloseHandle(file_handle);
      return NULL;
    }

    if (bytes_read > 0) {
      buffer[bytes_read + 1] = '\0';

      wprintf(L"%d bytes read\n%s\n", bytes_read, buffer);
    } else {
      error("No data read from file.");
    }

    if (CloseHandle(file_handle) == 0) {
      error("Couldn't close file handle!");
    }

   return NULL;
}