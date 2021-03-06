@echo off

where /q cl || (
  echo ERROR: could not find "cl" - run the "build.bat" from the MSVC x64 native tools command prompt.
  exit /b 1
)

set warnings=/WX /W4 /wd4201 /wd4100 /wd4189 /wd4505 /wd4701
set compilerFlags=/std:c++20 /MT /MP /O2 /Oi /EHsc /fp:fast /nologo /GS- /Gs999999 /GR- /FC /Z7  %warnings%
set linkerFlags=/OUT:main.exe /INCREMENTAL:NO /OPT:REF /CGTHREADS:6 /STACK:0x100000,0x100000 user32.lib gdi32.lib winmm.lib

IF NOT EXIST .\build mkdir .\build
pushd .\build
del *.pdb > NUL 2> NUL

cl.exe %compilerFlags% ../source/*.cpp /link %linkerFlags%

popd