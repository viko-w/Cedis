#include "texture.h"
#include "glad.h"

namespace cedis {
    Texture::Texture(const char* image, const char* texType, GLuint slot, GLenum format, GLenum pixelType){
        type = texType;

        int widthImg, heightImg, numColCh;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, format == GL_RED ? 1 : STBI_rgb_alpha);
        
        glGenTextures(1, &ID);

        glActiveTexture(GL_TEXTURE0 + slot);
        unit = slot;
        glBindTexture(GL_TEXTURE_2D, ID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(bytes);
        glBindTexture(GL_TEXTURE_2D, 0);

    }

    void Texture::texUnit(Shader *shader, const char *uniform, GLuint unit){
        GLuint location = glGetUniformLocation(shader->ID, uniform);
        shader->Activate();
        glUniform1i(location, unit);
    }
    void Texture::Bind(){
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, ID);
    }
    void Texture::Unbind(){
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    void Texture::Delete(){
        glDeleteTextures(1, &ID);
    }
}
