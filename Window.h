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
#include "Terrain.h"
#include "shader.h"
#include "Cube.h"
#include "loadBox.h"

#define WALK_SPEED 200.0f
#define CENTER_DIVIDER 2

class Window
{
public:
	static int width;
	static int height;
	static const char* windowTitle;
	static glm::mat4 projection;
	static glm::mat4 view;
	static glm::vec3 eye, center, up;
	static GLuint program, projectionLoc, viewLoc, modelLoc, colorLoc, normalColoringLoc;
	static GLuint skyboxProgram, skyboxProjectionLoc, skyboxViewLoc;
	static GLuint textureID;
	static Cube* skybox;
	static std::vector<std::string> faces;
	static GLfloat deltaTime;
	static GLfloat lastFrame;
	static GLfloat lastX, lastY;
	static GLfloat yaw, pitch;
	static GLboolean firstMouse;
	static double fov;
	static Terrain* terrain;
	static GLboolean normalColoring;
	static GLuint terrainTexture;

	static bool initializeProgram();
	static bool initializeObjects();
	static void cleanUp();
	static GLFWwindow* createWindow(int width, int height);
	static void resizeCallback(GLFWwindow* window, int width, int height);
	static void idleCallback();
	static void displayCallback(GLFWwindow*);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void processInput(GLFWwindow *window);
	static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);


};

#endif
