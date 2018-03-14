#pragma once
#include "glm/glm.hpp"
#include <iostream>
#include <vector>

#include "Type.h"

using namespace std;

class Vertex {
public:

	int index;
	bool is;
	bool isBoundary;

	int ind;

	glm::vec3 pos;
	glm::vec3 normal;
	glm::mat4 Q;

	//vector<Edge*> edges;
	vector<Face*>* faces;
	vector<Vertex*>* vertices;

	glm::vec3 get_vNormal();
	void normalize();

	void set_Q();
};