@echo off
echo ========================================================
echo   Building File Compression System (C++14 / MinGW GCC)
echo ========================================================

if not exist "bin" mkdir bin

g++ -std=c++14 -O2 -Iinclude ^
    src\BitStream.cpp ^
    src\FileHeader.cpp ^
    src\HuffmanTree.cpp ^
    src\Huffman.cpp ^
    src\RLE.cpp ^
    src\CompressorFactory.cpp ^
    src\Pipeline.cpp ^
    src\main.cpp ^
    -o bin\fcs.exe

if %ERRORLEVEL% EQU 0 (
    echo.
    echo [SUCCESS] Compilation completed successfully!
    echo Executable binary created at: bin\fcs.exe
    echo.
    echo Run "bin\fcs.exe -h" for CLI help.
) else (
    echo.
    echo [ERROR] Build failed! Check compiler output above.
)

