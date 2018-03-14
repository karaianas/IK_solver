#include "Model.h"
#include "Window.h"

Model::Model(int vaoID, Mesh *M, glm::mat4 matrix) : vaoID(vaoID), M(M),toWorld(matrix)
{
	lightPos = glm::vec3(0.0f, 5.0f, 100.f);
}

void Model::setMatrix(glm::mat4 matrix) { this->toWorld = matrix; }
glm::mat4 Model::getMatrix() const { return this->toWorld; }

int Model::getVaoID() const { return this->vaoID; }
void Model::setVaoID(int id) { this->vaoID = id; }

void Model::setAmbient(glm::vec3 ambient) { this->ambient = ambient; }
glm::vec3 Model::getAmbient() const { return this->ambient; }

void Model::setDiffuse(glm::vec3 diffuse) { this->diffuse = diffuse; }
glm::vec3 Model::getDiffuse() const { return this->diffuse; }

void Model::setSpecular(glm::vec3 specular) { this->specular = specular; }
glm::vec3 Model::getSpecular() const { return this->specular; }

void Model::rotate(float deg, glm::vec3 axis)
{
	this->toWorld = glm::rotate(glm::mat4(1.0f), deg, axis) * this->toWorld;
}

void Model::rotateLight(float deg, glm::vec3 axis)
{
	lightPos = glm::rotate(glm::mat4(1.0f), deg, axis) * glm::vec4(lightPos, 0.0f);
}

void Model::translate(glm::vec3 t)
{
	this->toWorld = glm::translate(glm::mat4(1.0f), t) * toWorld;
}

void Model::scale(float s)
{
	this->toWorld = glm::scale(glm::mat4(1.0f), glm::vec3(s,s,s)) * toWorld;
}

void Model::draw(GLuint shader)
{
	glUseProgram(shader);
	GLuint uModel = glGetUniformLocation(shader, "model");
	GLuint uView  = glGetUniformLocation(shader, "view");
	GLuint uProj  = glGetUniformLocation(shader, "proj");

	glUniformMatrix4fv(uModel, 1, GL_FALSE, &getMatrix()[0][0]);
	glUniformMatrix4fv(uView , 1, GL_FALSE, &Window::V[0][0]);
	glUniformMatrix4fv(uProj,  1, GL_FALSE, &Window::P[0][0]);

	// normal mode
	glUniform1f(glGetUniformLocation(shader, "isNormalMode"), Window::isNormalMode);

	// Light and View position
	glm::vec3 camView = Window::cam.Position;
	glUniform3f(glGetUniformLocation(shader, "viewPos"), camView.x, camView.y, camView.z);
	glUniform3f(glGetUniformLocation(shader, "light.direction"), lightPos.x, -lightPos.y, -lightPos.z);

	// Diffuse settings
	glUniform3f(glGetUniformLocation(shader, "material.diffuse"), diffuse.x, diffuse.y, diffuse.z);
	glUniform3f(glGetUniformLocation(shader, "light.diffuse"), 1.0f, 1.0f, 1.0f);

	// Specular settings
	glUniform3f(glGetUniformLocation(shader, "material.specular"), specular.x, specular.y, specular.z);
	glUniform1f(glGetUniformLocation(shader, "material.shininess"), 0.5f);
	glUniform3f(glGetUniformLocation(shader, "light.specular"), 1.0f, 1.0f, 1.0f);

	// Ambient settings
	glUniform3f(glGetUniformLocation(shader, "material.ambient"), ambient.x, ambient.y, ambient.z);
	glUniform3f(glGetUniformLocation(shader, "light.ambient"), 0.5f, 0.5f, 0.5f);

	glBindVertexArray(vaoID);
	glDrawElements(GL_TRIANGLES, M->infaces->size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}