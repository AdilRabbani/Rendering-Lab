#ifndef BVH_H
#define BVH_H

#include "RTIOW/vec3.h"
#include "bvh_node.h"
#include "hittable.h"
#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <exception>
#include <limits>
#include <memory>

struct bucket_info {
    int count = 0;
    aabb bounds;
};

class BVH {

public:
    BVHNode *root;
    std::vector<hit_record> hits;
    bool shadow_ray_hit;
    int intersection_tests = 0;
    
    BVH(){
        root = NULL;
        shadow_ray_hit = false;
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

            srand(time(NULL));
            int random_split = rand() % (2 + 1 - 0) + 0;

            std::sort(primitives.begin(), primitives.end(), 
            [&random_split](const std::shared_ptr<primitive> a1, const std::shared_ptr<primitive> a2) {
                aabb box1 = a1->construct_aabb();
                aabb box2 = a2->construct_aabb();
                return box1.min_.e[random_split] < box2.min_.e[random_split];
                // return a1->get_center().e[random_split] < a2->get_center().e[random_split];
            });

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
            srand(time(NULL));
            int random_split = rand() % (2 + 1 - 0) + 0;

            std::sort(primitives.begin(), primitives.end(), 
            [&random_split](const std::shared_ptr<primitive> a1, const std::shared_ptr<primitive> a2) {
                aabb box1 = a1->construct_aabb();
                aabb box2 = a2->construct_aabb();
                return box1.min_.e[random_split] < box2.min_.e[random_split];
                // return a1->get_center().e[random_split] < a2->get_center().e[random_split];
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
            root->right->primitives = std::vector<std::shared_ptr<primitive>>(primitives.begin() + (mid), primitives.end());

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
            build(root->right, (mid), last);
        }
    }

    void build_centroid_split(BVHNode *root) {
        auto primitives = root->primitives;

        std::vector<vec3> centroids;

        for (std::size_t j = 0; j < primitives.size(); j++) {
            aabb prim_aabb = primitives[j]->construct_aabb();
            vec3 centroid = (prim_aabb.min_ * 0.5) + (prim_aabb.max_ * 0.5);
            centroids.push_back(centroid);
        }

        vec3 centroid_min;
        vec3 centroid_max;

        for (std::size_t j = 0; j < centroids.size(); j++) {
            if (j == 0) {
                centroid_min = centroids[j];
                centroid_max = centroids[j];
            }
            else {
                centroid_min = get_minimum_vector_for_aabb(centroid_min, centroids[j]);
                centroid_max = get_maximum_vector_for_aabb(centroid_max, centroids[j]);
            }
        }

        aabb centroid_aabb(centroid_min, centroid_max);

        int dim = centroid_aabb.maximum_extent();

        double p_mid = (centroid_aabb.min_.e[dim] + centroid_aabb.max_.e[dim]) / 2;

        root->left = new BVHNode();
        root->right = new BVHNode();

        if (centroid_aabb.min_.e[dim] == centroid_aabb.max_.e[dim]) {
            int mid = primitives.size() / 2;

            for (std::size_t j = 0; j < mid; j++) {
                root->left->primitives.push_back(primitives[j]);
            }

            for (std::size_t j = mid; j < primitives.size(); j++) {
                root->right->primitives.push_back(primitives[j]);
            }

            root->left->is_leaf = true;
            root->right->is_leaf = true;

            vec3 left_node_aabb_max;
            vec3 left_node_aabb_min;

            vec3 right_node_aabb_max;
            vec3 right_node_aabb_min;

            for (size_t j = 0; j < root->right->primitives.size(); j++) {
                aabb primitive_aabb = root->right->primitives[j]->construct_aabb();
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

            for (size_t j = 0; j < root->left->primitives.size(); j++) {
                aabb primitive_aabb = root->left->primitives[j]->construct_aabb();
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
            root->right->node_aabb = aabb(right_node_aabb_min, right_node_aabb_max);
        }

        else {

            auto it = std::partition(primitives.begin(), primitives.end(), [&dim, &p_mid](const std::shared_ptr<primitive> a1){
                aabb prim_aabb = a1->construct_aabb();
                vec3 centroid = (prim_aabb.min_ * 0.5) + (prim_aabb.max_ * 0.5);
                return centroid.e[dim] < p_mid;
            });

            root->left->primitives =  std::vector<std::shared_ptr<primitive>>(primitives.begin(), it);
            root->right->primitives = std::vector<std::shared_ptr<primitive>>(it, primitives.end());

            vec3 left_node_aabb_max;
            vec3 left_node_aabb_min;

            vec3 right_node_aabb_max;
            vec3 right_node_aabb_min;

            for (size_t j = 0; j < root->right->primitives.size(); j++) {
                aabb primitive_aabb = root->right->primitives[j]->construct_aabb();
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

            for (size_t j = 0; j < root->left->primitives.size(); j++) {
                aabb primitive_aabb = root->left->primitives[j]->construct_aabb();
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
            root->right->node_aabb = aabb(right_node_aabb_min, right_node_aabb_max);

            build_centroid_split(root->left);
            build_centroid_split(root->right);
        }
    }

    int get_best_dim_based_on_centroids(std::vector<std::shared_ptr<primitive>> primitives, aabb &centroid_aabb, std::vector<vec3> &centroids, std::vector<aabb> &primitive_aabbs) {

        for (std::size_t j = 0; j < primitives.size(); j++) {
            aabb prim_aabb = primitives[j]->construct_aabb();
            primitive_aabbs.push_back(prim_aabb);
            vec3 centroid = (prim_aabb.min_ * 0.5) + (prim_aabb.max_ * 0.5);
            centroids.push_back(centroid);
        }

        vec3 centroid_min;
        vec3 centroid_max;

        for (std::size_t j = 0; j < centroids.size(); j++) {
            if (j == 0) {
                centroid_min = centroids[j];
                centroid_max = centroids[j];
            }
            else {
                centroid_min = get_minimum_vector_for_aabb(centroid_min, centroids[j]);
                centroid_max = get_maximum_vector_for_aabb(centroid_max, centroids[j]);
            }
        }

        centroid_aabb = aabb(centroid_min, centroid_max);

        int dim = centroid_aabb.maximum_extent();

        return dim;
    }

    void build_SAH_split(BVHNode *root) {

        auto primitives = root->primitives;
        std::vector<vec3> centroids;
        std::vector<aabb> primitive_aabbs;
        aabb centroid_aabb;

        int dim = get_best_dim_based_on_centroids(primitives, centroid_aabb, centroids, primitive_aabbs);
        double p_mid = (centroid_aabb.min_.e[dim] + centroid_aabb.max_.e[dim]) / 2;

        // std::cout << "Best dimension: " << dim << std::endl;

        root->left = new BVHNode();
        root->right = new BVHNode();

        if (primitives.size() <= 4) {
            
            int mid = primitives.size() / 2;
            
            std::sort(primitives.begin(), primitives.end(),
            [&dim](const std::shared_ptr<primitive> a, const std::shared_ptr<primitive> b){
                aabb prim_aabb_1 = a->construct_aabb();
                // vec3 centroid_1 = (prim_aabb_1.min_ * 0.5) + (prim_aabb_1.max_ * 0.5);

                aabb prim_aabb_2 = b->construct_aabb();
                // vec3 centroid_2 = (prim_aabb_2.min_ * 0.5) + (prim_aabb_2.max_ * 0.5);
                return prim_aabb_1.min_.e[dim] < prim_aabb_2.min_.e[dim];
            });

            root->left->primitives =  std::vector<std::shared_ptr<primitive>>(primitives.begin(), primitives.begin() + mid);
            root->right->primitives = std::vector<std::shared_ptr<primitive>>(primitives.begin() + mid, primitives.end());

            // std::cout << "Primitives in left node: " << root->left->primitives.size() << std::endl;
            // std::cout << "Primitives in right node: " << root->right->primitives.size() << std::endl;
            

            root->left->is_leaf = true;
            root->right->is_leaf = true;

            vec3 left_node_aabb_max;
            vec3 left_node_aabb_min;

            vec3 right_node_aabb_max;
            vec3 right_node_aabb_min;

            for (size_t j = 0; j < root->right->primitives.size(); j++) {
                aabb primitive_aabb = root->right->primitives[j]->construct_aabb();
                if (j == 0) {
                    right_node_aabb_max = primitive_aabb.max_;
                    right_node_aabb_min = primitive_aabb.min_;
                }
                else {
                    right_node_aabb_max = get_maximum_vector_for_aabb(right_node_aabb_max, primitive_aabb.max_);
                    right_node_aabb_min = get_minimum_vector_for_aabb(right_node_aabb_min, primitive_aabb.min_);
                }
            }

            for (size_t j = 0; j < root->left->primitives.size(); j++) {
                aabb primitive_aabb = root->left->primitives[j]->construct_aabb();
                if (j == 0) {
                    left_node_aabb_max = primitive_aabb.max_;
                    left_node_aabb_min = primitive_aabb.min_;
                }
                else {
                    left_node_aabb_max = get_maximum_vector_for_aabb(left_node_aabb_max, primitive_aabb.max_);
                    left_node_aabb_min = get_minimum_vector_for_aabb(left_node_aabb_min, primitive_aabb.min_);
                }
            }

            root->left->node_aabb = aabb(left_node_aabb_min, left_node_aabb_max);
            // std::cout << "Left node aabb max: " << root->left->node_aabb.max_ << " min: " << root->left->node_aabb.min_ << std::endl;
            root->right->node_aabb = aabb(right_node_aabb_min, right_node_aabb_max);
            // std::cout << "Right node aabb max: " << root->right->node_aabb.max_ << " min: " << root->right->node_aabb.min_ << std::endl;
        }

        else {
            int number_of_buckets = 6;
            bucket_info buckets[number_of_buckets];

            // std::cout << "Centroid min: " << centroid_aabb.min_ << " , max: " << centroid_aabb.max_ << std::endl;

            point3 division_of_axis = (centroid_aabb.max_ - centroid_aabb.min_) / 6;
            point3 current_division_point = centroid_aabb.min_;
            point3 next_division_point;

            // std::cout << "Division of axis: " << division_of_axis << std::endl;

            for (std::size_t i = 0; i < number_of_buckets; ++i) {
                point3 next_division_point = current_division_point + division_of_axis;
                // std::cout << "Next division point: " << next_division_point << std::endl; 
                buckets[i].bounds = aabb(current_division_point, next_division_point);
                current_division_point = next_division_point;
            }

            // std::cout << "Bucket Divisions:\n";
            // for (std::size_t i = 0; i < number_of_buckets; ++i) {
            //     std::cout << "Bucket: " << i + 1 << ", min: " << buckets[i].bounds.min_ << " , max: " << buckets[i].bounds.max_ << std::endl; 
            // }

            for (std::size_t i = 0; i < primitives.size(); ++i) {
                int bucket_index = number_of_buckets * centroid_aabb.offset(centroids[i]).e[dim];
                if (bucket_index == number_of_buckets) bucket_index = number_of_buckets - 1;
                // std::cout << "Primitive " << i << " lies in bucket " << bucket_index << std::endl;
                buckets[bucket_index].count++;
                vec3 updated_max_bound = get_maximum_vector_for_aabb(buckets[bucket_index].bounds.max_, primitive_aabbs[i].max_);
                vec3 updated_min_bound = get_minimum_vector_for_aabb(buckets[bucket_index].bounds.min_, primitive_aabbs[i].min_);
                buckets[bucket_index].bounds = aabb(updated_min_bound, updated_max_bound);
            }

            // for (int i = 0; i < number_of_buckets; i++) {
            //     std::cout << "Bucket " << i + 1 << " contains : " << buckets[i].count << " primitives\n"; 
            // }

            float cost[number_of_buckets - 1];

            for (std::size_t i = 0; i < number_of_buckets - 1; ++i) {
                aabb b0, b1;
                int count0 = 0, count1 = 0;
                b0 = aabb(std::numeric_limits<vec3>::max(), std::numeric_limits<vec3>::lowest());
                b1 = aabb(std::numeric_limits<vec3>::max(), std::numeric_limits<vec3>::lowest());
                
                for (int j = 0; j <= i; j++) {
                    vec3 updated_max_bound = get_maximum_vector_for_aabb(b0.max_, buckets[j].bounds.max_);
                    vec3 updated_min_bound = get_minimum_vector_for_aabb(b0.min_, buckets[j].bounds.min_);

                    b0 = aabb(updated_min_bound, updated_max_bound);
                    count0 += buckets[j].count;
                }

                for (int j = i+1; j < number_of_buckets; j++) {
                    vec3 updated_max_bound = get_maximum_vector_for_aabb(b1.max_, buckets[j].bounds.max_);
                    vec3 updated_min_bound = get_minimum_vector_for_aabb(b1.min_, buckets[j].bounds.min_);

                    b1 = aabb(updated_min_bound, updated_max_bound);
                    count1 += buckets[j].count;
                }

                cost[i] = .125f + (count0 * b0.surface_area() + count1 * b1.surface_area()) / root->node_aabb.surface_area();
            }

            float min_cost = cost[0];
            int min_cost_split_bucket = 0;

            for (int i = 1; i < number_of_buckets - 1; ++i) {
                if (cost[i] < min_cost) {
                    min_cost = cost[i];
                    min_cost_split_bucket = i;
                }
            }

            // std::cout << "Minimum cost: " << min_cost << " on bucket " << min_cost_split_bucket << std::endl;

            float leaf_cost = primitives.size();

            // std::cout << "Leaf cost: " << leaf_cost << std::endl;

            if (primitives.size() > 4 || min_cost < leaf_cost) {
                auto it = std::partition(primitives.begin(), primitives.end(), [&centroid_aabb, &number_of_buckets, &dim, &min_cost_split_bucket](const std::shared_ptr<primitive> a1){
                    aabb prim_aabb = a1->construct_aabb();
                    vec3 centroid = (prim_aabb.min_ * 0.5) + (prim_aabb.max_ * 0.5);
                    int b = number_of_buckets * centroid_aabb.offset(centroid).e[dim];
                    if (b == number_of_buckets) b = number_of_buckets - 1;
                    return b <= min_cost_split_bucket;
                });

                root->left->primitives =  std::vector<std::shared_ptr<primitive>>(primitives.begin(), it);
                root->right->primitives = std::vector<std::shared_ptr<primitive>>(it, primitives.end());

                // std::cout << "Primitives in left node: " << root->left->primitives.size() << std::endl;
                // std::cout << "Primitives in right node: " << root->right->primitives.size() << std::endl;

                vec3 left_node_aabb_max;
                vec3 left_node_aabb_min;

                vec3 right_node_aabb_max;
                vec3 right_node_aabb_min;

                for (size_t j = 0; j < root->right->primitives.size(); j++) {
                    aabb primitive_aabb = root->right->primitives[j]->construct_aabb();
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

                for (size_t j = 0; j < root->left->primitives.size(); j++) {
                    aabb primitive_aabb = root->left->primitives[j]->construct_aabb();
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
                root->right->node_aabb = aabb(right_node_aabb_min, right_node_aabb_max);

                build_SAH_split(root->left);
                build_SAH_split(root->right);
            }
            else {
                std::cout << std::endl;
                std::cout << "Woah: " << root->primitives.size() << std::endl << std::endl;
                root->is_leaf = true;
            }
        }
    }

    

    bool traverse_bvh(BVHNode *root, const ray &r, 
                      double scene_tmin, 
                      double scene_tmax,
                      const double shadow_tmin,
                      const double shadow_tmax, 
                      hit_record &hit) {

        if (r.shadow_ray) {
            scene_tmin = shadow_tmin;
            scene_tmax = shadow_tmax;
        }

        if (r.shadow_ray && shadow_ray_hit) {
            return true;
        }

        if (!root->node_aabb.intersect(r, scene_tmin, scene_tmax)) {
            return false;
        }

        intersection_tests = intersection_tests + 1;

        if (root->is_leaf) {
            if (root->primitives.size() == 1) {
                hit_record temp_hit = hit;
                bool hit_anything = false;
                double closest_so_far = scene_tmax;
                intersection_tests = intersection_tests + 1;
                if (root->primitives[0]->hit(r, scene_tmin, closest_so_far, temp_hit)) {
                    if (r.shadow_ray) {
                        shadow_ray_hit = true;
                        return true;
                    }
                    closest_so_far = temp_hit.t;
                    hit = temp_hit;
                    hits.push_back(hit);
                    hit_anything = true;
                }
                return hit_anything;
            }
            else if (root->primitives.size() == 2) {
                hit_record temp_hit = hit;
                bool hit_anything = false;
                double closest_so_far = scene_tmax;
                intersection_tests = intersection_tests + 1;
                if (root->primitives[0]->hit(r, scene_tmin, closest_so_far, temp_hit)) {
                    if (r.shadow_ray) {
                        shadow_ray_hit = true;
                        return true;
                    }
                    closest_so_far = temp_hit.t;
                    hit = temp_hit;
                    hits.push_back(hit);
                    hit_anything = true;
                }
                if (root->primitives[1]->hit(r, scene_tmin, closest_so_far, hit)) {
                    if (r.shadow_ray) {
                        shadow_ray_hit = true;
                        return true;
                    }
                    closest_so_far = temp_hit.t;
                    hit = temp_hit;
                    hits.push_back(hit);
                    hit_anything = true;
                }
                return hit_anything;
            }

            else {
                hit_record temp_hit = hit;
                bool hit_anything = false;
                double closest_so_far = scene_tmax;

                for (std::size_t j = 0; j < root->primitives.size(); j++) {
                    intersection_tests = intersection_tests + 1;
                    if (root->primitives[j]->hit(r, scene_tmin, closest_so_far, temp_hit)){
                        if (r.shadow_ray) {
                            shadow_ray_hit = true;
                            return true;
                        }
                        closest_so_far = temp_hit.t;
                        hit = temp_hit;
                        hits.push_back(hit);
                        hit_anything = true;
                    }
                }

                return hit_anything;
            }
        }

        bool left_hit = traverse_bvh(root->left, r, scene_tmin, scene_tmax, shadow_tmin, shadow_tmax, hit);
        bool right_hit = traverse_bvh(root->right, r, scene_tmin, left_hit ? hit.t : scene_tmax, shadow_tmin, left_hit ? hit.t : shadow_tmax, hit);

        return left_hit || right_hit;
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