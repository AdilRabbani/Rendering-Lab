#ifndef BVH_H
#define BVH_H

#include "bvh_node.h"
#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <memory>

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
    
    void init(std::vector<std::shared_ptr<primitive>> &scene_primitives, int first, int n, aabb scene_aabb) {
        root = new BVHNode;
        root->primitives = scene_primitives;
        root->left = NULL;
        root->right = NULL;
        root->node_aabb = aabb(scene_aabb);
        std::cout << root->node_aabb.min_ << std::endl;
        std::cout << root->node_aabb.max_ << std::endl;
    }

    void build(BVHNode *root, int first, int last) {
        
        auto primitives = root->primitives;

        if (primitives.size() == 1) {
            root->left = new BVHNode;
            root->left->is_leaf = true;
            root->left->primitives = std::vector<std::shared_ptr<primitive>>(primitives.begin(), primitives.end());
            root->left->node_aabb = aabb(primitives[0]->construct_aabb());

            root->right = new BVHNode;
            root->right->is_leaf = true;
            root->right->primitives = std::vector<std::shared_ptr<primitive>>(primitives.begin(), primitives.end());
            root->right->node_aabb = aabb(primitives[0]->construct_aabb());
        }

        else if (primitives.size() == 2) {
            root->left = new BVHNode;
            root->left->primitives = std::vector<std::shared_ptr<primitive>>(primitives.begin(), primitives.end() - 1);
            root->left->node_aabb = aabb(primitives[0]->construct_aabb());
            root->left->is_leaf = true;

            root->right = new BVHNode;
            root->right->primitives = std::vector<std::shared_ptr<primitive>>(primitives.begin() + 1, primitives.end());
            root->right->node_aabb = aabb(primitives[1]->construct_aabb());
            root->right->is_leaf = true;
        }

        else {
            // srand(time(NULL));
            // int random_split = rand() % (2 + 1 - 0) + 0;

            int random_split = 1;

            std::sort(primitives.begin(), primitives.end(), 
            [&random_split](const std::shared_ptr<primitive> a1, const std::shared_ptr<primitive> a2) {
                // aabb box1 = a1->construct_aabb();
                // aabb box2 = a2->construct_aabb();
                // return box1.min_.e[random_split] < box2.min_.e[random_split];
                return a1->get_center().e[random_split] < a2->get_center().e[random_split];
            });

            int mid = primitives.size() / 2;

            root->left = new BVHNode;
            root->left->primitives = std::vector<std::shared_ptr<primitive>>(primitives.begin(), primitives.begin() + mid);

            vec3 left_node_aabb_max;
            vec3 left_node_aabb_min;

            vec3 right_node_aabb_max;
            vec3 right_node_aabb_min;

            for (size_t j = 0; j < root->left->primitives.size(); j++) {
                aabb primitive_aabb = root->left->primitives[j]->construct_aabb();
                // std::cout << "Max: " << primitive_aabb.max_ << std::endl;
                if (j == 0) {
                    left_node_aabb_max = primitive_aabb.max_;
                    left_node_aabb_min = primitive_aabb.min_;
                }
                else {
                    vec3 current_max = get_maximum_vector_for_aabb(left_node_aabb_max, primitive_aabb.max_);
                    vec3 current_min = get_minimum_vector_for_aabb(left_node_aabb_min, primitive_aabb.min_);
                    left_node_aabb_max = current_max;
                    left_node_aabb_min = current_min;
                }
            }

            root->left->node_aabb = aabb(left_node_aabb_min, left_node_aabb_max);

            root->right = new BVHNode;
            root->right->primitives = std::vector<std::shared_ptr<primitive>>(primitives.begin() + mid, primitives.end());

            for (size_t j = 0; j < root->right->primitives.size(); j++) {
                aabb primitive_aabb = root->right->primitives[j]->construct_aabb();
                // std::cout << "Max: " << primitive_aabb.max_ << std::endl;
                if (j == 0) {
                    right_node_aabb_max = primitive_aabb.max_;
                    right_node_aabb_min = primitive_aabb.min_;
                }
                else {
                    vec3 current_max = get_maximum_vector_for_aabb(right_node_aabb_max, primitive_aabb.max_);
                    vec3 current_min = get_minimum_vector_for_aabb(right_node_aabb_min, primitive_aabb.min_);
                    right_node_aabb_max = current_max;
                    right_node_aabb_min = current_min;
                }
            }

            root->right->node_aabb = aabb(right_node_aabb_min, right_node_aabb_max);

            build(root->left, first, mid);
            build(root->right, mid, last);
        }
    }

    bool traverse_bvh(BVHNode *root, const ray &r, double &scene_tmin, double &scene_tmax, hit_record &hit) {

        if (!root->node_aabb.intersect(r, scene_tmin, scene_tmax)) {
            return false;
        }

        if (root->is_leaf) {
            if (root->primitives.size() == 1) {
                // hit_record temp_hit = hit;
                // bool hit_anything = false;
                // double closest_so_far = scene_tmax;
                if (root->primitives[0]->hit(r, scene_tmin, scene_tmax, hit)) { 
                    // closest_so_far = temp_hit.t;
                    // hit = temp_hit;
                    // hit_anything = true;
                }
                return true;
            }
            else if (root->primitives.size() == 2) {
                // hit_record temp_hit = hit;
                // bool hit_anything = false;
                // double closest_so_far = scene_tmax;
                if (root->primitives[0]->hit(r, scene_tmin, scene_tmax, hit)) {
                    // closest_so_far = temp_hit.t;
                    // hit = temp_hit;
                    // hit_anything = true;
                }
                if (root->primitives[1]->hit(r, scene_tmin, scene_tmax, hit)) {
                    // closest_so_far = temp_hit.t;
                    // hit = temp_hit;
                    // hit_anything = true;
                }
                return true;
            }
        }

        bool left_hit = traverse_bvh(root->left, r, scene_tmin, scene_tmax, hit);
        bool right_hit = traverse_bvh(root->right, r, scene_tmin, left_hit ? hit.t : scene_tmax, hit);

        return left_hit || right_hit;

        // hit_record temp_hit = hit;
        // bool hit_anything = false;
        // double closest_so_far = scene_tmax;

        // if (root->left->node_aabb.intersect(r, scene_tmin, scene_tmax)) {
        //     if (root->left->left->node_aabb.intersect(r, scene_tmin, scene_tmax)) {
        //         if (root->left->left->left->node_aabb.intersect(r, scene_tmin, scene_tmax)) {
                    
        //         }
        //     }
        // }
        // return hit_anything;
    }

    void print_aabbs(BVHNode *root, int count) {
        if (root->primitives.size() == 1) {
            std::cout << root->primitives[0]->construct_aabb().max_ << std::endl;
            std::cout << count << std::endl;
        }
        else if (root->primitives.size() == 2) {
            std::cout << root->primitives[0]->construct_aabb().max_ << std::endl;
            std::cout << root->primitives[1]->construct_aabb().max_ << std::endl;
            std::cout << count << std::endl;
        }
        else {
            print_aabbs(root->left, count + 1);
            print_aabbs(root->right, count + 1);
        }
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