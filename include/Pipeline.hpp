#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include "ICompressor.hpp"
#include "Benchmark.hpp"
#include <string>
#include <vector>

/**
 * @brief High-level Orchestrator coordinating file reading, header serialization,
 * algorithm compression/decompression, and disk writing.
 * 
 * Owned by: Member 1 (Team Leader - System Architect)
 */
class CompressionPipeline {
public:
    CompressionPipeline(bool verbose = false);

    /**
     * @brief Compresses an input file using the specified algorithm and writes to output file.
     */
    bool compressFile(const std::string& inputPath,
                      const std::string& outputPath,
                      AlgorithmType algoType);

    /**
     * @brief Decompresses an FCS compressed file and writes back the original uncompressed file.
     */
    bool decompressFile(const std::string& inputPath,
                        const std::string& outputPath);

    /**
     * @brief Runs all available compression algorithms on an input file, checks integrity,
     * and prints a detailed performance benchmark comparison table.
     */
    bool benchmarkFile(const std::string& inputPath);

    void setVerbose(bool verbose);

private:
    bool verbose_;

    bool readFileToBuffer(const std::string& path, std::vector<uint8_t>& buffer);
    bool writeBufferToFile(const std::string& path, const std::vector<uint8_t>& buffer);
    void printBanner() const;
};

#endif // PIPELINE_HPP

