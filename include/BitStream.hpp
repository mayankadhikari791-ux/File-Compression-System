#ifndef BITSTREAM_HPP
#define BITSTREAM_HPP

#include <vector>
#include <string>
#include <cstdint>
#include <cstddef>
#include <stdexcept>

/**
 * @brief BitWriter enables writing individual bits into a byte stream.
 * 
 * Owned by: Member 2 (Bitwise I/O & Serialization Lead)
 */
class BitWriter {
public:
    BitWriter();

    /**
     * @brief Writes a single bit (0 or 1).
     * @param bit The bit value (0 or 1).
     */
    void writeBit(int bit);

    /**
     * @brief Writes a sequence of bits given as a string of '0' and '1' characters.
     * @param bitString String containing '0' and '1' characters.
     */
    void writeBitString(const std::string& bitString);

    /**
     * @brief Writes an integer using a specified number of bits.
     * @param value The value to encode.
     * @param numBits Number of bits to write (1..32).
     */
    void writeBits(uint32_t value, int numBits);

    /**
     * @brief Flushes any remaining unwritten bits in the accumulator into the buffer.
     * Pads with zero bits if necessary to complete the last byte.
     */
    void flush();

    /**
     * @brief Returns the internal accumulated byte buffer.
     */
    const std::vector<uint8_t>& getBuffer() const;

    /**
     * @brief Clears the internal buffer and resets the bit accumulator.
     */
    void reset();

    /**
     * @brief Returns total number of bits written so far.
     */
    uint64_t getTotalBitsWritten() const;

private:
    std::vector<uint8_t> buffer_;
    uint8_t accumulator_;
    int bitCount_;
    uint64_t totalBitsWritten_;
};

/**
 * @brief BitReader enables reading individual bits sequentially from a byte buffer.
 * 
 * Owned by: Member 2 (Bitwise I/O & Serialization Lead)
 */
class BitReader {
public:
    /**
     * @brief Constructs a BitReader from an existing byte buffer.
     * @param data Reference to byte buffer to read from.
     */
    explicit BitReader(const std::vector<uint8_t>& data);

    /**
     * @brief Reads a single bit from the stream.
     * @return 0 or 1, or -1 if the end of the stream has been reached.
     */
    int readBit();

    /**
     * @brief Reads multiple bits and packs them into a 32-bit unsigned integer.
     * @param numBits Number of bits to read (1..32).
     * @param outValue Reference where the unpacked value will be stored.
     * @return True if all bits were successfully read, false if EOF reached.
     */
    bool readBits(int numBits, uint32_t& outValue);

    /**
     * @brief Checks if there are more bits available to read.
     */
    bool hasMore() const;

    /**
     * @brief Resets reading pointer back to the beginning.
     */
    void reset();

    /**
     * @brief Returns current bit read position index.
     */
    uint64_t getBitPosition() const;

private:
    const std::vector<uint8_t>& data_;
    size_t byteIndex_;
    int bitIndex_; // 7 down to 0
    uint64_t totalBitsRead_;
};

#endif // BITSTREAM_HPP

