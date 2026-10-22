CXX = g++
CXXFLAGS = -std=c++14 -O2 -Wall -Iinclude

SRCS = src/BitStream.cpp \
       src/FileHeader.cpp \
       src/HuffmanTree.cpp \
       src/Huffman.cpp \
       src/RLE.cpp \
       src/CompressorFactory.cpp \
       src/Pipeline.cpp \
       src/main.cpp

TARGET = bin/fcs

all: $(TARGET)

$(TARGET): $(SRCS)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -rf bin

