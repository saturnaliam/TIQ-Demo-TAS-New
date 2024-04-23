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

#define length(array) sizeof(array)/sizeof(array[0])
#define UNIMPLEMENTED printf("[\x1b[91m-\x1b[37m] Unimplemented!\n")
#define error(msg, ...) fprintf(stderr, "[\x1b[91m-\x1b[37m] " msg "\n", ##__VA_ARGS__)
#define okay(msg, ...) printf("[\x1b[92m+\x1b[37m] " msg "\n", ##__VA_ARGS__)
#define message(msg, ...) printf("[\x1b[94mi\x1b[37m] " msg "\n", ##__VA_ARGS__)
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

/**
 * @brief returns the base address of a process (technically of the module, but for our use case its the process)
 * @param module_name the name of the process window
 * @param PID the process id of the process
 * @return the base address of the process
 */
DWORD64 get_module_base(const wchar_t* module_name, const DWORD PID);

/**
 * @brief shows / hides the console cursor
 * @param show whether the cursor should be hidden or shown
 */
void set_console_cursor_visibility(BOOL show);

/**
 * @brief windows fuckery to get the coordinates of the window
 * @param window_handle the windows handle
 * @return a RECT with the coordinates 
 */
RECT get_window_coordinates(const HWND window_handle);

/**
 * @brief windows fuckery to get the relative cursor position
 * @param window_handle the windows handle
 * @return a POINT with the coordinates
 */
POINT get_relative_cursor_position(const HWND window_handle);

#endif // UTILS_H