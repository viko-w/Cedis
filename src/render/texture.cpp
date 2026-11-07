#include "texture.h"
#include "glad.h"

namespace cedis {
    Texture::Texture(const char* image, GLenum texType, GLuint slot, GLenum format, GLenum pixelType){
        type = texType;

        int widthImg, heightImg, numColCh;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, format == GL_RED ? 1 : STBI_rgb_alpha);
        
        glGenTextures(1, &ID);

        glActiveTexture(GL_TEXTURE0 + slot);
        unit = slot;
        glBindTexture(texType, ID);

        glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexImage2D(texType, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
        glGenerateMipmap(texType);

        stbi_image_free(bytes);
        glBindTexture(texType, 0);

    }

    void Texture::texUnit(Shader *shader, const char *uniform, GLuint unit){
        GLuint location = glGetUniformLocation(shader->ID, uniform);
        shader->Activate();
        glUniform1i(location, unit);
    }
    void Texture::Bind(){
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(type, ID);
    }
    void Texture::Unbind(){
        glBindTexture(type, 0);
    }
    void Texture::Delete(){
        glDeleteTextures(1, &ID);
    }
}
