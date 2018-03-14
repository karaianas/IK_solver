#include "Edge.h"
#include "Vertex.h"
#include <glm/glm.hpp>

void Edge::set_Q()
{
	Q = v0->Q + v1->Q;
}

glm::vec4 Edge::get_vbar()
{
	//glm::vec4 v_;
	glm::vec4 one(0, 0, 0, 1);

	//glm::mat4 D(Q[0][0], Q[0][1], Q[0][2], 0, \
	//	Q[0][1], Q[1][1], Q[1][2], 0, \
	//	Q[0][2], Q[1][2], Q[2][2], 0, \
	//	Q[0][3], Q[1][3], Q[2][3], 1);

	glm::mat4 D(Q[0][0], Q[1][0], Q[2][0], 0, \
		Q[1][0], Q[1][1], Q[2][1], 0, \
		Q[2][0], Q[2][1], Q[2][2], 0, \
		Q[3][0], Q[3][1], Q[3][2], 1);

	float det = glm::determinant(D);

	// Det = 0 i.e. not invertible
	if (abs(det) < 0.01f)
	{
		// Choose midpoint
		v_ = glm::vec4((v0->pos + v1->pos) / 2.0f, 1);
	}
	else
	{
		v_ = glm::inverse(D) * one;
	}

	return v_;
}

float Edge::get_error()
{
	//glm::vec4 v_ = get_vbar();
	glm::vec4 vtr = v_ * Q;

	error = glm::dot(vtr, v_);

	return error;
}
