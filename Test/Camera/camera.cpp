#include "camera.h"


Camera::Camera(glm::vec3 pos, GLfloat fov, GLfloat sens, GLfloat move_speed)
	: position(pos),
	FOV(fov),
	sensitivty(sens),
	movement_speed(move_speed)
{
	front = glm::vec3(.0f, .0f, 1.f);


	// default values
	// Angles
	yaw  = -90.f;
	pitch = 0.f;

	// Camera Setup
	last_x_mouse_position = 0.f;
	last_y_mouse_position = 0.f;

	// Directions
	world_up = glm::vec3(.0f, 1.f, .0f);

	update_vectors();
}


glm::mat4 Camera::get_view_matrix(void)
{
	return glm::lookAt(position,
		position + front,
		world_up
	);
}


void Camera::process_keyboard(camera_movements direction, GLfloat elapsed_time)
{
	GLfloat velocity = movement_speed * elapsed_time;

	switch (direction)
	{
		// Z axis
	case FORWARD:
		position += front * velocity;
		break;
	
	case BACKWARD:
		position -= front * velocity;
		break;
	
		// X axis
	case RIGHT:
		position += right * velocity;
		break;
	
	case LEFT:
		position -= right * velocity;
		break;

		// Y axis
	case UP:
		position += up * velocity;
		break;

	case DOWN:
		position -= up * velocity;
		break;
	}

	update_vectors();
}


void Camera::process_mouse_move(GLfloat x_pos, GLfloat y_pos)
{
	static bool first_time = true;
	if (first_time)
	{
		last_x_mouse_position = x_pos;
		last_y_mouse_position = y_pos;
		first_time = false;
	}

	GLfloat x_offset = (x_pos - last_x_mouse_position) * sensitivty;
	GLfloat y_offset = (last_y_mouse_position - y_pos) * sensitivty;

	last_x_mouse_position = x_pos;
	last_y_mouse_position = y_pos;

	yaw += x_offset;
	pitch += y_offset;

	if (pitch >= 89.9f)
		pitch = 89.9f;
	if (pitch <= -89.9f)
		pitch = -89.9f;

	update_vectors();
}


void Camera::process_scroll(GLfloat y_offset)
{
	if (FOV <= 120.f && FOV >= 1.f)
		FOV -= y_offset;

	if (FOV > 120.f)
		FOV = 120.f;
	if (FOV < 1.f)
		FOV = 1.f;

	update_vectors();
}


void Camera::timelaps(GLfloat time, glm::vec3 center, float radius, float speed)
{
	position = glm::vec3(sin(time * speed) * radius,
		.0f,
		cos(time * speed) * radius);
	front = center - position;
}


void Camera::update_vectors(void)
{
	glm::vec3 new_front(.0f);
	new_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	new_front.y = sin(glm::radians(pitch));
	new_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	
	front = glm::normalize(new_front);
	right = glm::normalize(glm::cross(front, world_up));
	up    = glm::normalize(glm::cross(right, front));
}