/**
 * @file ShaderProgram.cpp
 *
 * @brief ShaderProgram class implementation
 *
 * @author KUC0438 
  **/

#include "ShaderProgram.h"
#include "core/Camera.h"
#include "Light.h"

ShaderProgram::ShaderProgram(const std::string& vertexSource,const std::string& fragmentSource, Camera* camera):
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

	m_camera = camera;
	if (m_camera) m_camera->addObserver(this);	

	if (m_camera != nullptr){ 
	GLint location = glGetUniformLocation(id,"viewPos");
		if(location != -1){
			viewUfExist = 1;
		}
	location = glGetUniformLocation(id,"lightPos");
		if(location != -1){
			lightUfExist = 1;
		}
	location = glGetUniformLocation(id, "textureUnitID");
		if(location != -1){
			texUfExist = 1;
		}
	location = glGetUniformLocation(id, "modelMatrix");
		if(location != -1){
			modelUfExist = 1;
		}
	location = glGetAttribLocation(id, "instanceMatrix");
		if(location != -1){
			modelUfExist = 1;
		}
	}
}

ShaderProgram::~ShaderProgram(){
	if(m_camera) m_camera->delObserver(this);
	if (id != 0) {
		glDeleteProgram(id);
	}
}

void ShaderProgram::use() const{
    if(texUfExist){
	GLint location = glGetUniformLocation(id,"textureUnitID");
	glUniform1i(location, 0);
	}
	glUseProgram(id);
}

void ShaderProgram::setMat4(const std::string& name, const glm::mat4& mat) const {
    GLint location = glGetUniformLocation(id, name.c_str());
	if(location == -1){
		std::cerr << "Unhandled error! Couldn't find uniform value: "  << name << " in shader: " << this->id<< std::endl;
		exit(EXIT_FAILURE);
	}
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

void ShaderProgram::setVec3(const std::string& name, const glm::vec3& v) const {
	GLint location = glGetUniformLocation(id, name.c_str());
	if(location == -1){
		std::cerr << "Unhandled error! Couldn't find uniform value: "  << name << " in shader: " << this->id<< std::endl;
		exit(EXIT_FAILURE);
	}
    glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(v));
}


void ShaderProgram::setTranslation(const glm::vec3& t) const {
	setVec3("uTranslation", t); 
}

void ShaderProgram::setTranslation(float x, float y, float z) const { 
	setTranslation({x, y, z});
}

void ShaderProgram::setScaling(const glm::vec3& s) const { 
	setVec3("uScale", s); 
}

void ShaderProgram::setScaling(float x, float y, float z) const { 
	setScaling({x, y, z}); 
}

void ShaderProgram::setScaling(float s) const { 
	setScaling({s, s, s}); 
}

void ShaderProgram::setRotation(float angle, const glm::vec3& axis) const {
    setMat4("uRotation", glm::rotate(glm::mat4(1.0f), angle, glm::normalize(axis)));
}
void ShaderProgram::setRotation(const glm::vec3& e) const {
    setMat4("uRotation", glm::mat4_cast(glm::quat(e)));
}

void ShaderProgram::setTime(float time){
	GLint location = glGetUniformLocation(id, "utime");
	if(location == -1){
		std::cerr << "Unhandled error! Couldn't find uniform value:"  << "utime" << " in shader: " << this->id<< std::endl;
		exit(EXIT_FAILURE);
	}
	glUniform1f(location, time);
}

void ShaderProgram::update(const Camera& camera) {
	use();
	setMat4("projectionMatrix",camera.getProjMatrix());
	
	if(modelUfExist){ // If not skybox
	setMat4("viewMatrix",camera.getViewMatrix());
	}else{ // If skybox, then cut translation
	glm::mat4 view = glm::mat4(glm::mat3(camera.getViewMatrix()));
	setMat4("viewMatrix",view);
	}

	if(viewUfExist){ // For lighting
		setVec3("viewPos", camera.getPos());
	}
}

void ShaderProgram::update(const Light& light) {
	use();
	if(lightUfExist){
		setVec3("lightPos", light.getPos());
	}
}