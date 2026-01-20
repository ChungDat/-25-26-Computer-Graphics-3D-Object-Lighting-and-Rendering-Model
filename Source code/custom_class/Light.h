#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include <string>
#include "../constants.h"

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
	glm::vec3 baseDir;
	float yaw; // left right - in degree
	float pitch; // up down - in degree

	// transformation
	float nearPlane, farPlane;
	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 lightSpaceMatrix;

	virtual void setYaw(const int _angle);
	virtual void setPitch(const int _angle);

	// orbital motion
	bool orbitalMotion;
	float orbitAngle; // radian
	float rotationalFreq; // degree
	float radius;

	glm::vec3 orbitCenter; // same as position

	void updateOrbitalPosition();
	virtual void updateOrbitalDirection();

	// opengl
	static int nextID;
	int ID;

	static unsigned int VAO, VBO, EBO;
	unsigned int depthMap;

	static bool initialized;

	// visibility
	bool enabled;

	// shadow flag
	bool castsShadow;

public:
	Light(unsigned int& depthMapFBO);
	virtual ~Light();

	// ==============
	// SETTER
	// ==============

	// color

	void setColor(const glm::vec3 _color);
	
	// position and direction

	virtual void setPosition(const glm::vec3 _pos);
	virtual void setDirection(const float _pitch, const float _yaw);
	virtual void setDirection(const glm::vec3 _direction);

	// orbital motion

	void setOrbital(const bool _orbitalMotion);
	void setRadius(const float _radius); // orbital motion radius
	void setRotationalFreq(const float _freq);

	// shadow control
	void setCastShadow(const bool enable);
	bool isShadowCaster() const;

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

	// transformation

	float getNearPlane() const;
	float getFarPlane() const;
	glm::mat4 getView() const;
	glm::mat4 getProjection() const;
	glm::mat4 getLightMatrix() const;

	// color

	glm::vec3 getColor() const;
	//glm::vec3& getColor_Ref();
	glm::vec3 getStoredColor() const;

	glm::vec3 getAmbient() const;
	glm::vec3 getDiffuse() const;
	glm::vec3 getSpecular() const;

	// orbital motion

	bool isOrbital() const;
	float getRadius() const;
	float getRotationalFreq() const;

	// ==============
	// OPENGL
	// ==============

	virtual std::string getType() const = 0;
	int getID() const;
	unsigned int getVAO() const;
	unsigned int getDepthMap() const;
	unsigned int getVertexCount() const;

	virtual void draw(Shader& shader, const float deltaTime) = 0;
	virtual void updateObjectShader(Shader*& shader, unsigned int typeCount) const = 0;
	void updateDepthShader(Shader*& shader);
};

class DirectionalLight : public Light {
protected:
	float distance = 20.0f;

public:
	DirectionalLight(unsigned int& depthMapFBO);
	virtual ~DirectionalLight();

	// ==============
	// SETTER
	// ==============

	// position

	void setPosition(const glm::vec3 _pos) override;
	void setDirection(const float _pitch, const float _yaw) override;

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

	void draw(Shader& shader, const float deltaTime) override;

	void updateObjectShader(Shader*& shader, unsigned int typeCount) const override;
};

class PointLight : public Light {
protected:
	float constant;
	float linear;
	float quadratic;

public:
	PointLight(unsigned int& depthMapFBO);
	virtual ~PointLight();

	// ==============
	// SETTER
	// ==============

	// direction
	void setPosition(const glm::vec3 _pos) override;
	void setDirection(const float _pitch, const float _yaw) override;
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

	void draw(Shader& shader, const float deltaTime) override;

	void updateObjectShader(Shader*& objectShader, unsigned int typeCount) const override;
};

class SpotLight : public PointLight {
protected:
	float innerCutOff;
	float outerCutOff;
	
	void setYaw(const int _angle) override;
	void setPitch(const int _angle) override;

	void updateOrbitalDirection() override;

public:
	SpotLight(unsigned int& depthMapFBO);
	virtual ~SpotLight();

	// ==============
	// SETTER
	// ==============

	// direction

	void setDirection(const float _pitch, const float _yaw) override;
	void setDirection(const glm::vec3 _direction) override;

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

	void draw(Shader& shader, const float deltaTime) override;

	void updateObjectShader(Shader*& shader, unsigned int typeCount) const override;
};