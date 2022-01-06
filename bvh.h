#ifndef BVH_H
#define BVH_H

#include "bvh_node.h"

class BVH {

public:
    BVHNode *root;
    
    BVH(){
        root = NULL;
    };
    ~BVH() {
        destroy_bvh();
    }

    void destroy_bvh() {
        std::cout << "Destroying BVH" << std::endl;
        destroy_bvh(this->root);
    }
    
    void init(std::vector<aabb> scene_aabbs, int split, int first, int n) {
        root = new BVHNode;
        root->aabbs = scene_aabbs;
        root->split_axis = split;
        root->primitive_offset = first;
        root->left = NULL;
        root->right = NULL;
    }

    void build(std::vector<aabb>::iterator it, std::vector<aabb> aabbs) {
        
        // int left_count = 0;
        // int right_count = 0;
        // std::vector<aabb>::iterator it2;
        // std::cout << std::endl;
        // for (it2 = aabbs.begin(); it2 != it; it2++) {
        //     left_count++;
        // }
        // std::cout << "Primitives in left node: " << left_count << std::endl;
        // std::cout << std::endl;
        // for (it2 = it; it2 != aabbs.end(); it2++) {
        //     right_count++;
        // }
        // std::cout << "Primitives in right node: " << right_count << std::endl;
        // std::cout << std::endl;

    }

private:

    void destroy_bvh(BVHNode *leaf) {
        if (leaf != NULL) {
            destroy_bvh(leaf->left);
            destroy_bvh(leaf->right);
            delete leaf;
        }
    }
    
};

#endif