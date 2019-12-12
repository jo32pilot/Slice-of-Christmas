#ifndef ROBOT_H
#define ROBOT_H

#include <gl/glew.h>
#include <string>
#include <glm/gtx/transform.hpp>

#include "Model.hpp"

#define PART_SIZE 7
#define ARM_ALIGN 9
#define HEAD_ALIGN 8
#define LEG_ALIGN_Y -10
#define LEG_ALIGN_X 4

class Robot {

	private:
		Model* limb;
		Model* head;
		Model* body;
		glm::vec3 minCoord;
		glm::vec3 maxCoord;
		glm::vec3 midCoord;
		glm::vec3 color;
		glm::mat4 model;

	public:

		/* Put together robot in here. At end of call, find min, max, etc
		* for bounding sphere.
		*/
		Robot(std::string path);
		~Robot();
		void draw(GLuint shader);
		glm::vec3 getColor();
		glm::mat4 getModel();
};

#endif