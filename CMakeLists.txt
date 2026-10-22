cmake_minimum_required(VERSION 3.10)
project(FileCompressionSystem VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include)

set(SOURCES
    src/BitStream.cpp
    src/FileHeader.cpp
    src/HuffmanTree.cpp
    src/Huffman.cpp
    src/RLE.cpp
    src/CompressorFactory.cpp
    src/Pipeline.cpp
    src/main.cpp
)

add_executable(fcs ${SOURCES})

