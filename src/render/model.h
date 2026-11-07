#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/fwd.hpp>
#include <json.hpp>
#include <string>
#include <vector>
#include "camera.h"
#include "glad.h"
#include "mesh.h"
#include "texture.h"
#include "vbo.h"

using json = nlohmann::json;

namespace cedis {
    class Model{
        public:
        Model(const char* file);
        void draw(Shader &shader, Camera &camera);

        private:
        std::vector<unsigned char> getData();
        std::vector<float> getFloats(json accessor);
        std::vector<GLuint> getIndices(json accessor);
        std::vector<Texture> getTextures();

        void loadMesh(unsigned int indMesh);
        void traverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f));

        std::vector<Vertex> assembleVertices(
            std::vector<glm::vec3> positions,
            std::vector<glm::vec3> normals,
            std::vector<glm::vec2> texUVs
        );

        std::vector<glm::vec2> groupFloatsVec2(std::vector<float> floatVec);
        std::vector<glm::vec3> groupFloatsVec3(std::vector<float> floatVec);
        std::vector<glm::vec4> groupFloatsVec4(std::vector<float> floatVec);

        const char* file;
        std::vector<unsigned char> data;
        json JSON;

        std::vector<Mesh> meshes;
        std::vector<glm::vec3> translationsMeshes;
        std::vector<glm::quat> rotationsMeshes;
        std::vector<glm::vec3> scalesMeshes;
        std::vector<glm::mat4> matricesMeshes;

        std::vector<std::string> loadedTexName;
        std::vector<Texture> loadedTex;
    };
}