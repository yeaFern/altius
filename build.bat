@ECHO OFF

RMDIR /Q /S build
MKDIR build
PUSHD build

conan install ..
cmake .. -G "Visual Studio 16"
cmake --build . --config Debug
POPD

build\bin\altius.exe
