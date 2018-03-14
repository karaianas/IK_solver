#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Joint.h"
#include "Arm.h"

using namespace std;

#define PI 3.141592

void display_M(glm::mat4 M);
void display_M3(glm::mat3 M);

int main()
{
	// Few notes: must pass angle relative to the previous Joint
	// The length should be the joint offset

	// First joint
	glm::mat4* root;
	root = new glm::mat4();
	Joint J1(root, 0.0f, 0, 0, 0);

	cout << "J1 World Matrix" << endl;
	display_M(J1.W);

	// Second joint
	Joint J2(&J1.W, 1.0f, PI/2, 0, 0);

	cout << "J2 World Matrix" << endl;
	display_M(J2.W);

	// Third joint
	cout << "J3 World Matrix" << endl;
	Joint J3(&J2.W, 2.0f, -PI / 2, 0, 0);
	display_M(J3.W);

	// Effector
	cout << "E World Matrix" << endl;
	Joint E(&J3.W, 1.0f, 0, 0, 0);
	display_M(E.W);

	vector<Joint*>* joints = new vector<Joint*>();
	joints->push_back(&J1);
	joints->push_back(&J2);
	joints->push_back(&J3);
	joints->push_back(&E);

	Arm A(joints);

	A.iteration(glm::vec3(0, - 3, 1), 30, 0.1f, 0.1f);
	
 	return 0;
}


void display_M(glm::mat4 M)
{
	cout << round(M[0][0]) <<  " " << round(M[1][0]) <<  " " << round(M[2][0]) <<  " " << round(M[3][0]) <<  endl;
	cout << round(M[0][1]) <<  " " << round(M[1][1]) <<  " " << round(M[2][1]) <<  " " << round(M[3][1]) <<  endl;
	cout << round(M[0][2]) <<  " " << round(M[1][2]) <<  " " << round(M[2][2]) <<  " " << round(M[3][2]) <<  endl;
	cout << round(M[0][3]) <<  " " << round(M[1][3]) <<  " " << round(M[2][3]) <<  " " << round(M[3][3]) <<  endl;
}

void display_M3(glm::mat3 M)
{
	cout << M[0][0] << " " << M[1][0] << " " << M[2][0] << endl;
	cout << M[0][1] << " " << M[1][1] << " " << M[2][1] << endl;
	cout << M[0][2] << " " << M[1][2] << " " << M[2][2] << endl;
}