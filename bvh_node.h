#ifndef BVHNODE_H
#define BVHNODE_H

#include "Scratchapixel/aabb.h"

class BVHNode {

public:
    BVHNode() {
        this->left = this->right = NULL;
        this->split_axis = -1;
        this->n_primitives = -1;
        this->split_axis = -1;
    };

public:
        BVHNode *left;
        BVHNode *right;
        std::vector<aabb> aabbs;
        int split_axis;
        int primitive_offset;
        int n_primitives;
};

#endif