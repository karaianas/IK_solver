#pragma once
#include "glm/glm.hpp"
#include <iostream>
#include <vector>

#include "Type.h"

using namespace std;

class Edge {
public:

	int index;
	bool is;
	bool isBoundary;

	Vertex* v0;
	Vertex* v1;

	int n0;
	int n1;

	//vector<Face*> faces;

	float error;
	glm::mat4 Q;
	glm::vec4 v_;
	
	void set_Q();
	glm::vec4 get_vbar();
	float get_error();
};
