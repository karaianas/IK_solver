// #include <GL/glew.h>
// #include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <algorithm>

using namespace std;

#include "Mesh.h"

//int main()
//{
//
//	// Create empty mesh
//	Mesh M;
//
//	// Create mesh elements
//	M.testInit();
//	M.preallocate();
//
//	// Build mesh
//	M.buildMesh();
//
//	// Precompute vertex normals and errors
//	M.initialization();
//
//	// Mesh info
//	M.printMeshInfo();
//
//	// Mesh collapse
//	eit e = M.getNextEdgeCollapse();
//
//	// Fix later to vit instead of int
//	//M.updateCollapse(M.edges[0].he->edge);
//	M.updateCollapse(e);
//
//	cout << "DONE" << endl;
//	// Mesh Info
//	M.printMeshInfo();
//
//	// Update priority edge list
//	M.updateEdgeList();
//
//	/***************************************************/
//	// Mesh collapse
//	e = M.getNextEdgeCollapse();
//
//	// Fix later to vit instead of int
//	M.updateCollapse(e);
//
//	// Mesh Info
//	M.printMeshInfo();
//
//	// Update priority edge list
//	M.updateEdgeList();
//
//
//	//M.updateCollapse(2, 1);
//	//M.printMeshInfo();
//
//	return 0;
//}