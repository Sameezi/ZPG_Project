#pragma once
#include <glad/gl.h>

//Include the standard C++ headers  
#include <fstream>
#include <string>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

class ShaderProgram{
	public:
	ShaderProgram(const std::string& vertexSource,const std::string& fragmentSource);
	~ShaderProgram();
	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram& operator=(const ShaderProgram&) = delete;

	void use() const;
	void setMat4(const std::string& name, const glm::mat4& mat) const;
	private:
		Shader vertexShader;
		Shader fragmentShader;
		GLuint id{0};
};



