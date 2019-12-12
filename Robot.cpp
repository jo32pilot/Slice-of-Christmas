#include "Robot.h"

Robot::Robot(std::string path) {
	limb = new Model(path + "limb_s.obj");
	head = new Model(path + "head_s.obj");
	body = new Model(path + "body_s.obj");
	color = glm::vec3(0, 1, 0);
	model = glm::mat4(1);

	glm::mat4 expand = glm::scale(glm::vec3(PART_SIZE));
	limb->setModel(limb->getModel() * expand);
	head->setModel(head->getModel() * expand);
	body->setModel(body->getModel() * expand);


	maxCoord = glm::max(limb->getLargestCoord(), head->getLargestCoord());
	maxCoord = glm::max(maxCoord, body->getLargestCoord());

	minCoord = glm::min(limb->getSmallestCoord(), head->getSmallestCoord());
	minCoord = glm::min(minCoord, body->getSmallestCoord());

	midCoord = (maxCoord + minCoord);
	midCoord /= MID_POINT_DIVISOR;
	// translate sphere center by midpoint - body center

}

Robot::~Robot() {
	delete limb;
	delete head;
	delete body;
}

void Robot::draw(GLuint shader) {

	// Draw arms
	GLuint modelLoc = glGetUniformLocation(shader, "model");
	glm::mat4 armAlign = glm::translate(glm::vec3(ARM_ALIGN, 0, 0));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(armAlign * limb->getModel()));
	limb->draw(shader);
	armAlign = glm::translate(glm::vec3(-ARM_ALIGN, 0, 0));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(armAlign * limb->getModel()));
	limb->draw(shader);

	// Draw legs
	glm::mat4 legAlign = glm::translate(glm::vec3(LEG_ALIGN_X, LEG_ALIGN_Y, 0));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(legAlign * limb->getModel()));
	limb->draw(shader);
	legAlign = glm::translate(glm::vec3(-LEG_ALIGN_X, LEG_ALIGN_Y, 0));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(legAlign * limb->getModel()));
	limb->draw(shader);

	// Draw head
	glm::mat4 headAlign = glm::translate(glm::vec3(0, HEAD_ALIGN, 0));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(headAlign * head->getModel()));
	head->draw(shader);

	// Draw body
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(body->getModel()));
	body->draw(shader);

}

glm::vec3 Robot::getColor() {
	return this->color;
}

glm::mat4 Robot::getModel() {
	return this->model;
}