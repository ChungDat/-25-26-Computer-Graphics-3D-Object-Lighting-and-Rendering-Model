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

	static int nextID;
	int ID;
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
	virtual void setRadius(float _radius) {};
	virtual void setInnerCutOff(float _inner) {};
	virtual void setOuterCutOff(float _outer) {};
	void updateAttribute();
	static void initBuffers();
	unsigned int getVAO() const;
	unsigned int getVertexCount() const;

	void enable();
	void disable();
	bool isEnabled() const;

	virtual glm::vec3 getPosition() const;
	virtual glm::vec3 getDirection() const;
	virtual float getX() const;
	virtual float getY() const;
	virtual float getZ() const;
	glm::vec3 getColor() const;
	glm::vec3& getColor_Ref();
	glm::vec3 getStoredColor() const;
	glm::vec3 getAmbient() const;
	glm::vec3 getDiffuse() const;
	glm::vec3 getSpecular() const;
	virtual float getRadius() const;
	virtual float getInnerCutOff() const;
	virtual float getOuterCutOff() const;
	virtual std::string getType() const = 0;
	int getID() const;

	//virtual void draw(const glm::mat4& view, const glm::mat4& projection) const {};
	virtual void draw(float time) {};
	virtual void updatePosition(float time) {};                      // <- new
	virtual void updateObjectShader(Shader& shader, unsigned int typeCount) const = 0;
	virtual void setCircularMotion(bool _circularMotion) {};
	virtual ~Light();
};

class DirectionalLight : public Light {
protected:
	glm::vec3 direction;

public:
	DirectionalLight(Shader& shader);
	//void draw(const glm::mat4& view, const glm::mat4& projection) const override {};
	void draw(float time) override {};
	void setDirection(const glm::vec3& _dir);

	glm::vec3 getDirection() const override;
	std::string getType() const override;

	void updateObjectShader(Shader& shader, unsigned int typeCount) const;
};

class PointLight : public Light {
protected:
	glm::vec3 position;
	float constant;
	float linear;
	float quadratic;
	bool orbitalMotion = false;
	float rotationalFreq = 45; // degree
	float radius = 0.8f;

	glm::vec3 orbitCenter;     // <- new: center of circular motion

public:
	PointLight(Shader& shader);
	//void draw(const glm::mat4& view, const glm::mat4& projection) const override;
	void draw(float time) override;
	void updatePosition(float time) override;                     // <- new override
	void setPosition(const glm::vec3& _pos) override;
	void setRadius(float _radius) override;
	void setRotationalFreq(float _freq);
	void setCircularMotion(bool _circularMotion) override;
	std::string getType() const override;

	float getConstant() const;
	float getLinear() const;
	float getQuadratic() const;
	glm::vec3 getPosition() const override;
	float getX() const override;
	float getY() const override;
	float getZ() const override;
	float getRadius() const;

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
	void setInnerCutOff(float _inner) override;
	void setOuterCutOff(float _outer) override;

	float getInnerCutOff() const;
	float getOuterCutOff() const;
	glm::vec3 getDirection() const override;
	std::string getType() const override;

	void updateObjectShader(Shader& shader, unsigned int typeCount) const;
};