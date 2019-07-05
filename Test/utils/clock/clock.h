#pragma once
#include <glew.h>
#include <glfw3.h>


class Clock
{
public:
	Clock();
	void tick();
	GLfloat getFPS();

	GLfloat elapsed_time;

private:
	GLfloat last_time;
	GLfloat new_time;
};