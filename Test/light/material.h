#pragma once

#include <string>
#include <glew.h>
#include <glm.hpp>

#include "../shader/shader.h"


struct material
{
	std::string name;

	GLuint texture_diffuse;
	GLuint texture_specular;
	float shininess;


	material(std::string name_, GLuint diffuse, GLuint specular, float shininess_)
		: name(name_),
		texture_diffuse(diffuse), texture_specular(specular), shininess(shininess_)
	{
	}

	void transfer(Shader shader)
	{
		std::string field;

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_diffuse);
		field = ".diffuse";
		shader.set_texture((name + field).c_str(), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture_specular);
		field = ".specular";
		shader.set_texture((name + field).c_str(), 1);


		field = ".shininess";
		shader.set_float((name + field).c_str(), shininess);
	}
};