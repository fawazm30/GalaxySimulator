#include "OctTree.hpp"

// Constructor to initialize the octree with a root node centered at the given position and with the specified half size
OctTree::OctTree(const glm::vec3& center, float halfSize) {
    root = std::make_unique<OctNode>();
    root->center = center;
    root->halfSize = halfSize;
}

// Public method to insert a particle into the octree, which calls the private recursive insert function
void OctTree::clear(const glm::vec3& center, float halfSize) {
    root = std::make_unique<OctNode>();
    root->center = center;
    root->halfSize = halfSize;
}

void OctTree::insert(OctNode* node, Particle* p) {
    // Update the total mass and center of mass for this node
    node->totalMass += p->mass;
    node->centerOfMass += p->position * p->mass;

    // Case 1: Node is empty
    if (node->particle == nullptr && !node->children[0]) {
        node->particle = p;
        return;
    }

    // Case 2: leaf with existing particle — subdivide
    if (node->particle != nullptr && !node->children[0]) {
        Particle* existing = node->particle;
        node->particle = nullptr; // Clear the particle to make it an internal node

        // Subdivide into 8 children
        for (int i = 0; i < 8; i++) {
            node->children[i] = std::make_unique<OctNode>();
            node->children[i]->halfSize = node->halfSize * 0.5f;
            node->children[i]->center = node->center + glm::vec3(
                (i & 1 ? 0.5f : -0.5f) * node->halfSize,
                (i & 2 ? 0.5f : -0.5f) * node->halfSize,
                (i & 4 ? 0.5f : -0.5f) * node->halfSize
            );
        }

        // Reinsert the existing particle into the correct child
        int existingOctant = (existing->position.x > node->center.x ? 1 : 0) |
                     (existing->position.y > node->center.y ? 2 : 0) |
                     (existing->position.z > node->center.z ? 4 : 0);
        insert(node->children[existingOctant].get(), existing);

    }
    // Case 3: internal node — recurse into correct child
    int octant = (p->position.x > node->center.x ? 1 : 0) |
                (p->position.y > node->center.y ? 2 : 0) |
                (p->position.z > node->center.z ? 4 : 0);
    insert(node->children[octant].get(), p);
}

// Public method to compute forces on a particle by traversing the octree, which calls the private recursive computeForces function
void OctTree::computeForces(OctNode* node, Particle& p, float G, float theta) {
    if (!node || node->totalMass == 0) return;

    // Don't interact with yourself
    if (node->particle == &p) return;

    // Get the center of mass position
    glm::vec3 com = node->centerOfMass / node->totalMass;
    glm::vec3 dir = com - p.position;
    float dist = glm::length(dir);
    if (dist < 0.0001f) return;

    float size = node->halfSize * 2.0f;

    // Barnes-Hut approximation — treat node as single mass if far enough away
    if (node->particle != nullptr || (size / dist) < theta) {
        float softening = 0.1f;
        float dist2 = dist * dist + softening * softening;
        float force = G * p.mass * node->totalMass / dist2;
        p.acceleration += glm::normalize(dir) * force / p.mass;
    } else {
        // Too close — recurse into children
        for (int i = 0; i < 8; i++) {
            computeForces(node->children[i].get(), p, G, theta);
        }
    }
}

// Public wrappers
void OctTree::insert(Particle* p) {
    insert(root.get(), p);
}

void OctTree::computeForces(Particle& p, float G, float theta) {
    computeForces(root.get(), p, G, theta);
}