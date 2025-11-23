// --- WARNING! ---
// THIS IS A GENERATED FILE; DO NOT EDIT MANUALLY!
// ----------------


// Data types:

// FILE: C:/Dev/Headache/src/Headache.cpp:
enum class State;                                                                                               // LINE: 36
enum class Push_Type;                                                                                           // LINE: 61
struct Signature;                                                                                               // LINE: 68
struct Signature_Bucket;                                                                                        // LINE: 75
struct Signature_Root;                                                                                          // LINE: 82
struct Namespace_List;                                                                                          // LINE: 90
struct Parser;                                                                                                  // LINE: 105
struct Globals;                                                                                                 // LINE: 127
typedef void FUNCTION(int, int, int);;                                                                          // LINE: 168
typedef void FUNCTION(int, int, int);;                                                                          // LINE: 269
namespace Signature_Type
{
    enum T;                                                                                                     // LINE: 52
}


// FILE: C:/Dev/Headache/src/LibPrimordial\Azewin.h:
struct WSecurityAttributes;                                                                                     // LINE: 69
struct WSystemInfo;                                                                                             // LINE: 78
struct WRect;                                                                                                   // LINE: 102
struct WPoint;                                                                                                  // LINE: 112
struct WMessage;                                                                                                // LINE: 120
struct WWindowClass;                                                                                            // LINE: 133
struct WPixel_Format_Descriptor;                                                                                // LINE: 149
struct WMonitorInfo;                                                                                            // LINE: 180
struct WWindowPlacement;                                                                                        // LINE: 189
struct WRGBQuad;                                                                                                // LINE: 202
struct WBitMapInfoHeader;                                                                                       // LINE: 212
struct WBitmapInfo;                                                                                             // LINE: 229
struct WProcess_Memory_Counters_EX;                                                                             // LINE: 237
struct XInputGamepad;                                                                                           // LINE: 1574
struct XInputState;                                                                                             // LINE: 1588
enum WDSSCL;                                                                                                    // LINE: 1621
enum WDSBCAPS;                                                                                                  // LINE: 1630
enum WDSBPLAY;                                                                                                  // LINE: 1652
struct WDirectSoundCapabilities;                                                                                // LINE: 1669
struct WGUID;                                                                                                   // LINE: 1681
struct WWaveFormatEx;                                                                                           // LINE: 1693
struct WDirectSoundBufferDescription;                                                                           // LINE: 1706
struct DirectSoundCapabilities;                                                                                 // LINE: 1719
struct WDirectSound;                                                                                            // LINE: 1749
struct WCOMObjectDirectSound;                                                                                   // LINE: 1778
struct WDirectSoundBuffer;                                                                                      // LINE: 1785
struct WCOMObjectDirectSoundBuffer;                                                                             // LINE: 1834
enum WError : u32;                                                                                              // LINE: 1864
enum WPeekMessageRemove: u32;                                                                                   // LINE: 1872
enum WWindowMessage : s64;                                                                                      // LINE: 1880
enum WWindowStyle : u32;                                                                                        // LINE: 1913
enum WClassStyle : u32;                                                                                         // LINE: 1948
enum WProcessorArchitecture : u16;                                                                              // LINE: 1965
enum WIDC : u16;                                                                                                // LINE: 1977
enum WGL_Context : s32;                                                                                         // LINE: 2007
enum WMem;                                                                                                      // LINE: 2034
enum WPage;                                                                                                     // LINE: 2050
enum WVK;                                                                                                       // LINE: 2066
enum WGMEM;                                                                                                     // LINE: 2209
enum WCF;                                                                                                       // LINE: 2220
enum WFileAcces : u32;                                                                                          // LINE: 2260
enum WFileOpenMode;                                                                                             // LINE: 2269
enum WFileAttribute;                                                                                            // LINE: 2279
enum WPixel_Formats : u32;                                                                                      // LINE: 2306
enum WSWP;                                                                                                      // LINE: 2337
enum WGWL;                                                                                                      // LINE: 2353
enum WMONITOR;                                                                                                  // LINE: 2364
enum WSRC : u32;                                                                                                // LINE: 2372
enum WAccessRights;                                                                                             // LINE: 2393

// FILE: C:/Dev/Headache/src/LibPrimordial\String.cpp:
struct String;                                                                                                  // LINE: 8
struct U64_To_String_Memory;                                                                                    // LINE: 373

// FILE: C:/Dev/Headache/src/LibPrimordial\Arena.cpp:
struct Arena;                                                                                                   // LINE: 9
struct Arena_Snapshot;                                                                                          // LINE: 19


// Functions:

// FILE: C:/Dev/Headache/src/Headache.cpp:
SIG String Target_Directory(String path);                                                                       // LINE: 150
SIG u64 Push_Signature(Globals* globals, Parser* parser, String str, Signature_Type::T t, Push_Type push_type); // LINE: 170
SIG void Process_Mode_Code(u64* idx, char c, String str, Parser* parser, Globals* globals);                     // LINE: 276
SIG void Run_Parser(Globals* globals, Parser* parser);                                                          // LINE: 520
SIG void Parse_File(Globals* globals, String directory, String path);                                           // LINE: 672
SIG u64 Output_Indentation(Arena* arena, s64 depth);                                                            // LINE: 730
SIG void Output_Signature_List(Globals* globals, Signature_Root* root, u64 depth);                              // LINE: 749
SIG void Namespace_Walker(Globals* globals, Namespace_List* list, s64 depth, Signature_Type::T t);              // LINE: 843
SIG void Output(Globals* globals, Arena* arena, Signature_Type::T t);                                           // LINE: 878
SIG void Output_Results_To_Buffer(Globals* globals, String directory);                                          // LINE: 899

// FILE: C:/Dev/Headache/src/LibPrimordial\Basic.cpp:
SIG u64 Kilobytes(u64 v);                                                                                       // LINE: 11
SIG u64 Megabytes(u64 v);                                                                                       // LINE: 18
SIG u64 Gigabytes(u64 v);                                                                                       // LINE: 25
SIG void Mem_Zero(void* ptr, u64 count);                                                                        // LINE: 32
SIG void Mem_Copy(void* dest, void* src, u64 count);                                                            // LINE: 44
SIG u64 Weld(u32 a, u32 b);                                                                                     // LINE: 53

// FILE: C:/Dev/Headache/src/LibPrimordial\Basic_Generated.cpp:
SIG f64 Max(f64 a, f64 b);                                                                                      // LINE: 8
SIG f64 Min(f64 a, f64 b);                                                                                      // LINE: 15
SIG f64 Abs(f64 value);                                                                                         // LINE: 22
SIG f32 Max(f32 a, f32 b);                                                                                      // LINE: 29
SIG f32 Min(f32 a, f32 b);                                                                                      // LINE: 36
SIG f32 Abs(f32 value);                                                                                         // LINE: 43
SIG s64 Max(s64 a, s64 b);                                                                                      // LINE: 50
SIG s64 Min(s64 a, s64 b);                                                                                      // LINE: 57
SIG s64 Abs(s64 value);                                                                                         // LINE: 64
SIG s32 Max(s32 a, s32 b);                                                                                      // LINE: 71
SIG s32 Min(s32 a, s32 b);                                                                                      // LINE: 78
SIG s32 Abs(s32 value);                                                                                         // LINE: 85
SIG s16 Max(s16 a, s16 b);                                                                                      // LINE: 92
SIG s16 Min(s16 a, s16 b);                                                                                      // LINE: 99
SIG s16 Abs(s16 value);                                                                                         // LINE: 106
SIG s8 Max(s8 a, s8 b);                                                                                         // LINE: 113
SIG s8 Min(s8 a, s8 b);                                                                                         // LINE: 120
SIG s8 Abs(s8 value);                                                                                           // LINE: 127
SIG u64 Max(u64 a, u64 b);                                                                                      // LINE: 134
SIG u64 Min(u64 a, u64 b);                                                                                      // LINE: 141
SIG u32 Max(u32 a, u32 b);                                                                                      // LINE: 148
SIG u32 Min(u32 a, u32 b);                                                                                      // LINE: 155
SIG u16 Max(u16 a, u16 b);                                                                                      // LINE: 162
SIG u16 Min(u16 a, u16 b);                                                                                      // LINE: 169
SIG u8 Max(u8 a, u8 b);                                                                                         // LINE: 176
SIG u8 Min(u8 a, u8 b);                                                                                         // LINE: 183
SIG f64 Square(f64 v);                                                                                          // LINE: 190
SIG f64 Cube(f64 v);                                                                                            // LINE: 197
SIG f32 Square(f32 v);                                                                                          // LINE: 204
SIG f32 Cube(f32 v);                                                                                            // LINE: 211
SIG s64 Square(s64 v);                                                                                          // LINE: 218
SIG s64 Cube(s64 v);                                                                                            // LINE: 225
SIG s32 Square(s32 v);                                                                                          // LINE: 232
SIG s32 Cube(s32 v);                                                                                            // LINE: 239
SIG s16 Square(s16 v);                                                                                          // LINE: 246
SIG s16 Cube(s16 v);                                                                                            // LINE: 253
SIG s8 Square(s8 v);                                                                                            // LINE: 260
SIG s8 Cube(s8 v);                                                                                              // LINE: 267
SIG u64 Square(u64 v);                                                                                          // LINE: 274
SIG u64 Cube(u64 v);                                                                                            // LINE: 281
SIG u32 Square(u32 v);                                                                                          // LINE: 288
SIG u32 Cube(u32 v);                                                                                            // LINE: 295
SIG u16 Square(u16 v);                                                                                          // LINE: 302
SIG u16 Cube(u16 v);                                                                                            // LINE: 309
SIG u8 Square(u8 v);                                                                                            // LINE: 316
SIG u8 Cube(u8 v);                                                                                              // LINE: 323

// FILE: C:/Dev/Headache/src/LibPrimordial\String.cpp:
SIG u64 Null_Terminated_Lenght(char* buffer);                                                                   // LINE: 17
SIG String To_String(char* cstr);                                                                               // LINE: 26
SIG char Last(String str);                                                                                      // LINE: 34
SIG char First(String str);                                                                                     // LINE: 41
SIG char At(String str, u64 idx);                                                                               // LINE: 48
SIG bool Is_Capital_Letter(char c);                                                                             // LINE: 60
SIG char To_Lowercase(char c);                                                                                  // LINE: 67
SIG bool Is_Whitespace(char c);                                                                                 // LINE: 78
SIG bool Seek(String str, char c, u64* output);                                                                 // LINE: 102
SIG bool Seek(String str, bool(*check_fn)(char), u64* output);                                                  // LINE: 123
SIG u64 Line_Length(String str);                                                                                // LINE: 145
SIG String Forward(String str, u64 step);                                                                       // LINE: 153
SIG String Forward(String str, String step);                                                                    // LINE: 162
SIG bool Is_Integer(char c);                                                                                    // LINE: 169
SIG bool Is_Positive_Integer(String str);                                                                       // LINE: 176
SIG String Skip_Zeroes(String str);                                                                             // LINE: 192
SIG String Skip_While_True(String str, bool(*compare_fn)(char c));                                              // LINE: 201
SIG String Skip_Whitespace(String str);                                                                         // LINE: 210
SIG String Skip_Spaces_And_Tabs(String str);                                                                    // LINE: 219
SIG bool Match_Case_Insensitive(String str, String comp);                                                       // LINE: 228
SIG bool Match_Case_Sensitive(String str, String comp);                                                         // LINE: 252
SIG bool Match_Beginning_Case_Insensitive(String str, String comp);                                             // LINE: 277
SIG bool Match_Beginning_Case_Sensitive(String str, String comp);                                               // LINE: 302
SIG u64 To_U64(String str);                                                                                     // LINE: 351
SIG String To_String(u64 integer, U64_To_String_Memory* output);                                                // LINE: 379
SIG String Merge(String A, String B, Arena* arena);                                                             // LINE: 420

// FILE: C:/Dev/Headache/src/LibPrimordial\Arena.cpp:
SIG void Initialize_Arena(Arena* arena, u64 reserve_byte_count = Gigabytes(64), u64 pages_per_commit = 16);     // LINE: 29
SIG Arena Create_Arena(u64 reserve_byte_count = Gigabytes(64), u64 pages_per_commit = 16);                      // LINE: 47
SIG void Clear(Arena* arena);                                                                                   // LINE: 56
SIG void* Push(Arena* arena, u64 size);                                                                         // LINE: 63
SIG char* Push_String(Arena* arena, String str);                                                                // LINE: 106
SIG Arena_Snapshot Snapshot(Arena* arena);                                                                      // LINE: 115
SIG void Restore(Arena* arena, Arena_Snapshot snapshot);                                                        // LINE: 122
SIG bool Is_Taken(Arena_Snapshot snapshot);                                                                     // LINE: 128

// FILE: C:/Dev/Headache/src/LibPrimordial\Win32.cpp:
SIG f64 OS_Time();                                                                                              // LINE: 18
SIG String OS_Read_Entire_File(String path, Arena* arena);                                                      // LINE: 28
SIG bool OS_Write_File(String output, String path, Arena* arena);                                               // LINE: 102
SIG void* OS_Get_Executable_Base_Address();                                                                     // LINE: 164
