

// ===================================
// Copyright (c) 2025 by Valtteri Kois
// All rights reserved.
// ===================================


SIG s16 Abs(s16 v)
{
    s16 result = (v >= 0)? v : v * -1;
    return result;
}


SIG u64 Min(u64 a, u64 b)
{
    u64 result = (a < b)? a : b;
    return result;
}


SIG s16 Max(s16 a, s16 b)
{
    s16 result = (a > b)? a : b;
    return result;
}


SIG u64 Max(u64 a, u64 b)
{
    u64 result = (a > b)? a : b;
    return result;
}


SIG u64 Square(u64 v)
{
    u64 result = v * v;
    return result;
}


SIG u64 Cube(u64 v)
{
    u64 result = v * v * v;
    return result;
}


SIG u64 Kilobytes(u64 v)
{
    u64 result = v * 1024;
    return result;
}


SIG u64 Megabytes(u64 v)
{
    u64 result = v * Square(Kilobytes(1));
    return result;
}


SIG u64 Gigabytes(u64 v)
{
    u64 result = v * Cube(Kilobytes(1));
    return result;
}


SIG void Mem_Zero(void* ptr, u64 count)
{
    u8* begin = (u8*)ptr;
    u8* end = begin + count;
    
    for(u8* p = begin; p < end; ++p)
    {
        *p = 0;
    }
}


SIG void Mem_Copy(void* dest, void* src, u64 count)
{
    for(char* a = (char*)dest, *b = (char*)src; a < (char*)dest + count; ++a, ++b)
    {
        *a = *b;
    }
}