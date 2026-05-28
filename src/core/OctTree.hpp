#pragma once
#include <glm/glm.hpp>
#include <memory>
#include "Particle.hpp"
#include "core/OctTree.hpp"

struct OctNode {
    glm::vec3 center;      // center of this cube
    float halfSize;         // half the width of this cube

    float totalMass = 0.0f;
    glm::vec3 centerOfMass = glm::vec3(0.0f);

    Particle* particle = nullptr; // non-null only for leaf nodes
    std::unique_ptr<OctNode> children[8]; // 8 child nodes
};

// OctTree class to manage the octree structure for spatial partitioning and force calculations
class OctTree {
public:
    OctTree(const glm::vec3& center, float halfSize);
    void insert(Particle* p);
    void computeForces(Particle& p, float G, float theta = 0.5f);
    void clear(const glm::vec3& center, float halfSize);

private:
    std::unique_ptr<OctNode> root;
    void insert(OctNode* node, Particle* p);
    void computeForces(OctNode* node, Particle& p, float G, float theta);
};