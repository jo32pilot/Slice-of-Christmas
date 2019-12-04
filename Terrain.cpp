#include "Terrain.h"

Terrain::Terrain() {

	this->color = glm::vec3(0, 1, 0);
	this->model = glm::mat4(1);

	unsigned int timeSeed = (unsigned int)time(NULL);
	srand(timeSeed);

	for (int i = 0; i < DIM; ++i) {
		for (int j = 0; j < DIM; ++j) {
			heights[i][j] = 0;
		}
	}

	heights[0][DIM - 1] = rand() % DIM;
	heights[DIM - 1][0] = rand() % DIM;
	heights[DIM - 1][DIM - 1] = rand() % DIM;
	heights[0][0] = rand() % DIM;


	diamondSquare(heights, DIM, DIM);
	
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<Vert> verts;
	std::vector<Triangle> faces;
	std::vector<int> indices;
	std::vector<glm::vec2> texCoords;

	for (int i = 0; i < DIM; ++i) {

		for (int j = 0; j < DIM; ++j) {

			GLfloat x = i * SCALE_FACTOR - RE_CENTER;
			GLfloat z = j * SCALE_FACTOR - RE_CENTER;
			verts.push_back({
				glm::vec3(x, heights[i][j], z),
				glm::vec3(0.0f)
			});
		}
	}

	// Settup indices and triangle array
	// DIM * (DIM - 1) to prevent creating triangles that shouldn't exist.
	for (int i = 0; i < DIM * (DIM - 1); ++i) {

		// To prevent triangles that shouldn't exist.
		if ((i + 1) % DIM == 0) {
			continue;
		}

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

	// Settup textures
	for (int i = 0; i < DIM; ++i) {
		for (int j = 0; j < DIM; ++j) {
			texCoords.push_back({i, j});
		}
	}


	this->numIndices = indices.size();

	// Generate a vertex array (VAO) and two vertex buffer objects (VBO).
	glGenVertexArrays(1, &vao);
	glGenBuffers(4, vbos);

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

	// Bind to the third VBO. We will use it to store texture coordinates.
	glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
	// Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * texCoords.size(),
		texCoords.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

	// Bind to the third VBO. We will use it to store the indices.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[3]);
	// Pass in the data.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(),
		indices.data(), GL_STATIC_DRAW);

	// Unbind from the VBOs.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
}

Terrain::~Terrain() {
	glDeleteBuffers(4, vbos);
	glDeleteVertexArrays(1, &vao);
}

void Terrain::draw() {
	glBindVertexArray(vao);

	glDrawElements(GL_LINES, numIndices, GL_UNSIGNED_INT, 0);

	// Unbind from the VAO.
	glBindVertexArray(0);

}

void Terrain::draw(GLuint textureID) {
	glBindTexture(GL_TEXTURE_2D, textureID);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Terrain::update() {
	return;
}

GLint Terrain::getHeightOfTerrain(GLfloat worldX, GLfloat worldZ) {
	//std::cout << std::endl;
	GLfloat terrainX = worldX + RE_CENTER;
	GLfloat terrainZ = worldZ + RE_CENTER;
	//std::cout << "worldX: " << worldX << std::endl;
	//std::cout << "worldZ: " << worldZ << std::endl;
	GLfloat gridSquareSize = (GLfloat)(DIM * SCALE_FACTOR) / (GLfloat)(DIM - 1);
	//std::cout << "gridSquareSize: " << gridSquareSize << std::endl;
	//int gridX = worldX / gridSquareSize;
	int gridX = terrainX / gridSquareSize;
	//std::cout << "gridX: " << gridX << std::endl;
	//int gridZ = worldZ/ gridSquareSize;
	int gridZ = terrainZ / gridSquareSize;
	//std::cout << "gridZ: " << gridZ << std::endl;
	if (gridX >= DIM - 1 || gridZ >= DIM - 1 || gridX < 0 || gridZ < 0) {
		return 0;
	}
	GLfloat xCoord = (GLfloat)((GLint)terrainX % (GLint)gridSquareSize) / gridSquareSize;
	//std::cout << "xCoord: " << xCoord << std::endl;

	GLfloat zCoord = (GLfloat)((GLint)terrainZ % (GLint)gridSquareSize) / gridSquareSize;
	//std::cout << "zCoord: " << zCoord << std::endl;

	GLfloat answer;
	if (xCoord <= 1 - zCoord) {
		answer = barryCentric(
			{ 0, heights[gridX][gridZ], 0 },
			{ 1, heights[gridX + 1][gridZ], 0 },
			{ 0, heights[gridX][gridZ + 1], 1 },
			{ xCoord, zCoord });
	}
	else {
		answer = barryCentric(
			{ 1, heights[gridX + 1][gridZ], 0 },
			{ 1, heights[gridX + 1][gridZ + 1], 1 },
			{ 0, heights[gridX][gridZ + 1], 1 },
			{ xCoord, zCoord });
	}
	//std::cout << "answer: " << answer << std::endl;
	return answer;
}

GLfloat Terrain::barryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos) {
	/*std::cout << "p1: " << p1.x << " " << p1.y << " " << p1.z << std::endl;
	std::cout << "p2: " << p2.x << " " << p2.y << " " << p2.z << std::endl;
	std::cout << "p3: " << p3.x << " " << p3.y << " " << p3.z << std::endl;*/

	GLfloat det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	GLfloat l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
	GLfloat l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
	GLfloat l3 = 1.0f - l1 - l2;
	return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}