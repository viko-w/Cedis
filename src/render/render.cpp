#include "render.h"

#include "glad.h"
#include "model.h"
#include "shader.h"
#include "texture.h"
#include "vbo.h"
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <vector>



namespace cedis{
    const unsigned int width = 800;
    const unsigned int height = 800;
    constexpr float nearPlane = 0.001f;
    constexpr float farPlane = 100.0f;

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

        glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

        glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
        glm::mat4 lightModel = glm::mat4(1.0f);
        lightModel = glm::translate(lightModel, lightPos);


        shaderProgram.Activate();


        glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"),
            lightColor.x, lightColor.y, lightColor.z, lightColor.w);
        glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"),
            lightPos.x, lightPos.y, lightPos.z);
        glUniform1f(glGetUniformLocation(shaderProgram.ID, "nearPlane"), nearPlane);
        glUniform1f(glGetUniformLocation(shaderProgram.ID, "farPlane"), farPlane);


        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(m_window);


        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

        Model model("models/landscape.gltf");

        Render::loop(&shaderProgram, &camera, &model);

        glDeleteTextures(1, &texture);
        shaderProgram.Delete();

        glfwDestroyWindow(m_window);

        glfwTerminate();
        return 0;
    }

    int Render::loop(Shader *shaderProgram, Camera *camera, Model *model){
        while(!glfwWindowShouldClose(m_window)){
            // Render
            glClearColor(0.85f, 0.85f, 0.90f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            camera->Inputs(m_window);
            camera->updateMatrix(45.0f, nearPlane, farPlane);

            model->draw(*shaderProgram, *camera);

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
