#ifndef BVHNODE_H
#define BVHNODE_H

#include "Scratchapixel/aabb.h"
#include "primitive.h"
#include <memory>
class BVHNode {

public:
    BVHNode() {
        this->left = this->right = NULL;
        this->is_leaf = false;
    };

public:
        BVHNode *left;
        BVHNode *right;
        std::vector<std::shared_ptr<primitive>> primitives;
        aabb node_aabb;
        bool is_leaf;
};

#endif