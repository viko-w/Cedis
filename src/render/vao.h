#pragma once

#include "vbo.h"
#include_next <glad.h>

namespace cedis {
    class VAO{
        public:
        GLuint ID;
        VAO();

        void LinkVBO(VBO& VBO, GLuint layout);
        void Bind();
        void Unbind();
        void Delete();
    };
}