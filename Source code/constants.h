#pragma once

// opengl
#include <glm/glm.hpp>

// standard
#include <unordered_map>
#include <string_view>
#include <utility>
#include <iostream>

constexpr int WIDTH = 1280;
constexpr int HEIGHT = 720;

constexpr int NR_DIR_LIGHTS = 4;
constexpr int NR_POINT_LIGHTS = 8;
constexpr int NR_SPOT_LIGHTS = 4;

constexpr int NR_OBJECTS = 20;

const std::unordered_map<std::string_view, std::string_view> TEXTURE{
	{"wood crate", "custom_texture/container2.png"},
};

const std::unordered_map< std::string_view, glm::vec3> MATERIAL = {
	{"gold", glm::vec3(1.0f, 0.71f, 0.29f)},
	{"copper", glm::vec3(0.95, 0.64, 0.54)},
};

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;