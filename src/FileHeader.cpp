#include "FileHeader.hpp"
#include <cstring>

const uint8_t FileHeader::MAGIC[4] = {'F', 'C', 'S', 0x01};

FileHeader::FileHeader()
    : algorithm_(AlgorithmType::HUFFMAN), originalSize_(0) {}

FileHeader::FileHeader(AlgorithmType algo, uint64_t originalSize, const std::vector<uint8_t>& metadata)
    : algorithm_(algo), originalSize_(originalSize), metadata_(metadata) {}

AlgorithmType FileHeader::getAlgorithm() const {
    return algorithm_;
}

void FileHeader::setAlgorithm(AlgorithmType algo) {
    algorithm_ = algo;
}

uint64_t FileHeader::getOriginalSize() const {
    return originalSize_;
}

void FileHeader::setOriginalSize(uint64_t size) {
    originalSize_ = size;
}

const std::vector<uint8_t>& FileHeader::getMetadata() const {
    return metadata_;
}

void FileHeader::setMetadata(const std::vector<uint8_t>& metadata) {
    metadata_ = metadata;
}

std::vector<uint8_t> FileHeader::serialize() const {
    std::vector<uint8_t> bytes;
    
    // 1. Magic bytes (4 bytes)
    bytes.insert(bytes.end(), MAGIC, MAGIC + 4);

    // 2. Algorithm ID (1 byte)
    bytes.push_back(static_cast<uint8_t>(algorithm_));

    // 3. Original file size (8 bytes, little-endian)
    for (int i = 0; i < 8; ++i) {
        bytes.push_back(static_cast<uint8_t>((originalSize_ >> (i * 8)) & 0xFF));
    }

    // 4. Metadata length (4 bytes, little-endian)
    uint32_t metaLen = static_cast<uint32_t>(metadata_.size());
    for (int i = 0; i < 4; ++i) {
        bytes.push_back(static_cast<uint8_t>((metaLen >> (i * 8)) & 0xFF));
    }

    // 5. Metadata payload
    bytes.insert(bytes.end(), metadata_.begin(), metadata_.end());

    return bytes;
}

bool FileHeader::deserialize(const std::vector<uint8_t>& data, size_t& bytesRead) {
    bytesRead = 0;
    
    // Minimum header length = 4 (magic) + 1 (algo) + 8 (origSize) + 4 (metaLen) = 17 bytes
    if (data.size() < 17) {
        return false;
    }

    // Verify magic bytes
    if (std::memcmp(data.data(), MAGIC, 4) != 0) {
        return false;
    }

    algorithm_ = static_cast<AlgorithmType>(data[4]);

    // Read original file size (little-endian)
    originalSize_ = 0;
    for (int i = 0; i < 8; ++i) {
        originalSize_ |= (static_cast<uint64_t>(data[5 + i]) << (i * 8));
    }

    // Read metadata length (little-endian)
    uint32_t metaLen = 0;
    for (int i = 0; i < 4; ++i) {
        metaLen |= (static_cast<uint32_t>(data[13 + i]) << (i * 8));
    }

    size_t totalHeaderSize = 17 + metaLen;
    if (data.size() < totalHeaderSize) {
        return false;
    }

    metadata_.clear();
    if (metaLen > 0) {
        metadata_.insert(metadata_.end(), data.begin() + 17, data.begin() + 17 + metaLen);
    }

    bytesRead = totalHeaderSize;
    return true;
}

