#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#undef GL_INVALID_INDEX
#undef GL_TIMEOUT_IGNORED
#include <glad.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "shader.h"

namespace cedis {
    class Camera{
        public:
        glm::vec3 Position;
        glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::mat4 cameraMatrix = glm::mat4(1.0f);

        bool firstClick = true;

        int width;
        int height;

        float speed = 3.0f;
        float sensitiviy = 100.0f;

        Camera(int width, int height, glm::vec3 position);

        void Matrix(Shader &shader, const char *uniform);
        void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
        void Inputs(GLFWwindow *window, double dt);
    };
}