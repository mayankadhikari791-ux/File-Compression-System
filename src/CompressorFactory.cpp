#include "CompressorFactory.hpp"
#include "Huffman.hpp"
#include "RLE.hpp"
#include <algorithm>
#include <cctype>

std::unique_ptr<ICompressor> CompressorFactory::create(AlgorithmType type) {
    switch (type) {
        case AlgorithmType::HUFFMAN:
            return std::unique_ptr<ICompressor>(new HuffmanCompressor());
        case AlgorithmType::RLE:
            return std::unique_ptr<ICompressor>(new RLECompressor());
        default:
            return nullptr;
    }
}

std::unique_ptr<ICompressor> CompressorFactory::createByName(const std::string& name) {
    std::string lower = name;
    std::transform(lower.begin(), lower.end(), lower.begin(),
                   [](unsigned char c){ return static_cast<char>(std::tolower(c)); });

    if (lower == "huffman" || lower == "huff" || lower == "h") {
        return create(AlgorithmType::HUFFMAN);
    } else if (lower == "rle" || lower == "runlength" || lower == "r") {
        return create(AlgorithmType::RLE);
    }

    return nullptr;
}

std::vector<std::string> CompressorFactory::getSupportedAlgorithms() {
    return {"huffman", "rle"};
}

