#include "shader.h"
#include "glad.h"
#include <fstream>
#include <string>
#include <iostream>

namespace cedis {
    Shader::Shader(const char* vertFile, const char* fragFile){
        std::string vertexCode = Shader::getFileContents(vertFile);
        std::string fragmentCode = Shader::getFileContents(fragFile);

        const char* vertexShaderSource = vertexCode.c_str();
        const char* fragmentShaderSource = fragmentCode.c_str();


        // Create vertex shader
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        checkCompileErrors(vertexShader, "VERTEX");

        // Create fragment shader
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        checkCompileErrors(fragmentShader, "FRAGMENT");

        ID = glCreateProgram();

        // Wrap shaders in shader program
        glAttachShader(ID, vertexShader);
        glAttachShader(ID, fragmentShader);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");

        // Delete vertex and fragment shaders
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        // Done creating shader
    }

    void Shader::Activate(){
        glUseProgram(ID);
    }
    void Shader::Delete(){
        glDeleteProgram(ID);
    }

    void Shader::checkCompileErrors(unsigned int shader, const char *type){
        GLint hasCompiled;
        char infoLog[1024];
        if (type != "PROGRAM"){
            glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
            if(hasCompiled == GL_FALSE){
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "SHADER_COMPLICATION_ERROR for: " <<  type << "\n" << std::endl; 
            }
        }
        else{
            glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
            if(hasCompiled == GL_FALSE){
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "SHADER_LINKING_ERROR for: " <<  type << "\n" << std::endl; 
            }
        }
    }

    #pragma region TODO: Move into appropriate header eventually
    std::string Shader::getFileContents(const char *filename){
        std::ifstream in(filename, std::ios::binary);
        if(in){
            std::string contents;
            in.seekg(0, std::ios::end);
            contents.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&contents[0], contents.size());
            in.close();

            return (contents);
        }
        throw(errno);
    }

    #pragma endregion
}