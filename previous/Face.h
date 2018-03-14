#pragma once
#include "glm/glm.hpp"
#include <iostream>
#include <vector>

#include "Type.h"

using namespace std;

class Face {
public:

	int index;
	bool is;
	//bool isBoundary;

	Face* f0;
	Face* f1;
	Face* f2;

	// not active
	vector<Edge*> edges;

	Vertex* v0;
	Vertex* v1;
	Vertex* v2;

	glm::vec3 get_fNormal();
};
