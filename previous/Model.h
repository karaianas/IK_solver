#pragma once

#include "Mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Model
{
private:
	glm::mat4 toWorld;

	int vaoID;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

public:
	Model(int vaoID, Mesh *M, glm::mat4 matrix = glm::mat4(1.0f));

	Mesh *M;
	glm::vec3 lightPos;

	void setMatrix(glm::mat4 matrix);
	glm::mat4 getMatrix() const;

	int getVaoID() const;
	void setVaoID(int id);

	void setAmbient(glm::vec3 ambient);
	glm::vec3 getAmbient() const;

	void setDiffuse(glm::vec3 diffuse);
	glm::vec3 getDiffuse() const;

	void setSpecular(glm::vec3 specular);
	glm::vec3 getSpecular() const;


	void rotate(float deg, glm::vec3 axis);
	void rotateLight(float deg, glm::vec3 axis);
	void translate(glm::vec3 t);
	void scale(float s);

	void draw(GLuint shader);
};