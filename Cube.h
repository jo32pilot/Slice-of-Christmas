#ifndef _CUBE_H_
#define _CUBE_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>

#include "Object.h"

#define SKYBOX_SIZE 1000

class Cube : public Object
{
private:
	GLuint vao;
	GLuint vbos[2];
public:
	Cube(float size);
	~Cube();

	void draw();
	void update();
	void draw(GLuint textureID);

	void spin(float deg);
};

#endif

