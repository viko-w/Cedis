#include "render.h"
#include "glad.h"

#include "vbo.h"
#include "ebo.h"
#include "vao.h"

#include <cmath>
#include <ios>

#include <fstream>
#include <iostream>
#include <cerrno>
#include <string>
#include <cstring>


namespace cedis{
    Render::Render(){

    }
    Render::~Render(){

    }

    #pragma region TODO: Move into appropriate header eventually
    std::string Render::getFileContents(const char *filename){
        std::ifstream in(filename, std::ios::binary);
        if(in){
            std::string contents;
            in.seekg(0, std::ios::end);
            contents.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&contents[0], contents.size());
            in.close();

            return (contents);
        }
        throw(errno);
    }

    #pragma endregion


    int Render::initWindow(const char* vertFile, const char* fragFile){

        // Get shader source from shader files
        const char* vertexShaderSource = constructShaderSource(vertFile);
        const char* fragmentShaderSource = constructShaderSource(fragFile);

        // Init library
        if(!glfwInit()){
            return -1;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLfloat vertices[] = {
	    //  POSITION						COLORS
            -0.5f,	-0.5f * float(sqrt(3)) / 3,	0.0f,	0.8f, 0.3f,  0.02f,
             0.5f,	-0.5f * float(sqrt(3)) / 3,	0.0f,	0.8f, 0.3f,  0.02f,
             0.0f,	 0.5f * float(sqrt(3)) * 2 / 3,	0.0f,	1.0f, 0.6f,  0.32f,
            -0.5f / 2,	 0.5f * float(sqrt(3)) / 6,	0.0f,	0.9f, 0.45f, 0.17f,
             0.5f / 2,	 0.5f * float(sqrt(3)) / 6,	0.0f,	0.9f, 0.45f, 0.17f,
             0.0f,	-0.5f * float(sqrt(3)) / 3,	0.0f,	0.8f, 0.3f,  0.02f

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
        ID = glCreateProgram();
        Render::createShader(&ID, vertexShaderSource, fragmentShaderSource);

        // Create Vertex Array/Buffer Object

        VAO VAO1;
        VAO1.Bind();

        VBO VBO1(vertices, sizeof(vertices));
        EBO EBO1(indices, sizeof(indices));

        VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
        VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3*sizeof(float)));
        VAO1.Unbind();
        VBO1.Unbind();
        EBO1.Unbind();

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(m_window);

        Render::loop(&ID, &VAO1);

        VAO1.Delete();
        VBO1.Delete();
        EBO1.Delete();
        Delete();

        glfwDestroyWindow(m_window);

        glfwTerminate();
        return 0;
    }

    int Render::loop(GLuint *shaderProgram, VAO *VAO){
        while(!glfwWindowShouldClose(m_window)){
            // Render
            glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            //glUseProgram(*shaderProgram);
            Activate();

            //glBindVertexArray(*VAO);
            VAO->Bind();

            glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

            // Swap front and back buffers
            glfwSwapBuffers(m_window);

            // Poll for processed events
            glfwPollEvents();
        }
        return 0;
    }

    int Render::createShader(GLuint *shaderProgram, const char* vertexShaderSource, const char* fragmentShaderSource){
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

    char* Render::constructShaderSource(const char *file){
        std::string code = getFileContents(file);
        return strdup(code.c_str());
    }

    void Render::Activate(){
        glUseProgram(ID);
    }
    void Render::Delete(){
        glDeleteProgram(ID);
    }
}
