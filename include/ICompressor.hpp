#ifndef ICOMPRESSOR_HPP
#define ICOMPRESSOR_HPP

#include <string>
#include <vector>
#include <cstdint>

/**
 * @brief Identifies the compression algorithm type used for a file.
 */
enum class AlgorithmType : uint8_t {
    HUFFMAN = 1,
    RLE = 2
};

/**
 * @brief Abstract base interface for all compression algorithms.
 * 
 * Owned by: Member 4 (Compression Algorithms & Extensibility Lead)
 */
class ICompressor {
public:
    virtual ~ICompressor() = default;

    /**
     * @brief Gets the algorithm type enum.
     */
    virtual AlgorithmType getType() const = 0;

    /**
     * @brief Gets the human-readable algorithm name.
     */
    virtual std::string getName() const = 0;

    /**
     * @brief Compresses raw byte data into compressed payload.
     * @param input Raw input byte buffer.
     * @param output Output buffer containing compressed representation.
     * @return True if compression succeeded, false otherwise.
     */
    virtual bool compress(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) = 0;

    /**
     * @brief Decompresses compressed byte payload back to original byte data.
     * @param input Compressed byte buffer.
     * @param output Output buffer where decompressed bytes will be stored.
     * @param originalSize The expected size of uncompressed data in bytes.
     * @return True if decompression succeeded, false otherwise.
     */
    virtual bool decompress(const std::vector<uint8_t>& input, std::vector<uint8_t>& output, uint64_t originalSize) = 0;
};

#endif // ICOMPRESSOR_HPP

