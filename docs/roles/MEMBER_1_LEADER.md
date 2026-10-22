# Member 1: Team Leader — System Architect & Orchestration Lead

## 1. Role Overview
As the **Team Leader**, you are responsible for the overall project architecture, end-to-end integration, Command Line Interface (CLI), pipeline orchestration, and performance benchmarking. You define the standard interfaces that bind all teammates' modules together.

---

## 2. Assigned Responsibilities
- **High-level Orchestration**: Coordinate file reading, header encapsulation, algorithm dispatch, and disk serialization.
- **Command Line Interface (CLI)**: Robust argument parser (`-c`, `-d`, `-b`, `-a`, `-v`, `-h`) and an interactive menu fallback.
- **Performance Benchmarking Engine**: Measure execution time in milliseconds, compression ratios, space saving percentage, and bit-level data integrity validation.
- **Build System & Integration**: Maintain `CMakeLists.txt`, `Makefile`, and `build.bat`.
- **Code Review & Release**: Review and merge Git Pull Requests from Members 2, 3, and 4 into the `main` branch.

---

## 3. Assigned Files
| File Path | Description |
|-----------|-------------|
| `include/Pipeline.hpp` | Header for the high-level orchestration pipeline |
| `src/Pipeline.cpp` | Implementation of file I/O, compression/decompression dispatch, and benchmark reporting |
| `include/Benchmark.hpp` | High-resolution timer and metrics calculation structure |
| `src/main.cpp` | CLI entry point and interactive user menu |
| `CMakeLists.txt` / `Makefile` / `build.bat` | Build configuration and automated scripts |
| `README.md` | Comprehensive project documentation and report |

---

## 4. Key Functions Implemented
- `CompressionPipeline::compressFile(input, output, algo)`
- `CompressionPipeline::decompressFile(input, output)`
- `CompressionPipeline::benchmarkFile(input)`
- `BenchmarkResult::getCompressionRatio()`
- `BenchmarkResult::getSpaceSavingPercentage()`

---

## 5. Git Commit Commands for Leader
*(Run only when you are ready to push to GitHub)*
```bash
git checkout -b feature/leader-pipeline-cli
git add include/Pipeline.hpp src/Pipeline.cpp include/Benchmark.hpp src/main.cpp CMakeLists.txt Makefile build.bat README.md
git commit -m "feat(core): implement pipeline orchestration, CLI, benchmark engine and build system"
```

