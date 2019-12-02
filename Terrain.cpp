#include "Terrain.h"

Terrain::Terrain() {

	this->color = glm::vec3(0, 1, 0);
	this->model = glm::mat4(1);

	int heights[DIM][DIM] = {};
	diamondSquare(heights, DIM, DIM);
	
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;

	// if at last row of veritces, then no more triangles.
	for (int i = 0; i < DIM - 1; ++i) {

		// if at last column of veritces, then move on.
		for (int j = 0; j < DIM - 1; ++j) {

			GLfloat x = i * SCALE_FACTOR - RE_CENTER;
			GLfloat z = j * SCALE_FACTOR - RE_CENTER;
			GLfloat xNext = (i + 1) * SCALE_FACTOR - RE_CENTER;
			GLfloat zNext = (j + 1) * SCALE_FACTOR - RE_CENTER;

			glm::vec3 startVertex = glm::vec3(x, heights[i][j], z);
			glm::vec3 rightVertex = glm::vec3(x, heights[i][j + 1], zNext);
			glm::vec3 downVertex = glm::vec3(xNext, heights[i + 1][j], z);
			glm::vec3 endVertex = glm::vec3(xNext, heights[i + 1][j + 1], zNext);

			// Must be pushed in this order for correctly drawn triangles
			// and consistent orientation (clockwise).
			vertices.push_back(startVertex);
			normals.push_back(glm::normalize(glm::cross(downVertex - startVertex, rightVertex - startVertex)));

			vertices.push_back(rightVertex);
			normals.push_back(glm::normalize(glm::cross(startVertex - rightVertex, downVertex - rightVertex)));

			vertices.push_back(downVertex);
			normals.push_back(glm::normalize(glm::cross(rightVertex - downVertex, startVertex - downVertex)));

			vertices.push_back(downVertex);
			normals.push_back(glm::normalize(glm::cross(rightVertex - downVertex, startVertex - downVertex)));

			vertices.push_back(rightVertex);
			normals.push_back(glm::normalize(glm::cross(startVertex - rightVertex, downVertex - rightVertex)));

			vertices.push_back(endVertex);
			normals.push_back(glm::normalize(glm::cross(rightVertex - endVertex, downVertex - endVertex)));

		}
	}

	this->numVertices = vertices.size();

	// Generate a vertex array (VAO) and two vertex buffer objects (VBO).
	glGenVertexArrays(1, &vao);
	glGenBuffers(2, vbos);

	// Bind to the VAO.
	glBindVertexArray(vao);

	// Bind to the first VBO. We will use it to store the vertices.
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	// Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),
		vertices.data(), GL_STATIC_DRAW);
	// Enable vertex attribute 0. 
	// We will be able to access vertices through it.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Bind to the second VBO. We will use it to store vertex normals.
	glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
	// Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(),
		normals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Unbind from the VBOs.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
}

Terrain::~Terrain() {
	glDeleteBuffers(2, vbos);
	glDeleteVertexArrays(1, &vao);
}

void Terrain::draw() {
	glBindVertexArray(vao);

	glDrawArrays(GL_TRIANGLES, 0, numVertices);

	// Unbind from the VAO.
	glBindVertexArray(0);
}

void Terrain::draw(GLuint textureID) {
	return;
}

void Terrain::update() {
	return;
}