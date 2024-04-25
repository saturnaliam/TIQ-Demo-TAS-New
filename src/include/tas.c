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

void run_tas(TasMove* moves) {
  const INPUT left_click = { .type = INPUT_MOUSE, .mi.dwFlags = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE };
  const INPUT left_release = { .type = INPUT_MOUSE, .mi.dwFlags = MOUSEEVENTF_LEFTUP | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE};

  for (int i = 0; i >= 0; i++) {
    TasMove move = moves[i];
    if(move.end == TRUE) { break; }
    message("click flag: %d", move.click_type & LEFT_CLICK);
    message("release flag: %d", move.click_type & LEFT_RELEASE);

    INPUT* inputs = NULL;
    int input_size = 0;

    if ((move.click_type & LEFT_CLICK) > 0) {
      message("left click");
      inputs = realloc(inputs, input_size++);

      inputs[input_size - 1] = left_click;
      inputs[input_size - 1].mi.dx = move.x;
      inputs[input_size - 1].mi.dy = move.y;
    }

    if ((move.click_type & LEFT_RELEASE) > 0) {
      message("left release");
      inputs = realloc(inputs, input_size++);

      inputs[input_size - 1] = left_release;
      inputs[input_size - 1].mi.dx = move.x;
      inputs[input_size - 1].mi.dy = move.y;
    }

    message("input size: %d", input_size);
    if (SendInput(input_size, inputs, sizeof(INPUT)) != input_size) {
      error("didnt send all inputs :( %x", GetLastError());
    }

    free(inputs);
  }
}