#pragma once
#include <glad/gl.h>

//Include the standard C++ headers  
#include <fstream>
#include <string>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "core/Observer.h"


class Camera;
class ShaderProgram : public Observer {
	public:
		ShaderProgram(const std::string& vertexSource,const std::string& fragmentSource, Camera* camera = nullptr);
		~ShaderProgram();
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;

		void use() const;
		void setMat4(const std::string& name, const glm::mat4& mat) const;
		void setVec3(const std::string& name, const glm::vec3& vec) const;

		void setTranslation(const glm::vec3& t) const;
		void setTranslation(float x, float y, float z) const;

		void setScaling(const glm::vec3& s) const;
		void setScaling(float x, float y, float z) const;
		void setScaling(float uniformScale) const;

		void setRotation(float angleRadians, const glm::vec3& axis) const;
		void setRotation(const glm::vec3& eulerRadians) const;   

		void setTime(float time);

		void update(Camera& camera) const override; 

	private:
		Camera* m_camera{nullptr};
		Shader vertexShader;
		Shader fragmentShader;
		GLuint id{0};
	
};



