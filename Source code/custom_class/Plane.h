#pragma once

#include "Axis.h"
#include <vector>

class Plane: public Axis
{
protected:
	int vertexCount = 0;
	void initBuffers(float, int);
public:
	Plane(Shader& shader) : Axis(shader) {};
	virtual ~Plane();
	void draw(const glm::mat4& view, const glm::mat4& projection);

	unsigned int getVAO();
};

