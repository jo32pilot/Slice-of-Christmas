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
std::vector<Model*> Window::models;

glm::mat4 Window::projection; // Projection matrix.

glm::vec3 Window::eye(0, 0, 3); // Camera position.
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

bool Window::initializeProgram() {
	// Create a shader program with a vertex shader and a fragment shader.
	program = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
	skyboxProgram = LoadShaders("shaders/skybox.vert", "shaders/skybox.frag");
	importedProgram = LoadShaders("shaders/imported.vert", "shaders/imported.frag");
	noTexProgram = LoadShaders("shaders/no_texture.vert", "shaders/no_texture.frag");


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

	textureID = loadBox(faces);
	terrainTexture = loadTextures("assets/terrainTexture.png");

	return true;
}

bool Window::initializeObjects()
{
	skybox = new Cube(5.0f);
	terrain = new Terrain();
	tree = new Model("assets/christmas-tree/CartoonTree.obj");
	cottage = new Model("assets/cottage/Snow\ Covered\ CottageOBJ.obj");
	sphere = new Model("assets/sphere.obj");

	sphere->setColor(glm::vec3(1, 0, 1));

	models.push_back(tree);
	models.push_back(cottage);

	cottage->boundingSphere = sphere;
	tree->boundingSphere = sphere;

	GLfloat terrainHeight = terrain->getHeightOfTerrain(0, 0);
	glm::vec3 cottageMin = cottage->getSmallestCoord();
	glm::vec3 treeMin = tree->getSmallestCoord();
	cottage->setModel(glm::translate(glm::vec3(0, terrainHeight - cottageMin.y, 0)) * cottage->getModel());
	tree->setModel(glm::translate(glm::vec3(0, terrainHeight - treeMin.y, 0)) * tree->getModel());

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

	// Delete the shader program.
	glDeleteProgram(program);
	glDeleteProgram(skyboxProgram);
	glDeleteProgram(importedProgram);
	glDeleteProgram(noTexProgram);
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
		double(width) / (double)height, 1.0, 1000.0);
}

void Window::idleCallback()
{
	
	// Gravity to pull camera down
	upwardsSpeed += (GRAVITY * deltaTime);
	eye = glm::vec3(eye[0], eye[1] + upwardsSpeed * deltaTime, eye[2]);
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

	glUseProgram(program);
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(terrain->getModel()));
	glUniform3fv(colorLoc, 1, glm::value_ptr(terrain->getColor()));
	glUniform1i(normalColoringLoc, normalColoring);

	//terrain->draw();
	terrain->draw(terrainTexture);

	// Draw skybox
	glDepthMask(GL_FALSE);
	glUseProgram(skyboxProgram);
	glUniformMatrix4fv(skyboxProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(skyboxViewLoc, 1, GL_FALSE, glm::value_ptr(view));

	skybox->draw(textureID);

	glDisable(GL_CULL_FACE);

	glUseProgram(importedProgram);
	glUniformMatrix4fv(importedProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(importedViewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(importedModelLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));

	tree->draw(importedProgram);

	glUniformMatrix4fv(importedModelLoc, 1, GL_FALSE, glm::value_ptr(cottage->getModel()));
	cottage->draw(importedProgram);

	if (debugBounds) {
		glUseProgram(noTexProgram);
		glUniformMatrix4fv(noTexProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(noTexViewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniform3fv(noTexColorLoc, 1, glm::value_ptr(sphere->getColor()));
		for (int i = 0; i < models.size(); ++i) {
			glm::mat4 center = glm::translate(models[i]->getMidCoord());
			glm::mat4 scale = glm::scale(glm::vec3(models[i]->getRadius()));
			glm::mat4 sphereModel = center * models[i]->getModel() * scale;
			glUniformMatrix4fv(noTexModelLoc, 1, GL_FALSE, glm::value_ptr(sphereModel));
			models[i]->drawBound();
		}
	}

	Window::view = glm::lookAt(Window::eye, Window::center + Window::eye, Window::up);
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
	// of skybox restricts player too much. Fix later.
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		// So moving doesn't cause jumping
		glm::vec3 newPos = systemWalkSpeed * center;
		glm::vec3 movedEye = eye + glm::vec3(newPos[0], 0, newPos[2]);
		if (movedEye[0] < SKYBOX_SIZE - 1 && movedEye[0] > -SKYBOX_SIZE + 1 && movedEye[2] < SKYBOX_SIZE - 1 && movedEye[2] > -SKYBOX_SIZE + 1) {
			eye += glm::vec3(newPos[0], 0, newPos[2]);
		}
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		glm::vec3 newPos = systemWalkSpeed * center;
		glm::vec3 movedEye = eye - glm::vec3(newPos[0], 0, newPos[2]);
		if (movedEye[0] < SKYBOX_SIZE - 1 && movedEye[0] > -SKYBOX_SIZE + 1 && movedEye[2] < SKYBOX_SIZE - 1 && movedEye[2] > -SKYBOX_SIZE + 1) {
			eye -= glm::vec3(newPos[0], 0, newPos[2]);
		}
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		glm::vec3 newPos = glm::normalize(glm::cross(center, up)) * systemWalkSpeed;
		glm::vec3 movedEye = eye - glm::vec3(newPos[0], 0, newPos[2]);
		if (movedEye[0] < SKYBOX_SIZE - 1 && movedEye[0] > -SKYBOX_SIZE + 1 && movedEye[2] < SKYBOX_SIZE - 1 && movedEye[2] > -SKYBOX_SIZE + 1) {
			eye = movedEye;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		glm::vec3 newPos = glm::normalize(glm::cross(center, up)) * systemWalkSpeed;
		glm::vec3 movedEye = eye + glm::vec3(newPos[0], 0, newPos[2]);
		if (movedEye[0] < SKYBOX_SIZE - 1 && movedEye[0] > -SKYBOX_SIZE + 1 && movedEye[2] < SKYBOX_SIZE - 1 && movedEye[2] > -SKYBOX_SIZE + 1) {
			eye = movedEye;
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
		double(width) / (double)height, 1.0, 1000.0);
}

void Window::setDeltaTime() {
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}