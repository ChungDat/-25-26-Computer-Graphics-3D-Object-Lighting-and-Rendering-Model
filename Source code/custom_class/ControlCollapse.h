#pragma once
#include "../imgui.h"
#include "../imgui_impl_glfw.h"
#include "../imgui_impl_opengl3.h"
#include <vector>
#include <string>
#include <map>
#include "Light.h"
#include "Axis.h"
#include "Object.h"

class ControlCollapse
{
protected:
	const char* label;

public:
	ControlCollapse();
	ControlCollapse(const char*);
	virtual ~ControlCollapse() {};

	virtual void show() = 0;
};

class PresetScenesCollapse : public ControlCollapse {
protected:
	std::vector<const char*> scene = { "Empty Plane", "Classroom", "Living Room", "House" };
	int currentSelect = 0;

public:
	PresetScenesCollapse(const char*);
	virtual ~PresetScenesCollapse() {};
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
	virtual ~LightCollapse() {};
	void show();
};

class ObjectCollapse : public ControlCollapse {
protected:
	std::vector<const char*> object = { "Cube", "Sphere", "Pyramid", "Cylinder", "Lamp", "Flower Pot", "Glass" };
	std::vector<Object*>& objectList;
	void addObject(const char*);

	Shader* shader;

public:
	ObjectCollapse(const char*, std::vector<Object*>&, Shader*);
	virtual ~ObjectCollapse() {};
	void show();
};

class ObjectProperties : public ControlCollapse {
protected:
	std::vector<Object*>& objectList;
	int currentSelect = 0;

public:
	ObjectProperties(const char*, std::vector<Object*>&);
	virtual ~ObjectProperties() {};
	void show();
};

class LightProperties : public ControlCollapse {
protected:
	std::vector<Light*>& lightList;
	int currentSelect = 0;

public:
	LightProperties(const char*, std::vector<Light*>&);
	virtual ~LightProperties() {};
	void show();
};

class Settings : public ControlCollapse {
protected:
	Light* flashLight;
	Axis& axis;
	Cube& boxRoom;
	Shader*& objectShader;

	const std::vector<std::string>& shaderModel;
	std::vector<Shader*>& shaderList;

	int& currentRasterizationMode;

	int currentLighting;

public:
	Settings(const char*, Light*, Axis&, Cube&, Shader*&, const std::vector<std::string>&, std::vector<Shader*>&, int&);
	virtual ~Settings() {};
	void show();
};