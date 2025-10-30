#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "Shader.h"

class Axis
{
protected:
	unsigned int VAO, VBO, EBO;
	bool initialized;
	Shader& shader;
	void initBuffers();
	bool enabled;
public:
	Axis(Shader& shader);
	void draw(const glm::mat4& view, const glm::mat4& projection) ;

	unsigned int getVAO();
	void enable();
	void disable();
	bool isEnabled();
};

