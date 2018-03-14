#include "Window.h"
#define PI 3.141592

const char* window_title = "Inverse Kinematics Solver";
//Cube * cube;
Arm* A;
glm::mat4* root;
Joint* J1;
Joint* J2;
Joint* J3;
Joint* J4;
Joint* E;
int ind;
int JSIZE;

int cycle;
float log30;
GLint shaderProgram;

//glm::vec3 gp(0, 0, 100);

// Also well
//glm::vec3 init(0, -4, -1);
//glm::vec3 gp(0, -4, -1);

// Working well
//glm::vec3 init(0, -4, 0);
//glm::vec3 gp(0, -4, 0);

// Well
//glm::vec3 init(1, -2, -2);
//glm::vec3 gp(1, -2, -2);

// path
//glm::vec3 init(0, -3, 0.5f);
//glm::vec3 gp(0, -3, 0.5f);

// unreach
//glm::vec3 init(-4, -4, 0);
//glm::vec3 gp(-4, -4, 0);

glm::vec3 init(0, 0, 0);
glm::vec3 gp(0, 0, 0);

// Variables just for demo
// ------------------------
float angle = 0.0f;
float rho = 2.0f;
float phi = 0.0f;
float theta = 0.0f;
void trajectory01();
void trajectory02();
// ------------------------

bool iter = false;
bool start = false;
bool wireframe = true;
bool CCD = false;
bool JT = true;

glm::vec3 lastPos;

glm::vec3 viewPos(0.0f, 0.0f, 10.0f);// keep it the same as the camera
glm::vec3 lightPos(0.0f, 5.0f, 0.0f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
glm::vec3 objColor(0.2f, 0.8f, 1.0f);
//glm::vec3 objColor(1.f, 1.f, 1.0f);

// Default camera parameters
glm::vec3 cam_pos(0.0f, 0.0f, 10.0f);
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);

int Window::width;
int Window::height;

glm::mat4 Window::P;
glm::mat4 Window::V;

void Window::initialize_objects()
{
	float len1, len2, len3, len4;

	cout << "Enter the lengths for each joint: " << endl;
	cin >> len1 >> len2 >> len3 >> len4;

		
	// First joint
	root = new glm::mat4();
	J1 = new Joint(root, 0.0f, 0, 0, 0);

	// Second joint
	J2 = new Joint(&J1->W, len1, PI / 2, 0, 0);
	//J2 = new Joint(&J1->W, 1.0f, PI / 2, 0, 0);
	//J2 = new Joint(&J1->W, 0.5f, 0, 0, 0);

	// Third joint
	J3 = new Joint(&J2->W, len2, -PI / 2, 0, 0);
	//J3 = new Joint(&J2->W, 2.0f, -PI / 2, 0, 0);
	//J3 = new Joint(&J2->W, 1.0f, 0, 0, 0);

	// Fourth joint
	J4 = new Joint(&J3->W, len3, 0, 0, 0);
	//J4 = new Joint(&J3->W, 1.5f, 0, 0, 0);
	//J4 = new Joint(&J2->W, 2.0f, -PI / 2, 0, 0);

	// Effector
	E = new Joint(&J4->W, len4, 0, 0, 0);
	//E = new Joint(&J4->W, 0.5f, 0, 0, 0);
	//E = new Joint(&J3->W, 1.5f, 0, 0, 0);

	vector<Joint*>* joints = new vector<Joint*>();
	joints->push_back(J1);
	joints->push_back(J2);
	joints->push_back(J3);
	joints->push_back(J4);
	joints->push_back(E);

	JSIZE = joints->size() - 2;
	ind = JSIZE;
	
	cycle = 0;
	log30 = 1000;

	delete A;
	A = new Arm(joints);

	// Load the shader program. Similar to the .obj objects, different platforms expect a different directory for files
#ifdef _WIN32 // Windows (both 32 and 64 bit versions)
	shaderProgram = LoadShaders("../shader.vert", "../shader.frag");
#else // Not windows
	shaderProgram = LoadShaders("shader.vert", "shader.frag");
#endif
}

void Window::clean_up()
{
	//delete(cube);
	delete(A);
	glDeleteProgram(shaderProgram);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	// Initializae mouse position
	glfwSetCursorPos(window, width / 2, height / 2);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
	Window::width = width;
	Window::height = height;
	// Set the viewport size
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback()
{
	if (!iter && start)
	{
		//cout << "Cycle: " << cycle << endl;
		if (CCD)
		{
			glm::vec3 state = A->iterationCCD(ind, gp, 0.5f, 0.1f, JT);

			// Goal reached
			if (state[1])
			{
				cout << "Done" << endl;
				iter = true;
			}
			else
			{
				cout << cycle << ":" << log30 << " " << state[2] << endl;
				if (log30 <= state[2])
					cycle++;
				else
					log30 = state[2];

				if (cycle > 1000)
				{
					cout << "Switching to non-CCD mode" << endl;
					CCD = false;
					log30 = 1000;
					cycle = 0;
				}
				else
				{
					if (state[0] >= 0)
					{
						//if (ind != state[0])
						//{
						//	cycle = 0;
						//	log30 = 1000;
						//}
						ind = state[0];
					}
					else if (state[0] < 0)
					{
						//cycle = 0;
						//log30 = 1000;
						ind = JSIZE;
					}
				}
			}
		}
		else
		{
			glm::vec2 state = A->iteration(gp, 0.5f, JT);
			iter = state[0];

			if (log30 <= state[1])
				cycle++;
			else
				log30 = state[1];

			if (cycle > 200)
			{
				cout << "Impossible" << endl;
				iter = true;
			}
		}
		A->update_buffer(gp);
	}
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.05f, 0.05f, 0.05f, 0.0f);
	//glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	// Use the shader of programID
	glUseProgram(shaderProgram);

	GLint viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");
	glUniform3f(viewPosLoc, viewPos.x, viewPos.y, viewPos.z);

	GLint lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
	glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);

	GLint lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
	glUniform3f(lightColorLoc, lightColor.x, lightColor.y, lightColor.z);

	GLint objColorLoc = glGetUniformLocation(shaderProgram, "objColor");
	glUniform3f(objColorLoc, objColor.x, objColor.y, objColor.z);
	
	// Render the arm
	A->draw(shaderProgram);

	// Mouse

	//if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
	//	double mouseX, mouseY;
	//	glfwGetCursorPos(window, &mouseX, &mouseY);

	//	glm::mat4 projViewInverse = glm::inverse(P * V);
	//	glm::vec4 pos = glm::vec4(projViewInverse * glm::vec4((mouseX / width) * 2.0f - 1.0f, (mouseY / height) * 2.0f - 1.0f, -1, 1));

	//	pos.x *= pos.w;
	//	pos.y *= pos.w;

	//	//cout << pos.x << " " << -pos.y << " " << pos.z << " " << pos.w << endl;
	//	gp[0] = pos.x;
	//	gp[1] = -pos.y;
	//	
	//}

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		// Start
		else if (key == GLFW_KEY_S)
		{
			if (!start)
				start = true;
			else
				start = false;
		}
		// Wireframe mode
		else if (key == GLFW_KEY_P)
		{
			if (wireframe)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			wireframe = !wireframe;
		}
		// CCD mode
		else if (key == GLFW_KEY_C)
		{
			if (CCD)
			{
				cout << "Non-CCD mode" << endl;
				CCD = false;
				cycle = 0;
				log30 = 1000;
			}
			else
			{
				cout << "CCD mode" << endl;
				CCD = true;
				cycle = 0;
				log30 = 1000;
			}
		}
		// Restart
		else if (key == GLFW_KEY_R)
		{
			iter = false;
			start = false;
			wireframe = false;
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			CCD = false;

			gp = init;
			initialize_objects();
		}
		// JT <-> JP
		else if (key == GLFW_KEY_J)
		{
			if (JT)
			{
				cout << "Pseudo-inverse Jacobian method" << endl;
				JT = false;
			}
			else
			{
				cout << "Transpose Jacobian method" << endl;
				JT = true;
			}
		}
	}
}

void trajectory01()
{
	angle += 0.015f;
	gp = glm::vec3(3 * sin(angle), sqrt(7), 3 * cos(angle));
}

void trajectory02()
{
	phi -= 0.01f;
	theta += 0.01f;
	gp = glm::vec3(rho * sin(phi) * cos(theta), rho * sin(phi) * sin(theta), rho * cos(phi));
}

void Window::mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		lastPos = trackballMapping(xpos, ypos);
	}
}

void Window::cursor_pos_callback(GLFWwindow *window, double xpos, double ypos)
{
	bool left, right;
	left = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
	right = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);
	if (left || right)
	{
		glm::vec3 curPos = trackballMapping(xpos, ypos);
		glm::vec3 direction = curPos - lastPos;
		float velocity = glm::length(direction);
		if (velocity > 0.0001)
		{
			glm::vec3 rotAxis = glm::cross(lastPos, curPos);
			float deg = velocity * 2.0f;

			if (left)
			{
					double mouseX, mouseY;
					glfwGetCursorPos(window, &mouseX, &mouseY);
					glm::mat4 projViewInverse = glm::inverse(P * V);
					glm::vec4 pos = glm::vec4(projViewInverse * glm::vec4((mouseX / width) * 2.0f - 1.0f, (mouseY / height) * 2.0f - 1.0f, -1, 1));

					pos.x *= pos.w;
					pos.y *= pos.w;
					//pos.z *= pos.w;

					//cout << pos.x << " " << -pos.y << " " << pos.z << " " << pos.w << endl;
					gp[0] = pos.x;
					gp[1] = -pos.y;
					gp[2] = 0;
					//life = LIFE;
					cycle = 0;
					log30 = 1000;
			}
			else if (right)
			{
				A->toWorld = glm::rotate(glm::mat4(1.0f), deg, rotAxis) * A->toWorld;
			}
		}
		lastPos = curPos;
	}
}

glm::vec3 Window::trackballMapping(double xpos, double ypos)
{
	glm::vec3 v;
	v.x = (2.0f * xpos - Window::width) / Window::width;
	v.y = (Window::height - 2.0f * ypos) / Window::height;
	v.z = 0.0f;

	float d = glm::length(v);
	d = d < 1.0f ? d : 1.0;

	v.z = sqrtf(1.001f - d*d);

	return glm::normalize(v);
}