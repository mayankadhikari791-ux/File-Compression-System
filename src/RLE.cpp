#include "RLE.hpp"
#include <algorithm>

RLECompressor::RLECompressor() {}

AlgorithmType RLECompressor::getType() const {
    return AlgorithmType::RLE;
}

std::string RLECompressor::getName() const {
    return "Run-Length Encoding (PackBits)";
}

bool RLECompressor::compress(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) {
    output.clear();
    if (input.empty()) {
        return true;
    }

    size_t i = 0;
    const size_t n = input.size();

    while (i < n) {
        // Look for consecutive identical bytes
        size_t run = 1;
        while (i + run < n && input[i + run] == input[i] && run < 128) {
            run++;
        }

        if (run >= 3) {
            // Replicate run: control byte = 257 - run (as uint8_t: 1 - run)
            uint8_t control = static_cast<uint8_t>(257 - run);
            output.push_back(control);
            output.push_back(input[i]);
            i += run;
        } else {
            // Collect literal run until we hit a repeated sequence of >= 3 or reach 128 bytes
            size_t litStart = i;
            size_t litLen = 0;

            while (i < n && litLen < 128) {
                // Check if a repeat sequence starts here
                if (i + 2 < n && input[i] == input[i + 1] && input[i] == input[i + 2]) {
                    break;
                }
                i++;
                litLen++;
            }

            if (litLen > 0) {
                // Literal control byte = litLen - 1 (0 to 127)
                uint8_t control = static_cast<uint8_t>(litLen - 1);
                output.push_back(control);
                output.insert(output.end(), input.begin() + litStart, input.begin() + litStart + litLen);
            }
        }
    }

    return true;
}

bool RLECompressor::decompress(const std::vector<uint8_t>& input,
                              std::vector<uint8_t>& output,
                              uint64_t originalSize) {
    output.clear();
    if (originalSize == 0) {
        return true;
    }

    output.reserve(static_cast<size_t>(originalSize));
    size_t pos = 0;
    const size_t inSize = input.size();

    while (pos < inSize && output.size() < originalSize) {
        uint8_t control = input[pos++];

        if (control <= 127) {
            // Literal run: (control + 1) bytes follow
            size_t count = static_cast<size_t>(control) + 1;
            if (pos + count > inSize) {
                return false;
            }
            output.insert(output.end(), input.begin() + pos, input.begin() + pos + count);
            pos += count;
        } else if (control > 128) {
            // Replicate run: single byte repeated (257 - control) times
            size_t count = 257 - static_cast<size_t>(control);
            if (pos >= inSize) {
                return false;
            }
            uint8_t val = input[pos++];
            output.insert(output.end(), count, val);
        }
        // control == 128 is treated as a no-op pad byte
    }

    return (output.size() == originalSize);
}

