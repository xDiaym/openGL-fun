#pragma once
#include "clock.h"


Clock::Clock()
{
	last_time = glfwGetTime();
}


void Clock::tick()
{
	new_time = glfwGetTime();
	elapsed_time = new_time - last_time;
	last_time = new_time;
}

GLfloat Clock::getFPS()
{
	return 1.f / elapsed_time;
}