#ifndef COMPRESSORFACTORY_HPP
#define COMPRESSORFACTORY_HPP

#include "ICompressor.hpp"
#include <memory>
#include <string>
#include <vector>

/**
 * @brief Factory class to create compressor instances polymorphically.
 * 
 * Owned by: Member 4 (RLE Algorithm & Extensibility Lead)
 */
class CompressorFactory {
public:
    /**
     * @brief Creates a compressor instance given an AlgorithmType enum.
     */
    static std::unique_ptr<ICompressor> create(AlgorithmType type);

    /**
     * @brief Creates a compressor instance given a case-insensitive string name ("huffman", "rle").
     */
    static std::unique_ptr<ICompressor> createByName(const std::string& name);

    /**
     * @brief Returns a list of supported algorithm names for CLI help and prompts.
     */
    static std::vector<std::string> getSupportedAlgorithms();
};

#endif // COMPRESSORFACTORY_HPP

