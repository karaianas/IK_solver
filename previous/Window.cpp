#include "Window.h"
#include "path.h"

const char* window_title = "CSE163 ASSN2";

int Window::width;
int Window::height;

glm::mat4 Window::P;
glm::mat4 Window::V;

bool Window::isNormalMode = false;
bool Window::geoMode = true;

int count2 = 0;

// Shader
map<string, GLuint> shader;

// Default camera parameters
glm::vec3 camPos(0.0f, 0.0f, 5.0f);
Camera Window::cam(camPos);
glm::vec3 lastPos;

// Smooth Camera movement
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

Loader* loader;
Model* obj;

float factor = 0.2f;
float alpha = 0.0f;

bool once = false;
bool lineMode = false;
bool animate = false;
bool isEC = false;



void Window::initialize_objects(string name)
{
	// Load Object
	loader = new Loader();
	//<<<<<<< HEAD
		//obj = OBJLoader::loadOBJ(OBJECTS_PATH "testpatch.off", loader);//("C:\\Users\\karaianas\\Desktop\\models\\plane.off", loader);//
	//=======
	string file = OBJECTS_PATH + name + ".off";
	obj = OBJLoader::loadOBJ(file.c_str(), loader);
	//>>>>>>> origin/master
	obj->setDiffuse(glm::vec3(0.75164f, 0.60648f, 0.22648f));
	obj->setSpecular(glm::vec3(0.628281f, 0.555802f, 0.366065f));
	obj->setAmbient(glm::vec3(0.24725f, 0.1995f, 0.0745f));

	// Load Shader
	load_shaders();
}

void Window::load_shaders()
{
	shader["shader"] = LoadShaders(SHADERS_PATH "shader.vert", SHADERS_PATH "shader.frag");
	//WIN32_FIND_DATA data;
	//HANDLE hFind;

	//hFind = FindFirstFile(TEXT(SHADERS_PATH "*.vert"), &data);
	//if (hFind != INVALID_HANDLE_VALUE)
	//{
	//    do
	//    {
	//        _tprintf(L"File name: %s\n", data.cFileName);
	//        std::wstring fileName = data.cFileName;
	//        fprintf(stderr, "%s\n", fileName.c_str());
	//        
	//    } while (FindNextFile(hFind, &data));
	//}

	//FindClose(hFind);
}

void Window::clean_up()
{
	// Delete Object
	delete obj;
	// Delete Shaders
	for (auto it = shader.begin(); it != shader.end(); ++it)
		glDeleteProgram(it->second);
}

GLFWwindow* Window::create_window(int width, int height)
{
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);

	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwGetFramebufferSize(window, &width, &height);
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
	Window::width = width;
	Window::height = height;

	glViewport(0, 0, width, height);
}

void Window::idle_callback()
{
	if (animate)
	{
		Mesh *M = obj->M;
		glm::vec3 scp, tcp;
		glm::vec3 sip, tip;
		glm::vec3 sfp, tfp;

		sip = M->s_ini_pos;
		tip = M->t_ini_pos;

		sfp = M->s_fin_pos;
		tfp = M->t_fin_pos;

		alpha = alpha + 0.2f;

		scp = (1.0f - alpha) * sip + alpha * sfp;
		tcp = (1.0f - alpha) * tip + alpha * tfp;

		int sid = M->sIndex * 3;
		int tid = M->tIndex * 3;

		(*M->invertices)[sid] = scp.x;
		(*M->invertices)[sid + 1] = scp.y;
		(*M->invertices)[sid + 2] = scp.z;;

		(*M->invertices)[tid] = tcp.x;
		(*M->invertices)[tid + 1] = tcp.y;
		(*M->invertices)[tid + 2] = tcp.z;;


		if (alpha >= 0.998f)
		{
			alpha = 0.0f;
			animate = false;
			if (isEC)
				M->update_test();
		}
		loader->cleanUp();
		obj->setVaoID(loader->update(obj->M));
	}
}

void Window::display_callback(GLFWwindow* window)
{
	// Smooth camera control
	GLfloat currentFrame = (GLfloat)glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set view and projection matrix
	V = cam.GetViewMatrix();
	P = glm::perspective(glm::radians(cam.Zoom), (float)width / (float)height, 0.1f, 100.0f);

	obj->draw(shader["shader"]);

	//if (once)
	//{
	//	glLineWidth(2.5);
	//	glColor3f(1.0, 1.0, 1.0);
	//	glBegin(GL_LINE_LOOP);
	//	for (auto f : obj->M->faces)
	//	{
	//		glVertex3f(f->v0->pos[0], f->v0->pos[1], f->v0->pos[2]);
	//		glVertex3f(f->v1->pos[0], f->v1->pos[1], f->v1->pos[2]);
	//		glVertex3f(f->v2->pos[0], f->v2->pos[1], f->v2->pos[2]);
	//	}
	//	//glVertex3f(0.0, 0.0, 0.0);
	//	//glVertex3f(0.5, 0, 0);
	//	glEnd();
	//}

	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action== GLFW_REPEAT)
	{
		if (key == GLFW_KEY_ESCAPE)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		else if (key == GLFW_KEY_W)
		{
			cam.ProcessKeyboard(FORWARD, deltaTime);
		}
		else if (key == GLFW_KEY_S)
		{
			cam.ProcessKeyboard(BACKWARD, deltaTime);
		}
		else if (key == GLFW_KEY_A)
		{
			cam.ProcessKeyboard(LEFT, deltaTime);
		}
		else if (key == GLFW_KEY_D)
		{
			cam.ProcessKeyboard(RIGHT, deltaTime);
		}
		else if (key == GLFW_KEY_X)
		{
			// Translate along X axis
			glm::vec3 t(factor, 0.0f, 0.0f);
			if (mods == GLFW_MOD_SHIFT)
			{
				t *= -1.0f;
			}
			obj->translate(t);
		}
		else if (key == GLFW_KEY_Y)
		{
			// Translate along Y axis
			glm::vec3 t(0.0f, factor, 0.0f);
			if (mods == GLFW_MOD_SHIFT)
			{
				t *= -1.0f;
			}
			obj->translate(t);
		}
		else if (key == GLFW_KEY_Z)
		{
			// Translate along Z axis
			glm::vec3 t(0.0f, 0.0f, factor);
			if (mods == GLFW_MOD_SHIFT)
			{
				t *= -1.0f;
			}
			obj->translate(t);
		}
		else if (key == GLFW_KEY_MINUS)
		{
			// Scale down
			obj->scale(0.5f);
		}
		else if (key == GLFW_KEY_EQUAL)
		{
			// Scale up
			obj->scale(1.5f);
		}
		else if (key == GLFW_KEY_G)
		{
			if (geoMode)
			{
				if (obj->M->interpolateEC(animate))
				{
					animate = true;
					isEC = true;
				}
			}
			else
			{
				if (action != GLFW_REPEAT)
				{
					obj->M->update(true);
					loader->cleanUp();
					obj->setVaoID(loader->update(obj->M));
				}
			}
			//obj->M->update_test();
			//obj->M->information();
			//obj->M->get_mem();
			//obj->M->timetravel();
			//loader->cleanUp();
			//obj->setVaoID(loader->update(obj->M));
		}
		else if (key == GLFW_KEY_H)
		{
			if (geoMode)
			{
				if (obj->M->interpolateVS(animate))
				{
					obj->M->timetravel();
					animate = true;
					isEC = false;
				}
			}
			else
			{
				if (action != GLFW_REPEAT)
				{
					obj->M->update(false);
					loader->cleanUp();
					obj->setVaoID(loader->update(obj->M));
				}
			}
			//obj->M->update_test();
			//obj->M->information();
			//obj->M->get_mem();
			//obj->M->timetravel();
			//loader->cleanUp();
			//obj->setVaoID(loader->update(obj->M));
		}
		else if (key == GLFW_KEY_N)
		{
			isNormalMode = !isNormalMode;
		}
		else if (key == GLFW_KEY_P)
		{
			if (lineMode)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			lineMode = !lineMode;
		}
		else if (key == GLFW_KEY_R)
		{
			// Rest settings
			obj->setMatrix(glm::mat4(1.0f));
			cam.Zoom = 45.0f;
			cam.Position = camPos;
		}
		else if (key == GLFW_KEY_I)
		{
			system("clear");
			// Print cam info
			fprintf(stderr, "CAM INFO\n");
			fprintf(stderr, "Cam position => x:%f, y:%f, z:%f\n", cam.Position.x, cam.Position.y, cam.Position.z);
			fprintf(stderr, "Cam look at => x:%f, y:%f, z:%f\n", cam.Front.x, cam.Front.y, cam.Front.z);

			// Seperator
			fprintf(stderr, "\n");

			// Print obj info
			fprintf(stderr, "OBJ INFO\n");
			fprintf(stderr, "Number of vertex: %d\n", obj->M->numVertices);
			fprintf(stderr, "Number of face: %d\n", obj->M->numFaces);
			fprintf(stderr, "Number of edge: %d\n", obj->M->numEdges);
			glm::mat4 m = obj->getMatrix();
			fprintf(stderr, "Obj world matrix:\n");
			for (int j = 0; j < 4; ++j)
			{
				fprintf(stderr, "   ");
				for (int i = 0; i < 4; ++i)
				{
					fprintf(stderr, " %f", m[i][j]);
				}
				fprintf(stderr, "\n");
			}
		}
	}
}

void Window::mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		lastPos = Window::trackballMapping(xpos, ypos);
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
				obj->rotate(deg, rotAxis);
			else if (right)
				obj->rotateLight(deg, rotAxis);
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

void Window::scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
	cam.ProcessMouseScroll(yoffset);
}
