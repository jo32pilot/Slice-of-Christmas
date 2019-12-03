#include "Terrain.h"

Terrain::Terrain() {

	this->color = glm::vec3(0, 1, 0);
	this->model = glm::mat4(1);

	//TODO change 4 corner elements
	int heights[DIM][DIM] = {};
	heights[0][DIM - 1] = 10;
	heights[DIM - 1][0] = 2;
	heights[DIM - 1][DIM - 1] = 3;
	heights[0][0] = 0;
	diamondSquare(heights, DIM, DIM);
	
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<Vert> verts;
	std::vector<Triangle> faces;
	std::vector<int> indices;
	int ind = 0;

	for (int i = 0; i < DIM; ++i) {

		for (int j = 0; j < DIM; ++j) {

			GLfloat x = i * SCALE_FACTOR - RE_CENTER;
			GLfloat z = j * SCALE_FACTOR - RE_CENTER;
			verts.push_back({
				glm::vec3(x, heights[i][j], z),
				glm::vec3(0.0f)
			});
			/*GLfloat xNext = (i + 1) * SCALE_FACTOR - RE_CENTER;
			GLfloat zNext = (j + 1) * SCALE_FACTOR - RE_CENTER;

			Vert startVertex = {
				glm::vec3(x, heights[i][j], z)
			};
			Vert rightVertex = {
				glm::vec3(x, heights[i][j + 1], zNext)
			};
			Vert downVertex = {
				glm::vec3(xNext, heights[i + 1][j], z)
			};
			Vert endVertex = {
				glm::vec3(xNext, heights[i + 1][j + 1], zNext)
			};

			Triangle topTri = { {} };
			Triangle botTri = { {} };

			// Must be pushed in this order for correctly drawn triangles
			// and consistent orientation (clockwise).
			verts.push_back(startVertex);
			topTri.vertInd[0] = ind;

			verts.push_back(rightVertex);
			verts.push_back(downVertex);
			verts.push_back(downVertex);
			verts.push_back(rightVertex);
			verts.push_back(endVertex);*/

		}
	}

	// Settup indices and triangle array
	for (int i = 0; i < (DIM - 1) * (DIM - 1); ++i) {

		// Must be pushed in this order for correctly drawn triangles
		// and consistent orientation (clockwise).
		indices.push_back(i);
		indices.push_back(i + 1);
		indices.push_back(i + DIM);
		faces.push_back({i, i + 1, i + DIM});

		indices.push_back(i + DIM);
		indices.push_back(i + 1);
		indices.push_back(i + 1 + DIM);
		faces.push_back({ i + DIM, i + 1, i + 1 + DIM });

	}

	for (int i = 0; i < faces.size(); ++i) {
		const int ia = faces[i].vertInd[0];
		const int ib = faces[i].vertInd[1];
		const int ic = faces[i].vertInd[2];

		const glm::vec3 e1 = verts[ia].pos - verts[ib].pos;
		const glm::vec3 e2 = verts[ic].pos - verts[ib].pos;
		const glm::vec3 no = glm::cross(e2, e1);

		verts[ia].normal += no;
		verts[ib].normal += no;
		verts[ic].normal += no;
	}


	for (int i = 0; i < verts.size(); i++) {
		verts[i].normal = glm::normalize(verts[i].normal);
		vertices.push_back(verts[i].pos);
		normals.push_back(verts[i].normal);
	}

	this->numIndices = indices.size();

	// Generate a vertex array (VAO) and two vertex buffer objects (VBO).
	glGenVertexArrays(1, &vao);
	glGenBuffers(3, vbos);

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


	// Bind to the third VBO. We will use it to store the indices.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[2]);
	// Pass in the data.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(),
		indices.data(), GL_STATIC_DRAW);

	// Unbind from the VBOs.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);

	std::vector<glm::vec3> debug;
	for (int i = 0; i < indices.size(); ++i) {
		debug.push_back(vertices[indices[i]]);
		debug.push_back(vertices[indices[i]] + 4.0f * normals[indices[i]]);
	}
	this->debugNum = debug.size();

	glGenVertexArrays(1, &debugVao);
	glGenBuffers(1, &debugVbo);

	glBindVertexArray(debugVao);

	glBindBuffer(GL_ARRAY_BUFFER, debugVbo);
	glBufferData(GL_ARRAY_BUFFER, debug.size() * sizeof(glm::vec3),
		debug.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Terrain::~Terrain() {
	glDeleteBuffers(3, vbos);
	glDeleteVertexArrays(1, &vao);
}

void Terrain::draw() {
	glBindVertexArray(vao);

	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);

	// Unbind from the VAO.
	glBindVertexArray(0);

	glBindVertexArray(debugVao);
	glDrawArrays(GL_LINES, 0, debugNum);
	glBindVertexArray(0);
}

void Terrain::draw(GLuint textureID) {
	return;
}

void Terrain::update() {
	return;
}