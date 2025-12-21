#pragma once

#include "Light.h"
#include "Object.h"

struct LightContainer {
	Light* light;
	LightContainer* next;
};

struct ObjectContainer {
	Object* object;
	ObjectContainer* next;
};