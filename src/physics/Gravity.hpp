#pragma once 

#include "core/Particle.hpp"
#include <glm/glm.hpp>

class Gravity {
public:
    static void applyGravity(Particle& p1, Particle& p2, float G = 0.001f) {
        float softening = 0.1f; // Softening factor to prevent singularities

        glm::vec3 direction = p2.position - p1.position; // Vector from p1 to p2
        glm::vec3 forceDirection = glm::normalize(direction);

        float distance = glm::length(direction); // Distance between particles
        float forceMagnitude = G * (p1.mass * p2.mass) / (distance * distance + softening * softening); // acceleration = G * Mass / (distance² + softening²)
        
        glm::vec3 force = forceDirection * forceMagnitude;
        p1.acceleration += force / p1.mass;
        p2.acceleration -= force / p2.mass;
    }
};


