#pragma once

#include "glad.h"
#include <string>

namespace cedis {
    class Shader{
        public:
        GLuint ID;
        Shader(const char* vertFile, const char* fragFile);

        // TODO: move to separate header eventually?
        static std::string getFileContents(const char* filename);

        void Activate();
        void Delete();

        void checkCompileErrors(unsigned int shader, const char *type);
    };
}