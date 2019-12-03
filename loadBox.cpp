
#include "loadBox.h"

// Loads image as texture, returns ID of texture
GLuint loadBox(std::vector<std::string> pics){
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < SIDES; i++)
	{
		std::cout << "Loading side" << std::endl;
		unsigned char *data = stbi_load(pics[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			std::cout << width << " " << height << " " << nrChannels << std::endl;
			std::cout << pics[i].c_str() << std::endl;
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << pics[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	std::cout << textureID << std::endl;
	return textureID; // Return the ID of the texture
}

// Loads image as texture, returns ID of texture
GLuint loadTextures(std::string path)
{
	GLuint textureId;
	glGenTextures(1, &textureId); // Get unique ID for texture
	glBindTexture(GL_TEXTURE_2D, textureId); // Tell OpenGL which texture to edit
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // set bi-linear interpolation
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // for both filtering modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture edge mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	int width, height, nrChannels;
	unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

	// load image from disk; uses third party Image library
	// Depending on the image library, the texture image may have to be flipped vertically
	// Load image into OpenGL texture in GPU memory:
	glTexImage2D(GL_TEXTURE_2D, // Always GL_TEXTURE_2D for image textures
		0, // 0 for now
		GL_RGB, // Format OpenGL uses for image without alpha channel
		width, height, // Width and height
		0, // The border of the image
		GL_RGB, // GL_RGB, because pixels are stored in RGB format
		GL_UNSIGNED_BYTE, // GL_UNSIGNED_BYTE, because pixels are stored as unsigned numbers
		data); // The actual RGB image data
	return textureId; // Return the ID of the texture
}