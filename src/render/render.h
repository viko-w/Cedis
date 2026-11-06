#pragma once

#include "vao.h"
#include "shader.h"
#include "texture.h"
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
        int loop(Shader *shaderProgram, Texture *texture,  VAO *VAO, GLuint *uniformID);

        private:
        int createVertexObjects(GLuint *VAO, GLuint *VBO, GLuint *EBO, std::span<const GLfloat> vertices, std::span<const GLuint> indices);

        GLFWwindow* m_window;
        GLuint texture;

        float rotation = 0.0f;
        double prevTime;

    };
}