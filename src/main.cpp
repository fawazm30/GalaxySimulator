#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include <glm/vec3.hpp>
#include <vector>
#include "core/Particle.hpp"
#include "physics/Gravity.hpp"
#include "core/InitialConditions.hpp"

// Vertex shader source code
const char* vertexShaderSource = "#version 410 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vec4(aPos, 1.0); gl_PointSize = 5.0;\n" // Set the position of the vertex
    "}\n";

// Fragment shader source code
const char* fragmentShaderSource = "#version 410 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "    FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n" // Set the color of the fragment (white)
    "}\n";

// Error callback function for GLFW
static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

// Key callback function for GLFW to handle key events
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main(void) {
    // Set the error callback before initializing GLFW to catch any initialization errors
    glfwSetErrorCallback(error_callback);

    // Initialize GLFW and check for errors
    if (!glfwInit())
        exit(EXIT_FAILURE);
    
    srand(glfwGetTime()); // Initialize random seed using current time
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on MacOS
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    std::vector<Particle> particles = InitialConditions::createGalaxy(1000, 1.0f, 100.0f);

    std::vector<glm::vec3> positions;
    for(auto& p : particles) {
        positions.push_back(p.position);
    }

    GLFWwindow* window = glfwCreateWindow(640, 480, "Galaxy Simulator", NULL, NULL);
    if (!window)
    {
        // Window or OpenGL context creation failed
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); // Load OpenGL function pointers using GLAD and GLFW's function loader
    glEnable(GL_PROGRAM_POINT_SIZE); // Enable setting point size in the vertex shader
    glfwSwapInterval(1);

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Vertex Shader source code
    const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Fragment Shader source code
    const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Link shaders to create a shader program
    const GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
 
    while (!glfwWindowShouldClose(window))
    {
        //------Leapfrog Integration------//

        float dt = 0.01f; // Time step for the simulation

        // update velocitity by half-time step
        for(auto& p : particles) {
            p.velocity += p.acceleration * (dt * 0.5f);
        }

        // update position by full time step
        for(auto& p : particles) {
            p.position += p.velocity * dt;
        }

        // reset accelerations to zero before recalculating forces
        for(auto& p : particles) p.acceleration = glm::vec3(0.0f);
        // recalculate forces with new positions
        for(size_t i = 0; i < particles.size(); i++) {
            for(size_t j = i + 1; j < particles.size(); j++) {
                Gravity::applyGravity(particles[i], particles[j]);
            }
        }

        // update velocity by another half-time step
        for(auto& p : particles) {
            p.velocity += p.acceleration * (dt * 0.5f);
        }

        // Reupload updated positions to the GPU
        for(size_t i = 0; i < particles.size(); i++) {
            positions[i] = particles[i].position;
        }
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, positions.size() * sizeof(glm::vec3), positions.data());
        // Clear the screen and draw the particles
        glClearColor(0.02f, 0.02f, 0.05f, 1.0f); // dark blue-black
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, positions.size());  
        // Swap buffers and poll for events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
 
    glfwDestroyWindow(window);
 
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

