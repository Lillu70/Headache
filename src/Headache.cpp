
#define Terminate(MSG) *((s32*)0) = 666 
#define MAX_PATH 260

#include "Primitives.h"
#include "Win32.cpp"

enum class State
{
    code,
    error,
    seek,
    macro,
    block_comment,
    disabled,
};


constexpr String marker                 = STR("SIG ");
constexpr String _struct                = STR("struct ");
constexpr String single_line_comment    = STR("//");
constexpr String disabled_start         = STR("#if 0");
constexpr String disabled_end           = STR("#endif");
constexpr String block_comment_start    = STR("/*");
constexpr String block_comment_end      = STR("*/");
constexpr String include_directive      = STR("#include ");
constexpr String output_name            = STR("HEADACHE_OUTPUT.h");


struct Signature
{
    String slice;
    u64 line;
};


struct Signature_Bucket
{
    Signature signatures[32];
    Signature_Bucket* next;
};


struct Signature_Root
{
    Signature_Bucket* root;
    Signature_Bucket* head;
    u64 head_count;
};


struct Parser
{
    String path;

    State state;
    String file;
    u64 line_count;
    char* last_new_line;

    Signature_Root functions;
    Signature_Root structs;

    Parser* next;
};


struct Globals
{
    String directory;
    
    Arena arena;

    u64 longest_signature_length;

    Parser* parsers_root;
    Parser* parsers_head;
};


SIG String Target_Directory(String path)
{
    u64 length = 0;
    u64 max = path.length;
    for(u64 i = max - 1; i < max; --i)
    {
        char c = path.ptr[i];
        if(c == '\\' || c == '/')
        {
            length = i + 1;
            break;
        }
    }

    String result = {path.ptr, length};
    return result;
}


SIG u64 Push_Signature(Signature_Root* root, String str, u64 line_count, Globals* globals)
{
    String copy = str;

    for(u64 i = 0; i < str.length; ++i)
    {
        char c = str.ptr[i];
        if(c == '\n')
        {
            while(i < str.length)
            {
                if(Is_Whitespace(str.ptr[i]))
                {
                    i -= 1;
                }
                else
                {
                    break;
                }
            }
            
            str.length = i + 1;
            break;
        }
    }
    

    str = Skip_Whitespace(str);
    if(str.length)
    {
        //TODO: Add validation... does this look like a function signature?
        bool valid = true;

        {
            String tmp = Forward(copy, str.length);
            tmp = Skip_Whitespace(tmp);
            valid = First(tmp) == '{';
        }
        

        if(valid)
        {
            if(!root->root)
            {
                root->root = Push_Struct(&globals->arena, Signature_Bucket);
                root->head = root->root;
                root->head_count = 0;
            }
            else if(root->head_count == Array_Length(root->root->signatures))
            {
                Signature_Bucket* bucket = Push_Struct(&globals->arena, Signature_Bucket);
                root->head->next = bucket;
                root->head = bucket;
                root->head_count = 0;
            }

            Assert(root->head);

            Signature* sig = root->head->signatures + root->head_count;
            *sig = {str, line_count};

            root->head_count += 1;
        }
    }

    globals->longest_signature_length = Max(globals->longest_signature_length, str.length);

    return str.length;
}


// FORWARDS FUCKING DECLARE!!!! THIS WHY I AM DOING THIS PROJECT!!!!
void Parse_File(Globals* globals, String path);


SIG void Run_Parser(Globals* globals, Parser* parser)
{
    for(u64 i = 0; i < parser->file.length; ++i)
    {
        char c = parser->file.ptr[i];
        String str = Forward(parser->file, i);
        
        switch(parser->state)
        {
            case State::code:
            {
                if(!Is_Whitespace(c))
                {
                    switch(c)
                    {
                        case block_comment_start.ptr[0]:
                        {
                            String A = Forward(parser->file, i + 1);
                            String B = Forward(block_comment_start, 1);

                            if(Match_Beginning_Case_Sensitive(A, B))
                            {
                                parser->state = State::block_comment;
                                i += B.length + 1;
                            }

                        }break;


                        case marker.ptr[0]:
                        {
                            String A = Forward(parser->file, i + 1);
                            String B = Forward(marker, 1);

                            if(Match_Beginning_Case_Sensitive(A, B))
                            {
                                parser->state = State::seek;
                                
                                i += B.length + 1;
                                u64 length = Push_Signature(&parser->functions, Forward(parser->file, i), parser->line_count, globals);
                            }

                        }break;

                        case _struct.ptr[0]:
                        {
                            String A = Forward(parser->file, i + 1);
                            String B = Forward(_struct, 1);

                            if(Match_Beginning_Case_Sensitive(A, B))
                            {
                                parser->state = State::seek;

                                u64 length = Push_Signature(&parser->structs, Forward(parser->file, i), parser->line_count, globals);
                            }

                        }break;


                        case '#':
                        {
                            String A = Forward(parser->file, i + 1);
                            String B = Forward(disabled_start, 1);
                            String C = Forward(include_directive, 1);

                            // Beginning of #if 0
                            if(Match_Beginning_Case_Sensitive(A, B))
                            {
                                parser->state = State::disabled;
                                i += B.length + 1;
                            }

                            // #include ...
                            else if(Match_Beginning_Case_Sensitive(A, C))
                            {
                                i += C.length + 1;
                                String include  = Forward(parser->file, i);
                                include         = Skip_Spaces_And_Tabs(include);
                                include.length  = Line_Length(include);
                                
                                if(include.length)
                                {
                                    switch(include.ptr[0])
                                    {
                                        case '<':
                                        {
                                            include = Forward(include, 1);

                                            u64 end;
                                            if(Seek(include, '>', &end))
                                            {
                                                include.length = end;
                                                Parse_File(globals, include);
                                            }
                                        }break;

                                        case '"':
                                        {
                                            include = Forward(include, 1);

                                            u64 end;
                                            if(Seek(include, '"', &end))
                                            {
                                                include.length = end;
                                                Parse_File(globals, include);
                                            }
                                        }break;
                                    }
                                }
                            }
                            
                            // #define
                            else
                            {
                                parser->state = State::macro;
                            }

                        }break;


                        default:
                        {
                            parser->state = State::seek;
                        }
                    }
                }

            }break;


            case State::block_comment:
            {
                String A = Forward(parser->file, i);
                String B = block_comment_end;

                if(Match_Beginning_Case_Sensitive(A, B))
                {
                    parser->state = State::seek;
                    i += B.length;
                }
            }break;


            case State::disabled:
            {
                String A = Forward(parser->file, i);
                String B = disabled_end;

                if(Match_Beginning_Case_Sensitive(A, B))
                {
                    parser->state = State::code;
                    i += B.length;
                }
            }break;


            case State::macro:
            {
                if(c == '\n')
                {
                    char* head = parser->file.ptr + i;
                    while(head-- > parser->last_new_line)
                    {
                        char c2 = *head;
                        if(!Is_Whitespace(c2))
                        {
                            parser->state = (c2 == '\\')? State::macro : State::code;
                            break;
                        }
                    }
                }
            }break;
        }


        if(c == '\n')
        {
            parser->line_count += 1;
            parser->last_new_line = parser->file.ptr + i;
            if(parser->state == State::seek)
            {
                parser->state = State::code;
            }
        }
    }
}


SIG void Parse_File(Globals* globals, String path)
{
    bool is_output = Match_Case_Sensitive(output_name, path);
    // NOTE: To avoid pasting the already generated forward decloration back in don't go into a file with the output name.
    // TODO: Actually look if a signature looks like a forward declaration, if it does, skip it!

    bool not_yet_parsed = true;
    {
        Parser* parser = globals->parsers_root;
        
        while(parser)
        {
            if(Match_Case_Sensitive(path, parser->path))
            {
                not_yet_parsed = false;
                break;
            }

            parser = parser->next;
        }
    }

    if(not_yet_parsed && !is_output)
    {
        String full_path = Merge(globals->directory, path, &globals->arena);

        String file = OS_Read_Entire_File(full_path, &globals->arena);
        if(file.length)
        {
            Parser* parser = Push_Struct(&globals->arena, Parser);
            
            if(!globals->parsers_root)
            {
                globals->parsers_root = parser;
                globals->parsers_head = parser;
            }
            else
            {
                globals->parsers_head->next = parser;
                globals->parsers_head = parser;
            }
            
            parser->path = path;
            parser->file = file;
            parser->state = State::code;
            parser->line_count = 1;
            parser->last_new_line = parser->file.ptr;

            Run_Parser(globals, parser);
        }
    }
}


SIG void Output_Signature_List(Globals* globals, Signature_Root* root)
{
    String postfix = STR(" // LINE: ");
    
    Signature_Bucket* bucket = root->root;
    while(bucket)
    {
        bool is_head = (bucket == root->head);
        u64 count = (is_head)? root->head_count : Array_Length(bucket->signatures);
        for(u64 i = 0; i < count; ++i)
        {
            Signature sig = bucket->signatures[i];
            U64_To_String_Memory string_memory;
            String line_number = To_String(sig.line, &string_memory);
            
            u64 extra = 2 + line_number.length + postfix.length;
            
            char* tmp = (char*)Push(&globals->arena, globals->longest_signature_length + extra);
            Mem_Copy(tmp, sig.slice.ptr, sig.slice.length);
            tmp[sig.slice.length] = ';';
            
            for(u64 j = sig.slice.length + 1; j <= globals->longest_signature_length; ++j)
            {
                tmp[j] = ' ';
            }

            char* w = tmp + globals->longest_signature_length + 1;
            
            Mem_Copy(w, postfix.ptr, postfix.length);
            Mem_Copy(w + postfix.length, line_number.ptr, line_number.length);
            w[postfix.length + line_number.length] = '\n';
            w[postfix.length + line_number.length + 1] = 0;
        }

        bucket = bucket->next;
    }
                
}


#include <stdio.h>
SIG void Output_Results_To_Buffer(Globals* globals)
{
    Arena* arena = &globals->arena;

    if(globals->parsers_root)
    {
        // Produce the output buffer.

        String title = STR("// --- WARNING! ---\n// THIS IS A GENERATED FILE; DO NOT EDIT MANUALLY!\n// ----------------\n");
        char* output_ptr = Push_String(arena, title);

        Parser* parser = globals->parsers_root;
        while(parser)
        {
            if(parser->state != State::error)
            {
                if(parser->structs.root || parser->functions.root)
                {
                    Push_String(arena, STR("\n\n// FILE: "));
                    Push_String(arena, parser->path);
                    Push_String(arena, STR(":\n\n"));
                    
                    if(parser->structs.root)
                    {
                        Push_String(arena, STR("// Structs:\n"));
                        Output_Signature_List(globals, &parser->structs);
                    }
                    
                    if(parser->functions.root)
                    {
                        Push_String(arena, STR("\n// Functions:\n"));
                        Output_Signature_List(globals, &parser->functions);
                    }
                }
            }

            parser = parser->next;
        }

        u64 output_length = (char*)Push(arena, 0) - output_ptr;
        String output = {output_ptr, output_length};
        String output_full_path = Merge(globals->directory, output_name, arena);
        OS_Write_File(output, output_full_path, arena);
    }
}


s32 main(s32 argc, char** argv)
{
    s32 result = 0;

    char* _path = "C:\\Dev\\Headache\\src\\Headache.cpp"; // TODO: Parse this off the argv
    if(argc == 2)
    {
        Globals globals = {};
        String full_path = To_String(_path);
        globals.directory = Target_Directory(full_path);
        String path = Forward(full_path, globals.directory.length);

        Initialize_Arena(&globals.arena);

        Parse_File(&globals, path);

        Output_Results_To_Buffer(&globals);
    }
    else
    {
        printf("Usage: Pass a path to the main source file as the only argument.\n");
        result = -1;
    }
    

    return result;
}