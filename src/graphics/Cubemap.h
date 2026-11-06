#pragma once



#include <glad/gl.h>
#include <string>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>


class Cubemap{
    public:
        Cubemap(std::vector<std::string> faces);
        ~Cubemap();

        void bind(unsigned int unit) const;

    private:
        int width;
        int height;
        int channels;
        GLuint id; 

};