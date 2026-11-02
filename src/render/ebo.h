#pragma once

#include_next <glad.h>

namespace cedis {
    class EBO{
        public:
        GLuint ID;
        EBO(GLuint* indices, GLsizeiptr size);

        void Bind();
        void Unbind();
        void Delete();
    };
}