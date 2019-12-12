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
#include "PlayerBound.h"
#include "Model.hpp"
#include "Robot.h"

#define WALK_SPEED 200.0f
#define CENTER_DIVIDER 2
#define GRAVITY -300
#define JUMP_POWER 100
#define TERRAIN_HEIGHT 500
#define TREE_SIZE 200
#define PLAYER_HEIGHT 20
#define FLOOR_OFFSET 50
#define ENEMY_SPEED 125.0f
#define ENEMY_SPAWN_Z -800

typedef enum collision_type {
	WEAPON, 
	PLAYER,
	ENEMY,
	COTTAGE,
	TREE,
	NONE
} CollisionType;

typedef struct collision_info {
	CollisionType type;
	GLint cottangeInd;
} CollisionInfo;

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
	static GLuint importedProgram, importedProjectionLoc, importedViewLoc, importedModelLoc;
	static GLuint noTexProgram, noTexProjectionLoc, noTexViewLoc, noTexModelLoc, noTexColorLoc;
	static GLuint materialProgram, materialProjectionLoc, materialViewLoc, materialModelLoc;
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
	static GLfloat upwardsSpeed;
	static GLboolean inAir;
	static Model* tree;
	static Model* cottage;
	static Model* sphere;
	static GLboolean debugBounds;
	static std::vector<Model*> models;
	static PlayerBound p1;
	static std::vector<glm::vec3> cottagePlacements;
	static std::vector<glm::mat4> cottageMovements;
	static Model* enemy;
	static CollisionInfo recentCollision;
	static GLfloat redTime;

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
	static void setDeltaTime();
	static GLboolean assertInSkybox(glm::vec3 movedEye);
	static CollisionInfo assertPlayerCollision(glm::vec3 movedEye);
	static CollisionType assertEnemyCollision();

};

#endif
