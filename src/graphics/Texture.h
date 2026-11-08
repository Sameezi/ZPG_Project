#pragma once

#include <glad/gl.h>
#include <string>
#include <iostream>
#include <bits/stdc++.h>

class Texture{
    public:
        Texture(const std::string& path, const std::string& wrapping = "");
        ~Texture();

        void bind(unsigned int unit) const;

    private:
        int width;
        int height;
        int channels;
        GLuint id; 

};