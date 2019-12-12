#include "Robot.h"

Robot::Robot(std::string path) {
	limb = new Model(path + "limb_s.obj");
	head = new Model(path + "head_s.obj");
	body = new Model(path + "body_s.obj");
	eye = new Model(path + "eyeball_s.obj");
	color = glm::vec3(0, 1, 0);
	model = glm::mat4(1);

	glm::mat4 expand = glm::scale(glm::vec3(PART_SIZE));
	limb->setModel(limb->getModel() * expand);
	head->setModel(head->getModel() * expand);
	body->setModel(body->getModel() * expand);
	eye->setModel(eye->getModel() * expand);

	// put together robot before finding relevant data.




	maxCoord = glm::max(limb->getLargestCoord(), head->getLargestCoord());
	maxCoord = glm::max(maxCoord, body->getLargestCoord());
	maxCoord = glm::max(maxCoord, eye->getLargestCoord());

	minCoord = glm::min(limb->getSmallestCoord(), head->getSmallestCoord());
	minCoord = glm::min(minCoord, body->getSmallestCoord());
	minCoord = glm::min(minCoord, eye->getSmallestCoord());

	midCoord = (maxCoord + minCoord);
	midCoord /= MID_POINT_DIVISOR;
	// translate sphere center by midpoint - body center

}

Robot::~Robot() {
	delete limb;
	delete head;
	delete body;
	delete eye;
}

void Robot::draw(GLuint shader) {
	GLuint modelLoc = glGetUniformLocation(shader, "model");
	glm::mat4 armAlign = glm::translate(glm::vec3(ARM_ALIGN, 0, 0));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(armAlign * limb->getModel()));
	limb->draw(shader);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(-armAlign * limb->getModel()));
	limb->draw(shader);

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(head->getModel()));
	head->draw(shader);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(body->getModel()));
	body->draw(shader);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(eye->getModel()));
	eye->draw(shader);
}

glm::vec3 Robot::getColor() {
	return this->color;
}

glm::mat4 Robot::getModel() {
	return this->model;
}