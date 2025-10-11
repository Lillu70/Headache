

// ===================================
// Copyright (c) 2025 by Valtteri Kois
// All rights reserved.
// ===================================

struct String
{
    char* ptr;
    u64 length;
};
#define STR(X) String{X, (sizeof(X) - 1)}


// The null terminator is inclusive. This is usefull for copying.
SIG u64 Null_Terminated_Lenght(char* buffer)
{
    u64 i;
    for(i = 0; *(buffer + i); ++i);
    return i + 1;
}


// The null terminator is not included the length. As per the String convension.
SIG String To_String(char* cstr)
{
    String result = {cstr};
    result.length = Null_Terminated_Lenght(cstr) - 1;
    return result;
}


SIG char Last(String str)
{
    Assert(str.length);

    char last = str.ptr[str.length - 1];
    return last;
}


SIG char First(String str)
{
    Assert(str.length);
    
    char first = str.ptr[0];
    return first;
}


SIG bool Is_Capital_Letter(char c)
{
    bool result = (c >= 'A' && c <= 'Z');
    return result;
}


SIG char To_Lowercase(char c)
{
    if(Is_Capital_Letter(c))
    {
        c += 'a' - 'A';
    }
    
    return c;
}


SIG bool Is_Whitespace(char c)
{
    bool result;
    
    switch(c)
    {
        case ' ' :
        case '\n':
        case '\r':
        case '\t':
        {
            result = true;
        }break;
        
        default:
        {
            result = false;
        }
    }
    
    return result;
}


SIG bool Seek(String str, char c, u64* output)
{
    bool result = false;
    for(u64 i = 0; i < str.length; ++i)
    {
        if(str.ptr[i] == c)
        {
            result = true;
            if(*output)
            {
                *output = i;
            }
            break;
        }
    }

    return result;
}


SIG u64 Line_Length(String str)
{
    u64 result = str.length;
    Seek(str, '\n', &result);
    return result;
}


SIG String Forward(String str, u64 step)
{
    Assert(step < str.length);
    
    String result = {str.ptr + step, str.length - step};
    return result;
}


SIG String Forward(String str, String step)
{
    String result = Forward(str, step.length);
    return result;
}


SIG bool Is_Integer(char c)
{
    bool result = c >= '0' && c <= '9';
    return result;
}


SIG bool Is_Positive_Integer(String str)
{
    bool result = str.length && str.ptr;
    
    char* begin = str.ptr;
    char* end = begin + str.length;
    
    for(char* c = begin; result && c < end; ++c)
    {
        result = Is_Integer(*c);
    }
    
    return result;
}


SIG String Skip_Zeroes(String str)
{
    u64 i;
    for(i = 0; i < str.length && (*(str.ptr + i)) == '0'; ++i);
    String result = {str.ptr + i, str.length - i};
    return result;
}


SIG String Skip_While_True(String str, bool(*compare_fn)(char c))
{
    u64 i;
    for(i = 0; i < str.length && compare_fn(str.ptr[i]); ++i);
    String result = {str.ptr + i, str.length - i};
    return result;
}


SIG String Skip_Whitespace(String str)
{
    u64 i;
    for(i = 0; i < str.length && Is_Whitespace(str.ptr[i]); ++i);
    String result = {str.ptr + i, str.length - i};
    return result;
}


SIG String Skip_Spaces_And_Tabs(String str)
{
    u64 i;
    for(i = 0; i < str.length && (str.ptr[i] == ' ' || str.ptr[i] == '\t'); ++i);
    String result = {str.ptr + i, str.length - i};
    return result;
}


SIG bool Match_Case_Insensitive(String str, String comp)
{
    bool result = true;
    
    if(str.length == comp.length)
    {
        for(u64 i = 0; i < comp.length; ++i)
        {
            if(To_Lowercase(str.ptr[i]) != To_Lowercase(comp.ptr[i]))
            {
                result = false;
                break;
            }
        }
    }
    else
    {
        result = false;
    }
    
    return result;
}


SIG bool Match_Case_Sensitive(String str, String comp)
{
    bool result = true;
    
    if(str.length == comp.length)
    {
        for(u64 i = 0; i < comp.length; ++i)
        {
            if(str.ptr[i] != comp.ptr[i])
            {
                result = false;
                break;
            }
        }
    }
    else
    {
        result = false;
    }
    
    return result;
}


// Return true if the "str" matches with "comp" up the length of "comp". "str" can differ after that.
SIG bool Match_Beginning_Case_Insensitive(String str, String comp)
{
    bool result = true;
    
    if(str.length >= comp.length)
    {
        for(u64 i = 0; i < comp.length; ++i)
        {
            if(To_Lowercase(str.ptr[i]) != To_Lowercase(comp.ptr[i]))
            {
                result = false;
                break;
            }
        }
    }
    else
    {
        result = false;
    }
    
    return result;
}


// Return true if the "str" matches with "comp" up the length of "comp". "str" can differ after that.
SIG bool Match_Beginning_Case_Sensitive(String str, String comp)
{
    bool result = true;
    
    if(str.length >= comp.length)
    {
        for(u64 i = 0; i < comp.length; ++i)
        {
            if(str.ptr[i] != comp.ptr[i])
            {
                result = false;
                break;
            }
        }
    }
    else
    {
        result = false;
    }
    
    return result;
}


// A string specific version so that this file doesn't need any dependecies.
u64 STRING_Pow64(u64 base, u64 exp)
{
    u64 result = 1;
    for(;;)
    {
        if (exp & 1)
        {
            result *= base;
        }
        
        exp >>= 1;
        
        if (!exp)
        {
            break;
        }
        
        base *= base;
    }
    
    return result;
}


SIG u64 To_U64(String str)
{
    u64 result = 0;
    
    char* begin = str.ptr;
    char* end = str.ptr + str.length - 1;
    
    for(u64 i = 0; i < str.length; ++i)
    {
        u64 v = u64(*(end - i) - '0');
        if(i)
        {
            u64 m = STRING_Pow64(10, i);
            v = v * m;
        }
        result += v;
    }
    
    return result;
}


struct U64_To_String_Memory
{
    // Maximum value of a 64 bit unsigned integer is: 18446744073709551615
    // That is 20 digits. Buffer size is 21 to have space for a null terminator.
    char b[21];
};
SIG String To_String(u64 integer, U64_To_String_Memory* output)
{
    u64 ascii_numeric_offset = 48;
    
    char* buffer = (char*)&output->b;
    Mem_Zero(buffer, Array_Length(buffer));

    buffer[Array_Length(output->b) - 1] = 0;
    u32 last_non_zero = Array_Length(output->b) - 2;
    
    for(u32 i = 0; i < Array_Length(output->b) - 1; ++i)
    {
        u64 digit = 0;
        if(i > 0)
        {
            digit = u64(integer / STRING_Pow64(10, i)) % 10;   
        }
        else
        {
            digit = integer % 10;
        }
        
        u32 write_pos = Array_Length(output->b) - 2 - i;
        
        if(digit)
        {
            last_non_zero = write_pos;
        }
        
        buffer[write_pos] = char(ascii_numeric_offset + digit);
    }
    
    String result = {buffer + last_non_zero};
    result.length = Array_Length(output->b) - 1 - last_non_zero;
    
    return result;
}

struct Arena;
void* Push(Arena* arena, u64 size);

SIG String Merge(String A, String B, Arena* arena)
{
    u64 combined_length = A.length + B.length;
    String result = {(char*)Push(arena, combined_length), combined_length};
    Mem_Copy(result.ptr, A.ptr, A.length);
    Mem_Copy(result.ptr + A.length, B.ptr, B.length);

    return result;
}