#include "main.h"
#include <string>
#include <stdio.h>

GLFWwindow* window;
map<int, string> objs = {
	{1,  "armadillo"},
	{2,  "brain"},
	{3,  "bunny"},
	{4,  "bunny-holefilled"},
	{5,  "cone"},
	{6,  "cow"},
	{7,  "elephant"},
	{8,  "fandisk"},
	{9,  "hand"},
	{10, "heptoroid"},
	{11, "ico"},
	{12, "moomoo"},
	{13, "mountains"},
	{14, "mug"},
	{15, "plane"},
	{16, "rocker-arm"},
	{17, "sphere"},
	{18, "teapot"},
	{19, "testpatch"},
	{20, "torus"},
	{21, "turtle"}
};

void error_callback(int error, const char* description) { fputs(description, stderr); }

void setup_callbacks()
{
	glfwSetErrorCallback(error_callback);

	glfwSetKeyCallback(window, Window::key_callback);

	glfwSetMouseButtonCallback(window, Window::mouse_button_callback);

	glfwSetCursorPosCallback(window, Window::cursor_pos_callback);

	glfwSetScrollCallback(window, Window::scroll_callback);

	glfwSetFramebufferSizeCallback(window, Window::resize_callback);
}

void setup_glew()
{
	// Initialize GLEW. Not needed on OSX systems.
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		glfwTerminate();
	}
	fprintf(stdout, "Current GLEW version: %s\n", glewGetString(GLEW_VERSION));
}

void setup_opengl_settings()
{
	setup_glew();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
}

void print_versions()
{
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("OpenGL version supported %s\n", glGetString(GL_VERSION));

#ifdef GL_SHADING_LANGUAGE_VERSION
	std::printf("Supported GLSL version is %s.\n", (char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
#endif
}

void print_objs()
{
	for (auto o : objs)
	{
		fprintf(stderr, "%d : %s\n", o.first, o.second.c_str());
	}
}

bool isNumber(string s)
{
	if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;

	char * p;
	strtol(s.c_str(), &p, 10);

	return (*p == 0);
}


int main(void)
{
	bool run = true;
	while (run)
	{
		system("clear");
		cout << "CSE 163 Assignment2" << endl;
		print_objs();
		cout << "Please enter a number or write object's name (type 0 or \"exit\" to terminate): ";
		string in, obj;
		cin >> in;

		int n = -1;
		if (isNumber(in))
			n = stoi(in);

		if (in.compare("exit") == 0 || n == 0)
			run = false;
		else if (n == -1 && !in.empty())
			obj = in;
		else if (n > 0)
			obj = objs[n];

		if (run)
		{
			bool run = true;
			while(run)
			{
				system("clear");
				string in;
				cout << "Run in Geomod(animation)? y/n : ";
				cin >> in;
				if (in.compare("y") == 0)
				{
					Window::geoMode = true;
					run = false;
				}
				else if (in.compare("n") == 0)
				{
					Window::geoMode = false;
					run = false;
				}
			}
		}

		if (run)
		{
			system("clear");
			window = Window::create_window(640, 480);

			print_versions();
			setup_callbacks();
			setup_opengl_settings();

			Window::initialize_objects(obj);

			while (!glfwWindowShouldClose(window))
			{
				Window::display_callback(window);
				Window::idle_callback();
			}

			Window::clean_up();
			glfwDestroyWindow(window);
			glfwTerminate();
		}
	}
	exit(EXIT_SUCCESS);
}
