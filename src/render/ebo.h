#pragma once

#include_next <glad.h>
#include <vector>

namespace cedis {
    class EBO{
        public:
        GLuint ID;
        EBO(std::vector<GLuint> &indices);

        void Bind();
        void Unbind();
        void Delete();
    };
}