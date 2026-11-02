#pragma once

#include_next <glad.h>

namespace cedis {
    class VBO{
        public:
        GLuint ID;
        VBO(GLfloat* vertices, GLsizeiptr size);

        void Bind();
        void Unbind();
        void Delete();
    };
}