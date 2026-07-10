#pragma once

// opengl
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

// standard
#include <string>
#include <utility>

#include "constants.h"

unsigned int createTexture(const char*);

GLFWwindow* initWindow(int, int);

std::string getTexturePath(std::string);