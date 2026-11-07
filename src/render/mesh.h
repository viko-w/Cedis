#pragma once

#include <vector>
#include "glad.h"
#include "vao.h"
#include "camera.h"
#include "texture.h"

namespace cedis{
    class Mesh{
        public:
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        std::vector<Texture> textures;

        VAO vao;

        Mesh(std::vector<Vertex> &vertices, std::vector<GLuint> indices, std::vector<Texture> textures);

        void draw(Shader& shader, Camera& camera);

    };
}