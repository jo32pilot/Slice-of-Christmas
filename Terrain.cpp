#include "Terrain.h"

Terrain::Terrain() {

	this->color = glm::vec3(0, 1, 0);
	this->model = glm::mat4(1);

	int heights[DIM][DIM] = {};
	diamondSquare(heights, DIM, DIM);
	
	glm::vec3 terrain[NUM_VERTICES] = {};
	int k = 0;
	for (int i = 0; i < DIM; ++i) {
		for (int j = 0; j < DIM; ++j) {
			GLfloat x = i * SCALE_FACTOR;
			GLfloat z = j * SCALE_FACTOR;
			terrain[k] = glm::vec3(x, heights[i][j], z);
			++k;
		}
	}

	// Generate a vertex array (VAO) and two vertex buffer objects (VBO).
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	// Bind to the VAO.
	glBindVertexArray(vao);

	// Bind to the first VBO. We will use it to store the vertices.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(terrain),
		&terrain, GL_STATIC_DRAW);
	// Enable vertex attribute 0. 
	// We will be able to access vertices through it.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	/*// Bind to the second VBO. We will use it to store the indices.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[1]);
	// Pass in the data.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * indices.size(),
		indices.data(), GL_STATIC_DRAW);*/

	// Unbind from the VBOs.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
}

Terrain::~Terrain() {
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void Terrain::draw() {
	glBindVertexArray(vao);

	glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES);

	// Unbind from the VAO.
	glBindVertexArray(0);
}

void Terrain::draw(GLuint textureID) {
	return;
}

void Terrain::update() {
	return;
}