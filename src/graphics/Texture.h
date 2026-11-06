#pragma once



#include <glad/gl.h>
#include <string>
#include <iostream>

class Texture{
    public:
        Texture(const std::string& path);
        ~Texture();

        void bind(unsigned int unit) const;

    private:
        int width;
        int height;
        int channels;
        GLuint id; 

};