#include "Cube.h"
#include "Window.h"
#include <vector>

using namespace std;

Cube::Cube()
{
	this->toWorld = glm::mat4(1.0f);

	this->angle = 0.0f;

	// Create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);
	
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	//------------------------
	vector<glm::vec3> seeds;
	seeds.push_back(glm::vec3(0, 0, 0));
	seeds.push_back(glm::vec3(0, 0, 1));
	seeds.push_back(glm::vec3(0, -2, 1));
	seeds.push_back(glm::vec3(0, -2, 2));

	vector<float> lengths;
	lengths.push_back(1.0f);

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
		vertices[i] = glm::vec3(T * glm::vec4(vertices[i], 1));
		//vertices[i] = 0.2f * vertices[i];
	}
	//------------------------

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	
	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive vertex attributes. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
	
	// Wireframe mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

Cube::~Cube()
{
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);
}

void Cube::draw(GLuint shaderProgram)
{ 
	// Calculate combination of the model (toWorld), view (camera inverse), and perspective matrices
	glm::mat4 MVP = Window::P * Window::V * toWorld;
	// We need to calculate this because as of GLSL version 1.40 (OpenGL 3.1, released March 2009), gl_ModelViewProjectionMatrix has been
	// removed from the language. The user is expected to supply this matrix to the shader when using modern OpenGL.
	GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

	glBindVertexArray(VAO);
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	glBindVertexArray(0);
}

void Cube::update()
{
	
	spin(1.0f);
}

void Cube::spin(float deg)
{
	this->angle += deg;
	if (this->angle > 360.0f || this->angle < -360.0f) this->angle = 0.0f;
	// This creates the matrix to rotate the cube. You'd probably want to change this with the more favorable way of performing a spin
	// which you did in the previous project.
	this->toWorld = glm::rotate(glm::mat4(1.0f), this->angle / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
}

