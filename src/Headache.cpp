
// TODO: Support for multi-line function declaritions.

#define PRINT_RESULTS 0

#include <stdio.h>

#include "LibPrimordial\Primitives.h"

#include "LibPrimordial\Basic.cpp"
#include "LibPrimordial\String.cpp"
#include "LibPrimordial\Arena.cpp"
#include "LibPrimordial\Win32.cpp"

constexpr String marker                 = STR("SIG ");
constexpr String def_marker             = STR("DEF(");
constexpr String keyword_struct         = STR("struct ");
constexpr String keyword_union          = STR("union ");
constexpr String keyword_enum           = STR("enum ");
constexpr String keyword_enum_class     = STR("enum class ");
constexpr String keyword_namespace      = STR("namespace ");
constexpr String line_comment_start     = STR("//");
constexpr String disabled_start         = STR("#if 0");
constexpr String disabled_end           = STR("#endif");
constexpr String pound_define           = STR("#define");
constexpr String block_comment_start    = STR("/*");
constexpr String block_comment_end      = STR("*/");
constexpr String include_directive      = STR("#include ");
constexpr String paste_directive        = STR("HEADACHE(");
constexpr String output_name            = STR("HEADACHE_OUTPUT.h");

#define PASTE_AS_STRING(X) STR(PASTE_AS_CSTRING(X))
#define SPACES_PER_INDENTATION 4


enum class State
{
    code,
    error,
    macro,
    string_literal,
    character_literal,
    line_comment,
    block_comment,
    seek_paste_directive_end,
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


enum class Push_Type
{
    line,
    raw,
};


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
    u64 open_brace_count;
    u64 paste_directive_start;
    char* last_new_line;

    Namespace_List space;
    Namespace_List* headspace;

    Parser* deeper_next;
    Parser* parallel_next;

    bool line_start;
};


struct Globals
{
    Arena arena;

    u64 longest_signature_length;
    Parser* parsers_root;
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


SIG u64 Push_Signature(Globals* globals, Parser* parser, String str, Signature_Type::T t, Push_Type push_type)
{
    String copy = str;

    if(push_type == Push_Type::line)
    {
        for(u64 i = 0; i < str.length; ++i)
        {
            char c = str.ptr[i];
            if(c == '\n')
            {
                while(i < str.length)
                {
                    c = str.ptr[i];
                    if(Is_Whitespace(c))
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
    }

    u64 effective_length = str.length;
    
    if(str.length)
    {
        //TODO: Add validation... does this look like a function signature?
        bool valid = true;

        if(push_type != Push_Type::raw)
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


void Parse_File(Globals* globals, Parser* src, String directory, String path);


SIG void Process_Mode_Code(u64* idx, char c, String str, Parser* parser, Globals* globals)
{
    if(!Is_Whitespace(c))
    {
        if(c == '{')
        {
            parser->open_block_count += 1;
        }
        else if (c == '}')
        {
            Namespace_List* headspace = parser->headspace;
            if(parser->open_block_count == headspace->block_count && !Match_Case_Sensitive(headspace->spacename, STR("GLOBAL ROOT")))
            {
                parser->headspace = headspace->prev;
                if(!parser->headspace)
                {
                    Assert(0);
                    parser->state = State::error;
                }
            }
            
            Assert(parser->open_block_count);
            parser->open_block_count -= 1;
        }

        u64 root_block_count = parser->headspace->block_count;

        switch(c)
        {
            case '\'':
            {
                parser->state = State::character_literal;
            }break;


            case '"':
            {
                parser->state = State::string_literal;
            }break;


            case block_comment_start.ptr[0]:
            {
                String A = Forward(parser->file, *idx + 1);
                String B = Forward(block_comment_start, 1);
                String C = Forward(line_comment_start, 1);

                if(Match_Beginning_Case_Sensitive(A, B))
                {
                    parser->state = State::block_comment;
                    *idx += B.length;
                }
                else if(Match_Beginning_Case_Sensitive(A, C))
                {
                    parser->state = State::line_comment;
                    *idx += C.length;
                }
            }break;


            case '#':
            {
                String A = Forward(parser->file, *idx + 1);
                String B = Forward(disabled_start, 1);
                String C = Forward(include_directive, 1);
                String D = Forward(pound_define, 1);

                // Beginning of #if 0
                if(Match_Beginning_Case_Sensitive(A, B))
                {
                    parser->state = State::disabled;
                    *idx += B.length;
                }


                // #include ...
                else if(Match_Beginning_Case_Sensitive(A, C))
                {
                    *idx += C.length;
                    String include  = Forward(parser->file, *idx);
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
                                    Parse_File(globals, parser, parser->directory, include);
                                }
                            }break;

                            case '"':
                            {
                                include = Forward(include, 1);

                                u64 end;
                                if(Seek(include, '"', &end))
                                {
                                    include.length = end;
                                    if(include.length)
                                    {
                                        Parse_File(globals, parser, parser->directory, include);
                                    }
                                }
                            }break;
                        }
                    }
                }
                // #define ...
                else if(Match_Beginning_Case_Sensitive(A, D))
                {
                    parser->state = State::macro;
                }

            }break;
        }

        if(parser->open_block_count == root_block_count && parser->line_start)
        {
            switch(c)
            {
                case keyword_namespace.ptr[0]:
                {
                    String A = Forward(parser->file, *idx + 1);
                    String B = Forward(keyword_namespace, 1);

                    if(Match_Beginning_Case_Sensitive(A, B))
                    {
                        *idx += B.length;

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
                    String A = Forward(parser->file, *idx + 1);
                    String B = Forward(marker, 1);

                    if(Match_Beginning_Case_Sensitive(A, B))
                    {
                        *idx += B.length + 1;
                        u64 length = Push_Signature(globals, parser, Forward(parser->file, *idx), Signature_Type::function, Push_Type::line);
                    }

                }break;

                case paste_directive.ptr[0]:
                {
                    String A = Forward(parser->file, *idx + 1);
                    String B = Forward(paste_directive, 1);

                    if(Match_Beginning_Case_Sensitive(A, B))
                    {
                        *idx += B.length;
                        parser->paste_directive_start = *idx + 1;
                        parser->state = State::seek_paste_directive_end;

                        if(parser->open_brace_count)
                        {
                            Assert(0);
                            parser->state = State::error;
                        }

                        parser->open_brace_count = 1;
                    }

                }break;

                case keyword_union.ptr[0]:
                case keyword_enum.ptr[0]:
                case keyword_struct.ptr[0]:
                {
                    String A = Forward(parser->file, *idx);
                    
                    bool match = 
                        Match_Beginning_Case_Sensitive(A, keyword_struct)       || 
                        Match_Beginning_Case_Sensitive(A, keyword_union)        ||
                        Match_Beginning_Case_Sensitive(A, keyword_enum_class)   ||
                        Match_Beginning_Case_Sensitive(A, keyword_enum);

                    if(match)
                    {
                        u64 length = Push_Signature(globals, parser, Forward(parser->file, *idx), Signature_Type::data_type, Push_Type::line);
                    }

                }break;
            }
        }
    }
}


SIG void Run_Parser(Globals* globals, Parser* parser)
{
    // NOTE: Just a comment to test out the program output when it self is fed into it self.
    // { should not count as an open clause!

    for(u64 i = 0; i < parser->file.length; ++i)
    {
        char c = parser->file.ptr[i];
        String str = Forward(parser->file, i);

        switch(parser->state)
        {
            case State::code:
            {
                Process_Mode_Code(&i, c, str, parser, globals);
            }break;


            case State::character_literal:
            {
                if(c == '\'' && ( At(parser->file, i - 1) != '\\' || At(parser->file, i - 2) == '\\'))
                {
                    parser->state = State::code;
                }
            }break;


            case State::string_literal:
            {
                if(c == '\"' && ( At(parser->file, i - 1) != '\\' || At(parser->file, i - 2) == '\\'))
                {
                    parser->state = State::code;
                }
            }break;


            case State::seek_paste_directive_end:
            {
                if(c == ')')
                {
                    parser->open_brace_count -= 1;
                    if(!parser->open_brace_count)
                    {
                        String paste_content = Forward(parser->file, parser->paste_directive_start);
                        paste_content.length = i - parser->paste_directive_start;
                        if(paste_content.length && paste_content.length < parser->file.length)
                        {
                            Push_Signature(globals, parser, paste_content, Signature_Type::data_type, Push_Type::raw);
                            parser->state = State::code;
                        }
                        else
                        {
                            Assert(0);
                            parser->state = State::error;
                        }
                    }
                }
                else if(c == '(')
                {
                    parser->open_brace_count += 1;
                }

            }break;

            case State::block_comment:
            {
                String A = Forward(parser->file, i);
                String B = block_comment_end;

                if(Match_Beginning_Case_Sensitive(A, B))
                {
                    parser->state = State::code;
                    i += B.length - 1;
                }
            }break;


            case State::disabled:
            {
                String A = Forward(parser->file, i);
                String B = disabled_end;

                if(Match_Beginning_Case_Sensitive(A, B))
                {
                    parser->state = State::code;
                    i += B.length - 1;
                }
            }break;


            case State::macro:
            {
                if(c == '\n')
                {
                    parser->state = State::code;
                    char* head = parser->file.ptr + i;
                    while(head-- > parser->last_new_line)
                    {
                        char c2 = *head;
                        if(!Is_Whitespace(c2))
                        {
                            if(c2 == '\\')
                            {
                                parser->state = State::macro;
                            }
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
            parser->line_start = true;
            parser->line_count += 1;

            parser->last_new_line = parser->file.ptr + i;

            if(parser->state == State::line_comment)
            {
                parser->state = State::code;
            }
        }
        else if(!Is_Whitespace(c))
        {
            parser->line_start = false;
        }
    }

    Assert(parser->state == State::code);

    if(parser->open_block_count)
    {
        Assert(0);
        parser->state = State::error;
    }
}


SIG bool Is_Already_Parsed(Parser* parser, String path)
{
    bool result = false;
    if(parser)
    {
        if(parser->deeper_next)
        {
            result = Is_Already_Parsed(parser->deeper_next, path);
        }
        
        if(!result)
        {
            while(parser)
            {
                if(Match_Case_Sensitive(path, parser->path))
                {
                    result = true;
                    break;
                }

                parser = parser->parallel_next;
            }
        }
    }

    return result;
}


SIG void Parse_File(Globals* globals, Parser* src, String directory, String path)
{
    if(Match_Case_Sensitive(path, STR("Basic_Generated.cpp")))
    {
        int a = 0;
    }

    bool is_output = Match_Case_Sensitive(output_name, path);

    if(!is_output && !Is_Already_Parsed(globals->parsers_root, path))
    {
        String full_path = Merge(directory, path, &globals->arena);

        String file = OS_Read_File(full_path, &globals->arena);
        if(file.length)
        {
            Parser* parser = Push_Struct(&globals->arena, Parser);
            
            // Initialize
            parser->file = file;
            parser->path = full_path;
            parser->state = State::code;
            parser->line_start = true;
            parser->line_count = 1;
            parser->last_new_line = parser->file.ptr;
            parser->directory = directory;
            parser->headspace = &parser->space;
            parser->space.spacename = STR("GLOBAL ROOT");

            // Link
            if(globals->parsers_root)
            {
                Assert(src);
                if(src->deeper_next)
                {
                    parser->directory = src->deeper_next->directory;

                    // Seek end of chain.
                    Parser* node = src->deeper_next;
                    while(node->parallel_next)
                    {
                        node = node->parallel_next;
                    }
                    
                    Assert(!node->parallel_next);
                    node->parallel_next = parser;
                }
                else
                {
                    src->deeper_next = parser;
                    String path_without_file_name = Target_Directory(path);
                    parser->directory = Merge(directory, path_without_file_name, &globals->arena);
                }
            }
            else
            {
                Assert(!src);
                globals->parsers_root = parser;
            }

            // Execute
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


SIG void Output(Globals* globals, Parser* parser, Signature_Type::T t)
{
    while(parser)
    {
        if(parser->deeper_next)
        {
            Output(globals, parser->deeper_next, t);
        }
        
        if(parser->state != State::error && parser->space.counts[t])
        {       
            Push_String(&globals->arena, STR("\n// FILE: "));
            Push_String(&globals->arena, parser->path);
            Push_String(&globals->arena, STR(":\n"));
            
            Namespace_Walker(globals, &parser->space, 0, t);
        }
        
        parser = parser->parallel_next;
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
        Output(globals, globals->parsers_root, Signature_Type::data_type);
        
        Push_String(arena, STR("\n\n// Functions:\n"));
        Output(globals, globals->parsers_root, Signature_Type::function);

        #if PRINT_RESULTS
        
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

        Parse_File(&globals, 0, directory, path);

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
