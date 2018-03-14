#pragma once
#include <vector>
#include <glm/glm.hpp>

#include "Vertex.h"
#include "Edge.h"
#include "Face.h"
#include "Type.h"

using namespace std;

class Mesh {

public:
	Mesh(vector<float> *invertices, vector<Vertex*> vertices, vector<unsigned int> *infaces);

	unsigned int numVertices, numFaces, numEdges;
	unsigned int MAX_VERT;

	vector<float> *invertices;
	vector<unsigned int> *infaces;

	vector<Face*> faces;
	vector<Edge*> edges;
	vector<Vertex*> vertices;
	vector<Edge*>* elist;

	vector<int> updatemem;

	int sIndex, tIndex;
	vector<int> simem;
	vector<int> timem;
	vector<glm::vec3> sinimem;
	vector<glm::vec3> tinimem;
	vector<glm::vec3> stfinmem;
	glm::vec3 s_ini_pos, s_fin_pos;
	glm::vec3 t_ini_pos, t_fin_pos;
	glm::vec3 s_lastPos, t_lastPos;

	//void datastructure_B();
	void datastructure();

	vector<Face*>* get_fIntersection(Vertex* v0, Vertex* v1);
	vector<Face*>* get_fUnion(Vertex* v0, Vertex* v1);
	vector<Vertex*>* get_vIntersection(Vertex* v0, Vertex* v1);
	vector<Face*>* get_FA0(vector<Face*>* U, vector<Vertex*>* vI, Vertex* v0, Vertex* v1);
	vector<Vertex*>* get_vUnion(Vertex* v0, Vertex* v1);
	vector<Vertex*>* get_v0only(vector<Vertex*>* vI, Vertex* v0);

	void information();
	void update(bool isEC);
	void update_elist();

	void update_test();
	Face* get_face(Vertex* v0, Vertex* v1, Vertex* v2);

	void timetravel();
	vector<vector<int>*> vimem;
	vector<vector<glm::vec3>*> vpmem;
	vector<vector<int>*> fnmem;
	vector<vector<glm::vec3>*> fvmem;
	vector<vector<int>*> eimem;

	bool interpolateEC(bool animating);
	bool interpolateVS(bool animating);


};