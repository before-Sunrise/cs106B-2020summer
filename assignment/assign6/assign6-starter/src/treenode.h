#pragma once

#include "testing/MemoryDiagnostics.h"


/**
 * Type representing a node in an encoding tree. Each node stores two pointers,
 * one to the child labeled 0 and one to the child labeled 1. If the node is
 * a leaf, both child pointers are nullptr.
 *
 * A node also has character field ch. This field is only meaningful if the
 * node is a leaf (that is, both child pointers are nullptr). You should not
 * access node->ch of an interior (non-leaf) node.
 */

struct EncodingTreeNode {

    char ch;
    EncodingTreeNode* zero;
    EncodingTreeNode* one;

    EncodingTreeNode(char c) {  // use this constructor for new leaf node
        ch = c;
        zero = one = nullptr;
    }

    EncodingTreeNode(EncodingTreeNode* z, EncodingTreeNode* o) { // use this constructor for new interior node
        zero = z;
        one = o;
    }

    TRACK_ALLOCATIONS_OF(EncodingTreeNode); //  SimpleTest allocationg tracking
};
