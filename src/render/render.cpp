#include "render.h"
#include "glad.h"

#include "texture.h"
#include "vbo.h"
#include "ebo.h"
#include "vao.h"
#include "shader.h"
#include "camera.h"

#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/trigonometric.hpp>
#include <stb_image.h>
#include <cstring>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



namespace cedis{

    GLfloat vertices[] = {
    //  POSITION						    COLORS                              TEXTURE MAP
        -0.5f, 0.0f,  0.5f,  0.83f,  0.70f,  0.44f, 0.0f, 0.0f,
        -0.5f,	 0.0f,  -0.5f,  0.83f, 0.70f, 0.44f, 5.0f, 1.0f,
        0.5f,	0.0f, -0.5f, 0.83f, 0.70f, 0.44f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.5f, 0.83f, 0.70f, 0.44f, 5.0f, 0.0f,
        0.0f, 0.8f, 0.0f, 0.92f, 0.86f, 0.76f, 2.5f, 5.0f
    };

    GLuint indices[] = {
        0, 1, 2,
        0, 2, 3,
        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        3, 0, 4
    };

    const unsigned int width = 800;
    const unsigned int height = 800;

    Render::Render() {  }


    int Render::initWindow(const char* vertFile, const char* fragFile){

        // Init library
        if(!glfwInit()){
            return -1;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_window = glfwCreateWindow(width, height, "Spooky Amdus Window", NULL, NULL);
        if(!m_window){
            glfwTerminate();
            return -1;
        }

        glfwMakeContextCurrent(m_window);

        gladLoadGL((GLADloadfunc) glfwGetProcAddress);
        glViewport(0, 0, width, height);


        Shader shaderProgram(vertFile, fragFile);

        // Create Vertex Array/Buffer Object

        VAO VAO1;
        VAO1.Bind();

        VBO VBO1(vertices, sizeof(vertices));
        EBO EBO1(indices, sizeof(indices));

        VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
        VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3*sizeof(float)));
        VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6*sizeof(float)));
        VAO1.Unbind();
        VBO1.Unbind();
        EBO1.Unbind();


        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(m_window);

        Texture kitty("textures/texture.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
        kitty.texUnit(&shaderProgram, "tex0", 0);


        glEnable(GL_DEPTH_TEST);

        Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

        Render::loop(&shaderProgram, &kitty, &VAO1, &camera);

        VAO1.Delete();
        VBO1.Delete();
        EBO1.Delete();
        glDeleteTextures(1, &texture);
        shaderProgram.Delete();

        glfwDestroyWindow(m_window);

        glfwTerminate();
        return 0;
    }

    int Render::loop(Shader *shaderProgram, Texture *texture, VAO *VAO, Camera *camera){
        while(!glfwWindowShouldClose(m_window)){
            // Render
            glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            shaderProgram->Activate();

            camera->Inputs(m_window);
            camera->Matrix(45.0f, 0.1f, 100.0f, *shaderProgram, "camMatrix");
            
            texture->Bind();
            VAO->Bind();

            glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);

            // Swap front and back buffers
            glfwSwapBuffers(m_window);

            // Poll for processed events
            glfwPollEvents();
        }
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
