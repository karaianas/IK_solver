#include "Face.h"
#include "Vertex.h"

using namespace std;

glm::vec3 Face::get_fNormal()
{
	glm::vec3 p0 = v0->pos;
	glm::vec3 p1 = v1->pos;
	glm::vec3 p2 = v2->pos;

	//cout << p0[0] << " " << p0[1] << " " << p0[2] << endl;
	//cout << p1[0] << " " << p1[1] << " " << p1[2] << endl;
	//cout << p2[0] << " " << p2[1] << " " << p2[2] << endl;

	return glm::normalize(glm::cross(p1 - p0, p2 - p0));
}