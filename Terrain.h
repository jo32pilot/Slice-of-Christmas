#ifndef TERRAIN_H
#define TERRAIN_H

#include "Object.h"
#include "diamondSquare.h"

class Terrain : public Object {
	private:
		GLuint vao;
		GLuint vbo;

	public:
		Terrain();
		~Terrain();
		void draw();
		void draw(GLuint textureID);
		void update();
};
#endif