#pragma once

#include "bits.h"
#include "treenode.h"
#include "queue.h"
#include <string>


// Required prototypes
// Your function implementations must match these without changes

void deallocateTree(EncodingTreeNode* t);
EncodingTreeNode* buildHuffmanTree(std::string messageText);

std::string decodeText(EncodingTreeNode* tree, Queue<Bit>& messageBits);
Queue<Bit> encodeText(EncodingTreeNode* tree, std::string messageText);

void flattenTree(EncodingTreeNode* tree, Queue<Bit>& treeBits, Queue<char>& treeLeaves);
EncodingTreeNode* unflattenTree(Queue<Bit>& treeBits, Queue<char>& treeLeaves);

EncodedData compress(std::string messageText);
std::string decompress(EncodedData& data);
