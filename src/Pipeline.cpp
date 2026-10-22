#include "Pipeline.hpp"
#include "FileHeader.hpp"
#include "CompressorFactory.hpp"
#include <fstream>
#include <iostream>
#include <iomanip>

CompressionPipeline::CompressionPipeline(bool verbose)
    : verbose_(verbose) {}

void CompressionPipeline::setVerbose(bool verbose) {
    verbose_ = verbose;
}

bool CompressionPipeline::readFileToBuffer(const std::string& path, std::vector<uint8_t>& buffer) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "[Error] Unable to open input file: " << path << std::endl;
        return false;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    buffer.resize(static_cast<size_t>(size));
    if (size > 0 && !file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        std::cerr << "[Error] Failed reading data from file: " << path << std::endl;
        return false;
    }

    return true;
}

bool CompressionPipeline::writeBufferToFile(const std::string& path, const std::vector<uint8_t>& buffer) {
    std::ofstream file(path, std::ios::binary | std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "[Error] Unable to create output file: " << path << std::endl;
        return false;
    }

    if (!buffer.empty()) {
        file.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
    }

    return file.good();
}

bool CompressionPipeline::compressFile(const std::string& inputPath,
                                       const std::string& outputPath,
                                       AlgorithmType algoType) {
    auto compressor = CompressorFactory::create(algoType);
    if (!compressor) {
        std::cerr << "[Error] Invalid compression algorithm specified." << std::endl;
        return false;
    }

    if (verbose_) {
        std::cout << "[Pipeline] Reading input file: " << inputPath << " ...\n";
    }

    std::vector<uint8_t> inputData;
    if (!readFileToBuffer(inputPath, inputData)) {
        return false;
    }

    uint64_t originalSize = inputData.size();
    if (verbose_) {
        std::cout << "[Pipeline] Loaded " << originalSize << " bytes.\n";
        std::cout << "[Pipeline] Compressing using algorithm: " << compressor->getName() << " ...\n";
    }

    BenchmarkTimer timer;
    timer.start();

    std::vector<uint8_t> compressedPayload;
    if (!compressor->compress(inputData, compressedPayload)) {
        std::cerr << "[Error] Compression algorithm failed." << std::endl;
        return false;
    }

    double elapsedMs = timer.stopMs();

    if (verbose_) {
        std::cout << "[Pipeline] Raw payload compressed to " << compressedPayload.size()
                  << " bytes in " << std::fixed << std::setprecision(2) << elapsedMs << " ms.\n";
        std::cout << "[Pipeline] Packing binary FCS container header ...\n";
    }

    // Wrap with FileHeader container
    FileHeader header(algoType, originalSize);
    std::vector<uint8_t> finalFile = header.serialize();
    finalFile.insert(finalFile.end(), compressedPayload.begin(), compressedPayload.end());

    if (!writeBufferToFile(outputPath, finalFile)) {
        return false;
    }

    double ratio = (originalSize > 0) ? (static_cast<double>(finalFile.size()) / originalSize * 100.0) : 100.0;
    double saving = 100.0 - ratio;

    std::cout << "\n================ Compression Summary ================\n";
    std::cout << "  Input File       : " << inputPath << "\n";
    std::cout << "  Output File      : " << outputPath << "\n";
    std::cout << "  Algorithm        : " << compressor->getName() << "\n";
    std::cout << "  Original Size    : " << originalSize << " bytes\n";
    std::cout << "  Compressed Size  : " << finalFile.size() << " bytes (including header)\n";
    std::cout << "  Compression Ratio: " << std::fixed << std::setprecision(2) << ratio << "%\n";
    std::cout << "  Space Saved      : " << std::fixed << std::setprecision(2) << saving << "%\n";
    std::cout << "  Elapsed Time     : " << std::fixed << std::setprecision(2) << elapsedMs << " ms\n";
    std::cout << "=====================================================\n\n";

    return true;
}

bool CompressionPipeline::decompressFile(const std::string& inputPath,
                                         const std::string& outputPath) {
    if (verbose_) {
        std::cout << "[Pipeline] Reading compressed file: " << inputPath << " ...\n";
    }

    std::vector<uint8_t> fileBytes;
    if (!readFileToBuffer(inputPath, fileBytes)) {
        return false;
    }

    if (verbose_) {
        std::cout << "[Pipeline] Parsing binary header ...\n";
    }

    FileHeader header;
    size_t headerBytesRead = 0;
    if (!header.deserialize(fileBytes, headerBytesRead)) {
        std::cerr << "[Error] Invalid or corrupted FCS file header." << std::endl;
        return false;
    }

    auto compressor = CompressorFactory::create(header.getAlgorithm());
    if (!compressor) {
        std::cerr << "[Error] Unknown or unsupported algorithm ID in file header." << std::endl;
        return false;
    }

    if (verbose_) {
        std::cout << "[Pipeline] Detected Algorithm : " << compressor->getName() << "\n";
        std::cout << "[Pipeline] Expected Plain Size: " << header.getOriginalSize() << " bytes\n";
        std::cout << "[Pipeline] Decompressing payload ...\n";
    }

    std::vector<uint8_t> compressedPayload(fileBytes.begin() + headerBytesRead, fileBytes.end());

    BenchmarkTimer timer;
    timer.start();

    std::vector<uint8_t> decompressedData;
    if (!compressor->decompress(compressedPayload, decompressedData, header.getOriginalSize())) {
        std::cerr << "[Error] Decompression failed! Stream was corrupted or size mismatched." << std::endl;
        return false;
    }

    double elapsedMs = timer.stopMs();

    if (!writeBufferToFile(outputPath, decompressedData)) {
        return false;
    }

    std::cout << "\n=============== Decompression Summary ===============\n";
    std::cout << "  Compressed File  : " << inputPath << "\n";
    std::cout << "  Restored File    : " << outputPath << "\n";
    std::cout << "  Algorithm        : " << compressor->getName() << "\n";
    std::cout << "  Restored Size    : " << decompressedData.size() << " bytes\n";
    std::cout << "  Elapsed Time     : " << std::fixed << std::setprecision(2) << elapsedMs << " ms\n";
    std::cout << "  Integrity Check  : PASSED (Exact size match)\n";
    std::cout << "=====================================================\n\n";

    return true;
}

bool CompressionPipeline::benchmarkFile(const std::string& inputPath) {
    std::cout << "\n======================================================\n";
    std::cout << "           RUNNING PERFORMANCE BENCHMARK              \n";
    std::cout << "======================================================\n";
    std::cout << "Input File: " << inputPath << "\n\n";

    std::vector<uint8_t> originalData;
    if (!readFileToBuffer(inputPath, originalData)) {
        return false;
    }

    uint64_t originalSize = originalData.size();
    std::cout << "Original Size: " << originalSize << " bytes (" 
              << std::fixed << std::setprecision(2) << (originalSize / 1024.0) << " KB)\n\n";

    std::vector<AlgorithmType> algos = {AlgorithmType::HUFFMAN, AlgorithmType::RLE};
    std::vector<BenchmarkResult> results;

    for (auto algo : algos) {
        auto compressor = CompressorFactory::create(algo);
        if (!compressor) continue;

        BenchmarkResult res;
        res.algorithmName = compressor->getName();
        res.originalSizeBytes = originalSize;

        // 1. Measure compression
        BenchmarkTimer cTimer;
        cTimer.start();
        std::vector<uint8_t> compressedPayload;
        bool cOk = compressor->compress(originalData, compressedPayload);
        res.compressionTimeMs = cTimer.stopMs();

        if (!cOk) {
            std::cerr << "[Warn] Compression failed for " << res.algorithmName << "\n";
            continue;
        }

        FileHeader header(algo, originalSize);
        std::vector<uint8_t> packed = header.serialize();
        packed.insert(packed.end(), compressedPayload.begin(), compressedPayload.end());
        res.compressedSizeBytes = packed.size();

        // 2. Measure decompression
        BenchmarkTimer dTimer;
        dTimer.start();
        std::vector<uint8_t> decompressedData;
        bool dOk = compressor->decompress(compressedPayload, decompressedData, originalSize);
        res.decompressionTimeMs = dTimer.stopMs();

        // 3. Verify integrity
        res.integrityVerified = (dOk && decompressedData == originalData);
        results.push_back(res);
    }

    // Print Comparative Table
    std::cout << std::left 
              << std::setw(28) << "Algorithm"
              << std::setw(16) << "Comp. Size"
              << std::setw(14) << "Ratio (%)"
              << std::setw(14) << "Saved (%)"
              << std::setw(16) << "Comp Time(ms)"
              << std::setw(16) << "Decomp(ms)"
              << std::setw(12) << "Integrity"
              << "\n";
    std::cout << std::string(106, '-') << "\n";

    for (const auto& r : results) {
        std::cout << std::left
                  << std::setw(28) << r.algorithmName
                  << std::setw(16) << (std::to_string(r.compressedSizeBytes) + " B")
                  << std::setw(14) << std::fixed << std::setprecision(2) << (r.getCompressionRatio() * 100.0)
                  << std::setw(14) << std::fixed << std::setprecision(2) << r.getSpaceSavingPercentage()
                  << std::setw(16) << std::fixed << std::setprecision(2) << r.compressionTimeMs
                  << std::setw(16) << std::fixed << std::setprecision(2) << r.decompressionTimeMs
                  << std::setw(12) << (r.integrityVerified ? "VERIFIED" : "FAILED")
                  << "\n";
    }
    std::cout << std::string(106, '-') << "\n\n";

    return true;
}

