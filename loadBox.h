#ifndef LOAD_BOX_H
#define LOAD_BOX_H


#include <GL/glew.h>
#include <iostream>
#include <string>
#include <vector>
#include "stb_image.h"

#define SIDES 6

GLuint loadBox(std::vector<std::string> pics);
GLuint loadTextures(std::string path);
unsigned int TextureFromFile(const char *path, const std::string &directory);


#endif