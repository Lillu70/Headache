// --- WARNING! ---
// THIS IS A GENERATED FILE; DO NOT EDIT MANUALLY!
// ----------------


// Data types:

// FILE: src\Headache.cpp:
enum class State;                                                                                       // LINE: 52
struct Signature;                                                                                       // LINE: 74
struct Signature_Bucket;                                                                                // LINE: 81
struct Signature_Root;                                                                                  // LINE: 88
struct Namespace_List;                                                                                  // LINE: 96
struct Parser;                                                                                          // LINE: 114
struct Globals;                                                                                         // LINE: 132
namespace Signature_Type
{
    enum T;                                                                                             // LINE: 65
}


// FILE: src\LibPrimordial\Azewin.h:
struct WSecurityAttributes;                                                                             // LINE: 69
struct WSystemInfo;                                                                                     // LINE: 78
struct WRect;                                                                                           // LINE: 102
struct WPoint;                                                                                          // LINE: 112
struct WMessage;                                                                                        // LINE: 120
struct WWindowClass;                                                                                    // LINE: 133
struct WPixel_Format_Descriptor;                                                                        // LINE: 149
struct WMonitorInfo;                                                                                    // LINE: 180
struct WWindowPlacement;                                                                                // LINE: 189
struct WRGBQuad;                                                                                        // LINE: 202
struct WBitMapInfoHeader;                                                                               // LINE: 212
struct WBitmapInfo;                                                                                     // LINE: 229
struct WProcess_Memory_Counters_EX;                                                                     // LINE: 237
struct XInputGamepad;                                                                                   // LINE: 1574
struct XInputState;                                                                                     // LINE: 1588
enum WDSSCL;                                                                                            // LINE: 1621
enum WDSBCAPS;                                                                                          // LINE: 1630
enum WDSBPLAY;                                                                                          // LINE: 1652
struct WDirectSoundCapabilities;                                                                        // LINE: 1669
struct WGUID;                                                                                           // LINE: 1681
struct WWaveFormatEx;                                                                                   // LINE: 1693
struct WDirectSoundBufferDescription;                                                                   // LINE: 1706
struct DirectSoundCapabilities;                                                                         // LINE: 1719
struct WDirectSound;                                                                                    // LINE: 1749
struct WCOMObjectDirectSound;                                                                           // LINE: 1778
struct WDirectSoundBuffer;                                                                              // LINE: 1785
struct WCOMObjectDirectSoundBuffer;                                                                     // LINE: 1834
enum WError : u32;                                                                                      // LINE: 1864
enum WPeekMessageRemove: u32;                                                                           // LINE: 1872
enum WWindowMessage : s64;                                                                              // LINE: 1880
enum WWindowStyle : u32;                                                                                // LINE: 1913
enum WClassStyle : u32;                                                                                 // LINE: 1948
enum WProcessorArchitecture : u16;                                                                      // LINE: 1965
enum WIDC : u16;                                                                                        // LINE: 1977
enum WGL_Context : s32;                                                                                 // LINE: 2007
enum WMem;                                                                                              // LINE: 2034
enum WPage;                                                                                             // LINE: 2050
enum WVK;                                                                                               // LINE: 2066
enum WGMEM;                                                                                             // LINE: 2209
enum WCF;                                                                                               // LINE: 2220
enum WFileAcces : u32;                                                                                  // LINE: 2260
enum WFileOpenMode;                                                                                     // LINE: 2269
enum WFileAttribute;                                                                                    // LINE: 2279
enum WPixel_Formats : u32;                                                                              // LINE: 2306
enum WSWP;                                                                                              // LINE: 2337
enum WGWL;                                                                                              // LINE: 2353
enum WMONITOR;                                                                                          // LINE: 2364
enum WSRC : u32;                                                                                        // LINE: 2372
enum WAccessRights;                                                                                     // LINE: 2393

// FILE: src\LibPrimordial\String.cpp:
struct String;                                                                                          // LINE: 8
struct U64_To_String_Memory;                                                                            // LINE: 361

// FILE: src\LibPrimordial\Arena.cpp:
struct Arena;                                                                                           // LINE: 9


// Functions:

// FILE: src\Headache.cpp:
String Target_Directory(String path);                                                                   // LINE: 143
u64 Push_Signature(Globals* globals, Parser* parser, String str, Signature_Type::T t);                  // LINE: 162

// FILE: src\LibPrimordial\Basic.cpp:
s16 Abs(s16 v);                                                                                         // LINE: 9
u64 Min(u64 a, u64 b);                                                                                  // LINE: 16
s16 Max(s16 a, s16 b);                                                                                  // LINE: 23
u64 Max(u64 a, u64 b);                                                                                  // LINE: 30
u64 Square(u64 v);                                                                                      // LINE: 37
u64 Cube(u64 v);                                                                                        // LINE: 44
u64 Kilobytes(u64 v);                                                                                   // LINE: 51
u64 Megabytes(u64 v);                                                                                   // LINE: 58
u64 Gigabytes(u64 v);                                                                                   // LINE: 65
void Mem_Zero(void* ptr, u64 count);                                                                    // LINE: 72
void Mem_Copy(void* dest, void* src, u64 count);                                                        // LINE: 84

// FILE: src\LibPrimordial\String.cpp:
u64 Null_Terminated_Lenght(char* buffer);                                                               // LINE: 17
String To_String(char* cstr);                                                                           // LINE: 26
char Last(String str);                                                                                  // LINE: 34
char First(String str);                                                                                 // LINE: 41
bool Is_Capital_Letter(char c);                                                                         // LINE: 48
char To_Lowercase(char c);                                                                              // LINE: 55
bool Is_Whitespace(char c);                                                                             // LINE: 66
bool Seek(String str, char c, u64* output);                                                             // LINE: 90
bool Seek(String str, bool(*check_fn)(char), u64* output);                                              // LINE: 111
u64 Line_Length(String str);                                                                            // LINE: 133
String Forward(String str, u64 step);                                                                   // LINE: 141
String Forward(String str, String step);                                                                // LINE: 150
bool Is_Integer(char c);                                                                                // LINE: 157
bool Is_Positive_Integer(String str);                                                                   // LINE: 164
String Skip_Zeroes(String str);                                                                         // LINE: 180
String Skip_While_True(String str, bool(*compare_fn)(char c));                                          // LINE: 189
String Skip_Whitespace(String str);                                                                     // LINE: 198
String Skip_Spaces_And_Tabs(String str);                                                                // LINE: 207
bool Match_Case_Insensitive(String str, String comp);                                                   // LINE: 216
bool Match_Case_Sensitive(String str, String comp);                                                     // LINE: 240
bool Match_Beginning_Case_Insensitive(String str, String comp);                                         // LINE: 265
bool Match_Beginning_Case_Sensitive(String str, String comp);                                           // LINE: 290
u64 To_U64(String str);                                                                                 // LINE: 339
String To_String(u64 integer, U64_To_String_Memory* output);                                            // LINE: 367
String Merge(String A, String B, Arena* arena);                                                         // LINE: 408

// FILE: src\LibPrimordial\Arena.cpp:
void Initialize_Arena(Arena* arena, u64 reserve_byte_count = Gigabytes(64), u64 pages_per_commit = 16); // LINE: 21
void Clear(Arena* arena);                                                                               // LINE: 37
void* Push(Arena* arena, u64 size);                                                                     // LINE: 43
char* Push_String(Arena* arena, String str);                                                            // LINE: 83
u64 Snapshot(Arena* arena);                                                                             // LINE: 92
void Restore(Arena* arena, u64 position);                                                               // LINE: 99

// FILE: src\LibPrimordial\Win32.cpp:
f64 OS_Time();                                                                                          // LINE: 18
String OS_Read_Entire_File(String path, Arena* arena);                                                  // LINE: 28
bool OS_Write_File(String output, String path, Arena* arena);                                           // LINE: 102
