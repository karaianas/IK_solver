#include "Joint.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

Joint::Joint(glm::mat4* W_p_, float length, float theta, float rho, float phi)
{
	len = length;
	W_p = W_p_;

	glm::mat4 R_x = glm::rotate(theta, glm::vec3(1, 0, 0));
	glm::mat4 R_y = glm::rotate(rho, glm::vec3(0, 1, 0));
	glm::mat4 R_z = glm::rotate(phi, glm::vec3(0, 0, 1));

	W = *W_p * glm::translate(glm::vec3(0, 0, length)) * R_z * R_y * R_x;
	//jAngle = glm::vec3();

}

void Joint::set_W(glm::vec3 angle)
{
	glm::mat4 R_x = glm::rotate(angle[0], glm::vec3(1, 0, 0));
	glm::mat4 R_y = glm::rotate(angle[1], glm::vec3(0, 1, 0));
	glm::mat4 R_z = glm::rotate(angle[2], glm::vec3(0, 0, 1));

	W = W * R_z * R_y * R_x;
	
	glm::mat4 test = *W_p * glm::translate(glm::vec3(0, 0, len));
	W[3] = test[3];

}

void Joint::set_jAngle(glm::vec3 angle)
{
	jAngle += angle;
}

glm::vec3 Joint::get_joint_pos()
{
	return glm::vec3(W[3]);
}

glm::vec3 Joint::get_x_axis()
{
	return glm::vec3(W[0]);
}

glm::vec3 Joint::get_y_axis()
{
	return glm::vec3(W[1]);
}

glm::vec3 Joint::get_z_axis()
{
	return glm::vec3(W[2]);
}

glm::mat4 Joint::get_W()
{
	return W;
}

glm::vec3 Joint::get_relAngle()
{
	float x = glm::acos(glm::dot(W[0], (*W_p)[0]) / (glm::length(W[0]) * glm::length((*W_p)[0])));
	float y = glm::acos(glm::dot(W[1], (*W_p)[1]) / (glm::length(W[1]) * glm::length((*W_p)[1])));
	float z = glm::acos(glm::dot(W[2], (*W_p)[2]) / (glm::length(W[2]) * glm::length((*W_p)[2])));

	return glm::vec3(x, y, z);
}

void Joint::y_rot(float ang)
{
	glm::mat4 R_y = glm::rotate(ang, glm::vec3(0, 1, 0));

	W = R_y * W;
}
