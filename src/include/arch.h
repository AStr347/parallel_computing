#ifndef ARCH_H
#define ARCH_H
#include <stdint.h>
#include <string.h>
#include "stdio.h"
#include "stdbool.h"
#include <stdlib.h>

#define DBG_LOG() printf("%d\n", __LINE__)

typedef bool b8;
typedef uint8_t u8;
typedef int8_t s8;
typedef uint16_t u16;
typedef int16_t s16;
typedef uint32_t u32;
typedef int32_t s32;
typedef uint64_t u64;
typedef int64_t s64;
typedef float f16;

#endif//ARCH_H