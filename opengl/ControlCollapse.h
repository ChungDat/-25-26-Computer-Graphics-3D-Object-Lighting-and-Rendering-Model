#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <vector>
#include <string>
#include "Light.h"
#include "Object.h"

class ControlCollapse
{
protected:
	const char* label;
public:
	ControlCollapse();
	ControlCollapse(const char*);
	virtual void show() = 0;
};

class PresetScenesCollapse : public ControlCollapse {
protected:
	std::vector<const char*> scene = { "Empty Plane", "Classroom", "Living Room", "House" };
	int currentSelect = 0;
public:
	PresetScenesCollapse(const char*);
	void show();
};

class LightCollapse : public ControlCollapse {
protected:
	std::vector<const char*> lightType = { "Directional Light", "Point Light", "Spot Light" };
	std::vector<Light*>& lightList;
	Shader& shader;
	void addLight(const char*);
public:
	LightCollapse(const char*, std::vector<Light*>&, Shader&);
	void show();
};

class ObjectCollapse : public ControlCollapse {
protected:
	std::vector<const char*> object = { "Cube", "Sphere", "Pyramid", "Cylinder", "Lamp", "Flower Pot", "Glass" };
	std::vector<Object*>& objectList;
	Shader& shader;
	void addObject(const char*);
public:
	ObjectCollapse(const char*, std::vector<Object*>&, Shader&);
	void show();
};

class ObjectProperties : public ControlCollapse {
protected:
	unsigned int objectID;
public:
	ObjectProperties(const char*);
	void show(unsigned int);
};

class LightProperties : public ControlCollapse {
protected:
	unsigned int lightID;
public:
	LightProperties(const char*);
	void show();
};