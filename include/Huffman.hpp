#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP

#include "ICompressor.hpp"
#include "HuffmanTree.hpp"
#include <vector>
#include <cstdint>

/**
 * @brief Huffman coding lossless compression engine.
 * 
 * Owned by: Member 3 (Huffman Coding Algorithm Specialist)
 */
class HuffmanCompressor : public ICompressor {
public:
    HuffmanCompressor();
    ~HuffmanCompressor() override = default;

    AlgorithmType getType() const override;
    std::string getName() const override;

    /**
     * @brief Encodes input bytes using Huffman coding.
     */
    bool compress(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) override;

    /**
     * @brief Decodes Huffman bitstream back to original bytes.
     */
    bool decompress(const std::vector<uint8_t>& input, std::vector<uint8_t>& output, uint64_t originalSize) override;
};

#endif // HUFFMAN_HPP

