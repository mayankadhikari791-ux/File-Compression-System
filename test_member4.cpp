#include "RLE.hpp"
#include "CompressorFactory.hpp"
#include <iostream>
#include <string>
#include <cassert>

int main() {
    std::cout << "========================================\n";
    std::cout << "    MEMBER 4 UNIT TEST (RLE & Factory)  \n";
    std::cout << "========================================\n\n";

    // 1. Test Factory
    std::cout << "[Test 1] Testing CompressorFactory...\n";
    auto compressor = CompressorFactory::create(AlgorithmType::RLE);
    assert(compressor != nullptr);
    assert(compressor->getName() == "Run-Length Encoding (PackBits)");
    std::cout << " -> Factory instantiated algorithm: " << compressor->getName() << "\n";

    // 2. Test RLE on repetitive data
    std::cout << "\n[Test 2] Compressing repetitive dataset...\n";
    std::string text = "AAAAABBBBBCCCCCCCCCDDDDDDDDDDEEEEEEEEEE11111111112222222222";
    std::vector<uint8_t> input(text.begin(), text.end());

    std::vector<uint8_t> compressed;
    bool compOk = compressor->compress(input, compressed);
    assert(compOk);
    std::cout << " -> Original size  : " << input.size() << " bytes\n";
    std::cout << " -> Compressed size: " << compressed.size() << " bytes\n";
    std::cout << " -> Space saved    : " << (1.0f - (float)compressed.size() / input.size()) * 100.0f << "%\n";

    // 3. Test Decompression
    std::cout << "\n[Test 3] Decompressing RLE payload...\n";
    std::vector<uint8_t> decompressed;
    bool decompOk = compressor->decompress(compressed, decompressed, input.size());
    assert(decompOk);
    assert(decompressed == input);

    std::string restored(decompressed.begin(), decompressed.end());
    std::cout << " -> Decompressed text: \"" << restored << "\"\n";
    std::cout << " -> Verification: 100% Exact match!\n";

    std::cout << "\n>>> [ALL MEMBER 4 TESTS PASSED SUCCESSFULLY!] <<<\n";
    return 0;
}
