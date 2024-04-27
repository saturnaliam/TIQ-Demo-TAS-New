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

  for (int i = 0; i < BUFFER_SIZE; i++) {
    // if we get a whitespace, push the last field into the move
    if ((isspace(file_contents[i]) != 0 || file_contents[i] == '\0') && buffer_index != 0) {
      switch (move_field) {
        case MOVE_X: {
          current_move.x = atoi(buffer);
          message("move x set: %d", current_move.x);
          break;
        }

        case MOVE_Y: {
          current_move.y = atoi(buffer);
          message("move y set: %d", current_move.y);
          break;
        }
        
        case MOVE_CLICK: {
          current_move.click_type = atoi(buffer);
          message("move click set: %d", current_move.click_type);

          movements = realloc(movements, sizeof(TasMove) * ++moves_length);
          break;
        }
      }

      move_field++;
      move_field %= MOVE_CLICK;

      ZeroMemory(buffer, sizeof(WCHAR) * 4);
      buffer_index = 0;

      if (file_contents[i] == '\0') break;
    } else {
      if (buffer_index >= 4) {
        error("More characters than allowed given! Maximum length for a field is 4!");
        printf("buffer: %s\nbuffer length: %d\n", buffer, buffer_index);
        return;
      }

      buffer[buffer_index++] = file_contents[i];
    }
  }

  Tas full_tas = { .moves = movements, .moves_length = moves_length };
  message("tas length: %d", moves_length);

  run_tas(full_tas);

  message("tas ran");
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

  message("moves length: %d", tas.moves_length);
  for (int i = 0; i < tas.moves_length; i++) {
    TasMove move = tas.moves[i];
    message("move x: %d", move.x);
    message("move y: %d", move.y);
    message("move click type: %d", move.click_type);

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