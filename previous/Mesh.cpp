#include "Mesh.h"
#include <iostream>
#include <algorithm>
#include <map>

using namespace std;

int test = 0;

Mesh::Mesh(vector<float>* invertices, vector<Vertex*> vertices, vector<unsigned int>* infaces)
{
	this->invertices = invertices;
	this->vertices = vertices;
	this->infaces = infaces;

	MAX_VERT = vertices.size();

	datastructure();
}

void Mesh::datastructure()
{
	int fcount = 0;
	int ecount = 0;

	map< pair< int, int >, Edge* > ehe;

	for (int i = 0; i < infaces->size() - 2; i += 3)
	{
		Face* f = new Face();
		f->is = true;
		f->index = fcount;
		fcount++;

		int v0 = infaces->at(i);
		int v1 = infaces->at(i + 1);
		int v2 = infaces->at(i + 2);

		// Vertices consisting a face
		f->v0 = vertices[v0];
		f->v1 = vertices[v1];
		f->v2 = vertices[v2];

		// A face consisting vertices
		vertices[v0]->faces->push_back(f);
		vertices[v1]->faces->push_back(f);
		vertices[v2]->faces->push_back(f);

		// Update vertex normals
		glm::vec3 fnormal = f->get_fNormal();
		vertices[v0]->normal = fnormal;
		vertices[v1]->normal = fnormal;
		vertices[v2]->normal = fnormal;

		int a = v0;
		int b = v1;
		if (a > b) swap(a, b);

		// Edge already created
		if (ehe.find(pair<int, int>(a, b)) != ehe.end() || ehe.find(pair<int, int>(b, a)) != ehe.end())
		{
			f->edges.push_back(ehe[pair<int, int>(a, b)]);
			//ehe[pair<int, int>(a, b)]->faces.push_back(f);
		}
		else
		{
			edges.insert(edges.end(), new Edge());
			edges.back()->v0 = vertices[v0];
			edges.back()->v1 = vertices[v1];
			edges.back()->is = true;
			f->edges.push_back(edges.back());
			vertices[v0]->vertices->push_back(vertices[v1]);
			vertices[v1]->vertices->push_back(vertices[v0]);
		}

		ehe[pair<int, int>(a, b)]++;

		a = v1;
		b = v2;
		if (a > b) swap(a, b);

		// Edge already created
		if (ehe.find(pair<int, int>(a, b)) != ehe.end())
		{
			f->edges.push_back(ehe[pair<int, int>(a, b)]);
			//ehe[pair<int, int>(a, b)]->faces.push_back(f);
		}
		else
		{
			edges.insert(edges.end(), new Edge());
			edges.back()->v0 = vertices[v1];
			edges.back()->v1 = vertices[v2];
			edges.back()->is = true;
			f->edges.push_back(edges.back());
			vertices[v1]->vertices->push_back(vertices[v2]);
			vertices[v2]->vertices->push_back(vertices[v1]);
		}

		ehe[pair<int, int>(a, b)]++;

		a = v2;
		b = v0;
		if (a > b) swap(a, b);

		// Edge already created
		if (ehe.find(pair<int, int>(a, b)) != ehe.end())
		{
			f->edges.push_back(ehe[pair<int, int>(a, b)]);
			//ehe[pair<int, int>(a, b)]->faces.push_back(f);
		}
		else
		{
			edges.insert(edges.end(), new Edge());
			edges.back()->v0 = vertices[v2];
			edges.back()->v1 = vertices[v0];
			edges.back()->is = true;
			f->edges.push_back(edges.back());
			vertices[v2]->vertices->push_back(vertices[v0]);
			vertices[v0]->vertices->push_back(vertices[v2]);
		}

		ehe[pair<int, int>(a, b)]++;

		faces.push_back(f);
	}

	// Neighboring faces
	for (auto f : faces)
	{
		vector<Face*>* t = get_fIntersection(f->v0, f->v1);

		if (t->size() == 2)
		{
			Face* x = t->at(0);
			Face* y = t->at(1);

			if (x != f)
				f->f0 = x;
			else
				f->f0 = y;
		}
		else if (t->size() == 1)
		{
			Face* x = t->at(0);
			f->f0 = x;
		}

		t = get_fIntersection(f->v1, f->v2);

		if (t->size() == 2)
		{
			Face* x = t->at(0);
			Face* y = t->at(1);

			if (x != f)
				f->f1 = x;
			else
				f->f1 = y;
		}
		else if (t->size() == 1)
		{
			Face* x = t->at(0);
			f->f1 = x;
		}

		t = get_fIntersection(f->v2, f->v0);

		if (t->size() == 2)
		{
			Face* x = t->at(0);
			Face* y = t->at(1);

			if (x != f)
				f->f2 = x;
			else
				f->f2 = y;
		}
		else if (t->size() == 1)
		{
			Face* x = t->at(0);
			f->f2 = x;
		}

		if (f->f0 == f)
			f->f0 = NULL;
		if (f->f1 == f)
			f->f1 = NULL;
		if (f->f2 == f)
			f->f2 = NULL;
	}

	// Normalize all the vertex normals
	for (auto v : vertices)
		v->normalize();

	// Initialize all the Q's of vertices *********************************** inefficient
	for (auto v : vertices)
		v->set_Q();

	// Initialize all the Q's and errors of edges
	int number = 0;
	for (auto e : edges)
	{
		e->set_Q();
		e->get_vbar();
		e->get_error();
		e->index = number;
		number++;
		e->n0 = e->v0->ind;
		e->n1 = e->v1->ind;
		
		e->isBoundary = false;
	}

	// Test elist *** change to quadric error elist later
	elist = new vector<Edge*>();

	// Index sorting
	std::vector<int> y(edges.size());
	std::size_t n(0);
	std::generate(std::begin(y), std::end(y), [&] { return n++; });

	std::sort(std::begin(y),
		std::end(y),
		[&](int i1, int i2) { return edges[i1]->error < edges[i2]->error; });

	for (auto e : y)
		elist->push_back(edges[e]);

	/*
	// Remove all boundary edges from the elist
	vector<Edge*>* newelist = new vector<Edge*>();
	for (auto e : edges)
	{
		Vertex* v0 = e->v0;
		Vertex* v1 = e->v1;
		vector<Face*>* sth = get_fIntersection(v0, v1);

		if (sth->size() == 1)
			e->isBoundary = true;
	}

	// --------------------------------
	for (auto v : vertices)
		v->isBoundary = false;
	for (auto e : *elist)
	{
		if (e->isBoundary)
		{
			e->v0->isBoundary = true;
			e->v1->isBoundary = true;
		}
	}

	for (auto e : *elist)
	{
		if (e->v0->isBoundary || e->v1->isBoundary)
			e->isBoundary = true;
	}

	// --------------------------------

	//for (auto e : *elist)
	//{
	//	if (!e->isBoundary)
	//		newelist->push_back(e);
	//}
	//delete elist;
	//elist = newelist;
	*/

	cout << "[DA] Number of vertices: " << vertices.size() << endl;
	cout << "[DA] Number of edges: " << edges.size() << endl;
	cout << "[DA] Number of elist edges: " << elist->size() << endl;
	cout << "[DA] Number of faces: " << faces.size() << endl;
}

vector<Face*>* Mesh::get_fIntersection(Vertex* v0, Vertex* v1)
{
	vector<Face*>* F0 = v0->faces;
	vector<Face*>* F1 = v1->faces;

	vector<Face*>* fs = new vector<Face*>();

	for (auto f0 : *F0)
	{
		for (auto f1 : *F1)
		{
			if (f0 == f1)
			{
				fs->push_back(f0);
			}
		}
	}

	return fs;
}

vector<Face*>* Mesh::get_fUnion(Vertex* v0, Vertex* v1)
{
	vector<Face*>* F0 = v0->faces;
	vector<Face*>* F1 = v1->faces;

	vector<Face*>* fs = new vector<Face*>();

	for (auto f : *F1)
		fs->push_back(f);

	for (auto f0 : *F0)
	{
		bool exist = false;
		for (auto f1 : *F1)
		{
			if (f0 == f1)
			{
				exist = true;
				break;
			}
		}

		if (!exist)
			fs->push_back(f0);
	}

	return fs;
}

vector<Vertex*>* Mesh::get_vIntersection(Vertex* v0, Vertex* v1)
{
	vector<Vertex*>* V0 = v0->vertices;
	vector<Vertex*>* V1 = v1->vertices;

	vector<Vertex*>* vs = new vector<Vertex*>();

	for (auto v0 : *V0)
	{
		for (auto v1 : *V1)
		{
			if (v0 == v1)
			{
				vs->push_back(v0);
			}
		}
	}

	return vs;
}

vector<Face*>* Mesh::get_FA0(vector<Face*>* U, vector<Vertex*>* vI, Vertex* v0, Vertex* v1)
{
	vector<Face*>* fs = new vector<Face*>();
	vI->push_back(v0);
	vI->push_back(v1);

	for (auto f : *U)
	{
		bool a = false; bool b = false; bool c = false;
		for (auto v : *vI)
		{
			if (f->v0 == v)
				a = true;
			if (f->v1 == v)
				b = true;
			if (f->v2 == v)
				c = true;
		}

		if (a && b && c)
			fs->push_back(f);
	}

	return fs;
}

vector<Vertex*>* Mesh::get_vUnion(Vertex* v0, Vertex* v1)
{
	vector<Vertex*>* V0 = v0->vertices;
	vector<Vertex*>* V1 = v1->vertices;

	vector<Vertex*>* vs = new vector<Vertex*>();

	for (auto f : *V1)
		vs->push_back(f);

	for (auto v0 : *V0)
	{
		bool exist = false;
		for (auto v1 : *V1)
		{
			if (v0 == v1)
			{
				exist = true;
				break;
			}
		}

		if (!exist)
			vs->push_back(v0);
	}

	return vs;
}

vector<Vertex*>* Mesh::get_v0only(vector<Vertex*>* vI, Vertex* v0)
{
	vector<Vertex*>* vs = new vector<Vertex*>();

	for (auto v : *v0->vertices)
	{
		bool exist = false;
		for (auto v_ : *vI)
		{
			if (v_ == v)
			{
				exist = true;
				break;
			}
		}

		if (!exist)
			vs->push_back(v);
	}

	return vs;
}

void Mesh::information()
{
	int vnum = 0; int ednum = 0; int fnum = 0;

	for (auto e : *elist)
		if (e->is)
			ednum++;

	cout << "--------------------------------------------------------" << endl;
	fprintf(stderr, "Vertices: %d\t Edges: %d\t Faces: %d\n", invertices->size() / 3, ednum, infaces->size() / 3);

}

void Mesh::update(bool isEC)
{
	if (isEC)
	{
		int count = 0;
		for (int i = 0; i < elist->size() / 20; ++i)
		{
			if (elist->empty())	return;
			update_test();
			++count;
		}
		if (count > 0) updatemem.push_back(count);
	}
	else
	{
		if (updatemem.empty()) return;
		for (int i = 0; i < updatemem.back(); ++i)
		{
			if (vimem.empty()) return;
			timetravel();
		}
		updatemem.pop_back();
	}
}

void Mesh::update_elist()
{
	// Normalize all the vertex normals
	for (auto v : vertices)
		if (v->is)
			v->get_vNormal();

	// Initialize all the Q's of vertices
	for (auto v : vertices)
		if (v->is)
			v->set_Q();

	// Initialize errors
	for (auto e : *elist)
	{
		if (e->is)
		{
			e->set_Q();
			e->get_vbar();
			e->get_error();
		}
	}

	vector<Edge*>* newlist = new vector<Edge*>();

	std::vector<int> y(edges.size());
	std::size_t n(0);
	std::generate(std::begin(y), std::end(y), [&] { return n++; });

	std::sort(std::begin(y),
		std::end(y),
		[&](int i1, int i2) { return edges[i1]->error <  edges[i2]->error; });

	for (auto v : y)
	{
		Edge* e = edges[v];
		if (e->is)
			newlist->push_back(e);
	}

	//std::vector<int> y(elist->size());
	//std::size_t n(0);
	//std::generate(std::begin(y), std::end(y), [&] { return n++; });

	//std::sort(std::begin(y),
	//	std::end(y),
	//	[&](int i1, int i2) { return elist->at(i1)->error < elist->at(i2)->error; });

	//for (auto v : y)
	//{
	//	Edge* e = elist->at(v);
	//	if(e->is)
	//		newlist->push_back(e);
	//}

	delete elist;
	elist = newlist;

	//cout << "++++++++++++++++++++++" << endl;
	//cout << "elist has been updated: " << elist->size() << endl;
	
}

void Mesh::timetravel()
{
	test--;
	cout << "-----" << test << "-----" << endl;

	vector<int>* vn = vimem.back();
	vimem.pop_back();

	vector<glm::vec3>* vp = vpmem.back();
	vpmem.pop_back();

	vector<int>* fn = fnmem.back();
	fnmem.pop_back();

	vector<glm::vec3>* fv = fvmem.back();
	fvmem.pop_back();

	vector<int>* en = eimem.back();
	eimem.pop_back();

	for (int i = 0; i < vn->size(); i++)
	{
		Vertex* x = vertices[vn->at(i)];
		
		// Reset position
		x->pos = vp->at(i);

		// Reset others
		x->is = true;
		x->isBoundary = false;
	}

	for (int i = 0; i < fn->size(); i++)
	{
		Face* f = faces[fn->at(i)];

		// Reset vertices
		Vertex* v0 = vertices[fv->at(i)[0]];
		Vertex* v1 = vertices[fv->at(i)[1]];
		Vertex* v2 = vertices[fv->at(i)[2]];

		f->v0 = v0;
		f->v1 = v1;
		f->v2 = v2;
		v0->faces->push_back(f);
		v1->faces->push_back(f);
		v2->faces->push_back(f);

		f->is = true;

	}

	for (int i = 0; i < vn->size(); i++)
	{
		Vertex* x = vertices[vn->at(i)];

		vector<Face*>* newfaces = new vector<Face*>();
		for (auto f_ : *x->faces)
		{
			if (x == f_->v0 || x == f_->v1 || x == f_->v2)
			{
				newfaces->push_back(f_);
			}
		}
		delete x->faces;
		x->faces = newfaces;
	}

	for (int i = 0; i < vn->size(); i++)
	{
		Vertex* x = vertices[vn->at(i)];

		delete x->vertices;
		x->vertices = new vector<Vertex*>();

		for (auto f : *x->faces)
		{
			if(f->v0->ind != x->ind)
				x->vertices->push_back(f->v0);
			if (f->v1->ind != x->ind)
				x->vertices->push_back(f->v1);
			if (f->v2->ind != x->ind)
				x->vertices->push_back(f->v2);
		}
	}


	for (int i = 0; i < vn->size(); i++)
	{
		Vertex* x = vertices[vn->at(i)];

		vector<Vertex*>* newvertices = new vector<Vertex*>();

		for (auto v : *x->vertices)
		{
			bool exist = false;
			for (auto v_ : *newvertices)
			{
				if (v_ == v)
				{
					exist = true;
					break;
				}
			}
			if (!exist)
				newvertices->push_back(v);
		}

		delete x->vertices;
		x->vertices = newvertices;
	}

	// Vertex update
	int countv = 0;
	delete invertices;
	vector<float>* upvertices = new vector<float>();
	for (auto v : vertices)
	{
		if (v->is)
		{
			v->index = countv;
			countv++;
			upvertices->push_back(v->pos[0]);
			upvertices->push_back(v->pos[1]);
			upvertices->push_back(v->pos[2]);
		}
	}
	invertices = upvertices;

	// Face update
	delete infaces;
	vector<unsigned int>* upfaces = new vector<unsigned int>();
	for (auto f : faces)
	{
		if (f->is)
		{
			upfaces->push_back(f->v0->index);
			upfaces->push_back(f->v1->index);
			upfaces->push_back(f->v2->index);
		}
	}

	infaces = upfaces;

	// Edge update
	for (int i = 0; i < en->size(); i++)
	{
		Edge* e = edges[en->at(i)];
		
		e->is = true;
		elist->push_back(e);
	}

	update_elist();
	cout << "faces: " << infaces->size() / 3 << endl;

}

bool Mesh::interpolateEC(bool animating)
{
	if (elist->empty() || animating) return false;
	Edge* e = elist->front();

	Vertex* v0 = e->v0;
	Vertex* v1 = e->v1;

	sIndex = v0->index;
	tIndex = v1->index;
	simem.push_back(sIndex);
	timem.push_back(tIndex);

	s_ini_pos = v0->pos;
	t_ini_pos = v1->pos;
	sinimem.push_back(s_ini_pos);
	tinimem.push_back(t_ini_pos);

	glm::vec3 finPos = e->get_vbar();

	s_fin_pos = finPos;
	t_fin_pos = finPos;
	stfinmem.push_back(finPos);

	return true;
}

bool Mesh::interpolateVS(bool animating)
{
	
	if (animating || stfinmem.empty()) return false;

	s_ini_pos = stfinmem.back();
	t_ini_pos = s_ini_pos;
	stfinmem.pop_back();

	s_fin_pos = sinimem.back();
	t_fin_pos = tinimem.back();

	sinimem.pop_back();
	tinimem.pop_back();

	sIndex = simem.back(); simem.pop_back();
	tIndex = timem.back(); timem.pop_back();

	return true;
}

void Mesh::update_test()
{
	test++;
	cout << "-----" << test << "-----" << endl;

	Edge* e = elist->front();

	e->is = false;
	//cout << "Edge Collapse: " << e->v0->ind << " " << e->v1->ind << endl;

	// Get vertices
	Vertex* v0 = e->v0;//vertices[e->n0];
	Vertex* v1 = e->v1;//vertices[e->n1];

	// For later use
	glm::vec3 tmp_p = v1->pos;
	glm::vec3 tmp_n = v1->normal;

	// Get union of faces
	vector<Face*>* U = get_fUnion(v0, v1);

	// Get union of vertices
	vector<Vertex*>* vU = get_vUnion(v0, v1);

	// Remember them
	vector<int>* vnumbers = new vector<int>();
	vector<glm::vec3>* vpositions = new vector<glm::vec3>();
	vector<int>* fnumbers = new vector<int>();
	vector<glm::vec3>* fvertices = new vector<glm::vec3>();
	for (auto v : *vU)
	{
		vnumbers->push_back(v->ind);
		vpositions->push_back(v->pos);
	}

	for (auto f : *U)
	{
		fnumbers->push_back(f->index);
		fvertices->push_back(glm::vec3(f->v0->ind, f->v1->ind, f->v2->ind));
	}
	vimem.push_back(vnumbers);
	vpmem.push_back(vpositions);
	fnmem.push_back(fnumbers);
	fvmem.push_back(fvertices);

	// Get intersecting vertices
	vector<Vertex*>* vI = get_vIntersection(v0, v1);

	// Get faces of area 0
	vector<Face*>* FA0 = get_FA0(U, vI, v0, v1);

	// Get vertices connected only to v0
	vector<Vertex*>* v0Only = get_v0only(vI, v0);

	// Make faces of area 0 false
	for (auto f : *FA0)
		f->is = false;

	// Check if v0 or v1 is embedded
	bool embed = false;
	int ecount = 0;
	for (auto f : *v0->faces)
	{
		for (auto f_ : *FA0)
		{
			if (f == f_)
			{
				ecount++;
				break;
			}
		}
	}
	if (ecount <= v0->faces->size())
		embed = true;

	for (auto f : *v1->faces)
	{
		for (auto f_ : *FA0)
		{
			if (f == f_)
			{
				ecount++;
				break;
			}
		}
	}
	if (ecount <= v1->faces->size())
		embed = true;

	if (embed)
	{
		// Take care of weird fin corner case
		int f0count = 0;
		for (auto f : *v0->faces)
		{
			for (auto a : *FA0)
			{
				if (a == f)
					f0count++;
			}
		}

		if (f0count <= v0->faces->size())
		{
			for (auto f : *FA0)
			{
				if (f->v0 != v1 && f->v1 != v1 && f->v2 != v1)
					f->is = true;
			}
		}

		int f1count = 0;
		for (auto f : *v1->faces)
		{
			for (auto a : *FA0)
			{
				if (a == f)
					f1count++;
			}
		}

		if (f1count <= v1->faces->size())
		{
			for (auto f : *FA0)
			{
				if (f->v0 != v0 && f->v1 != v0 && f->v2 != v0)
					f->is = true;
			}
		}
	}

	// Update each vertex's face set
	for (auto v : *vI)
	{
		vector<Face*>* fs = new vector<Face*>();
		for (auto f : *v->faces)
		{
			if (f->is)
				fs->push_back(f);
		}
		delete v->faces;
		v->faces = fs;
	}

	// Add v0's vertex set to v1's vertex set
	for (auto v : *v0->vertices)
	{
		bool exist = false;
		for (auto v_ : *v1->vertices)
		{
			if (v == v_)
			{
				exist = true;
				break;
			}
		}
		if (!exist && v != v1)
			v1->vertices->push_back(v);
	}

	// Add v0's face set to v1's face set
	for (auto f : *v0->faces)
	{
		if (f->is)
			v1->faces->push_back(f);
	}

	// Delete v0 and interior vertices
	v0->is = false;
	for (auto v : *vI)
	{
		if (v->faces->size() == 0)
		{
			//v->pos = e->get_vbar();
			v->is = false;
		}
	}

	// Update v1 position
	glm::vec3 temp = e->get_vbar();
	v1->pos = temp;

	// Update those only connected to v0 to v1
	for (auto v : *v0Only)
		v->vertices->push_back(v1);

	// Update all true faces with v0 to v1
	for (auto f : *U)
	{
		if (f->is)
		{
			if (f->v0 == v0)
				f->v0 = v1;
			if (f->v1 == v0)
				f->v1 = v1;
			if (f->v2 == v0)
				f->v2 = v1;
		}
	}

	// Update all true vertices of their neighboring vertex set
	// Also update vertex normals
	for (auto v : *vU)
	{
		if (v->is)
		{
			vector<Vertex*>* vs = new vector<Vertex*>();
			for (auto v_ : *v->vertices)
			{
				if (v_->is)
				{
					vs->push_back(v_);
					v_->get_vNormal();
				}
			}

			delete v->vertices;
			v->vertices = vs;
		}
	}

	// Update elist
	//vector<Edge*>* newlist = new vector<Edge*>();
	for (auto e : *elist)
	{
		for (auto v : *v0Only)
		{
			if (e->v0 == v && e->v1 == v0)
				e->v1 = v1;
			else if (e->v0 == v0 && e->v1 == v)
				e->v0 = v1;
		}

		if (e->v0->is == false || e->v1->is == false)
			e->is = false;
	}

	int countv = 0;
	delete invertices;
	vector<float>* upvertices = new vector<float>();
	for (auto v : vertices)
	{
		if (v->is)
		{
			v->index = countv;
			countv++;
			upvertices->push_back(v->pos[0]);
			upvertices->push_back(v->pos[1]);
			upvertices->push_back(v->pos[2]);
		}
	}
	invertices = upvertices;

	// Update face list
	delete infaces;
	vector<unsigned int>* upfaces = new vector<unsigned int>();
	for (auto f : faces)
	{
		if (f->is)
		{
			upfaces->push_back(f->v0->index);
			upfaces->push_back(f->v1->index);
			upfaces->push_back(f->v2->index);
		}
	}

	infaces = upfaces;

	// Initialize all the Q's and errors of edges
	vector<int>* enumbers = new vector<int>();

	for (auto e : *elist)
		if (!e->is)
			enumbers->push_back(e->index);
	eimem.push_back(enumbers);

	// Re-sort elist
	update_elist();

	delete U;
	delete vU;
	delete vI;
	delete FA0;
	delete v0Only;
	cout << "faces: " << infaces->size() / 3 << endl;
}

Face* Mesh::get_face(Vertex* v0, Vertex* v1, Vertex* v2)
{
	for (auto f : *v0->faces)
	{
		int count = 0;
		if (f->v0 == v0 || f->v1 == v0 || f->v2 == v0)
			count++;
		if (f->v0 == v1 || f->v1 == v1 || f->v2 == v1)
			count++;
		if (f->v0 == v2 || f->v1 == v2 || f->v2 == v2)
			count++;

		if (count == 3)
			return f;
	}
	return NULL;
}
