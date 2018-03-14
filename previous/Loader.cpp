#include "Loader.h"

/**
 * loadModel loads raw model that is in OFF format.
 */
Model* Loader::loadModel(vector<float> *invertices, vector<Vertex*> vertices, vector<unsigned int> *infaces)
{
	Mesh *M = new Mesh(invertices, vertices, infaces);

	GLuint vaoID = loadToVAO();

	// Load VBO for vertices
	loadToVBO(0, 3, M->invertices);

	auto normal = new vector<float>();
	for (auto v : M->vertices)
	{
		normal->push_back(v->normal.x);
		normal->push_back(v->normal.y);
		normal->push_back(v->normal.z);
	}
	// Load VBO for normals
	loadToVBO(1, 3, normal);

	// Load EBO for faces 
	loadToEBO(M->infaces);

	// Unbind VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return new Model(vaoID, M);
}

/**
 * Generate VAO and returns its VAO ID.
 * Must call glBindVertexArray(0) after calling this fuction.
 */
int Loader::loadToVAO()
{
	GLuint vao;

	glGenVertexArrays(1, &vao);
	vaos.push_back(&vao);
	glBindVertexArray(vao);

	return vao;
}

/**
 * Generate VBO and bind buffer data.
 * The first parameter sets the vertex attrib location.
 * The second parameter sets the size of the data (e.g vector of 3 component should pass 3. In other words vec2 has 2 components)
 * The third parameter is vectors that contains data like verticies or normals.
 */
void Loader::loadToVBO(int attrNum, int size, vector<float>* data)
{
	GLuint vbo;
	glGenBuffers(1, &vbo);
	vbos.push_back(&vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, data->size() * sizeof(float), data->data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(attrNum);
	glVertexAttribPointer(attrNum, size, GL_FLOAT, GL_FALSE, size * sizeof(GLfloat), (GLvoid*)0);
}

/**
 * Generate EBO if model has indices.
 */
void Loader::loadToEBO(vector<unsigned int>* indices)
{
	GLuint ebo;
	glGenBuffers(1, &ebo);
	ebos.push_back(&ebo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size() * sizeof(unsigned int), indices->data(), GL_STATIC_DRAW);
}

void Loader::cleanUp()
{
	for (auto vao : vaos)
		glDeleteVertexArrays(1, vao);

	for (auto vbo : vbos)
		glDeleteBuffers(1, vbo);

	for (auto ebo : ebos)
		glDeleteBuffers(1, ebo);
}

int Loader::update(Mesh *M)
{
	GLuint vaoID = loadToVAO();

	// Load VBO for vertices
	loadToVBO(0, 3, M->invertices);

	auto normal = new vector<float>();
	for (auto v : M->vertices)
	{
		if (v->is)
		{
			normal->push_back(v->normal.x);
			normal->push_back(v->normal.y);
			normal->push_back(v->normal.z);
		}

	}
	// Load VBO for normals
	loadToVBO(1, 3, normal);

	// Load EBO for faces 
	loadToEBO(M->infaces);

	// Unbind VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return vaoID;
}