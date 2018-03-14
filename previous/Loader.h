#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif

#include <assert.h>
#include <vector>

#include "Model.h"

using namespace std;

class Loader
{
private:
	vector<GLuint*> vaos;
	vector<GLuint*> vbos;
	vector<GLuint*> ebos;

	int  loadToVAO();
	void loadToVBO(int attrNum, int size, vector<float>* data);
	void loadToEBO(vector<unsigned int>* indices);

public:

	Model* loadModel(vector<float>* invertices, vector<Vertex*> vertices, vector<unsigned int>* infaces);
	int update(Mesh * M);
	void cleanUp();
};