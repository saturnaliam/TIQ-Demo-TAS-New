#include "tas.h"
#include <strsafe.h>

#define BUFFER_SIZE 10240 // this is kinda arbitrary but i cant imagine a world where youd need more than 10kb of tas data ¯\_(ツ)_/¯
                        // just adding on for the person who will inevitably say "erm actually" each line needs ~12 bytes, so 10kb is nearly 900 moves in a game with like 30 questions

// teehee
#define CLICK_TYPE(click_input) \
  inputs = realloc(inputs, sizeof(INPUT) * (inputs_length + 1)); \
  inputs_length += 1; \
  inputs[inputs_length] = click_input; \
  inputs[inputs_length].mi.dx = move.x; \
  inputs[inputs_length].mi.dy = move.y; \

/**
 * @brief dealing with reading the file and getting its data
 * @param filename the filename
 * @return the contents of the file
 */
WCHAR* read_file(const WCHAR* filename);

Tas parse_tas_file(const WCHAR* filename) {
  WCHAR* file_contents = read_file(filename);

  TasMove* movements;
  

  free(file_contents);
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

void run_tas(Tas tas) {
  const INPUT left_click = { .type = INPUT_MOUSE, .mi.dwFlags = MOUSEEVENTF_LEFTDOWN };
  const INPUT left_release = { .type = INPUT_MOUSE, .mi.dwFlags = MOUSEEVENTF_LEFTUP };

  for (int i = 0; i < tas.moves_length; i++) {
    TasMove move = tas.moves[i];

    int inputs_length = 0;
    INPUT inputs[2];

    if ((move.click_type & LEFT_CLICK) > 0) {
      inputs[inputs_length] = left_click;
      inputs_length += 1;
    }

    if ((move.click_type & LEFT_RELEASE) > 0) {
      inputs[inputs_length] = left_release;
      inputs_length += 1;
    }

    if (SetCursorPos(move.x, move.y) == FALSE) {
      error("error while setting cursors position :(");
    }

    if (SendInput(inputs_length, inputs, sizeof(INPUT)) != inputs_length) {
      error("error while input send :(");
    }
  }
}