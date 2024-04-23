#ifndef UTILS_H
#define UTILS_H

#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif  

#include <stdint.h>
#include <stdio.h>
#include <windows.h>
#include <tlhelp32.h>

static const char* check = "[\x1b[92m+\x1b[37m]";
static const char* minus = "[\x1b[91m-\x1b[37m]";
static const char* info = "[\x1b[94mi\x1b[37m]";

#define length(array) sizeof(array)/sizeof(array[0])
#define UNIMPLEMENTED printf("%s Unimplemented!\n", minus)
#define error(msg, ...) fprintf(stderr, minus); fprintf(stderr, " " msg "\n", ##__VA_ARGS__)
#define okay(msg, ...) printf(check); printf(" " msg "\n", ##__VA_ARGS__)
#define message(msg, ...) printf(info); printf(" " msg "\n", ##__VA_ARGS__)
#define reprint(msg, ...) printf("\r" msg "    \n", ##__VA_ARGS__)

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t   s8;
typedef int16_t  s16;
typedef int32_t  s32;
typedef int64_t  s64;
typedef double   f64;

DWORD get_module_base(const wchar_t* module_name, const DWORD PID);
void set_console_cursor_visibility(BOOL show);
RECT get_window_coordinates(const HWND window_handle);
POINT get_relative_cursor_position(const HWND window_handle);

#endif // UTILS_H