#pragma once
#include <glad/gl.h>

//Include the standard C++ headers  
#include <fstream>
#include <string>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "core/Observer.h"



class Camera;
class Light;
class ShaderProgram : public Observer<Camera>, public Observer<Light> {
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

		void update(const Camera& camera) override; 
		void update(const Light& light) override;

	private:
		Camera* m_camera;
		Shader vertexShader;
		Shader fragmentShader;
		GLint viewUfExist{0};
		GLint lightUfExist{0};
		GLint texUfExist{0};
		GLint modelUfExist{0};
		GLuint id{0};
	
};



