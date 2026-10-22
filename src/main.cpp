#include "Pipeline.hpp"
#include "CompressorFactory.hpp"
#include <iostream>
#include <string>
#include <vector>

void printBanner() {
    std::cout << "========================================================\n";
    std::cout << "      FILE COMPRESSION SYSTEM (C++14 / OOP Edition)    \n";
    std::cout << "   Developed by 4-Member Engineering Team (Team Lead)   \n";
    std::cout << "========================================================\n\n";
}

void printHelp(const char* progName) {
    printBanner();
    std::cout << "Usage:\n";
    std::cout << "  " << progName << " -c <inputFile> <outputFile> [-a <algorithm>] [-v]\n";
    std::cout << "  " << progName << " -d <inputFile> <outputFile> [-v]\n";
    std::cout << "  " << progName << " -b <inputFile>\n";
    std::cout << "  " << progName << " -h | --help\n\n";
    std::cout << "Options:\n";
    std::cout << "  -c <in> <out>     Compress input file to output file (.fcs container)\n";
    std::cout << "  -d <in> <out>     Decompress .fcs file to restored output file\n";
    std::cout << "  -b <input>        Benchmark & compare all algorithms on input file\n";
    std::cout << "  -a <algorithm>    Choose algorithm: 'huffman' (default) or 'rle'\n";
    std::cout << "  -v                Verbose mode (shows detailed process steps)\n";
    std::cout << "  -h, --help        Show this documentation\n\n";
    std::cout << "Examples:\n";
    std::cout << "  " << progName << " -c document.txt document.fcs -a huffman\n";
    std::cout << "  " << progName << " -d document.fcs restored.txt\n";
    std::cout << "  " << progName << " -b document.txt\n\n";
}

void runInteractiveMenu() {
    printBanner();
    CompressionPipeline pipeline(true);

    while (true) {
        std::cout << "Select an operation:\n";
        std::cout << "  1. Compress File\n";
        std::cout << "  2. Decompress File\n";
        std::cout << "  3. Benchmark / Compare Algorithms on File\n";
        std::cout << "  4. Exit\n";
        std::cout << "\nEnter choice (1-4): ";

        int choice = 0;
        if (!(std::cin >> choice)) {
            break;
        }

        if (choice == 1) {
            std::string inPath, outPath, algoStr;
            std::cout << "Enter path of file to compress: ";
            std::cin >> inPath;
            std::cout << "Enter path for output compressed file (e.g. output.fcs): ";
            std::cin >> outPath;
            std::cout << "Select algorithm (huffman / rle) [default: huffman]: ";
            std::cin >> algoStr;

            AlgorithmType algo = AlgorithmType::HUFFMAN;
            if (algoStr == "rle" || algoStr == "RLE") {
                algo = AlgorithmType::RLE;
            }

            pipeline.compressFile(inPath, outPath, algo);
        } else if (choice == 2) {
            std::string inPath, outPath;
            std::cout << "Enter path of compressed .fcs file: ";
            std::cin >> inPath;
            std::cout << "Enter path for restored output file: ";
            std::cin >> outPath;

            pipeline.decompressFile(inPath, outPath);
        } else if (choice == 3) {
            std::string inPath;
            std::cout << "Enter path of file to benchmark: ";
            std::cin >> inPath;

            pipeline.benchmarkFile(inPath);
        } else if (choice == 4) {
            std::cout << "Exiting File Compression System. Goodbye!\n";
            break;
        } else {
            std::cout << "[Error] Invalid selection. Please choose 1, 2, 3, or 4.\n\n";
        }
    }
}

int main(int argc, char* argv[]) {
    // If no arguments provided, launch interactive menu mode
    if (argc == 1) {
        runInteractiveMenu();
        return 0;
    }

    std::string mode;
    std::string inputFile;
    std::string outputFile;
    std::string algoName = "huffman";
    bool verbose = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            printHelp(argv[0]);
            return 0;
        } else if (arg == "-v" || arg == "--verbose") {
            verbose = true;
        } else if (arg == "-c" && i + 2 < argc) {
            mode = "compress";
            inputFile = argv[++i];
            outputFile = argv[++i];
        } else if (arg == "-d" && i + 2 < argc) {
            mode = "decompress";
            inputFile = argv[++i];
            outputFile = argv[++i];
        } else if (arg == "-b" && i + 1 < argc) {
            mode = "benchmark";
            inputFile = argv[++i];
        } else if (arg == "-a" && i + 1 < argc) {
            algoName = argv[++i];
        } else {
            std::cerr << "[Error] Unrecognized or incomplete argument: " << arg << "\n";
            std::cerr << "Run '" << argv[0] << " -h' for usage guide.\n";
            return 1;
        }
    }

    CompressionPipeline pipeline(verbose);

    if (mode == "compress") {
        AlgorithmType algo = AlgorithmType::HUFFMAN;
        if (algoName == "rle" || algoName == "RLE") {
            algo = AlgorithmType::RLE;
        } else if (algoName != "huffman" && algoName != "HUFFMAN") {
            std::cerr << "[Error] Unknown algorithm '" << algoName << "'. Supported: 'huffman', 'rle'.\n";
            return 1;
        }
        return pipeline.compressFile(inputFile, outputFile, algo) ? 0 : 1;
    } else if (mode == "decompress") {
        return pipeline.decompressFile(inputFile, outputFile) ? 0 : 1;
    } else if (mode == "benchmark") {
        return pipeline.benchmarkFile(inputFile) ? 0 : 1;
    } else {
        printHelp(argv[0]);
        return 1;
    }
}

