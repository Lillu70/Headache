

#ifndef _WIN32
#define _WIN32 0
#endif

#if _WIN32

u64 Win_Performance_Frequency()
{
    u64 result;
    QueryPerformanceFrequency(&result);
    return result;
}


SIG f64 OS_Time()
{
    static f64 freq = f64(Win_Performance_Frequency());
    u64 v;
    QueryPerformanceCounter(&v);
    f64 result = f64(v) / freq;
    return result;   
}


SIG String OS_Read_File(String path, Arena* arena)
{
    // TODO: Test with a file bigger than 4 Gigabytes.
    // Windows expects a null terminated buffer. Strings as they are slices; don't have that quarentee.
    // Therefore make a copy and inject zero at the end.
    
    Arena_Snapshot snapshot = Snapshot(arena);

    char* path_buffer = (char*)Push(arena, path.length + 1);
    Mem_Copy(path_buffer, path.ptr, path.length);
    path_buffer[path.length] = 0;

    String result = {};

    WHandle file_handle = CreateFileA
    (
        path_buffer, 
        GENERIC_READ, 
        0, 
        0, 
        OPEN_EXISTING, 
        FILE_ATTRIBUTE_NORMAL, 
        0
    );

    Restore(arena, snapshot);

    if(file_handle != INVALID_HANDLE_VALUE)
    {
        union File_Size
        {
            struct
            {
                u32 low;
                u32 high;
            };

            u64 size;
        };

        File_Size fz = {};
        fz.low = GetFileSize(file_handle, &fz.high);
        if(fz.low != INVALID_FILE_SIZE)
        {
            char* buffer = (char*)Push(arena, fz.size);
            result = {buffer, fz.size};
            
            while(fz.size)
            {
                u32 number_of_bytes_to_read = (fz.high)? U32_MAX : fz.low;
                u32 number_of_bytes_read = 0;

                b32 success = ReadFile
                (
                    file_handle,
                    buffer,
                    number_of_bytes_to_read,
                    &number_of_bytes_read,
                    0
                );

                fz.size -= number_of_bytes_read;
                buffer  += number_of_bytes_read;

            }
            
            CloseHandle(file_handle);
        }
    }

    return result;
}


SIG bool OS_Write_File(String buffer, String path, Arena* arena)
{
    // TODO: Test with a file bigger than 4 Gigabytes.
    // Windows expects a null terminated buffer. Strings as they are slices; don't have that quarentee.
    // Therefore make a copy and inject zero at the end.
    bool result = true;

    Arena_Snapshot snapshot = Snapshot(arena);

    char* path_buffer = (char*)Push(arena, path.length + 1);
    Mem_Copy(path_buffer, path.ptr, path.length);
    path_buffer[path.length] = 0;

    WHandle file_handle = CreateFileA
    (
        path_buffer, 
        GENERIC_WRITE, 
        0, 
        0, 
        CREATE_ALWAYS, 
        FILE_ATTRIBUTE_NORMAL, 
        0
    );

    Restore(arena, snapshot);

    if(file_handle != INVALID_HANDLE_VALUE)
    {
        u64 fz = buffer.length;
        char* buffer_ptr = buffer.ptr;

        while(fz)
        {
            u32 number_of_bytes_to_write = (fz > U32_MAX)? U32_MAX : u32(fz);
            u32 number_of_bytes_written = 0;

            b32 success = WriteFile
            (
                file_handle, 
                buffer_ptr, 
                number_of_bytes_to_write, 
                &number_of_bytes_written, 
                0
            );

            if(!success)
            {
                result = false;
                break;
            }

            fz -= number_of_bytes_written;
            buffer_ptr += number_of_bytes_written;
        }

        CloseHandle(file_handle);
    }

    return result;
}


SIG void* OS_Get_Executable_Base_Address()
{
    WModuleHandle module = GetModuleHandleA(0);
    return module;
}


#endif