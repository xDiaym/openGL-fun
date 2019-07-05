#pragma once
#include <glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>


enum camera_movements {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	// TEMP (NOT USE NOW)
	UP,
	DOWN
};



class Camera
{
public:
	Camera(glm::vec3 pos, GLfloat fov=45.f, GLfloat sens=.1f, GLfloat move_speed=5.f);

	glm::mat4 get_view_matrix(void);
	void process_keyboard(camera_movements direction, GLfloat elapsed_time);
	void process_mouse_move(GLfloat xoffset, GLfloat yoffset);
	void process_scroll(GLfloat y_offset);

	void timelaps(GLfloat time, glm::vec3 center, float radius, float speed);

private:
	void update_vectors(void);


public:
	glm::vec3 position;
	glm::vec3 front;
	// Angles
	GLfloat yaw;	// default: -90.f
	GLfloat pitch;	// default: 0.f

	GLfloat FOV;	// default: 45.f

private:
	// Camera Setup
	// FOV a.k.a zoom
	GLfloat last_x_mouse_position;	// default: 0.f
	GLfloat last_y_mouse_position;	// default: 0.f

	// Coefficients
	GLfloat sensitivty;	// default: .2f
	GLfloat movement_speed;	// default: 5.f

	// Directions
	glm::vec3 world_up;	// default: glm::vec3(.0f, 1.f, .0f)
	
	// These values are calculated automatically
	glm::vec3 up;
	glm::vec3 right;
};