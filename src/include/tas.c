#include "tas.h"

#define BUFFER_SIZE 10240 // this is kinda arbitrary but i cant imagine a world where youd need more than 10kb of tas data ¯\_(ツ)_/¯
                        // just adding on for the person who will inevitably say "erm actually" each line needs ~12 bytes, so 10kb is nearly 900 moves in a game with like 30 questions

#define MOVE_X 0 // used to keep track of the last field in the move thing filled
#define MOVE_Y 1
#define MOVE_CLICK 2

/**
 * @brief dealing with reading the file and getting its data
 * @param filename the filename
 * @return the contents of the file
 */
char* read_file(const char* filename);

void start_tas(const char* filename) {
  char* file_contents = read_file(filename);

  TasMove* movements = NULL;

  TasMove current_move;
  char buffer[4]; // only a length of 4 bc idk any monitors with a 5 digit number of pixels
  int buffer_index = 0;
  int moves_length = 0;
  int move_field = MOVE_X;

  HWND window_handle = FindWindowW(NULL, L"Adobe Flash Player 32");
  POINT cursor; // neede for fuckery

  if (window_handle == NULL) {
    error("Failed getting window handle! (code %d)", GetLastError());
  }

  for (int i = 0; i < BUFFER_SIZE; i++) {
    // if we get a whitespace, push the last field into the move
    if ((isspace(file_contents[i]) != 0 || file_contents[i] == '\0') && buffer_index != 0) {
      switch (move_field) {
        case MOVE_X: {
          cursor.x = atoi(buffer);
          break;
        }

        case MOVE_Y: {
          cursor.y = atoi(buffer);
          break;
        }
        
        case MOVE_CLICK: {
          ClientToScreen(window_handle, &cursor);
          current_move.x = cursor.x;
          current_move.y = cursor.y;
          current_move.click_type = atoi(buffer);

          movements = realloc(movements, sizeof(TasMove) * ++moves_length);
          movements[moves_length - 1] = current_move;
          break;
        }
      }

      move_field++;
      move_field %= MOVE_CLICK + 1;

      ZeroMemory(buffer, sizeof(WCHAR) * 4);
      buffer_index = 0;

      if (file_contents[i] == '\0') break;
    } else {
      if (buffer_index >= 4) {
        error("More characters than allowed given! Maximum length for a field is 4!");
        return;
      }

      buffer[buffer_index++] = file_contents[i];
    }
  }

  Tas full_tas = { .moves = movements, .moves_length = moves_length };

  run_tas(full_tas);

  free(file_contents);
  free(movements);
}

char* read_file(const char* filename) {
  DWORD bytes_read;
  char* buffer = malloc(BUFFER_SIZE + 1);

  HANDLE file_handle = CreateFileA(filename,
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
      error("Could not move mouse (code %d)", GetLastError());
    }

    if (SendInput(inputs_length, inputs, sizeof(INPUT)) != inputs_length) {
      error("Could not send input (code %d)", GetLastError());
    }
  }
}