#pragma once

#include <glm/glm.hpp>

class Particle {
public:
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    float mass;

    // Constructor to initialize the particle with position, velocity, and mass, and set acceleration to zero
    Particle(const glm::vec3& pos, const glm::vec3& vel, float m)
        : position(pos), velocity(vel), acceleration(0.0f), mass(m) {}
};