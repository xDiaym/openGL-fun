#define GLEW_STATIC
#define LAMPS 4

#include <iostream>
#include <vector>
#include <cmath>
#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "data.h"
#include "utils/clock/clock.h"
#include "light/lights.h"
#include "light/material.h"
#include "shader/shader.h"
#include "Camera/camera.h"


Camera cam(glm::vec3(.0f, .0f, 3.f));

bool pressed_keys[1024];
GLfloat alpha = 0.2f;


extern GLfloat vertices[];
extern GLuint indices[];
extern glm::vec3 cubePositions[];
extern glm::vec3 lampPositions[];


void key_callback(GLFWwindow *window, int keycode, int scancode, int action, int mode)
{
	if (action == GLFW_RELEASE)
	{
		pressed_keys[keycode] = false;
		if (keycode ==GLFW_KEY_ESCAPE)
		{
			std::clog << "KEY ESCAPE WAS RELEASED, EXITING" << std::endl;
			glfwSetWindowShouldClose(window, GL_TRUE);
			exit(0);
		}
	}
	else if (action == GLFW_PRESS)
	{
		pressed_keys[keycode] = true;
	}
}

void key_manager(GLfloat d_time)
{

	if (pressed_keys[GLFW_KEY_UP])
	{
		if (alpha < 1.0f)
			alpha += .5f * d_time;
	}

	if (pressed_keys[GLFW_KEY_DOWN])
	{
		if (alpha > 0.0f)
			alpha -= .5f * d_time;
	}

	if (pressed_keys[GLFW_KEY_W])
		cam.process_keyboard(camera_movements::FORWARD,  d_time);

	if (pressed_keys[GLFW_KEY_S])
		cam.process_keyboard(camera_movements::BACKWARD, d_time);

	if (pressed_keys[GLFW_KEY_D])
		cam.process_keyboard(camera_movements::RIGHT,    d_time);

	if (pressed_keys[GLFW_KEY_A])
		cam.process_keyboard(camera_movements::LEFT,     d_time);

	if (pressed_keys[GLFW_KEY_R])
		cam.process_keyboard(camera_movements::UP,       d_time);

	if (pressed_keys[GLFW_KEY_F])
		cam.process_keyboard(camera_movements::DOWN,     d_time);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	cam.process_mouse_move(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	cam.process_scroll(yoffset);
}

void resize_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}



int main(int argc, char** argv)
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	GLFWwindow *window = glfwCreateWindow(1920, 1080, "That`s Work!", glfwGetPrimaryMonitor(), nullptr);
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSwapInterval(1);

	glEnable(GL_DEPTH_TEST);

	glfwSetKeyCallback(window, key_callback);
	glfwSetWindowSizeCallback(window, resize_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	// glViewport(0, 0, 800, 600);

	if (window == nullptr)
	{
		std::cerr << "CREATE WINDOW ERROR" << std::endl;
		glfwTerminate();
		exit(-100);
	}
	else
		std::clog << "CREATE WINDOW SUCCESS" << std::endl;

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "LINK ERROR :: GLEW" << std::endl;
		glfwTerminate();
		exit(-2);
	}
	else
		std::clog << "LINK SUCCESS :: GLEW" << std::endl;


	// ======================================

	GLuint VBO, VAO, EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STREAM_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	
	glBindVertexArray(0);


	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * (sizeof(GLfloat)), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	// ======================================

	Shader shaders("shaders_sources/vertex.glsl", "shaders_sources/fragment.glsl");
	Shader lamp_shaders("shaders_sources/vertex_light.glsl", "shaders_sources/fragment_light.glsl");

	// ======================================

	GLuint container_diffuse = load_texture("images/container2.png");
	GLuint container_specular = load_texture("images/container2_specular.png");

	// ======================================

	// CONTAINER MATERIAL
	std::string name = "material";
	material container(name, container_diffuse, container_specular, 32.0f);

	// SUN
	name = "dirlight";

	dirLight sun(name,
		glm::vec3(.0f, -1.f, .0f),
		glm::vec3(.05f), glm::vec3(.4f), glm::vec3(.5f));

	// CUBE LAMP
	pointLight lamps[LAMPS];

	for (int i = 0; i < LAMPS; i++)
	{
		glm::vec3 pointl_position(1.f, .0f, 1.f);
		std::stringstream temp;
		temp << "pointLights[" << i << "]";
		temp >> name;

		std::cout << name << std::endl;

		pointLight tmp(name,
			lampPositions[i],
			glm::vec3(.05f), glm::vec3(.8f), glm::vec3(1.f),
			//glm::vec3(.0f), glm::vec3(.0f), glm::vec3(.0f),
			1.f, .045f, .0075f);
		lamps[i] = tmp;
	}
	glm::vec3 light_color(1.f, 1.f, 1.f);

	// POCKET LIGHT
	name = "projector";

	pocketLight pocket_light(name,
		cam.position, cam.front,
		glm::vec3(.05f), glm::vec3(.8f), glm::vec3(1.f),
		1.f, .045f, .0075f,
		glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(20.f)));


	Clock clock;

	while (!glfwWindowShouldClose(window))
	{
		clock.tick();

		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		glfwPollEvents();
		key_manager(clock.elapsed_time);

		glClearColor(.3f, .3f, .3f, 1.f);
		glClearDepth(std::numeric_limits<GLfloat>::max());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		// =====================
		/*
		******** NOTE ********
		:f      means     float
		:i      means     integer
		:ui     means     unsigned int
		:xf     means     x argument, type float
		:fv     means     float vector
		******** END  ********
		*/

		if (pressed_keys[GLFW_KEY_T])
			cam.timelaps(glfwGetTime(), glm::vec3(.0f), 6, -.1);

		shaders.enable();
		glBindVertexArray(VAO);

		glm::mat4 view(1.0f);
		view = cam.get_view_matrix();

		glm::mat4 projection(1.0f);
		projection = glm::perspective(glm::radians(cam.FOV), width / (float)height, 0.1f, 100.0f);

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f));

		// Global
		shaders.set_vec3("viewPos", cam.position);

		shaders.set_mat("view", view);
		shaders.set_mat("projection", projection);
		shaders.set_mat("model", model);

		// Material
		container.transfer(shaders);

		// Direction Light
		sun.transfer(shaders);
		
		// Point Light
		for (int i = 0; i < LAMPS; i++)
			lamps[i].transfer(shaders);

		// PROJECTOR
		pocket_light.update(cam.position, cam.front);
		pocket_light.transfer(shaders);


		glDrawArrays(GL_TRIANGLES, 0, 36);
		

		// ================================

		for (int i = 0; i < 10; i++)
		{
			glm::mat4 model(1.0f);
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, glm::radians(i * 20.f), glm::vec3(.3f, .6f, .9f));

			glUniformMatrix4fv(glGetUniformLocation(shaders.programID, "model"), 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
			//glDrawElements(GL_TRIANGLES, 3 * 2, GL_UNSIGNED_INT, 0);
		}
		shaders.disable();
		glBindVertexArray(0);
		//glBindTexture(GL_TEXTURE_2D, 0);
		// =====================

		lamp_shaders.enable();
		glBindVertexArray(lightVAO);

		lamp_shaders.set_mat("view", view);
		lamp_shaders.set_mat("projection", projection);
		lamp_shaders.set_vec3("light_color", light_color);

		for (int i = 0; i < LAMPS; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, lampPositions[i]);
			model = glm::scale(model, glm::vec3(0.5f));

			lamp_shaders.set_mat("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		lamp_shaders.disable();
		glBindVertexArray(0);
		glfwSwapBuffers(window);	
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}
