#pragma once

#include <map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "Camera.h"
#include "Loader.h"
#include "OBJLoader.h"

class Window
{
public:
    static int width;
    static int height;
	static bool isNormalMode;
	static bool geoMode;

    static glm::mat4 P; // P for projection
    static glm::mat4 V; // V for view
	static Camera cam;
    static void initialize_objects(string name);
    static void load_shaders();
    static void clean_up();
    static void idle_callback();

    static GLFWwindow* create_window(int width, int height);

    static void display_callback(GLFWwindow*);
    static void resize_callback(GLFWwindow* window, int width, int height);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
	static glm::vec3 trackballMapping(double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};