#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

bool once = false;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 800;

// Shaders
const GLchar* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";
const GLchar* fragmentShaderSource = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(1.0f, 0.f, 0.f, 1.0f);\n"
"}\n\0";

// The MAIN function, from here we start the application and run the game loop
int main()
{
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "My Little Fork Crane", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	// Define the viewport dimensions
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);


	// Build and compile our shader program
	// Vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Check for compile time errors
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Check for compile time errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Link shaders
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Set up vertex data (and buffer(s)) and attribute pointers
	GLuint VBO, VAO;// , EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	vector<glm::vec3> seeds;
	seeds.push_back(glm::vec3(0, 0, 0));
	seeds.push_back(glm::vec3(0, 0, 1));
	seeds.push_back(glm::vec3(0, -2, 1));
	seeds.push_back(glm::vec3(0, -2, 2));

	vector<float> lengths;
	lengths.push_back(1.0f);
	
	vector<glm::vec3> vertices;
	vector<unsigned int> indices;

	float pi = 3.141592;
	for (int i = 0; i < 3; i++)
	{
		int theta = 60;
		float r = 0.25f;
		if (i == 0)
		{
			for (int j = 0; j < 360 / theta; j++)
			{
				vertices.push_back(seeds[i]);
				glm::vec3 a0(r * cos(j * theta * pi / 180), r * sin(j * theta* pi / 180), 0);
				glm::vec3 a1(r * cos((j + 1) * theta* pi / 180), r * sin((j + 1) * theta* pi / 180), 0);
				vertices.push_back(a0);
				vertices.push_back(a1);

				vertices.push_back(a0);
				vertices.push_back(a1);
				vertices.push_back(seeds[i + 1]);
			}
		}
		else if (i == 1)
		{
			for (int j = 0; j < 360 / theta; j++)
			{
				vertices.push_back(seeds[i]);
				glm::vec3 a0(r * cos(j * theta * pi / 180), r * sin(j * theta* pi / 180), 0);
				glm::vec3 a1(r * cos((j + 1) * theta* pi / 180), r * sin((j + 1) * theta* pi / 180), 0);

				glm::mat4 T(1, 0, 0, 0, 0, 0, 1, 0, 0, -1, 0, 0, 0, 0, 1, 1);
				glm::vec4 a0_ = T * glm::vec4(a0, 1);
				glm::vec4 a1_ = T * glm::vec4(a1, 1);

				vertices.push_back(glm::vec3(a0_));
				vertices.push_back(glm::vec3(a1_));

				vertices.push_back(glm::vec3(a0_));
				vertices.push_back(glm::vec3(a1_));
				vertices.push_back(seeds[i + 1]);
			}
		}
		else if (i == 2)
		{
			for (int j = 0; j < 360 / theta; j++)
			{
				vertices.push_back(seeds[i]);
				glm::vec3 a0(r * cos(j * theta * pi / 180), r * sin(j * theta* pi / 180), 0);
				glm::vec3 a1(r * cos((j + 1) * theta* pi / 180), r * sin((j + 1) * theta* pi / 180), 0);

				glm::mat4 T(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, -2, 1, 1);
				glm::vec4 a0_ = T * glm::vec4(a0, 1);
				glm::vec4 a1_ = T * glm::vec4(a1, 1);

				vertices.push_back(glm::vec3(a0_));
				vertices.push_back(glm::vec3(a1_));

				vertices.push_back(glm::vec3(a0_));
				vertices.push_back(glm::vec3(a1_));
				vertices.push_back(seeds[i + 1]);
			}
		}

	}
	
	for (int i = 0; i < vertices.size(); i++)
	{
		glm::mat4 T(0.7071, 0, 0.7071, 0, 0, 1, 0, 0, -0.7071, 0, 0.7071, 0, 0, 0, 0, 1);
		vertices[i]= glm::vec3(T * glm::vec4(vertices[i], 1));
		vertices[i] = 0.2f * vertices[i];
	}


	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	////glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO


						  // Uncommenting this call will result in wireframe polygons.
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.f, 0.f, 0.f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw our first triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	else if (key == GLFW_KEY_U && action == GLFW_PRESS)
	{

	}
}