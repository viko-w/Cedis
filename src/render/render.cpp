#include "render.h"

#include "glad.h"
#include "model.h"
#include "shader.h"
#include "texture.h"
#include "vbo.h"

#include <cstring>
#include <stb_image_write.h>

#include <GLFW/glfw3.h>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <glm/ext/scalar_uint_sized.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <string>
#include <vector>



namespace cedis{
    const unsigned int width = 800;
    const unsigned int height = 800;
    constexpr float nearPlane = 0.001f;
    constexpr float farPlane = 3000.0f;

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
        glDisable(GL_CULL_FACE);
        glFrontFace(GL_CCW);

        Camera camera(width, height, glm::vec3(0.0f, 300.0f, 800.0f));
        camera.Orientation = glm::normalize(glm::vec3(0.0f, -0.35f, -1.0f));

        Model model = genLandscape();

        // Disable VSync
        glfwSwapInterval(0);

        Render::loop(&shaderProgram, &camera, &model);

        glDeleteTextures(1, &texture);
        shaderProgram.Delete();

        glfwDestroyWindow(m_window);

        glfwTerminate();
        return 0;
    }

    int Render::loop(Shader *shaderProgram, Camera *camera, Model *model){
        while(!glfwWindowShouldClose(m_window)){
            int windowWidth, windowHeight, framebufferWidth, framebufferHeight;
            glfwGetWindowSize(m_window, &windowWidth, &windowHeight);
            glfwGetFramebufferSize(m_window, &framebufferWidth, &framebufferHeight);
            if (windowWidth == 0 || windowHeight == 0 || framebufferWidth == 0 || framebufferHeight == 0) {
                glfwPollEvents();
                continue;
            }
            glViewport(0, 0, framebufferWidth, framebufferHeight);
            camera->width = windowWidth;
            camera->height = windowHeight;

            crntTime = glfwGetTime();
            timeDiff = crntTime - prevTime;
            counter++;
            if(timeDiff >= 1.0 / 30.0){
                std::string FPS = std::to_string((1.0 / timeDiff) * counter);
                std::string ms = std::to_string((timeDiff  / counter) * 1000);
                std::string newTitle = "Spooky Amdus Window — "+FPS+"fps — "+ms+"ms";
                glfwSetWindowTitle(m_window, newTitle.c_str());
                prevTime = crntTime;
                counter = 0;
            }

            // Calc deltatime
            dt = crntTime - deltaPrevTime;
            deltaPrevTime = crntTime;
            
            // Render
            glClearColor(0.85f, 0.85f, 0.90f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            camera->Inputs(m_window, dt);
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

    Model Render::genLandscape(int seed){
        if (seed == 0) srand(time(0));
        else srand(seed);

        int mWidth, mHeight;
        std::vector<std::vector<float>> pixels = genPerlinNoise(seed);
        std::vector<Vertex> vertices;

        for(int i = 0; i < pixels.size(); i++){
            for(int j = 0; j < pixels[i].size(); j++){
                Vertex vertex{
                    {static_cast<float>(i) - width, pixels[i][j], -(static_cast<float>(j) - height)},
                    {0.0f, 1.0f, 0.0f},
                    {0.15f, 0.65f, 0.20f},
                    {static_cast<float>(i) / (pixels.size() - 1),
                    1.0f - static_cast<float>(j) / (pixels[i].size()- 1)}
                };
                vertices.push_back(vertex);
            }
        }

        mWidth = pixels.size();
        mHeight = pixels[0].size();


        Model model;
        model.loadTexture("textures/perlin_baseColor.png");
        model.loadFromVertices(vertices, mWidth, mHeight);
        //model->loadFromFile("models/perlin.gltf");
        return model;
    }
    std::vector<std::vector<float>> Render::genPerlinNoise(int seed){
        srand(time(0));
        if(seed == 0) seed = rand();
        
        noiseSeed = seed;

        glm::uint8 *pixels = new glm::uint8[(width * 2) * (height * 2) * 4];
        std::vector<std::vector<float>> data;
        const int gridSize = 600;

        for (int x = 0; x < width * 2; x++){
            data.push_back({ });
            for (int y = 0; y < height * 2; y++){
                int index = (y * width * 2 + x) * 4;
                float val = 0;
                float freq = 1;
                float amp = 1;

                for(int i = 0; i < 6; i++){
                    val += perlin(x * freq / gridSize, y * freq / gridSize) *amp;
                    freq *= 2;
                    amp /= 2;
                }

                // Contrast
                val *= 1.2;

                // Clip
                if(val > 1.0f) val = 1.0f;
                else if (val < -1.0f) val = -1.0f;

                int color = (int)(((val + 1.0f) * 0.5f) * 255);

                pixels[index] = color;
                pixels[index + 1] = color;
                pixels[index + 2] = color;
                pixels[index + 3] = 255;

                data[x].push_back(color);
            }
        }

        stbi_write_png("textures/perlin_baseColor.png", width * 2, height * 2, 4, pixels, 0);
        return data;
    }

    glm::vec2 Render::randomGradient(int ix, int iy) {
        uint32_t h = (uint32_t)ix * 374761393u
                + (uint32_t)iy * 668265263u
                + (uint32_t)noiseSeed * 1442695041u;

        h = (h ^ (h >> 13)) * 1274126177u;
        h ^= h >> 16;

        float angle = (h / 4294967296.0f) * 6.28318530718f;

        glm::vec2 v;
        v.x = cos(angle);
        v.y = sin(angle);

        return v;
    }

    float Render::dotGridGradient(int ix, int iy, float x, float y){
        glm::vec2 gradient = randomGradient(ix, iy);

        float dx = x - (float)ix;
        float dy = y - (float)iy;

        return (dx * gradient.x + dy * gradient.y);
    }

    float Render::interpolate(float a0, float a1, float w){
        float t = w * w * w * (w * (w * 6.0f - 15.0f) + 10.0f);
        return a0 + t * (a1 - a0);
    }

    float Render::perlin(float x, float y){
        // Nearest grid point
        int x0 = (int)floor(x);
        int y0 = (int)floor(y);
        int x1 = x0 + 1;
        int y1 = y0 + 1;

        // Interpolation weights
        float sx = x - (float)x0;
        float sy = y - (float)y0;

        // Compute & interpolate top two corners
        float n0 = dotGridGradient(x0, y0, x, y);
        float n1 = dotGridGradient(x1, y0, x, y);
        float ix0 = interpolate(n0, n1, sx);

        // Compute & interpolate bottom two corners
        n0 = dotGridGradient(x0, y1, x, y);
        n1 = dotGridGradient(x1, y1, x, y);
        float ix1 = interpolate(n0, n1, sx);

        // Interpolate between two previously interpolated values
        float value = interpolate(ix0, ix1, sy);
        return value;
    }

}
