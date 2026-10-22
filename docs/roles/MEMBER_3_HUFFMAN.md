# Member 3: Huffman Coding Algorithm Specialist

## 1. Role Overview
Member 3 is responsible for the core Huffman Coding compression algorithm. Huffman Coding is a greedy algorithm that builds an optimal prefix code based on the frequency of each byte character. More frequent characters receive shorter bit sequences, while rarer characters receive longer bit sequences, minimizing total file footprint.

---

## 2. Assigned Responsibilities
- **Character Frequency Analysis**: Tabulate byte frequencies across the entire input buffer.
- **Priority Queue / Min-Heap Tree Construction**: Pairwise combine the lowest-frequency nodes to build the optimal binary tree.
- **Prefix-free Codebook Generation**: Traverse the binary tree to produce deterministic bit codes for all symbols.
- **Tree Bit-level Serialization & Deserialization**: Save the exact tree structure to the stream in compact preorder traversal and reconstruct it during decompression.
- **Huffman Compressor Engine**: Implement `compress()` and `decompress()` conforming to `ICompressor`.

---

## 3. Assigned Files
| File Path | Description |
|-----------|-------------|
| `include/HuffmanTree.hpp` | Declarations for `HuffmanNode`, `HuffmanNodeComparator`, and `HuffmanTree` |
| `src/HuffmanTree.cpp` | Tree construction, prefix code generation, and preorder bit serialization |
| `include/Huffman.hpp` | Declaration of `HuffmanCompressor` subclassing `ICompressor` |
| `src/Huffman.cpp` | Full encoding and decoding pipeline using BitReader/BitWriter |

---

## 4. Key Functions Implemented
- `HuffmanTree::buildFromFrequencies()`: Min-heap tree generation.
- `HuffmanTree::generateCodeTable()`: Recursive preorder code generator.
- `HuffmanTree::serialize()` / `deserialize()`: Bitstream representation of the code tree.
- `HuffmanCompressor::compress()`: Full byte-to-bitstream compression.
- `HuffmanCompressor::decompress()`: Bitstream-to-original-byte decompression.

---

## 5. Git Commit Commands for Member 3
*(To be executed by Member 3 when ready)*
```bash
# 1. Switch to a feature branch
git checkout -b feature/member3-huffman-algorithm

# 2. Stage only Member 3's assigned files
git add include/HuffmanTree.hpp src/HuffmanTree.cpp include/Huffman.hpp src/Huffman.cpp docs/roles/MEMBER_3_HUFFMAN.md

# 3. Commit with a clear descriptive message
git commit -m "feat(algo): implement Huffman tree construction, serialization, and codec"

# 4. Push to remote feature branch
git push -u origin feature/member3-huffman-algorithm
```

