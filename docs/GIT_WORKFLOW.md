# GitHub Team Collaboration & Contribution Workflow

This document details how our 4-person team will contribute, commit, and merge our work on GitHub so that every member receives individual commit recognition and verified contributions on GitHub graphs.

---

## 1. Team Distribution Summary

| Team Member | Branch Name | Key Assigned Files |
|-------------|-------------|-------------------|
| **Member 1 (Leader)** | `main` & `feature/leader-pipeline-cli` | `src/main.cpp`, `include/Pipeline.hpp`, `src/Pipeline.cpp`, `include/Benchmark.hpp`, `CMakeLists.txt`, `Makefile`, `build.bat`, `README.md` |
| **Member 2** | `feature/member2-bitstream-header` | `include/BitStream.hpp`, `src/BitStream.cpp`, `include/FileHeader.hpp`, `src/FileHeader.cpp` |
| **Member 3** | `feature/member3-huffman-algorithm` | `include/HuffmanTree.hpp`, `src/HuffmanTree.cpp`, `include/Huffman.hpp`, `src/Huffman.cpp` |
| **Member 4** | `feature/member4-rle-factory` | `include/ICompressor.hpp`, `include/RLE.hpp`, `src/RLE.cpp`, `include/CompressorFactory.hpp`, `src/CompressorFactory.cpp` |

---

## 2. Step-by-Step Instructions

### Step 1: Initial Repository Setup (Team Leader)
*Note: Do this only when you are ready to upload to GitHub.*
```bash
git init
git add .gitignore README.md
git commit -m "chore: initial project structure"
git branch -M main
git remote add origin https://github.com/<your-username>/File-Compression-System.git
git push -u origin main
```

---

### Step 2: Member 2 Commits Their Work
Member 2 pulls the repository, creates their feature branch, stages their assigned files, and pushes:
```bash
git checkout -b feature/member2-bitstream-header
git add include/BitStream.hpp src/BitStream.cpp include/FileHeader.hpp src/FileHeader.cpp docs/roles/MEMBER_2_BITSTREAM.md
git commit -m "feat(io): implement bitwise streams and binary container serialization"
git push -u origin feature/member2-bitstream-header
```
*Member 2 then opens a Pull Request on GitHub targeting `main`.*

---

### Step 3: Member 3 Commits Their Work
Member 3 pulls the repository, creates their feature branch, stages their assigned files, and pushes:
```bash
git checkout -b feature/member3-huffman-algorithm
git add include/HuffmanTree.hpp src/HuffmanTree.cpp include/Huffman.hpp src/Huffman.cpp docs/roles/MEMBER_3_HUFFMAN.md
git commit -m "feat(algo): implement Huffman tree construction, serialization, and codec"
git push -u origin feature/member3-huffman-algorithm
```
*Member 3 then opens a Pull Request on GitHub targeting `main`.*

---

### Step 4: Member 4 Commits Their Work
Member 4 pulls the repository, creates their feature branch, stages their assigned files, and pushes:
```bash
git checkout -b feature/member4-rle-factory
git add include/ICompressor.hpp include/RLE.hpp src/RLE.cpp include/CompressorFactory.hpp src/CompressorFactory.cpp docs/roles/MEMBER_4_RLE_FACTORY.md
git commit -m "feat(algo): implement PackBits RLE compressor, abstract interface, and factory"
git push -u origin feature/member4-rle-factory
```
*Member 4 then opens a Pull Request on GitHub targeting `main`.*

---

### Step 5: Leader Reviews, Merges & Commits Core Orchestration
The Team Leader reviews the Pull Requests on GitHub, merges them sequentially into `main`, and pushes their own orchestration commits:
```bash
git checkout main
git pull origin main
git add include/Pipeline.hpp src/Pipeline.cpp include/Benchmark.hpp src/main.cpp CMakeLists.txt Makefile build.bat test_samples/ docs/
git commit -m "feat(core): implement pipeline orchestration, CLI, benchmark engine and tests"
git push origin main
```

---

## 3. Why This Workflow Guarantees Zero Merge Conflicts
- Each member owns distinct headers (`.hpp`) and source (`.cpp`) files.
- The modules communicate strictly through decoupled interfaces (`ICompressor`, `FileHeader`, and `BitStream`).
- No two teammates modify the same file during development.
- Each teammate gets their name, commits, and PR badges prominently displayed in GitHub repository insights.

