#include "shader.h"


Shader::Shader(const char* vertexPath, const char* fragmentPath)
	: vertex_path(vertexPath), fragment_path(fragmentPath)
{
	this->programID = this->load();
}


void Shader::enable(void) const
{
	glUseProgram(programID);
}


void Shader::disable(void) const
{
	glUseProgram(0);
}


void Shader::set_vec3(const char* field, glm::vec3 &vector) const
{
	glUniform3fv(glGetUniformLocation(programID, field), 1, &vector[0]);
}


void Shader::set_float(const char* field, float number) const
{
	glUniform1f(glGetUniformLocation(programID, field), number);
}


void Shader::set_mat(const char* field, glm::mat4 &matrix) const
{
	glUniformMatrix4fv(glGetUniformLocation(programID, field), 1, GL_FALSE, &matrix[0][0]);
}


void Shader::set_texture(const char* field, GLuint count) const
{
	glUniform1i(glGetUniformLocation(programID, field), count);
}


GLuint Shader::load(void)
{
	GLuint program = glCreateProgram();
	GLuint vertex_shader   = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

	std::string vertex_source   = load_file(this->vertex_path);
	std::string fragment_source = load_file(this->fragment_path);

	const char* vertex_correct_format = vertex_source.c_str();
	const char* fragment_correct_format = fragment_source.c_str();

	// ===========================================================
	
	glShaderSource(vertex_shader, 1, &vertex_correct_format, NULL);
	glCompileShader(vertex_shader);

	glShaderSource(fragment_shader, 1, &fragment_correct_format, NULL);
	glCompileShader(fragment_shader);

	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);
	glValidateProgram(program);
	// TODO add getProigramiv() (ERROR HANDLER)

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	return program;
}