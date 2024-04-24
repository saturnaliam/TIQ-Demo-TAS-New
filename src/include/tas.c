#include "tas.h"
#include <strsafe.h>

#define BUFFER_SIZE 10240 // this is kinda arbitrary but i cant imagine a world where youd need more than 10kb of tas data ¯\_(ツ)_/¯
                        // just adding on for the person who will inevitably say "erm actually" each line needs ~12 bytes, so 10kb is nearly 900 moves in a game with like 30 questions

/**
 * @brief dealing with reading the file and getting its data
 * @param filename the filename
 * @return the contents of the file
 */
WCHAR* read_file(const WCHAR* filename);

TasMove* parse_tas_file(const WCHAR* filename) {
  WCHAR* file_contents = read_file(filename);

  TasMove* movements;
  

  free(file_contents);
  return NULL;
}

WCHAR* read_file(const WCHAR* filename) {
  DWORD bytes_read;
  WCHAR* buffer = malloc(BUFFER_SIZE + 1);

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
  } else {
    error("No data read from file.");
    return NULL;
  }

  if (CloseHandle(file_handle) == 0) {
    error("Couldn't close file handle!");
  }

  return buffer;
}