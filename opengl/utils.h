#pragma once

// openGL libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

// standard
#include <string>
#include <iostream>

unsigned int createTexture(const char*);

GLFWwindow* initWindow(int, int);