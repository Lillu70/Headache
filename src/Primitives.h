

// ===================================
// Copyright (c) 2025 by Valtteri Kois
// All rights reserved.
// ===================================

#pragma once

#define SIG // Used for generating header files.
 
#define _inline __forceinline
#define Array_Length(array) (sizeof(array) / sizeof(array[0]))

#define WELD(A, B) A##B
#define PASTE(name) name
#define PASTE_AS_CSTRING(name) #name
#define local_storage static
#define Assert(X) if(!(X)) *((s32*)0) = 666 


#if _WIN32
typedef unsigned __int8  u8;
typedef unsigned __int16 u16;
typedef unsigned __int32 u32;
typedef unsigned __int64 u64;
typedef __int8  s8;
typedef __int16 s16;
typedef __int32 s32;
typedef __int64 s64;
#else
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef signed char s8;
typedef signed short s16;
typedef signed long s32;
typedef signed long long s64;
#endif
typedef float f32;
typedef double f64;
typedef s32 b32;    

static_assert(sizeof(s8)  == sizeof(u8));
static_assert(sizeof(s16) == sizeof(u16));
static_assert(sizeof(s32) == sizeof(u32));
static_assert(sizeof(s64) == sizeof(u64));

static_assert(sizeof(s8)  == 8  / 8);
static_assert(sizeof(s16) == 16 / 8);
static_assert(sizeof(s32) == 32 / 8);
static_assert(sizeof(s64) == 64 / 8);
static_assert(sizeof(f32) == 32 / 8);
static_assert(sizeof(f64) == 64 / 8);

static_assert(sizeof(void*) == sizeof(u64), "This codebase assumes a 64 bit platform!");

constexpr f64 PI = 3.141592653589793;
constexpr f32 PI32 = f32(PI);
constexpr f32 HALF_PI32 = f32(PI / 2);
constexpr f64 TAU = 6.283185307179586;
constexpr f32 TAU32 = f32(TAU);
constexpr u32 DEG_FULL_CIRCLE = 360;
constexpr u32 DEG_HALF_CIRCLE = DEG_FULL_CIRCLE / 2;

constexpr u64 U64_MAX = 0xFFFFFFFFFFFFFFFF;
constexpr u32 U32_MAX = 0xFFFFFFFF;
constexpr u16 U16_MAX = 0xFFFF;
constexpr u8   U8_MAX = 0xFF;

constexpr s64 S64_MIN = s64(u64(u64(1) << 63));
constexpr s32 S32_MIN = s32(u32(u32(1) << 31));
constexpr s16 S16_MIN = s16(u16(u16(1) << 15));
constexpr s8  S8_MIN  =  s8( u8( u8(1) <<  7));

constexpr s64 S64_MAX = s64(U64_MAX & ~(u64(1) << 63));
constexpr s32 S32_MAX = s32(U32_MAX & ~(u32(1) << 31));
constexpr s16 S16_MAX = s16(U16_MAX & ~(u16(1) << 15));
constexpr s8   S8_MAX =  s8( U8_MAX & ~( u8(1) <<  7));


#if 0
constexpr f32 F32_MIN = 1.175494351e-38F;
constexpr f32 F32_MAX = 3.402823466e+38F;
constexpr f64 F64_MAX = 1.7976931348623157E+308;
#endif

#include "Azewin.h"


#include "Basic.cpp"
#include "String.cpp"
#include "Arena.cpp"
#include "OS.h"