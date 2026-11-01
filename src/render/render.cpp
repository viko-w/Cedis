#include "render.h"
#include "glad.h"
#include <GL/glext.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <cstddef>


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
            0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f

        };

        m_window = glfwCreateWindow(640, 480, "Spooky Amdus Window", NULL, NULL);
        if(!m_window){
            glfwTerminate();
            return -1;
        }

        glfwMakeContextCurrent(m_window);

        gladLoadGL((GLADloadfunc) glfwGetProcAddress);
        glViewport(0, 0, 640, 480);


        // Create vertex shader
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        // Create fragment shader
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        // Wrap shaders in shader program
        GLuint shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        // Delete vertex and fragment shaders
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);


        // Create vertex buffers
        GLuint VAO, VBO;

        glGenVertexArrays(1, &VAO); // Must be generated before VBO
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);




        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(m_window);

        Render::loop(&shaderProgram, &VAO);

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
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
            glDrawArrays(GL_TRIANGLES, 0, 3);

            // Swap front and back buffers
            glfwSwapBuffers(m_window);

            // Poll for processed events
            glfwPollEvents();
        }
        return 0;
    }
}