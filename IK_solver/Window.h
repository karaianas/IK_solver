#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#include "Cube.h"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include "Arm.h"
#include "Joint.h"

class Window
{
public:
	static int width;
	static int height;
	static glm::mat4 P;
	static glm::mat4 V;
	static void initialize_objects();
	static void clean_up();
	static GLFWwindow* create_window(int width, int height);
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback();
	static void display_callback(GLFWwindow*);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
	static void cursor_pos_callback(GLFWwindow *window, double xpos, double ypos);
	static glm::vec3 trackballMapping(double xpos, double ypos);
};

#endif
