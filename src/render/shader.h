#pragma once

#include "glad.h"
#include <string>

namespace cedis {
    // TODO: move to separate header eventually?
    std::string getFileContents(const char* filename);

    class Shader{
        public:
        GLuint ID;
        Shader(const char* vertFile, const char* fragFile);

        void Activate();
        void Delete();

        void checkCompileErrors(unsigned int shader, const char *type);
    };
}