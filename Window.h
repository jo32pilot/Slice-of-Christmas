#ifndef _WINDOW_H_
#define _WINDOW_H_

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <memory>

#include "Object.h"
#include "shader.h"
#include "Cube.h"
#include "loadBox.h"

#define WALK_SPEED 200.0f

class Window
{
public:
	static int width;
	static int height;
	static const char* windowTitle;
	static glm::mat4 projection;
	static glm::mat4 view;
	static glm::vec3 eye, center, up;
	static GLuint program, projectionLoc, viewLoc, modelLoc, colorLoc;
	static GLuint skyboxProgram, skyboxProjectionLoc, skyboxViewLoc;
	static GLuint textureID;
	static Cube* skybox;
	static std::vector<std::string> faces;
	static GLfloat deltaTime;
	static GLfloat lastFrame;

	static bool initializeProgram();
	static bool initializeObjects();
	static void cleanUp();
	static GLFWwindow* createWindow(int width, int height);
	static void resizeCallback(GLFWwindow* window, int width, int height);
	static void idleCallback();
	static void displayCallback(GLFWwindow*);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void Window::cursorPosFun(GLFWwindow* wind, double x, double y);
	static void processInput(GLFWwindow *window);

};

#endif
