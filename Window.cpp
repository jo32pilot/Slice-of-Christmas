#include "Window.h"

int Window::width;
int Window::height;

const char* Window::windowTitle = "GLFW Starter Project";

// Objects to display.
Cube* Window::skybox;

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

GLuint Window::skyboxProgram;

GLuint Window::skyboxProjectionLoc;
GLuint Window::skyboxViewLoc;

GLuint Window::textureID;

std::vector<std::string> Window::faces = {
	"assets/right.png",
	"assets/left.png",
	"assets/up.png",
	"assets/down.png",
	"assets/front.png",
	"assets/back.png"
};

GLfloat Window::deltaTime = 0.0f;
GLfloat Window::lastFrame = 0.0f;


bool Window::initializeProgram() {
	// Create a shader program with a vertex shader and a fragment shader.
	program = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
	skyboxProgram = LoadShaders("shaders/skybox.vert", "shaders/skybox.frag");


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

	glUseProgram(skyboxProgram);
	skyboxProjectionLoc = glGetUniformLocation(skyboxProgram, "projection");
	skyboxViewLoc = glGetUniformLocation(skyboxProgram, "view");

	textureID = loadBox(faces);

	return true;
}

bool Window::initializeObjects()
{
	skybox = new Cube(5.0f);

	return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
	delete skybox;

	// Delete the shader program.
	glDeleteProgram(program);
	glDeleteProgram(skyboxProgram);
}

GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

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
	Window::projection = glm::perspective(glm::radians(60.0), 
		double(width) / (double)height, 1.0, 1000.0);
}

void Window::idleCallback()
{
	// Perform any updates as necessary.
	return;
}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	


	// Draw skybox
	glDepthMask(GL_FALSE);
	glUseProgram(skyboxProgram);
	glUniformMatrix4fv(skyboxProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(skyboxViewLoc, 1, GL_FALSE, glm::value_ptr(view));

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	skybox->draw(textureID);

	// Gets events, including input such as keyboard and mouse or window resizing.
	glfwPollEvents();
	// Swap buffers.
	glfwSwapBuffers(window);
}

/*
void Window::cursorPosFun(GLFWwindow* wind, double x, double y) {
	glm::vec3 Window::eye(0, 0, 20); // Camera position.
	glm::vec3 Window::center(0, 0, 0); // The point we are looking at.
	glm::vec3 Window::up(0, 1, 0); // The up direction of the camera.

	// View matrix, defined by eye, center and up.
	glm::mat4 Window::view = glm::lookAt(Window::eye, Window::center, Window::up)
}*/

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

		default:
			break;
		}
	}
}

void Window::processInput(GLFWwindow *window)
{
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	GLfloat systemWalkSpeed = WALK_SPEED * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		eye += systemWalkSpeed * center;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		eye -= systemWalkSpeed * center;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		eye -= glm::normalize(glm::cross(center, up)) * systemWalkSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		eye += glm::normalize(glm::cross(center, up)) * systemWalkSpeed;
	}
	Window::view = glm::lookAt(Window::eye, Window::center + Window::eye, Window::up);
}