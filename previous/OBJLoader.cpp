#include "OBJLoader.h"
#include "Mesh.h"

Model* OBJLoader::loadOBJ(const char* obj, Loader *loader)
{
	unsigned int numVertices, numFaces, numEdges;

	// Open File
	FILE *fp;
	fprintf(stderr, "Opening file %s\n", obj);
	fp = fopen(obj, "rb");

	if (fp == nullptr)
	{
		fprintf(stderr, "Open FAIL!\n");
		exit(-1);
	}

	// Ignore OFF
	fscanf(fp, "%*s");

	// Get number of vertices
	fscanf(fp, "%d", &numVertices);

	// Get number of faces 
	fscanf(fp, "%d", &numFaces);

	// Get number of edges 
	fscanf(fp, "%d", &numEdges);

	float minX, minY, minZ;
	float maxX, maxY, maxZ;
	minX = minY = minZ = INFINITY;
	maxX = maxY = maxZ = -INFINITY;

	// Store vertices
	vector<Vertex*> vertices;
	for (int i =  0; i < numVertices; i++)
	{
		float x, y, z;
		fscanf(fp, "%f %f %f", &x, &y, &z);

		// Create Vertex
		Vertex* v = new Vertex();
		v->pos = glm::vec3(x, y, z);
		v->is = true;
		v->index = i;
		v->ind = i;
		v->normal = glm::vec3();
		v->faces = new vector<Face*>();
		v->vertices = new vector<Vertex*>();
		vertices.push_back(v);

		// Find min of x, y, and z;
		minX = fmin(x, minX);
		minY = fmin(y, minY);
		minZ = fmin(z, minZ);

		// Find max of x, y, and z;
		maxX = fmax(x, maxX);
		maxY = fmax(y, maxY);
		maxZ = fmax(z, maxZ);
	}
	// Store faces
	auto infaces = new vector<unsigned int>();
	for (int i = 0; i < numFaces; i++)
	{
		// Ignore number of vertices attribute
		fscanf(fp, "%*d");

		unsigned int x, y, z;
		fscanf(fp, "%d %d %d\n", &x, &y, &z);
		infaces->push_back(x);
		infaces->push_back(y);
		infaces->push_back(z);
	}

	// Make sure file is all read
	assert(fscanf(fp, "%*s") == EOF);

	fclose(fp);

	// Center of Objects
	float avgX = (minX + maxX) / 2.0f;
	float avgY = (minY + maxY) / 2.0f;
	float avgZ = (minZ + maxZ) / 2.0f;

	// Determine the longest axis
	float longX = fabsf(minX) + fabsf(minX);
	float longY = fabsf(minY) + fabsf(minY);
	float longZ = fabsf(minZ) + fabsf(minZ);
	float big = fmax(longX, fmax(longY, longZ));

	auto invertices = new vector<float>();
	for (auto v : vertices)
	{
		// Centering objects
		glm::vec3 pos = v->pos;
		glm::vec3 v_(pos.x - avgX, pos.y - avgY, pos.z - avgZ);

		// Fits to imaginary box
		v_ = v_ / big;

		// Rescale to fit 2x2x2 imaginary box
		v->pos = 2.0f * v_;

		invertices->push_back(v->pos.x);
		invertices->push_back(v->pos.y);
		invertices->push_back(v->pos.z);
	}

	return loader->loadModel(invertices, vertices, infaces);
}