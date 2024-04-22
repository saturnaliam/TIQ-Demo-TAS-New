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

#define UNIMPLEMENTED printf("%s Unimplemented!\n", minus)

static const char* check = "[\x1b[92m+\x1b[37m]";
static const char* minus = "[\x1b[91m-\x1b[37m]";

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

DWORD get_module_base(const wchar_t* module_name, const DWORD PID);

#endif // UTILS_H