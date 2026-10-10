#pragma once
#include <glad/gl.h>

#include <vector>
#include <iostream>
#include <fstream>

enum class ShaderType{
	Vertex,
	Fragment
};

class Shader{
	public:
		Shader(ShaderType type, const std::string& shaderFile);
		~Shader();
		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;
		GLuint getId() const;
	private:
		GLuint id;
		ShaderType type;
};
