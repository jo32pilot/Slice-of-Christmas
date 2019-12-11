#ifndef PRINT_VEC3_HPP
#define PRINT_VEC3_HPP

#include <iostream>
#include <string>
#include <glm/glm.hpp>

void printVec(glm::vec3 vec, std::string name) {
	std::cout << name + ": " << vec.x << " " << vec.y << " " << vec.z << std::endl;
}

#endif