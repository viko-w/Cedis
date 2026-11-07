#pragma once

#include "camera.h"
#include "mesh.h"
#include "shader.h"
#include <glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <span>

namespace cedis {
    class Render{
        public:
        Render();
        ~Render();

        int initWindow(const char* vertFile, const char* fragFile);
        int loop(Shader *shaderProgram, Shader *lightShader, Camera *camera, Mesh *floor, Mesh *light);

        private:
        int createVertexObjects(GLuint *VAO, GLuint *VBO, GLuint *EBO, std::span<const GLfloat> vertices, std::span<const GLuint> indices);

        GLFWwindow* m_window;
        GLuint texture;

        // Might change eventually?
        float rotation = 0.0f;
        double prevTime;

    };
}