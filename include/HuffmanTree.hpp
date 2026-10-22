#ifndef HUFFMANTREE_HPP
#define HUFFMANTREE_HPP

#include "BitStream.hpp"
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <cstdint>

/**
 * @brief Node representation inside the binary Huffman coding tree.
 */
struct HuffmanNode {
    uint8_t character;
    uint64_t frequency;
    std::shared_ptr<HuffmanNode> left;
    std::shared_ptr<HuffmanNode> right;

    HuffmanNode(uint8_t ch, uint64_t freq);
    HuffmanNode(uint64_t freq, std::shared_ptr<HuffmanNode> l, std::shared_ptr<HuffmanNode> r);

    bool isLeaf() const;
};

/**
 * @brief Comparator for min-heap priority queue ordering.
 */
struct HuffmanNodeComparator {
    bool operator()(const std::shared_ptr<HuffmanNode>& a, const std::shared_ptr<HuffmanNode>& b) const;
};

/**
 * @brief Constructs, serializes, and traverses the Huffman Tree for prefix encoding/decoding.
 * 
 * Owned by: Member 3 (Huffman Coding Algorithm Specialist)
 */
class HuffmanTree {
public:
    HuffmanTree();

    /**
     * @brief Builds Huffman Tree based on character frequencies.
     * @param frequencies Array of 256 elements containing occurrence counts of each byte.
     */
    void buildFromFrequencies(const std::vector<uint64_t>& frequencies);

    /**
     * @brief Generates prefix-free variable length binary code table.
     * @return Map of byte character to string of '0' and '1'.
     */
    std::unordered_map<uint8_t, std::string> generateCodeTable() const;

    /**
     * @brief Serializes tree structure into a bitstream.
     * Uses preorder traversal: 0 for internal node, 1 followed by 8-bit character for leaf.
     */
    void serialize(BitWriter& writer) const;

    /**
     * @brief Reconstructs tree structure from bitstream.
     */
    void deserialize(BitReader& reader);

    /**
     * @brief Gets pointer to root node.
     */
    std::shared_ptr<HuffmanNode> getRoot() const;

    /**
     * @brief Checks if tree is empty.
     */
    bool isEmpty() const;

private:
    std::shared_ptr<HuffmanNode> root_;

    void generateCodesRecursive(const std::shared_ptr<HuffmanNode>& node,
                                const std::string& currentCode,
                                std::unordered_map<uint8_t, std::string>& codeTable) const;

    void serializeRecursive(const std::shared_ptr<HuffmanNode>& node, BitWriter& writer) const;
    std::shared_ptr<HuffmanNode> deserializeRecursive(BitReader& reader);
};

#endif // HUFFMANTREE_HPP

