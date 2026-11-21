#pragma once

#include "camera.h"
#include "mesh.h"
#include "model.h"
#include "shader.h"
#include "texture.h"
#include <cstddef>
#include <glad.h>
#include <glm/ext/vector_float2.hpp>
#include <vector>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <span>

namespace cedis {
    class Render{
        public:
        Render();
        ~Render();

        int initWindow(const char* vertFile, const char* fragFile);
        int loop(Shader *shaderProgram, Camera *camera, Model *model);

        private:
        int createVertexObjects(GLuint *VAO, GLuint *VBO, GLuint *EBO, std::span<const GLfloat> vertices, std::span<const GLuint> indices);

        // Temporary random generator
        Model genLandscape(int seed = 0);
        std::vector<std::vector<float>> genPerlinNoise(int seed = 0);
        float perlin(float x, float y);
        float dotGridGradient(int ix, int iy, float x, float y);
        glm::vec2 randomGradient(int ix, int iy);
        float interpolate(float a0, float a1, float w);

        GLFWwindow* m_window;
        GLuint texture;

        // Might change eventually?
        float rotation = 0.0f;

        double dt = 0.0f;
        double prevTime = 0.0f;
        double deltaPrevTime = 0.0f;
        double crntTime = 0.0f;
        double timeDiff;
        unsigned int counter = 0;
        int noiseSeed = 0;

    };
}