# Member 4: Run-Length Encoding (RLE) & Extensibility Lead

## 1. Role Overview
Member 4 is responsible for the secondary high-speed compression algorithm (Run-Length Encoding using PackBits) and the object-oriented extensible architecture that allows multiple compression algorithms to coexist seamlessly.

---

## 2. Assigned Responsibilities
- **Abstract Compressor Interface (`ICompressor`)**: Define the polymorphic interface used by the pipeline to interact with any current or future compression algorithm.
- **PackBits Run-Length Encoding (`RLECompressor`)**: Implement an industry-standard RLE algorithm that handles consecutive duplicate bytes without blowing up non-repeating data.
- **Compressor Factory (`CompressorFactory`)**: Factory pattern for creating compressors by enum (`AlgorithmType`) or string name (`"huffman"`, `"rle"`).

---

## 3. Assigned Files
| File Path | Description |
|-----------|-------------|
| `include/ICompressor.hpp` | Pure virtual base class interface for compression engines |
| `include/RLE.hpp` | Header for PackBits Run-Length Encoding compressor |
| `src/RLE.cpp` | Implementation of run scanning, literal collection, and decoding |
| `include/CompressorFactory.hpp` | Factory method declaration for algorithm instantiation |
| `src/CompressorFactory.cpp` | Mapping of algorithm types/names to concrete instances |

---

## 4. Key Functions Implemented
- `ICompressor::compress(input, output)`: Interface definition.
- `ICompressor::decompress(input, output, originalSize)`: Interface definition.
- `RLECompressor::compress()`: Encodes runs >= 3 as repeat blocks and non-repeats as literal blocks.
- `RLECompressor::decompress()`: Decodes literal and repeat runs up to exact original size.
- `CompressorFactory::create(AlgorithmType)`: Polymorphic factory instantiator.
- `CompressorFactory::createByName(string)`: Case-insensitive factory lookup.

---

## 5. Git Commit Commands for Member 4
*(To be executed by Member 4 when ready)*
```bash
# 1. Switch to a feature branch
git checkout -b feature/member4-rle-factory

# 2. Stage only Member 4's assigned files
git add include/ICompressor.hpp include/RLE.hpp src/RLE.cpp include/CompressorFactory.hpp src/CompressorFactory.cpp docs/roles/MEMBER_4_RLE_FACTORY.md

# 3. Commit with a clear descriptive message
git commit -m "feat(algo): implement PackBits RLE compressor, abstract interface, and factory"

# 4. Push to remote feature branch
git push -u origin feature/member4-rle-factory
```

