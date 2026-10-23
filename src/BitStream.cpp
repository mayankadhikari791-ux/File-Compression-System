#include "BitStream.hpp"

// ==========================================
// BitWriter Implementation
// ==========================================

BitWriter::BitWriter()
    : accumulator_(0), bitCount_(0), totalBitsWritten_(0) {}

void BitWriter::writeBit(int bit) {
    // Pack bit from MSB to LSB
    accumulator_ = (accumulator_ << 1) | (bit ? 1 : 0);
    bitCount_++;
    totalBitsWritten_++;

    if (bitCount_ == 8) {
        buffer_.push_back(accumulator_);
        accumulator_ = 0;
        bitCount_ = 0;
    }
}

void BitWriter::writeBitString(const std::string& bitString) {
    for (char c : bitString) {
        if (c == '0') {
            writeBit(0);
        } else if (c == '1') {
            writeBit(1);
        }
    }
}

void BitWriter::writeBits(uint32_t value, int numBits) {
    if (numBits <= 0 || numBits > 32) return;
    for (int i = numBits - 1; i >= 0; --i) {
        int bit = (value >> i) & 1;
        writeBit(bit);
    }
}

void BitWriter::flush() {
    if (bitCount_ > 0) {
        // Shift remaining bits to the left to align to MSB
        accumulator_ <<= (8 - bitCount_);
        buffer_.push_back(accumulator_);
        accumulator_ = 0;
        bitCount_ = 0;
    }
}

const std::vector<uint8_t>& BitWriter::getBuffer() const {
    return buffer_;
}

void BitWriter::reset() {
    buffer_.clear();
    accumulator_ = 0;
    bitCount_ = 0;
    totalBitsWritten_ = 0;
}

uint64_t BitWriter::getTotalBitsWritten() const {
    return totalBitsWritten_;
}

// ==========================================
// BitReader Implementation
// ==========================================

BitReader::BitReader(const std::vector<uint8_t>& data)
    : data_(data), byteIndex_(0), bitIndex_(7), totalBitsRead_(0) {}

int BitReader::readBit() {
    if (byteIndex_ >= data_.size()) {
        return -1; // End of stream
    }

    int bit = (data_[byteIndex_] >> bitIndex_) & 1;
    totalBitsRead_++;

    bitIndex_--;
    if (bitIndex_ < 0) {
        bitIndex_ = 7;
        byteIndex_++;
    }

    return bit;
}

bool BitReader::readBits(int numBits, uint32_t& outValue) {
    if (numBits <= 0 || numBits > 32) return false;
    outValue = 0;
    for (int i = 0; i < numBits; ++i) {
        int bit = readBit();
        if (bit == -1) return false;
        outValue = (outValue << 1) | bit;
    }
    return true;
}

bool BitReader::hasMore() const {
    return byteIndex_ < data_.size();
}

void BitReader::reset() {
    byteIndex_ = 0;
    bitIndex_ = 7;
    totalBitsRead_ = 0;
}

uint64_t BitReader::getBitPosition() const {
    return totalBitsRead_;
}

