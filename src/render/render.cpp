#include "render.h"

#include "glad.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "vbo.h"
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <vector>



namespace cedis{

    Vertex vertices[] = {
        Vertex{glm::vec3(-1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
        Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
        Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
        Vertex{glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)}
    };

    GLuint indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    Vertex lightVertices[] = {
        Vertex{glm::vec3(-0.1f, -0.1f, 0.1f)},
        Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
        Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
        Vertex{glm::vec3(0.1f, -0.1f, 0.1f)},
        Vertex{glm::vec3(-0.1f, 0.1f, 0.1f)},
        Vertex{glm::vec3(-0.1f, 0.1f, -0.1f)},
        Vertex{glm::vec3(0.1f, 0.1f, -0.1f)},
        Vertex{glm::vec3(0.1f, 0.1f, 0.1f)}
    };

    GLuint lightIndices[] = {
        0, 1, 2,
        0, 2, 3,
        0, 4, 7,
        0, 7, 3,
        3, 7, 6,
        3, 6, 2,
        2, 6, 5,
        2, 5, 1,
        1, 5, 4,
        1, 4, 0,
        4, 5, 6,
        4, 6, 7
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

        Texture textures[] {
            Texture("textures/texture.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
            Texture("textures/textureSpec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
        };


        Shader shaderProgram(vertFile, fragFile);
        std::vector<Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
        std::vector<GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
        std::vector<Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));

        Mesh floor(verts, ind, tex);


        Shader lightShader("shaders/light.vert", "shaders/light.frag");
        std::vector<Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
        std::vector<GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));

        Mesh light(lightVerts, lightInd, tex);


        glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

        glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
        glm::mat4 lightModel = glm::mat4(1.0f);
        lightModel = glm::translate(lightModel, lightPos);

        glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::mat4 pyramidModel = glm::mat4(1.0f);
        pyramidModel = glm::translate(pyramidModel, pyramidPos);

        lightShader.Activate();
        glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"),
            1, GL_FALSE, glm::value_ptr(lightModel));
        glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"),
            lightColor.x, lightColor.y, lightColor.z, lightColor.w);
        shaderProgram.Activate();

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"),
            1, GL_FALSE, glm::value_ptr(pyramidModel));
        glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"),
            lightColor.x, lightColor.y, lightColor.z, lightColor.w);
        glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"),
            lightPos.x, lightPos.y, lightPos.z);


        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(m_window);


        glEnable(GL_DEPTH_TEST);

        Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

        Render::loop(&shaderProgram, &lightShader, &camera, &floor, &light);

        glDeleteTextures(1, &texture);
        shaderProgram.Delete();
        lightShader.Delete();

        glfwDestroyWindow(m_window);

        glfwTerminate();
        return 0;
    }

    int Render::loop(Shader *shaderProgram, Shader *lightShader, Camera *camera, Mesh *floor, Mesh *light){
        while(!glfwWindowShouldClose(m_window)){
            // Render
            glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            camera->Inputs(m_window);
            camera->updateMatrix(45.0f, 0.1f, 100.0f);

            floor->draw(*shaderProgram, *camera);
            light->draw(*lightShader, *camera);
            

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
