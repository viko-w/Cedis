#include "render.h"
#include "glad.h"
#include <GL/glext.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <cstddef>
#include <span>


namespace cedis{
    Render::Render(){

    }
    Render::~Render(){

    }


    int Render::initWindow(){
        // Init library
        if(!glfwInit()){
            return -1;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLfloat vertices[] = {
            -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
            0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
            0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f,
            -0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,
            0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,
            0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f

        };

        GLuint indices[] = {
            0, 3, 5,
            3, 2, 4,
            5, 4, 1
        };

        m_window = glfwCreateWindow(800, 800, "Spooky Amdus Window", NULL, NULL);
        if(!m_window){
            glfwTerminate();
            return -1;
        }

        glfwMakeContextCurrent(m_window);

        gladLoadGL((GLADloadfunc) glfwGetProcAddress);
        glViewport(0, 0, 800, 800);

        // Create Shader Program
        GLuint shaderProgram = glCreateProgram();
        Render::createShader(&shaderProgram);

        // Create Vertex Array/Buffer Object, and 
        GLuint VAO, VBO, EBO;
        Render::createVertexObjects(&VAO, &VBO, &EBO, vertices, indices);


        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(m_window);

        Render::loop(&shaderProgram, &VAO);

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteProgram(shaderProgram);

        glfwDestroyWindow(m_window);

        glfwTerminate();
        return 0;
    }

    int Render::loop(GLuint *shaderProgram, GLuint *VAO){
        while(!glfwWindowShouldClose(m_window)){
            // Render
            glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glUseProgram(*shaderProgram);

            glBindVertexArray(*VAO);
            glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

            // Swap front and back buffers
            glfwSwapBuffers(m_window);

            // Poll for processed events
            glfwPollEvents();
        }
        return 0;
    }

    int Render::createShader(GLuint *shaderProgram){
        // Create vertex shader
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        // Create fragment shader
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        // Wrap shaders in shader program
        glAttachShader(*shaderProgram, vertexShader);
        glAttachShader(*shaderProgram, fragmentShader);
        glLinkProgram(*shaderProgram);

        // Delete vertex and fragment shaders
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return 0;
    }

    int Render::createVertexObjects(GLuint *VAO, GLuint *VBO, GLuint *EBO, std::span<const GLfloat> vertices, std::span<const GLuint> indices){
        glGenVertexArrays(1, &*VAO); // Must be generated before VBO
        glGenBuffers(1, &*VBO);
        glGenBuffers(1, &*EBO);

        glBindVertexArray(*VAO);
        glBindBuffer(GL_ARRAY_BUFFER, *VBO);

        glBufferData(GL_ARRAY_BUFFER, vertices.size_bytes(), vertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size_bytes(), indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        return 0;
    }
}