// --- WARNING! ---
// THIS IS A GENERATED FILE; DO NOT EDIT MANUALLY!
// ----------------


// FILE: Headache.cpp:

// Structs:
struct Signature;                                                                                       // LINE: 30
struct Signature_Bucket;                                                                                // LINE: 37
struct Signature_Root;                                                                                  // LINE: 44
struct Parser;                                                                                          // LINE: 52
struct Globals;                                                                                         // LINE: 68

// Functions:
String Target_Directory(String path);                                                                   // LINE: 81
u64 Push_Signature(Signature_Root* root, String str, u64 line_count, Globals* globals);                 // LINE: 100
void Run_Parser(Globals* globals, Parser* parser);                                                      // LINE: 175
void Parse_File(Globals* globals, String path);                                                         // LINE: 363
void Output_Signature_List(Globals* globals, Signature_Root* root);                                     // LINE: 417
void Output_Results_To_Buffer(Globals* globals);                                                        // LINE: 458


// FILE: Azewin.h:

// Structs:
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
struct WDirectSoundCapabilities;                                                                        // LINE: 1669
struct WGUID;                                                                                           // LINE: 1681
struct WWaveFormatEx;                                                                                   // LINE: 1693
struct WDirectSoundBufferDescription;                                                                   // LINE: 1706
struct DirectSoundCapabilities;                                                                         // LINE: 1719
struct WDirectSound;                                                                                    // LINE: 1749
struct WCOMObjectDirectSound;                                                                           // LINE: 1778
struct WDirectSoundBuffer;                                                                              // LINE: 1785
struct WCOMObjectDirectSoundBuffer;                                                                     // LINE: 1834


// FILE: Basic.cpp:


// Functions:
s16 Abs(s16 v);                                                                                         // LINE: 9
s16 Max(s16 a, s16 b);                                                                                  // LINE: 16
u64 Max(u64 a, u64 b);                                                                                  // LINE: 23
u64 Square(u64 v);                                                                                      // LINE: 30
u64 Cube(u64 v);                                                                                        // LINE: 37
u64 Kilobytes(u64 v);                                                                                   // LINE: 44
u64 Megabytes(u64 v);                                                                                   // LINE: 51
u64 Gigabytes(u64 v);                                                                                   // LINE: 58
void Mem_Zero(void* ptr, u64 count);                                                                    // LINE: 65
void Mem_Copy(void* dest, void* src, u64 count);                                                        // LINE: 77


// FILE: String.cpp:

// Structs:
struct String;                                                                                          // LINE: 8
struct U64_To_String_Memory;                                                                            // LINE: 342

// Functions:
u64 Null_Terminated_Lenght(char* buffer);                                                               // LINE: 17
String To_String(char* cstr);                                                                           // LINE: 26
char Last(String str);                                                                                  // LINE: 34
char First(String str);                                                                                 // LINE: 43
bool Is_Capital_Letter(char c);                                                                         // LINE: 52
char To_Lowercase(char c);                                                                              // LINE: 59
bool Is_Whitespace(char c);                                                                             // LINE: 70
bool Seek(String str, char c, u64* output);                                                             // LINE: 94
u64 Line_Length(String str);                                                                            // LINE: 114
String Forward(String str, u64 step);                                                                   // LINE: 122
String Forward(String str, String step);                                                                // LINE: 131
bool Is_Integer(char c);                                                                                // LINE: 138
bool Is_Positive_Integer(String str);                                                                   // LINE: 145
String Skip_Zeroes(String str);                                                                         // LINE: 161
String Skip_While_True(String str, bool(*compare_fn)(char c));                                          // LINE: 170
String Skip_Whitespace(String str);                                                                     // LINE: 179
String Skip_Spaces_And_Tabs(String str);                                                                // LINE: 188
bool Match_Case_Insensitive(String str, String comp);                                                   // LINE: 197
bool Match_Case_Sensitive(String str, String comp);                                                     // LINE: 221
bool Match_Beginning_Case_Insensitive(String str, String comp);                                         // LINE: 246
bool Match_Beginning_Case_Sensitive(String str, String comp);                                           // LINE: 271
u64 To_U64(String str);                                                                                 // LINE: 320
String To_String(u64 integer, U64_To_String_Memory* output);                                            // LINE: 348
String Merge(String A, String B, Arena* arena);                                                         // LINE: 389


// FILE: Arena.cpp:

// Structs:
struct Arena;                                                                                           // LINE: 9

// Functions:
void Initialize_Arena(Arena* arena, u64 reserve_byte_count = Gigabytes(64), u64 pages_per_commit = 16); // LINE: 21
void Clear(Arena* arena);                                                                               // LINE: 37
void* Push(Arena* arena, u64 size);                                                                     // LINE: 43
char* Push_String(Arena* arena, String str);                                                            // LINE: 83
u64 Snapshot(Arena* arena);                                                                             // LINE: 92
void Restore(Arena* arena, u64 position);                                                               // LINE: 99


// FILE: Win32.cpp:


// Functions:
f64 OS_Time();                                                                                          // LINE: 18
bool OS_Write_File(String output, String path, Arena* arena);                                           // LINE: 102
