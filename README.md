# Galaxy Simulator
This C++ program uses OpenGL to render a galaxy simulation and two colliding galaxies.
### Single Galaxy
<img width="1066" height="423" alt="image" src="https://github.com/user-attachments/assets/09c8e56f-07db-457c-bc9a-e20d1c27a28a" />
<img width="1066" alt="single_small" src="https://github.com/user-attachments/assets/83b76fea-f95c-4cec-91cc-e4e5808e8c08" />

### Colliding Galaxies
<img width="941" height="423" alt="image" src="https://github.com/user-attachments/assets/98dcde86-d95f-4860-8d9f-e268bc13bebe" />
<img width="1066" alt="demo_smaller" src="https://github.com/user-attachments/assets/41d9733e-fbd7-4cd5-aee6-b45a4735b982" />
### Languages used
* C++17
* GLSL (Shader Language)
### Libraries 
* GLFW —> window creation and input handling
* GLAD —> OpenGL function loader
* GLM —> math library (vectors, matrices)
* OpenGL 4.1 —> GPU rendering

## Concepts and Techniques Used
### Barnes-Hut Octree (O(N log N) Gravity)
Gravitational force calculation is the most computationally expensive part of an N-body simulation. A naive approach computes interactions between every pair of particles, scaling at O(N²); with 15,000 particles, that's over 100 million calculations per frame.

This simulation uses a **Barnes-Hut Octree** to reduce this to O(N log N). Space is recursively divided into an octree (8 children per node), and distant clusters of particles are approximated as a single mass at their center of mass. This allows 15,000 particles to be simulated in real time on consumer hardware.

### Leapfrog Integration
Particle positions and velocities are updated using the **leapfrog integrator** rather than simple Euler integration. Leapfrog staggers velocity and position updates by half a timestep, which conserves energy over long simulation runs. This prevents the common problem of particles slowly gaining energy and flying off to infinity.

### N-Body Gravity Simulation
Each particle exerts a gravitational force on every other particle according to Newton's law of universal gravitation. A **softening parameter** is added to the distance term to prevent infinite forces when particles get too close, stabilizing the simulation during close encounters.

### Galaxy Initial Conditions
Particles are initialized in a rotating disk using **polar coordinates**, with each particle given a circular orbital velocity calculated from `v = sqrt(G * M / r)`. This ensures particles orbit the central black hole rather than collapsing straight into it. A second galaxy can be spawned with a rotated disk orientation, simulating an off-axis galactic merger similar to the Milky Way & the Andromeda collision.

### OpenGL Rendering Pipeline
Particles are uploaded to the GPU as a **Vertex Buffer Object (VBO)** and rendered as point sprites using a custom **GLSL vertex and fragment shader**. Each particle's speed is passed as a per-vertex attribute and used to color stars from warm orange (slow) to blue-white (fast), mimicking real stellar color temperature. **Additive blending** causes overlapping particles to stack their brightness, naturally producing a glowing galactic core without any post-processing.

### Orbit Camera
The camera uses **spherical coordinates** (yaw, pitch, distance) to orbit around the galaxy center. Mouse drag updates yaw and pitch, scroll updates distance, and a **perspective projection matrix** via GLM gives the scene a true 3D feel.

## Building & Running

### Prerequisites
* macOS (OpenGL 4.1+)
* **Linux** — should work with minor CMake adjustments
* **Windows** — requires vcpkg or manual library setup (not officially tested)
* Xcode Command Line Tools
* Homebrew

### Install Dependencies
```bash
brew install cmake glfw glm
```

### Clone the Repository
```bash
git clone https://github.com/fawazm30/GalaxySimulator.git
cd GalaxySimulator
```

### Build
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j4
```

### Run
```bash
./GalaxySimulator
```

### Controls
| Input | Action |
|-------|--------|
| Click + Drag | Orbit camera around galaxy |
| Scroll | Zoom in / out |
| Space | Toggle between single galaxy and collision mode |
| Escape | Quit |
