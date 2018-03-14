#pragma once
#include <glm/glm.hpp>

using namespace std;

class Joint {
public:

	// Variables
	glm::vec3 start;
	glm::vec3 end;
	float len;
	glm::mat4 W;
	glm::mat4* W_p;
	glm::vec3 jAngle;

	// Constructor
	Joint(glm::mat4* W_p_, float length, float theta, float rho, float phi);

	// Functions
	void set_W(glm::vec3 angle);
	void set_jAngle(glm::vec3 angle);

	glm::vec3 get_joint_pos();
	glm::vec3 get_x_axis();
	glm::vec3 get_y_axis();
	glm::vec3 get_z_axis();
	glm::mat4 get_W();
	glm::vec3 get_relAngle();
	void y_rot(float ang);
};