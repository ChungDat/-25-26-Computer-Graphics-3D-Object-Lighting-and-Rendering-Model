#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include <string>

class Light
{
protected:
	// color

	glm::vec3 color;
	glm::vec3 storedColor; // To remember the color when disabled

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	void updateAttribute();

	// position and direction

	glm::vec3 position;
	glm::vec3 direction;
	float yaw; // left right - in degree
	float pitch; // up down - in degree

	// orbital motion

	bool orbitalMotion = false;
	float rotationalFreq = 45; // degree
	float radius = 0.8f;

	glm::vec3 orbitCenter; // same as position

	void updateOrbitalPosition(const float time);
	void updateOrbitalDirection(const float time);

	// opengl

	static int nextID;
	int ID;

	static unsigned int VAO, VBO, EBO;
	static bool initialized;

	Shader& shader;

	// visibility

	bool enabled;

public:
	Light(Shader& _shader);
	virtual ~Light();

	// ==============
	// SETTER
	// ==============

	// color

	void setColor(const glm::vec3 _color);
	
	// position and direction

	virtual void setPosition(const glm::vec3 _pos);
	virtual void setDirection(const float _yaw, const float _pitch);
	virtual void setDirection(const glm::vec3 _direction);
	virtual void setYaw(const int _angle);
	virtual void setPitch(const int _angle);

	// orbital motion

	void setOrbital(const bool _orbitalMotion);
	void setRadius(const float _radius); // orbital motion radius
	void setRotationalFreq(const float _freq);

	// opengl

	static void initBuffers();

	// visibility

	void enable();
	void disable();
	bool isEnabled() const;

	// ==============
	// GETTER
	// ==============

	// position and direction

	virtual glm::vec3 getPosition() const;
	virtual glm::vec3 getDirection() const;
	virtual float getX() const;
	virtual float getY() const;
	virtual float getZ() const;
	virtual int getYaw() const;
	virtual int getPitch() const;

	// color

	glm::vec3 getColor() const;
	//glm::vec3& getColor_Ref();
	glm::vec3 getStoredColor() const;

	glm::vec3 getAmbient() const;
	glm::vec3 getDiffuse() const;
	glm::vec3 getSpecular() const;

	// orbital motion

	float getRadius() const;
	float getRotationalFreq() const;

	// ==============
	// OPENGL
	// ==============

	virtual std::string getType() const = 0;
	int getID() const;
	unsigned int getVAO() const;
	unsigned int getVertexCount() const;

	virtual void draw(const float time) = 0;

	virtual void updateObjectShader(Shader& shader, unsigned int typeCount) const = 0;
};

class DirectionalLight : public Light {
public:
	DirectionalLight(Shader& _shader);
	virtual ~DirectionalLight();

	// ==============
	// SETTER
	// ==============

	// position

	void setPosition(const glm::vec3 _pos) override;

	// ==============
	// GETTER
	// ==============

	// position

	glm::vec3 getPosition() const override;
	float getX() const override;
	float getY() const override;
	float getZ() const override;

	// ==============
	// OPENGL
	// ==============

	std::string getType() const override;

	void draw(const float time) override;

	void updateObjectShader(Shader& shader, unsigned int typeCount) const override;
};

class PointLight : public Light {
protected:
	float constant;
	float linear;
	float quadratic;

public:
	PointLight(Shader& _shader);
	virtual ~PointLight();

	// ==============
	// SETTER
	// ==============

	// direction

	void setDirection(const float _yaw, const float _pitch) override;
	void setDirection(const glm::vec3 _direction) override;
	void setYaw(const int _angle) override;
	void setPitch(const int _angle) override;

	// attenuation
	// 
	//void setConstant(const float _constant);
	//void setLinear(const float _linear);
	//void setQuadratic(const float _quadratic);

	// ==============
	// GETTER
	// ==============

	// attenuation

	float getConstant() const;
	float getLinear() const;
	float getQuadratic() const;

	// direction

	glm::vec3 getDirection() const override;

	// ==============
	// OPENGL
	// ==============

	std::string getType() const override;

	void draw(const float time) override;

	void updateObjectShader(Shader& objectShader, unsigned int typeCount) const override;
};

class SpotLight : public PointLight {
protected:
	float innerCutOff;
	float outerCutOff;

public:
	SpotLight(Shader& _shader);
	virtual ~SpotLight();

	// ==============
	// SETTER
	// ==============

	// direction

	void setDirection(const float _yaw, const float _pitch) override;
	void setDirection(const glm::vec3 _direction) override;
	void setYaw(const int _angle) override;
	void setPitch(const int _angle) override;


	void setInnerCutOff(const float _inner);
	void setOuterCutOff(const float _outer);

	// ==============
	// GETTER
	// ==============

	// direction

	glm::vec3 getDirection() const override;


	float getInnerCutOff() const;
	float getOuterCutOff() const;

	// ==============
	// OPENGL
	// ==============

	std::string getType() const override;

	void updateObjectShader(Shader& shader, unsigned int typeCount) const override;
};