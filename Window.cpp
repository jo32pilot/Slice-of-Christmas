#define STB_IMAGE_IMPLEMENTATION

#include "Window.h"

int Window::width;
int Window::height;

const char* Window::windowTitle = "GLFW Starter Project";

// Objects to display.
Cube* Window::skybox;
Terrain* Window::terrain;
Model* Window::tree;
Model* Window::cottage;
Model* Window::sphere;
Model* Window::enemy;
std::vector<Model*> Window::models;

glm::mat4 Window::projection; // Projection matrix.

glm::vec3 Window::eye(-500, 0, 3); // Camera position.
glm::vec3 Window::center(0, 0, -1); // The point we are looking at.
glm::vec3 Window::up(0, 1, 0); // The up direction of the camera.

// View matrix, defined by eye, center and up.
glm::mat4 Window::view = glm::lookAt(Window::eye, Window::center + Window::eye, Window::up);

GLuint Window::program; // The shader program id.
GLuint Window::projectionLoc; // Location of projection in shader.
GLuint Window::viewLoc; // Location of view in shader.
GLuint Window::modelLoc; // Location of model in shader.
GLuint Window::colorLoc; // Location of color in shader.
GLuint Window::normalColoringLoc; // Location of normalColoring boolean in shader

GLuint Window::skyboxProgram;
GLuint Window::skyboxProjectionLoc;
GLuint Window::skyboxViewLoc;

GLuint Window::importedProgram;
GLuint Window::importedProjectionLoc;
GLuint Window::importedViewLoc;
GLuint Window::importedModelLoc;

GLuint Window::noTexProgram; 
GLuint Window::noTexProjectionLoc;
GLuint Window::noTexViewLoc; 
GLuint Window::noTexModelLoc;
GLuint Window::noTexColorLoc;

GLuint Window::materialProgram;
GLuint Window::materialProjectionLoc;
GLuint Window::materialViewLoc;
GLuint Window::materialModelLoc;

GLuint Window::textureID;

std::vector<std::string> Window::faces = {
	"assets/front.png",
	"assets/back.png",
	"assets/up.png",
	"assets/down.png",
	"assets/right.png",
	"assets/left.png"
};

GLfloat Window::deltaTime = 0.0f;
GLfloat Window::lastFrame = 0.0f;
GLfloat Window::lastX;
GLfloat Window::lastY;
GLfloat Window::yaw = 0;
GLfloat Window::pitch = 0;
GLboolean Window::firstMouse = true;
double Window::fov = 60.0f;
GLboolean Window::normalColoring = false;
GLuint Window::terrainTexture;
GLfloat Window::upwardsSpeed = 0;
GLboolean Window::inAir = false;
GLboolean Window::debugBounds = false;
PlayerBound Window::p1 = {1.0f, eye};
std::vector<glm::vec3> Window::cottagePlacements = {
	{200, 0, 300},
	{700, 0, 0},
	{-500, 0, -700},
	{400, 0, 200},
	{100, 0, -1000},
	{250, 0, 750},
	{750, 0, 750},
	{-200, 0, -900},
	{-300, 0, -400}
};
std::vector<glm::mat4> Window::cottageMovements;

bool Window::initializeProgram() {
	// Create a shader program with a vertex shader and a fragment shader.
	program = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
	skyboxProgram = LoadShaders("shaders/skybox.vert", "shaders/skybox.frag");
	importedProgram = LoadShaders("shaders/imported.vert", "shaders/imported.frag");
	noTexProgram = LoadShaders("shaders/no_texture.vert", "shaders/no_texture.frag");
	materialProgram = LoadShaders("shaders/material.vert", "shaders/material.frag");

	// Check the shader program.
	if (!program)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}

	// Activate the shader program.
	glUseProgram(program);
	// Get the locations of uniform variables.
	projectionLoc = glGetUniformLocation(program, "projection");
	viewLoc = glGetUniformLocation(program, "view");
	modelLoc = glGetUniformLocation(program, "model");
	colorLoc = glGetUniformLocation(program, "color");
	normalColoringLoc = glGetUniformLocation(program, "normalColoring");

	glUseProgram(skyboxProgram);
	skyboxProjectionLoc = glGetUniformLocation(skyboxProgram, "projection");
	skyboxViewLoc = glGetUniformLocation(skyboxProgram, "view");

	glUseProgram(importedProgram);
	importedProjectionLoc = glGetUniformLocation(importedProgram, "projection");
	importedViewLoc = glGetUniformLocation(importedProgram, "view");
	importedModelLoc = glGetUniformLocation(importedProgram, "model");

	glUseProgram(noTexProgram);
	noTexProjectionLoc = glGetUniformLocation(noTexProgram, "projection");
	noTexViewLoc = glGetUniformLocation(noTexProgram, "view");
	noTexModelLoc = glGetUniformLocation(noTexProgram, "model");
	noTexColorLoc = glGetUniformLocation(noTexProgram, "color");

	glUseProgram(materialProgram);
	materialProjectionLoc = glGetUniformLocation(materialProgram, "projection");
	materialViewLoc = glGetUniformLocation(materialProgram, "view");
	materialModelLoc = glGetUniformLocation(materialProgram, "model");

	textureID = loadBox(faces);
	terrainTexture = loadTextures("assets/snow_floor.jpg");

	return true;
}

bool Window::initializeObjects()
{
	skybox = new Cube(5.0f);
	terrain = new Terrain();
	tree = new Model("assets/test_tree/ChristmasTree.obj");
	cottage = new Model("assets/cottage/Snow\ Covered\ CottageOBJ.obj");
	sphere = new Model("assets/sphere.obj");
	enemy = new Model("assets/enemy/source/untitled.fbx");

	sphere->setColor(glm::vec3(1, 0, 1));

	models.push_back(tree);

	cottage->boundingSphere = sphere;
	tree->boundingSphere = sphere;
	enemy->boundingSphere = sphere;

	// Set up tree
	GLfloat terrainHeight = terrain->getHeightOfTerrain(0, 0);
	glm::vec3 treeMin = tree->getSmallestCoord();
	glm::mat4 treeMove = glm::translate(glm::vec3(0, terrainHeight - treeMin.y - FLOOR_OFFSET, 0));
	glm::mat4 treeScale = glm::scale(glm::vec3(TREE_SIZE));
	tree->setModel(treeMove * tree->getModel() * treeScale);
	tree->updateMembersScale(treeScale);
	tree->updateCenter(treeMove);

	// Create translation matrices for each cottage and store for collision 
	// detection later.
	for (int i = 0; i < cottagePlacements.size(); ++i) {
		glm::vec3 place = cottagePlacements[i];
		glm::vec3 cottageMin = cottage->getSmallestCoord();
		glm::mat4 placeCottage = glm::translate(place);
		terrainHeight = terrain->getHeightOfTerrain(place.x, place.z);
		glm::mat4 cottageMove = glm::translate(glm::vec3(0, terrainHeight - cottageMin.y, 0));
		glm::mat4 totalMovement = placeCottage * cottageMove;
		cottageMovements.push_back(totalMovement);
	}

	// Transform enemy
	
	// rotate to get correct orientation of min, max, center points
	glm::mat4 rotate = glm::rotate(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	enemy->updateMembersRotate(rotate);

	// Get on right y level
	glm::vec3 enemyMin = enemy->getSmallestCoord();
	terrainHeight = terrain->getHeightOfTerrain(0, ENEMY_SPAWN_Z);
	glm::mat4 enemyMove = glm::translate(glm::vec3(0, terrainHeight - enemyMin.y, 0));

	glm::mat4 enemySpawn = enemyMove * glm::translate(glm::vec3(0, 0, ENEMY_SPAWN_Z));
	glm::mat4 scale = glm::scale(glm::vec3(10));

	enemy->setModel(enemySpawn * scale * rotate * enemy->getModel());
	enemy->updateMembersScale(scale);
	enemy->updateCenter(enemySpawn);

	glm::vec3 min = enemy->getSmallestCoord();
	glm::vec3 max = enemy->getLargestCoord();
	glm::vec3 cent = enemy->getTrueCenter();
	glm::vec3 mid = enemy->getMidCoord();

	return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
	delete skybox;
	delete terrain;
	delete tree;
	delete cottage;
	delete sphere;
	delete enemy;

	// Delete the shader program.
	glDeleteProgram(program);
	glDeleteProgram(skyboxProgram);
	glDeleteProgram(importedProgram);
	glDeleteProgram(noTexProgram);
	glDeleteProgram(materialProgram);
}

GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	lastX = width / CENTER_DIVIDER;
	lastY = height / CENTER_DIVIDER;

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ 
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height); 
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size.
	glViewport(0, 0, width, height);

	// Set the projection matrix.
	Window::projection = glm::perspective(glm::radians(fov), 
		double(width) / (double)height, 1.0, 2250.0);
}

void Window::idleCallback()
{
	GLfloat enemyFrameDist = ENEMY_SPEED * deltaTime;
	glm::vec3 moveTowards = enemyFrameDist * glm::normalize(eye - enemy->getTrueCenter());
	glm::mat4 move = glm::translate(moveTowards);
	enemy->setModel(move * enemy->getModel());
	enemy->updateCenter(move);

	// Gravity to pull camera down
	upwardsSpeed += (GRAVITY * deltaTime);
	glm::vec3 movedEye = glm::vec3(eye[0], eye[1] + upwardsSpeed * deltaTime, eye[2]);
	if (!assertPlayerCollision(movedEye)) {
		eye = movedEye;
	}
	GLfloat terrainHeight = terrain->getHeightOfTerrain(eye[0], eye[2]);
	if (eye[1] < terrainHeight + PLAYER_HEIGHT) {
		inAir = false;
		upwardsSpeed = 0;
		eye[1] = terrainHeight + PLAYER_HEIGHT;
	}
}


void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	setDeltaTime();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Draw terrain
	glUseProgram(program);
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(terrain->getModel()));
	glUniform3fv(colorLoc, 1, glm::value_ptr(terrain->getColor()));
	glUniform1i(normalColoringLoc, normalColoring);
	//terrain->draw(terrainTexture);

	// Draw skybox
	glDepthMask(GL_FALSE);
	glUseProgram(skyboxProgram);
	glUniformMatrix4fv(skyboxProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(skyboxViewLoc, 1, GL_FALSE, glm::value_ptr(view));
	skybox->draw(textureID);

	glDisable(GL_CULL_FACE);

	// Draw tree
	glUseProgram(materialProgram);
	glUniformMatrix4fv(materialProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(materialViewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(materialModelLoc, 1, GL_FALSE, glm::value_ptr(tree->getModel()));
	glm::vec3 lightColor = glm::vec3(1.0f);
	glUniform3fv(glGetUniformLocation(materialProgram, "viewPos"), 1, glm::value_ptr(eye));
	glUniform3fv(glGetUniformLocation(materialProgram, "light.position"), 1, glm::value_ptr(glm::vec3(100)));
	glUniform3fv(glGetUniformLocation(materialProgram, "light.ambient"), 1, glm::value_ptr(lightColor));
	glUniform3fv(glGetUniformLocation(materialProgram, "light.diffuse"), 1, glm::value_ptr(lightColor));
	glUniform3fv(glGetUniformLocation(materialProgram, "light.specular"), 1, glm::value_ptr(lightColor));
	//tree->draw(materialProgram);

	// Draw cottages
	glUseProgram(importedProgram);
	glUniformMatrix4fv(importedProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(importedViewLoc, 1, GL_FALSE, glm::value_ptr(view));
	for (int i = 0; i < cottageMovements.size(); ++i) {

		glUniformMatrix4fv(importedModelLoc, 1, GL_FALSE, glm::value_ptr(cottageMovements[i] * cottage->getModel()));
		cottage->draw(importedProgram);
	}

	// Draw enemy
	glUniformMatrix4fv(importedModelLoc, 1, GL_FALSE, glm::value_ptr(enemy->getModel()));
	enemy->draw(importedProgram);

	if (debugBounds) {
		glUseProgram(noTexProgram);
		glUniformMatrix4fv(noTexProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(noTexViewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniform3fv(noTexColorLoc, 1, glm::value_ptr(sphere->getColor()));
		for (int i = 0; i < cottageMovements.size(); ++i) {
			glm::mat4 place = cottageMovements[i];
			glm::mat4 center = glm::translate(cottage->getMidCoord());
			glm::mat4 scale = glm::scale(glm::vec3(cottage->getRadius()));
			glm::mat4 sphereModel = place * center * cottage->getModel() * scale;
			glUniformMatrix4fv(noTexModelLoc, 1, GL_FALSE, glm::value_ptr(sphereModel));
			cottage->drawBound();
		}
		for (int i = 0; i < models.size(); ++i) {
			glm::mat4 center = glm::translate(models[i]->getMidCoord());
			glm::mat4 scale = glm::scale(glm::vec3(models[i]->getRadius()));
			glm::mat4 sphereModel = center * models[i]->getModel() * scale;
			glUniformMatrix4fv(noTexModelLoc, 1, GL_FALSE, glm::value_ptr(sphereModel));
			models[i]->drawBound();
		}
		glm::mat4 center = glm::translate(enemy->getMidCoord());
		glm::mat4 scale = glm::scale(glm::vec3(enemy->getRadius()));
		glm::mat4 sphereModel = center * enemy->getModel() * scale;
		glUniformMatrix4fv(noTexModelLoc, 1, GL_FALSE, glm::value_ptr(sphereModel));
		enemy->drawBound();

	}

	Window::view = glm::lookAt(Window::eye, Window::center + Window::eye, Window::up);
	p1.center = eye;
	// Gets events, including input such as keyboard and mouse or window resizing.
	glfwPollEvents();
	// Swap buffers.
	glfwSwapBuffers(window);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

	// Check for a key press.
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);				
			break;
		case GLFW_KEY_N:
			normalColoring = !normalColoring;
			break;
		case GLFW_KEY_B:
			debugBounds = !debugBounds;
		default:
			break;
		}
	}
}

void Window::processInput(GLFWwindow *window)
{

	GLfloat systemWalkSpeed = WALK_SPEED * deltaTime;

	// Current means of preventing player from going outside 
	// of skybox restricts player movement too much. Fix later.
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		// So moving doesn't cause jumping
		glm::vec3 newPos = systemWalkSpeed * center;
		glm::vec3 movedEye = eye + glm::vec3(newPos[0], 0, newPos[2]);
		if (assertInSkybox(movedEye) && !assertPlayerCollision(movedEye)) {
			eye += glm::vec3(newPos[0], 0, newPos[2]);
		}
		else {
			eye -= glm::vec3(newPos[0], 0, newPos[2]);
		}

	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		glm::vec3 newPos = systemWalkSpeed * center;
		glm::vec3 movedEye = eye - glm::vec3(newPos[0], 0, newPos[2]);
		if (assertInSkybox(movedEye) && !assertPlayerCollision(movedEye)) {
			eye -= glm::vec3(newPos[0], 0, newPos[2]);
		}
		else {
			eye += glm::vec3(newPos[0], 0, newPos[2]);
		}
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		glm::vec3 newPos = glm::normalize(glm::cross(center, up)) * systemWalkSpeed;
		glm::vec3 movedEye = eye - glm::vec3(newPos[0], 0, newPos[2]);
		if (assertInSkybox(movedEye) && !assertPlayerCollision(movedEye)) {
			eye = movedEye;
		}
		else{
			eye += glm::vec3(newPos[0], 0, newPos[2]);
		}
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		glm::vec3 newPos = glm::normalize(glm::cross(center, up)) * systemWalkSpeed;
		glm::vec3 movedEye = eye + glm::vec3(newPos[0], 0, newPos[2]);
		if (assertInSkybox(movedEye) && !assertPlayerCollision(movedEye)) {
			eye = movedEye;
		}
		else {
			eye -= glm::vec3(newPos[0], 0, newPos[2]);
		}
	}
	if (!inAir && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		inAir = true;
		upwardsSpeed = JUMP_POWER;
	}
}

void Window::mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	center = glm::normalize(front);
}


void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f) {
		fov -= yoffset;
	}
	if (fov <= 1.0f) {
		fov = 1.0f;
	}
	if (fov >= 45.0f) {
		fov = 45.0f;
	}
	Window::projection = glm::perspective(glm::radians(fov),
		double(width) / (double)height, 1.0, 2250.0);
}

void Window::setDeltaTime() {
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

GLboolean Window::assertInSkybox(glm::vec3 movedEye) {
	if (movedEye[0] < SKYBOX_SIZE - 1 && 
			movedEye[0] > -SKYBOX_SIZE + 1 && 
			movedEye[2] < SKYBOX_SIZE - 1 && 
			movedEye[2] > -SKYBOX_SIZE + 1) {
		return true;
	}
	return false;
}

/*
* Currently only checks against player, will need to check for other moving
* things as well.
*/
GLboolean Window::assertPlayerCollision(glm::vec3 movedEye) {
	for (int i = 0; i < cottageMovements.size(); ++i) {
		GLfloat dist = glm::distance(movedEye, 
				glm::vec3(cottageMovements[i] * glm::vec4(cottage->getTrueCenter(), 1.0f)));
		if (dist <= p1.radius + cottage->getCollisionRadius()) {
			return true;
		}
	}
	for (int i = 0; i < models.size(); ++i) {
		GLfloat dist = glm::distance(movedEye, models[i]->getTrueCenter());
		if (dist <= p1.radius + models[i]->getCollisionRadius()) {
			return true;
		}
	}
	return false;
}