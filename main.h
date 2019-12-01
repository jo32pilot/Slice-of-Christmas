#ifndef _MAIN_H_
#define _MAIN_H_

#define STB_IMAGE_IMPLEMENTATION

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>
#include <ctime>
#include "Window.h"

#endif
