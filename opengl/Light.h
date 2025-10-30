#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include <string>

class Light
{
protected:
	glm::vec3 color;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	static unsigned int VAO, VBO, EBO;
	static bool initialized;
	bool enabled;
	glm::vec3 storedColor; // To remember the color when disabled
	Shader& shader;
public:
	Light(Shader& shader);
	virtual void setPosition(const glm::vec3& _pos) {};
	virtual void setDirection(const glm::vec3& _dir) {};
	void setColor(const glm::vec3& _color);
	static void initBuffers();
	virtual void getPosition() {};
	virtual void getDirection() {};
	unsigned int getVAO() const;
	unsigned int getVertexCount() const;

	void enable();
	void disable();
	bool isEnabled() const;

	glm::vec3 getPosition() const;
	glm::vec3 getAmbient() const;
	glm::vec3 getDiffuse() const;
	glm::vec3 getSpecular() const;

	virtual void draw(const glm::mat4& view, const glm::mat4& projection) const {};
	virtual void update(float time) {};                      // <- new
	virtual void updateObjectShader(Shader& shader, unsigned int typeCount) const = 0;
	virtual void setCircularMotion(bool _circularMotion) {};
	virtual ~Light();
};

class DirectionalLight : public Light {
protected:
	glm::vec3 direction;
public:
	DirectionalLight(Shader& shader);
	void draw(const glm::mat4& view, const glm::mat4& projection) const override {};
	void setDirection(const glm::vec3& _dir);

	glm::vec3 getDirection() const;

	void updateObjectShader(Shader& shader, unsigned int typeCount) const;
};

class PointLight :public Light {
protected:
	glm::vec3 position;
	float constant;
	float linear;
	float quadratic;
	bool circularMotion = false;
	float rotationalFreq = 45; // degree
	float radius = 0.8f;

	glm::vec3 orbitCenter;     // <- new: center of circular motion
public:
	PointLight(Shader& shader);
	void draw(const glm::mat4& view, const glm::mat4& projection) const override;
	void update(float time) override;                     // <- new override
	void setPosition(const glm::vec3& _pos) override;
	void setRadius(float _radius);
	void setRotationalFreq(float _freq);
	void setCircularMotion(bool _circularMotion) override;

	float getConstant() const;
	float getLinear() const;
	float getQuadratic() const;

	void updateObjectShader(Shader& objectShader, unsigned int typeCount) const;
};

class SpotLight : public PointLight {
protected:
	glm::vec3 direction;
	float innerCutOff;
	float outerCutOff;

public:
	SpotLight(Shader& shader);
	void setDirection(const glm::vec3& _dir);
	void setInnerCutOff(float _inner);
	void setOuterCutOff(float _outer);

	float getInnerCutOff() const;
	float getOuterCutOff() const;

	void updateObjectShader(Shader& shader, unsigned int typeCount) const;
};