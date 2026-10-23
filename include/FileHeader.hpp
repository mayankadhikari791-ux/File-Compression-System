#ifndef FILEHEADER_HPP
#define FILEHEADER_HPP

#include "ICompressor.hpp"
#include <vector>
#include <cstdint>
#include <string>

/**
 * @brief Binary Header for File Compression System container files (.fcs).
 * 
 * Layout:
 * [0..3]   Magic Bytes: "FCS\x01"
 * [4]      Algorithm Type (1 = Huffman, 2 = RLE)
 * [5..12]  Original File Size (uint64_t, 8 bytes)
 * [13..16] Metadata Length (uint32_t, 4 bytes)
 * [17..N]  Metadata payload (e.g. Huffman frequency table / tree)
 * [N+1..]  Compressed data payload
 * 
 * Owned by: Member 2 (Bitwise I/O & Serialization Lead)
 */
class FileHeader {
public:
    static const uint8_t MAGIC[4];

    FileHeader();
    FileHeader(AlgorithmType algo, uint64_t originalSize, const std::vector<uint8_t>& metadata = {});

    AlgorithmType getAlgorithm() const;
    void setAlgorithm(AlgorithmType algo);

    uint64_t getOriginalSize() const;
    void setOriginalSize(uint64_t size);

    const std::vector<uint8_t>& getMetadata() const;
    void setMetadata(const std::vector<uint8_t>& metadata);

    /**
     * @brief Serializes the header into a byte buffer.
     */
    std::vector<uint8_t> serialize() const;

    /**
     * @brief Deserializes header from byte stream.
     * @param data Raw input byte buffer.
     * @param bytesRead Output parameter receiving the total number of bytes consumed by the header.
     * @return True if valid FCS header was parsed, false otherwise.
     */
    bool deserialize(const std::vector<uint8_t>& data, size_t& bytesRead);

private:
    AlgorithmType algorithm_;
    uint64_t originalSize_;
    std::vector<uint8_t> metadata_;
};

#endif // FILEHEADER_HPP

