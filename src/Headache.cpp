
#include <stdio.h>

#include "LibPrimordial\Primitives.h"
#include "LibPrimordial\Win32.cpp"

constexpr String marker                 = STR("SIG ");
constexpr String def_marker             = STR("DEF(");
constexpr String keyword_struct         = STR("struct ");
constexpr String keyword_union          = STR("union ");
constexpr String keyword_enum           = STR("enum ");
constexpr String keyword_enum_class     = STR("enum class ");
constexpr String keyword_namespace      = STR("namespace ");
constexpr String single_line_comment    = STR("//");
constexpr String disabled_start         = STR("#if 0");
constexpr String disabled_end           = STR("#endif");
constexpr String block_comment_start    = STR("/*");
constexpr String block_comment_end      = STR("*/");
constexpr String include_directive      = STR("#include ");
constexpr String output_name            = STR("HEADACHE_OUTPUT.h");


#define SPACES_PER_INDENTATION 4

enum class State
{
    code,
    error,
    seek,
    macro,
    block_comment,
    disabled,
};


namespace Signature_Type
{
    enum T
    {   
        data_type,
        function,
        COUNT,
    };
}


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


struct Namespace_List
{
    Namespace_List* parallel;
    Namespace_List* deeper;
    Namespace_List* prev;

    String spacename;
    u64 block_count;
    u64 depth;

    //Signature_Root functions;
    //Signature_Root structs;
    
    Signature_Root signatures[Signature_Type::COUNT];
    u64 counts[Signature_Type::COUNT];
};


struct Parser
{
    String path;
    String directory;

    State state;
    String file;
    u64 line_count;
    u64 open_block_count;
    char* last_new_line;

    Namespace_List space;
    Namespace_List* headspace;

    Parser* next;
};


struct Globals
{
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


SIG u64 Push_Signature(Globals* globals, Parser* parser, String str, Signature_Type::T t)
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
    u64 effective_length = str.length;
    
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
            for(u64 i = 0; i < str.length; ++i)
            {
                String A = Forward(str, i);
                String B = def_marker;
                if(Match_Beginning_Case_Sensitive(A, B))
                {
                    i += B.length;
                    effective_length -= def_marker.length + 1 - 2;
                }
            }

            Signature_Root* root = parser->headspace->signatures + t;
                
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
            *sig = {str, parser->line_count};

            root->head_count += 1;

            effective_length += parser->headspace->depth * SPACES_PER_INDENTATION;

            Namespace_List* nl = parser->headspace;
            while(nl)
            {
                nl->counts[t] += 1;
                nl = nl->prev;
            }
        }
    }

    globals->longest_signature_length = Max(globals->longest_signature_length, effective_length);

    
    return str.length;
}

// FORWARDS FUCKING DECLARE!!!! THIS WHY I AM DOING THIS PROJECT!!!!
void Parse_File(Globals* globals, String directory, String path);


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
                if(c == '{')
                {
                    parser->open_block_count += 1;
                }
                else if (c == '}')
                {
                    Namespace_List* headspace = parser->headspace;
                    if(parser->open_block_count == headspace->block_count)
                    {
                        parser->headspace = headspace->prev;
                        if(!parser->headspace)
                        {
                            parser->state = State::error;
                            continue;
                        }
                    }
                    
                    parser->open_block_count -= 1;
                }

                u64 root_block_count = parser->headspace->block_count;

                if(!Is_Whitespace(c) && parser->open_block_count == root_block_count)
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

                        case keyword_namespace.ptr[0]:
                        {
                            String A = Forward(parser->file, i + 1);
                            String B = Forward(keyword_namespace, 1);

                            if(Match_Beginning_Case_Sensitive(A, B))
                            {
                                i += B.length;

                                A = Forward(A, B.length);
                                A = Skip_Whitespace(A);
                                String next_word = A;
                                if(Seek(next_word, Is_Whitespace, &next_word.length))
                                {
                                    // Check that the namespace is actually opened.
                                    A = Forward(A, next_word);
                                    A = Skip_Whitespace(A);
                                    if(First(A) == '{')
                                    {
                                        Namespace_List* nl = Push_Struct(&globals->arena, Namespace_List);
                                        Namespace_List* headspace = parser->headspace;
                                        
                                        if(headspace->deeper == 0)
                                        {
                                            headspace->deeper = nl;
                                        }
                                        else
                                        {
                                            Namespace_List* parallel_head = headspace->deeper;
                                            while(parallel_head->parallel)
                                            {
                                                parallel_head = parallel_head->parallel;
                                            }

                                            parallel_head->parallel = nl;

                                            Assert(parser->space.parallel == 0);
                                        }

                                        nl->prev = headspace;
                                        nl->block_count = parser->open_block_count + 1;
                                        nl->spacename = next_word;
                                        nl->depth = nl->prev->depth + 1;
                                        
                                        parser->headspace = nl;
                                    }

                                    else
                                    {
                                        // CONSIDER: Error?
                                    }
                                }
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
                                //u64 length = Push_Signature(&parser->functions, Forward(parser->file, i), parser->line_count, globals);
                                u64 length = Push_Signature(globals, parser, Forward(parser->file, i), Signature_Type::function);
                            }

                        }break;

                        
                        case keyword_union.ptr[0]:
                        case keyword_enum.ptr[0]:
                        case keyword_struct.ptr[0]:
                        {
                            String A = Forward(parser->file, i);
                            
                            bool match = 
                                Match_Beginning_Case_Sensitive(A, keyword_struct)       || 
                                Match_Beginning_Case_Sensitive(A, keyword_union)        ||
                                Match_Beginning_Case_Sensitive(A, keyword_enum_class)   ||
                                Match_Beginning_Case_Sensitive(A, keyword_enum);

                            if(match)
                            {
                                parser->state = State::seek;

                                //u64 length = Push_Signature(&parser->structs, Forward(parser->file, i), parser->line_count, globals);
                                u64 length = Push_Signature(globals, parser, Forward(parser->file, i), Signature_Type::data_type);
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
                                                Parse_File(globals, parser->directory, include);
                                            }
                                        }break;

                                        case '"':
                                        {
                                            include = Forward(include, 1);

                                            u64 end;
                                            if(Seek(include, '"', &end))
                                            {
                                                include.length = end;
                                                Parse_File(globals, parser->directory, include);
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


            case State::error:
            {
                // NOTE: This is effectively just a break statement, cpp just doesn't support double break.
                i = parser->file.length;
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


SIG void Parse_File(Globals* globals, String directory, String path)
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
        String full_path = Merge(directory, path, &globals->arena);

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
            
            parser->path = full_path;
            parser->file = file;
            parser->state = State::code;
            parser->line_count = 1;
            parser->last_new_line = parser->file.ptr;
            parser->directory = Target_Directory(full_path);
            parser->headspace = &parser->space;
            parser->space.spacename = STR("GLOBAL ROOT");

            Run_Parser(globals, parser);
        }
    }
}


SIG u64 Output_Indentation(Arena* arena, s64 depth)
{
    u64 result = 0;
    if(depth > 0)
    {
        result = u64(depth) * SPACES_PER_INDENTATION;
        for(s64 i = 0; i < depth; ++i)
        {
            for(u64 j = 0; j < SPACES_PER_INDENTATION; ++j)
            {
                Push_String(arena, STR(" "));
            }
        }
    }

    return result;
}


SIG void Output_Signature_List(Globals* globals, Signature_Root* root, u64 depth)
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
            
            u64 def_count = 0;
            for(u64 j = 0; j < sig.slice.length; ++j)
            {
                String A = Forward(sig.slice, j);
                String B = def_marker;
                if(Match_Beginning_Case_Sensitive(A, B))
                {
                    j += B.length;
                    def_count += 1;
                }
            }

            U64_To_String_Memory string_memory;
            String line_number = To_String(sig.line, &string_memory);
            u64 extra = 2 + line_number.length + postfix.length;

            u64 indent_characters = Output_Indentation(&globals->arena, depth);
            
            u64 desired_line_length = globals->longest_signature_length - indent_characters;
            u64 effective_slice_length = sig.slice.length - (def_count * (def_marker.length + 1)) + (def_count * 2);
            
            
            char* tmp = (char*)Push(&globals->arena, desired_line_length + extra);
            
            for(u64 dest = 0, src = 0; dest < effective_slice_length; )
            {
                String A = Forward(sig.slice, src);
                String B = def_marker;

                if(Match_Beginning_Case_Sensitive(A, B))
                {
                    tmp[dest++]  = '=';
                    tmp[dest++]  = ' ';
                    src += B.length;
                    u64 open_brace_count = 1;
                    while(open_brace_count > 0 && dest < effective_slice_length)
                    {
                        char c = sig.slice.ptr[src++];
                        if(c == '(')
                        {
                            open_brace_count += 1;
                        }
                        else if (c == ')')
                        {
                            open_brace_count -= 1;
                        }
                        
                        if(open_brace_count)
                        {
                            tmp[dest++] = c;
                        }
                    }
                }
                else
                {
                    tmp[dest++] = sig.slice.ptr[src++];
                }
            }

            tmp[effective_slice_length] = ';';
            
            for(u64 j = effective_slice_length + 1; j <= desired_line_length; ++j)
            {
                tmp[j] = ' ';
            }

            char* w = tmp + desired_line_length + 1;
            
            Mem_Copy(w, postfix.ptr, postfix.length);
            Mem_Copy(w + postfix.length, line_number.ptr, line_number.length);
            w[postfix.length + line_number.length] = '\n';
            w[postfix.length + line_number.length + 1] = 0;
        }

        bucket = bucket->next;
    }
                
}


SIG void Namespace_Walker(Globals* globals, Namespace_List* list, s64 depth, Signature_Type::T t)
{
    Arena* arena = &globals->arena;

    while(list)
    {

        if(depth && list->counts[t])
        {
            Output_Indentation(arena, depth - 1);
            Push_String(arena, keyword_namespace);
            Push_String(arena, list->spacename);
            Push_String(arena, STR("\n"));
            Output_Indentation(arena, depth - 1);
            Push_String(arena, STR("{\n"));
        }
        
        Output_Signature_List(globals, list->signatures + t, depth);

        if(list->deeper)
        {
            Namespace_Walker(globals, list->deeper, depth + 1, t);
        }

        if(depth && list->counts[t])
        {
            Output_Indentation(arena, depth - 1);
            Push_String(arena, STR("}\n\n"));
        }

        list = list->parallel;
    }
}


SIG void Output(Globals* globals, Arena* arena, Signature_Type::T t)
{
    Parser* parser = globals->parsers_root;

    while(parser)
    {
        if(parser->state != State::error && parser->space.counts[t])
        {       
            Push_String(arena, STR("\n// FILE: "));
            Push_String(arena, parser->path);
            Push_String(arena, STR(":\n"));
            
            Namespace_Walker(globals, &parser->space, 0, t);
        }
        

        parser = parser->next;
    }
}


SIG void Output_Results_To_Buffer(Globals* globals, String directory)
{
    Arena* arena = &globals->arena;

    if(globals->parsers_root)
    {
        // Produce the output buffer.

        String title = STR("// --- WARNING! ---\n// THIS IS A GENERATED FILE; DO NOT EDIT MANUALLY!\n// ----------------\n");
        char* output_ptr = Push_String(arena, title);

        Push_String(arena, STR("\n\n// Data types:\n"));
        Output(globals, arena, Signature_Type::data_type);
        
        Push_String(arena, STR("\n\n// Functions:\n"));
        Output(globals, arena, Signature_Type::function);

        #if 0
        
        printf(output_ptr);
        
        #else
        
        u64 output_length = (char*)Push(arena, 0) - output_ptr;
        String output = {output_ptr, output_length};
        String output_full_path = Merge(directory, output_name, arena);
        OS_Write_File(output, output_full_path, arena);

        #endif
    }
}


s32 main(s32 argc, char** argv)
{
    s32 result = 0;

    if(argc == 2)
    {
        Globals globals = {};
        String full_path    = To_String(argv[1]);
        String directory    = Target_Directory(full_path);
        String path         = Forward(full_path, directory.length);

        Initialize_Arena(&globals.arena, Gigabytes(64), 16);

        Parse_File(&globals, directory, path);

        Output_Results_To_Buffer(&globals, directory);
    }
    else
    {
        printf("Usage: Pass a path to the main source file as the only argument.\n");
        result = -1;
    }
    
    Terminate(X);
    return result;
}