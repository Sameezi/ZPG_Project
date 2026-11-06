#include "Cubemap.h"

#include <stb/stb_image.h>

Cubemap::Cubemap(std::vector<std::string> faces){
    stbi_set_flip_vertically_on_load(false);
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);

    for( unsigned int i = 0; i < faces.size(); i++){
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);

        if(data){
            GLenum format = channels == 4 ? GL_RGBA : GL_RGB; 
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0 , format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else{
            std::cerr << "Cubemap failed to load at path" << faces[i]<< "\n";
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
}

Cubemap::~Cubemap() {
    if (id != 0) glDeleteTextures(1, &id);
}

void Cubemap::bind(unsigned int unit) const{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
}