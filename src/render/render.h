#pragma once

#include "vao.h"
#include <glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <span>
#include <string>

namespace cedis {
    class Render{
        public:
        Render();
        ~Render();

        int initWindow(const char* vertFile, const char* fragFile);
        int loop(VAO *VAO, GLuint *uniformID);

        void Activate();
        void Delete();

        // TODO: Move into appropriate header eventually
        std::string getFileContents(const char* filename);

        GLuint ID;

        private:
        int createShader(GLuint *shaderProgram, const char* vertexShaderSource, const char* fragmentShaderSource);
        int createVertexObjects(GLuint *VAO, GLuint *VBO, GLuint *EBO, std::span<const GLfloat> vertices, std::span<const GLuint> indices);

        void checkCompileErrors(unsigned int shader, const char* type);

        enum SHADER_TYPE{
            VERTEX,
            FRAGMEN
        };
        char* constructShaderSource(const char* file);

        GLFWwindow* m_window;

    };
}