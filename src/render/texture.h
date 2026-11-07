#pragma once

#include <glad.h>
#include <stb_image.h>

#include "shader.h"



namespace cedis {
    class Texture{
        public:
        GLuint ID;
        GLuint type;
        GLuint unit;
        Texture(const char* image, GLenum texType, GLuint slot, GLenum format, GLenum pixelType);

        void texUnit(Shader *shader, const char* uniform, GLuint unit);
        void Bind();
        void Unbind();
        void Delete();
    };
}