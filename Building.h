#ifndef BUILDING_H
#define BUILDING_H

#include "Object.h"

class Building : public Object {

	private:
		GLuint vao;
		GLuint vbos[4];

	public:
		void draw();
		void draw(GLuint textureID);
		void update();
};

#endif