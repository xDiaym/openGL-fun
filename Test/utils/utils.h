#pragma once
#include <string>
#include <sstream>
#include <fstream>

#include "SOIL.h"


inline std::string load_file(std::string file_path)
{
	// TODO [IMPORTANT] ADD ERROR WITH OPEN ICORECT FILE
	std::fstream data(file_path);
	std::string ret = std::string(std::istreambuf_iterator<char>(data.rdbuf()), std::istreambuf_iterator<char>());
	data.close();
	return ret;
}

inline GLuint load_texture(const char* file_path)
{
	int width, height;
	unsigned char* image = SOIL_load_image(file_path, &width, &height, 0, SOIL_LOAD_RGB);

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);


	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}
