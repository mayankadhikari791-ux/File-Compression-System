#include "Huffman.hpp"
#include <iostream>

HuffmanCompressor::HuffmanCompressor() {}

AlgorithmType HuffmanCompressor::getType() const {
    return AlgorithmType::HUFFMAN;
}

std::string HuffmanCompressor::getName() const {
    return "Huffman Coding";
}

bool HuffmanCompressor::compress(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) {
    output.clear();
    if (input.empty()) {
        return true;
    }

    // 1. Build frequency table (256 bytes)
    std::vector<uint64_t> frequencies(256, 0);
    for (uint8_t byte : input) {
        frequencies[byte]++;
    }

    // 2. Build Huffman tree
    HuffmanTree tree;
    tree.buildFromFrequencies(frequencies);

    // 3. Serialize tree to BitWriter
    BitWriter treeWriter;
    tree.serialize(treeWriter);
    treeWriter.flush();
    const auto& treeBytes = treeWriter.getBuffer();

    // 4. Generate code table
    auto codeTable = tree.generateCodeTable();

    // 5. Encode data bytes
    BitWriter dataWriter;
    for (uint8_t byte : input) {
        const std::string& code = codeTable[byte];
        dataWriter.writeBitString(code);
    }
    dataWriter.flush();
    const auto& dataBytes = dataWriter.getBuffer();

    // 6. Pack format: [4 bytes: treeBytes.size()][treeBytes][dataBytes]
    uint32_t treeSize = static_cast<uint32_t>(treeBytes.size());
    for (int i = 0; i < 4; ++i) {
        output.push_back(static_cast<uint8_t>((treeSize >> (i * 8)) & 0xFF));
    }
    output.insert(output.end(), treeBytes.begin(), treeBytes.end());
    output.insert(output.end(), dataBytes.begin(), dataBytes.end());

    return true;
}

bool HuffmanCompressor::decompress(const std::vector<uint8_t>& input,
                                   std::vector<uint8_t>& output,
                                   uint64_t originalSize) {
    output.clear();
    if (originalSize == 0) {
        return true;
    }

    if (input.size() < 4) {
        return false;
    }

    // 1. Read tree byte length (little-endian)
    uint32_t treeSize = 0;
    for (int i = 0; i < 4; ++i) {
        treeSize |= (static_cast<uint32_t>(input[i]) << (i * 8));
    }

    if (input.size() < 4 + treeSize) {
        return false;
    }

    // 2. Deserialize tree
    std::vector<uint8_t> treeBytes(input.begin() + 4, input.begin() + 4 + treeSize);
    BitReader treeReader(treeBytes);
    HuffmanTree tree;
    tree.deserialize(treeReader);

    auto root = tree.getRoot();
    if (!root) {
        return false;
    }

    // 3. Decode payload bits
    std::vector<uint8_t> dataBytes(input.begin() + 4 + treeSize, input.end());
    BitReader dataReader(dataBytes);

    output.reserve(static_cast<size_t>(originalSize));
    auto current = root;

    while (output.size() < originalSize && dataReader.hasMore()) {
        int bit = dataReader.readBit();
        if (bit == -1) break;

        current = (bit == 0) ? current->left : current->right;
        if (!current) {
            return false;
        }

        if (current->isLeaf()) {
            output.push_back(current->character);
            current = root;
        }
    }

    return (output.size() == originalSize);
}

