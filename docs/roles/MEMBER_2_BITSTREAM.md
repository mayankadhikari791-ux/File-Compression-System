# Member 2: Bitwise I/O & Binary Serialization Specialist

## 1. Role Overview
Member 2 is responsible for the low-level binary data transport layer. Because standard C++ file streams only operate in byte chunks (8 bits), Huffman compression requires writing and reading variable-length bit sequences (e.g., 3 bits, 5 bits, 12 bits). Member 2 provides the bit-packing mechanism and the binary container header format (`.fcs`).

---

## 2. Assigned Responsibilities
- **Bit-level Writer (`BitWriter`)**: Buffer individual bits from MSB to LSB and flush completed 8-bit bytes to memory or disk.
- **Bit-level Reader (`BitReader`)**: Read bits sequentially from an arbitrary byte array, keeping track of bit positions and end-of-stream conditions.
- **File Container Header (`FileHeader`)**: Define the 17-byte custom container protocol (`FCS\x01` magic bytes, algorithm ID, 64-bit uncompressed size, and extensible metadata field).

---

## 3. Assigned Files
| File Path | Description |
|-----------|-------------|
| `include/BitStream.hpp` | Class declarations for `BitWriter` and `BitReader` |
| `src/BitStream.cpp` | Implementation of bitwise shifting, masking, and accumulation |
| `include/FileHeader.hpp` | Header structure definition for the `.fcs` container format |
| `src/FileHeader.cpp` | Serialization and deserialization of file metadata |

---

## 4. Key Functions Implemented
- `BitWriter::writeBit(int bit)`: Accumulates single bit into a byte register.
- `BitWriter::writeBitString(const std::string& bitString)`: Encodes string of '0'/'1' bits.
- `BitWriter::writeBits(uint32_t val, int numBits)`: Packs up to 32 bits.
- `BitWriter::flush()`: Pads remaining bits with zeroes and flushes to buffer.
- `BitReader::readBit()`: Extracts next individual bit.
- `BitReader::readBits(int numBits, uint32_t& outVal)`: Extracts multi-bit integers.
- `FileHeader::serialize()`: Encodes container header into byte stream.
- `FileHeader::deserialize()`: Validates magic bytes and unpacks container properties.

---

## 5. Git Commit Commands for Member 2
*(To be executed by Member 2 when ready)*
```bash
# 1. Switch to a feature branch
git checkout -b feature/member2-bitstream-header

# 2. Stage only Member 2's assigned files
git add include/BitStream.hpp src/BitStream.cpp include/FileHeader.hpp src/FileHeader.cpp docs/roles/MEMBER_2_BITSTREAM.md

# 3. Commit with a clear descriptive message
git commit -m "feat(io): implement bitwise streams and binary container serialization"

# 4. Push to remote feature branch
git push -u origin feature/member2-bitstream-header
```

