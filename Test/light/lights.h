#pragma once
// TODO add constant


#include <string>
#include <glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "../shader/shader.h"


struct dirLight
{
	const std::string name;

	glm::vec3 direction;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;


	dirLight(std::string name_, glm::vec3 direction_, glm::vec3 ambient_, glm::vec3 diffuse_, glm::vec3 specular_)
		: name(name_), direction(direction_), ambient(ambient_), diffuse(diffuse_), specular(specular_)
	{
	}

	void transfer(Shader shader)
	{
		std::string shader_field_name;

		shader_field_name = ".direction";
		shader.set_vec3((name + shader_field_name).c_str(), direction);

		shader_field_name = ".ambient";
		shader.set_vec3((name + shader_field_name).c_str(), ambient);

		shader_field_name = ".diffuse";
		shader.set_vec3((name + shader_field_name).c_str(), diffuse);

		shader_field_name = ".specular";
		shader.set_vec3((name + shader_field_name).c_str(), specular);
	}
};


struct pointLight
{
	std::string name;

	glm::vec3 position;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;


	pointLight(std::string name_,
		glm::vec3 position_,
		glm::vec3 ambient_, glm::vec3 diffuse_, glm::vec3 specular_,
		float constant_, float linear_, float quadratic_)

		: name(name_),
		position(position_),
		ambient(ambient_), diffuse(diffuse_), specular(specular_),
		constant(constant_), linear(linear_), quadratic(quadratic_)
	{
	}

	pointLight()
	{
	}
	void transfer(Shader shader)
	{
		std::string shader_field_name;

		shader_field_name = ".position";
		shader.set_vec3((name + shader_field_name).c_str(), position);

		shader_field_name = ".ambient";
		shader.set_vec3((name + shader_field_name).c_str(), ambient);
		shader_field_name = ".diffuse";
		shader.set_vec3((name + shader_field_name).c_str(), diffuse);
		shader_field_name = ".specular";
		shader.set_vec3((name + shader_field_name).c_str(), specular);
	
		shader_field_name = ".constant";
		shader.set_float((name + shader_field_name).c_str(), constant);
		shader_field_name = ".linear";
		shader.set_float((name + shader_field_name).c_str(), linear);
		shader_field_name = ".quadratic";
		shader.set_float((name + shader_field_name).c_str(), quadratic);
	}
};


struct pocketLight
{
	const std::string name;

	glm::vec3 position;
	glm::vec3 direction;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;

	float innerCutOff;
	float outerCutOff;


	pocketLight(std::string name_,
		glm::vec3 position_, glm::vec3 direction_,
		glm::vec3 ambient_, glm::vec3 diffuse_, glm::vec3 specular_,
		float constant_, float linear_, float quadratic_, 
		float innerCutOff_, float outerCutOff_)

		: name(name_),
		position(position_), direction(direction_),
		ambient(ambient_), diffuse(diffuse_), specular(specular_),
		constant(constant_), linear(linear_), quadratic(quadratic_),
		innerCutOff(innerCutOff_), outerCutOff(outerCutOff_)
	{
	}

	void transfer(Shader shader)
	{
		std::string shader_field_name;

		shader_field_name = ".position";
		shader.set_vec3((name + shader_field_name).c_str(), position);
		shader_field_name = ".direction";
		shader.set_vec3((name + shader_field_name).c_str(), direction);

		shader_field_name = ".ambient";
		shader.set_vec3((name + shader_field_name).c_str(), ambient);
		shader_field_name = ".diffuse";
		shader.set_vec3((name + shader_field_name).c_str(), diffuse);
		shader_field_name = ".specular";
		shader.set_vec3((name + shader_field_name).c_str(), specular);

		shader_field_name = ".constant";
		shader.set_float((name + shader_field_name).c_str(), constant);
		shader_field_name = ".linear";
		shader.set_float((name + shader_field_name).c_str(), linear);
		shader_field_name = ".quadratic";
		shader.set_float((name + shader_field_name).c_str(), quadratic);
	
		shader_field_name = ".innerCutOff";
		shader.set_float((name + shader_field_name).c_str(), innerCutOff);
		shader_field_name = ".outerCutOff";
		shader.set_float((name + shader_field_name).c_str(), outerCutOff);
	}

	void update(glm::vec3 position_, glm::vec3 direction_)
	{
		position = position_;
		direction = direction_;
	}
};