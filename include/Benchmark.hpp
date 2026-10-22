#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP

#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <iostream>

/**
 * @brief Results structure holding compression and performance metrics.
 */
struct BenchmarkResult {
    std::string algorithmName;
    uint64_t originalSizeBytes;
    uint64_t compressedSizeBytes;
    double compressionTimeMs;
    double decompressionTimeMs;
    bool integrityVerified;

    double getCompressionRatio() const {
        if (originalSizeBytes == 0) return 1.0;
        return static_cast<double>(compressedSizeBytes) / static_cast<double>(originalSizeBytes);
    }

    double getSpaceSavingPercentage() const {
        if (originalSizeBytes == 0) return 0.0;
        return (1.0 - getCompressionRatio()) * 100.0;
    }
};

/**
 * @brief High-precision timer and benchmark reporter.
 * 
 * Owned by: Member 1 (Team Leader - System Architect)
 */
class BenchmarkTimer {
public:
    void start() {
        startTime_ = std::chrono::high_resolution_clock::now();
    }

    double stopMs() {
        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = endTime - startTime_;
        return duration.count();
    }

private:
    std::chrono::high_resolution_clock::time_point startTime_;
};

#endif // BENCHMARK_HPP

