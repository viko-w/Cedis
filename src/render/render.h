#pragma once

#include "glad.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <span>

namespace cedis {
    class Render{
        public:
        Render();
        ~Render();

        int initWindow();
        int loop(GLuint *shaderProgram, GLuint *VAO);

        private:
        int createShader(GLuint *shaderProgram);
        int createVertexObjects(GLuint *VAO, GLuint *VBO, std::span<const GLfloat> vertices);

        GLFWwindow* m_window;

        // SHADERS
        const char* vertexShaderSource =
            "#version 330 core \n"
            "layout (location = 0) in vec3 aPos;\n"
            "void main(){\n"
            "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
            "}\0";
        
        const char* fragmentShaderSource =
            "#version 330 core\n"
            "out vec4 FragColor;\n"
            "void main(){\n"
            "   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
            "}\0";

    };
}