#ifndef PLAYER_BOUND_H
#define PLAYER_BOUND_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

typedef struct p_b {
	GLfloat radius;
	glm::vec3 center;
} PlayerBound;

#endif