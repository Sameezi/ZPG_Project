/**
 * @file ShaderProgram.cpp
 *
 * @brief ShaderProgram class implementation
 *
 * @author KUC0438 
  **/

#include "ShaderProgram.h"


ShaderProgram::ShaderProgram(const std::string& vertexSource,const std::string& fragmentSource):
	vertexShader(ShaderType::Vertex, vertexSource),
      fragmentShader(ShaderType::Fragment, fragmentSource){

	id = glCreateProgram();
	// Create and compile the vertex and fragment shaders
	glAttachShader(id,vertexShader.getId());
	glAttachShader(id,fragmentShader.getId());

	glLinkProgram(id);

	GLint success;
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[1024];
		glGetProgramInfoLog(id, sizeof(infoLog), nullptr, infoLog);
		std::cerr << "Program linking failed:\n" << infoLog << std::endl;
		exit(EXIT_FAILURE);
	}

	glDetachShader(id, vertexShader.getId());
	glDetachShader(id, fragmentShader.getId());
	
}

ShaderProgram::~ShaderProgram(){
	if (id != 0) {
		glDeleteProgram(id);
	}
}

void ShaderProgram::use() const{
	glUseProgram(id);
}

void ShaderProgram::setMat4(const std::string& name, const glm::mat4& mat) const {
    GLint location = glGetUniformLocation(id, name.c_str());
	if(location == -1){
		std::cerr << "Unhandled error! Couldn't find uniform value:"  << name << " in shader: " << this->id<< std::endl;
		exit(EXIT_FAILURE);
	}
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}
