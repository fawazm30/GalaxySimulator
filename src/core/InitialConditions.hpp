#pragma once
#include "Particle.hpp"
#include <vector>

class InitialConditions {
public:
    static std::vector<Particle> createGalaxy(int numParticles, float radius, float centralMass);
    static std::vector<Particle> createTwoGalaxies(int numParticles, float radius, float centralMass);
};