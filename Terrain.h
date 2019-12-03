#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>
#include <iostream>
#include <random>
#include <time.h>
#include "Object.h"
#include "diamondSquare.h"

typedef struct VertexStruct {
	glm::vec3 pos;
	glm::vec3 normal;
} Vert;

typedef struct TriangleStruct {
	int vertInd[3];
} Triangle;

class Terrain : public Object {
	private:
		GLuint vao;
		GLuint vbos[4];
		GLint numIndices;


	public:
		Terrain();
		~Terrain();
		void draw();
		void draw(GLuint textureID);
		void update();
};
#endif