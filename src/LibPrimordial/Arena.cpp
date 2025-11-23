

// ===================================
// Copyright (c) 2025 by Valtteri Kois
// All rights reserved.
// ===================================


struct Arena
{
    u8* memory;
    u64 used;
    u64 commited;
    u64 reserved;
    u64 pages_per_commit;
};


struct Arena_Snapshot
{
    u64 position;
};


enum class Zero_Memory : u8
{
    no,
    yes
};


#define Push_Struct(ARENA, TYPE) (TYPE*)Push(ARENA, sizeof(TYPE))
#define Push_Array(ARENA, TYPE, COUNT) (TYPE*)Push(ARENA, sizeof(TYPE) * (COUNT))


SIG void Initialize_Arena(Arena* arena, u64 reserve_byte_count DEF(Gigabytes(64)), u64 pages_per_commit DEF(16))
{
    Assert(reserve_byte_count && pages_per_commit);

    arena->pages_per_commit = pages_per_commit;
    arena->reserved = reserve_byte_count;
    arena->commited = Kilobytes(4) * arena->pages_per_commit;
    arena->used = 0;
    
    #if _WIN32
    arena->memory = (u8*)VirtualAlloc(0, arena->reserved, MEM_RESERVE, PAGE_READWRITE);
    VirtualAlloc(arena->memory, arena->commited, MEM_COMMIT, PAGE_READWRITE);
    #endif
    
    Assert(arena->memory);
}


SIG Arena Create_Arena(u64 reserve_byte_count DEF(Gigabytes(64)), u64 pages_per_commit DEF(16))
{
    Arena arena = {};
    Initialize_Arena(&arena, reserve_byte_count, pages_per_commit);

    return arena;
}


SIG void Clear(Arena* arena, Zero_Memory zero_memory)
{
    if(zero_memory == Zero_Memory::yes)
    {
        Mem_Zero(arena->memory, arena->used);
    }

    arena->used = 0;
}


SIG void* Push(Arena* arena, u64 size)
{
    Assert(arena->memory);
    Assert(arena->reserved);

    if(arena->used + size > arena->commited)
    {
        u64 page = Kilobytes(4);
        u64 extra = page * arena->pages_per_commit;
        
        if(extra < size)
        {
            extra = (size / page + 1) * page;
        }
        
        arena->commited += extra;
        
        if(arena->commited <= arena->reserved)
        {
            // CONSIDER: OS_Commit_Memory() call instead of the ifdef?
            #if _WIN32
            VirtualAlloc(arena->memory, arena->commited, MEM_COMMIT, PAGE_READWRITE);
            #endif
        }
        else
        {
            Terminate("Arena upper bound reached.");
        }
        
    }
    
    void* memory = arena->memory + arena->used;
    
    arena->used += size;
    
    Assert((u8*)memory);
    
    Mem_Zero(memory, size);
    
    return memory;
}


SIG char* Push_String(Arena* arena, String str)
{
    char* ptr = (char*)Push(arena, str.length);
    Mem_Copy(ptr, str.ptr, str.length);

    return ptr;
}


SIG Arena_Snapshot Snapshot(Arena* arena)
{
    Arena_Snapshot position = { arena->used };
    return position;
}


SIG void Restore(Arena* arena, Arena_Snapshot snapshot)
{
    Assert(arena->used >= snapshot.position);
    arena->used = snapshot.position;
}


SIG bool Is_Taken(Arena_Snapshot snapshot)
{
    bool result = snapshot.position > 0;
    return result;
}


SIG String To_String(Arena* arena)
{
    String str = {(char*)arena->memory, arena->used};
    return str;
}