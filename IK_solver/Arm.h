#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Joint.h"
#include "Window.h"

using namespace std;

class Arm {
public:

	// Variables
	vector<Joint*> joints;

	GLuint VBO, VAO, EBO, VBO1;
	vector<glm::vec3> vertices;
	vector<unsigned int> indices;
	vector<glm::vec3> normals;
	glm::mat4 toWorld;
	float angle;

	GLuint uProjection, uModelView, uModel;

	// Constructor
	Arm(vector<Joint*>* js);
	Arm(int num);
	~Arm();

	// Functions
	void initialization();
	void update_buffer(glm::vec3 gp);

	void draw_goal(glm::vec3 gp, float radius, int stacks, int slices);
	void draw_cone();
	void draw_shadow();

	glm::vec2 singleCCD(int ind, glm::vec3 goal, float alpha, float prev, bool JT);
	glm::vec3 iterationCCD(int ind, glm::vec3 goal, float alpha, float prev, bool JT);
	glm::vec2 iteration(glm::vec3 goal, float alpha, bool JT);
	void iteration(glm::vec3 goal, int num, float threshold, float alpha);


	void draw(GLuint shaderProgram);
	void update();
	void spin(float);
};