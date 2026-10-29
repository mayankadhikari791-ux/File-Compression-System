#ifndef RLE_HPP
#define RLE_HPP

#include "ICompressor.hpp"
#include <vector>
#include <cstdint>

/**
 * @brief PackBits-style Run-Length Encoding (RLE) Compressor.
 * 
 * Compresses repeating sequences efficiently while avoiding large overhead
 * on non-repeating binary or text data.
 * 
 * Owned by: Member 4 (RLE Algorithm & Extensibility Lead)
 */
class RLECompressor : public ICompressor {
public:
    RLECompressor();
    ~RLECompressor() override = default;

    AlgorithmType getType() const override;
    std::string getName() const override;

    bool compress(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) override;
    bool decompress(const std::vector<uint8_t>& input, std::vector<uint8_t>& output, uint64_t originalSize) override;
};

#endif // RLE_HPP

