#include "HuffmanTree.hpp"
#include <queue>

// ==========================================
// HuffmanNode Implementation
// ==========================================

HuffmanNode::HuffmanNode(uint8_t ch, uint64_t freq)
    : character(ch), frequency(freq), left(nullptr), right(nullptr) {}

HuffmanNode::HuffmanNode(uint64_t freq, std::shared_ptr<HuffmanNode> l, std::shared_ptr<HuffmanNode> r)
    : character(0), frequency(freq), left(l), right(r) {}

bool HuffmanNode::isLeaf() const {
    return left == nullptr && right == nullptr;
}

bool HuffmanNodeComparator::operator()(const std::shared_ptr<HuffmanNode>& a,
                                       const std::shared_ptr<HuffmanNode>& b) const {
    return a->frequency > b->frequency; // Min-heap
}

// ==========================================
// HuffmanTree Implementation
// ==========================================

HuffmanTree::HuffmanTree() : root_(nullptr) {}

void HuffmanTree::buildFromFrequencies(const std::vector<uint64_t>& frequencies) {
    std::priority_queue<std::shared_ptr<HuffmanNode>,
                        std::vector<std::shared_ptr<HuffmanNode>>,
                        HuffmanNodeComparator> minHeap;

    for (size_t i = 0; i < frequencies.size(); ++i) {
        if (frequencies[i] > 0) {
            minHeap.push(std::make_shared<HuffmanNode>(static_cast<uint8_t>(i), frequencies[i]));
        }
    }

    if (minHeap.empty()) {
        root_ = nullptr;
        return;
    }

    // Special case: only 1 distinct character in file
    if (minHeap.size() == 1) {
        auto onlyNode = minHeap.top();
        minHeap.pop();
        // Point both left and right to copies/leaf so any bit (0 or 1) decodes the character
        auto leftLeaf = std::make_shared<HuffmanNode>(onlyNode->character, onlyNode->frequency);
        auto rightLeaf = std::make_shared<HuffmanNode>(onlyNode->character, onlyNode->frequency);
        root_ = std::make_shared<HuffmanNode>(onlyNode->frequency, leftLeaf, rightLeaf);
        return;
    }

    while (minHeap.size() > 1) {
        auto left = minHeap.top();
        minHeap.pop();

        auto right = minHeap.top();
        minHeap.pop();

        auto parent = std::make_shared<HuffmanNode>(left->frequency + right->frequency, left, right);
        minHeap.push(parent);
    }

    root_ = minHeap.top();
}

std::unordered_map<uint8_t, std::string> HuffmanTree::generateCodeTable() const {
    std::unordered_map<uint8_t, std::string> codeTable;
    if (!root_) return codeTable;

    // Single distinct character special case
    if (root_->isLeaf()) {
        codeTable[root_->character] = "0";
        return codeTable;
    }

    generateCodesRecursive(root_, "", codeTable);
    return codeTable;
}

void HuffmanTree::generateCodesRecursive(const std::shared_ptr<HuffmanNode>& node,
                                         const std::string& currentCode,
                                         std::unordered_map<uint8_t, std::string>& codeTable) const {
    if (!node) return;

    if (node->isLeaf()) {
        codeTable[node->character] = currentCode.empty() ? "0" : currentCode;
        return;
    }

    generateCodesRecursive(node->left, currentCode + "0", codeTable);
    generateCodesRecursive(node->right, currentCode + "1", codeTable);
}

void HuffmanTree::serialize(BitWriter& writer) const {
    serializeRecursive(root_, writer);
}

void HuffmanTree::serializeRecursive(const std::shared_ptr<HuffmanNode>& node, BitWriter& writer) const {
    if (!node) return;

    if (node->isLeaf()) {
        writer.writeBit(1);
        writer.writeBits(node->character, 8);
    } else {
        writer.writeBit(0);
        serializeRecursive(node->left, writer);
        serializeRecursive(node->right, writer);
    }
}

void HuffmanTree::deserialize(BitReader& reader) {
    root_ = deserializeRecursive(reader);
}

std::shared_ptr<HuffmanNode> HuffmanTree::deserializeRecursive(BitReader& reader) {
    int bit = reader.readBit();
    if (bit == -1) return nullptr;

    if (bit == 1) {
        uint32_t ch = 0;
        if (!reader.readBits(8, ch)) return nullptr;
        return std::make_shared<HuffmanNode>(static_cast<uint8_t>(ch), 0);
    } else {
        auto left = deserializeRecursive(reader);
        auto right = deserializeRecursive(reader);
        return std::make_shared<HuffmanNode>(0, left, right);
    }
}

std::shared_ptr<HuffmanNode> HuffmanTree::getRoot() const {
    return root_;
}

bool HuffmanTree::isEmpty() const {
    return root_ == nullptr;
}
