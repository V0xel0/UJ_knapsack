#pragma once
#include <cinttypes>
using f32 = float;
using f64 = double;

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using s8 = int8_t;
using s16 = int16_t;
using s32 = int32_t;
using s64 = int64_t;

using b8 = s8;
using b16 = s16;
using b32 = s32;

using byte = u8;

#define KiB(Value) ((Value)*1024LL)
#define MiB(Value) (KiB(Value) * 1024LL)
#define GiB(Value) (MiB(Value) * 1024LL)
#define TiB(Value) (GiB(Value) * 1024LL)

#define internal static
#define local_persist static
#define global_variable static