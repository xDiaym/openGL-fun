#pragma once
#include <string>
#include <glew.h>
#include <glm.hpp>

#include "../utils/utils.h"


class Shader
{
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	void enable(void) const;
	void disable(void) const;

	void set_vec3    (const char* field, glm::vec3 &vector) const;
	void set_float   (const char* field, float number)      const;
	void set_mat     (const char* field, glm::mat4 &matrix) const;
	void set_texture (const char* field, GLuint count)    const;

	const char* vertex_path;
	const char* fragment_path;
	GLuint programID;

private:
	GLuint load(void);
};