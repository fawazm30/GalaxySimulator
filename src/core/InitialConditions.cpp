#include "InitialConditions.hpp"    
#include "Particle.hpp"
#include "physics/Gravity.hpp"
#include <vector>
#include <cmath>
#include <glm/glm.hpp>


std::vector<Particle> InitialConditions::createGalaxy(int numParticles, float radius, float centralMass) {
    static constexpr float G = 0.0001f;
    // Create a vector to hold the particles (stars) in the galaxy
    std::vector<Particle> particles;
    particles.reserve(numParticles);

    // Create central mass (e.g., supermassive black hole)
    particles.emplace_back(glm::vec3(0.0f), glm::vec3(0.0f), centralMass);

    for(int i = 1; i < numParticles; i++) {
        float angle = ((float)rand() / (float)RAND_MAX) * 2.0f * 3.14159265f; // Random angle around the center
        float r = ((float)rand() / (float)RAND_MAX) * radius; // Random radius within the specified limit

        // Position in a disk-like distribution
        float x = r * cos(angle);
        float z = r * sin(angle);
        float y = ((float)rand() / (float)RAND_MAX - 0.5f) * (radius * 0.1f); // Small random z to create thickness

        // Circular velocity for stable orbit around the central mass
        float distance = sqrt(x*x + z*z);
        float velocityMagnitude = sqrt(G * centralMass / distance); // v = sqrt(G*M/r)
        float vx = -velocityMagnitude * sin(angle); // Perpendicular to radius vector
        float vy = 0.0f;
        float vz = velocityMagnitude * cos(angle);

        particles.emplace_back(glm::vec3(x, y, z), glm::vec3(vx, vy, vz), 1.0f); // Mass of each star is set to 1.0f
    }

    return particles;
}