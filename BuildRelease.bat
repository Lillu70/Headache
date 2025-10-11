@echo off

if not exist "bin\" mkdir "bin"

cls
pushd bin

set common_linker_flags= /incremental:no /opt:ref User32.lib Gdi32.lib

set disapled_warnings= /wd4100 /wd4702 /wd4201 /wd4127 /wd4189

cl ../src/Headache.cpp /W4 /WX /O2 %disapled_warnings% /nologo /Zi /std:c++17 /link %common_linker_flags%
popd
