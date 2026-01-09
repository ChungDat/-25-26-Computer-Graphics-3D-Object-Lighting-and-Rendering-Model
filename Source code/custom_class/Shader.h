#pragma once
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
protected:
	unsigned int ID;
	const char* vertexPath;
	const char* fragmentPath;
public:
	Shader(const char*, const char*);
	// Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;
	// use/activate the shader
	void use();
	// utility uniform functions
	void setBool(const std::string&, bool) const;
	void setInt(const std::string&, int) const;
	void setFloat(const std::string&, float) const;
	void setVec3f(const std::string&, float, float, float) const;
	void setMat4fv(const std::string&, glm::mat4) const;
	void setVec4fv(const std::string&, glm::vec4) const;
	void setVec3fv(const std::string&, glm::vec3) const;
	unsigned int getID();
	virtual ~Shader();
};

