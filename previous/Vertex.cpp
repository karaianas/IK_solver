#include "Vertex.h"
#include "Face.h"

glm::vec3 Vertex::get_vNormal()
{
	glm::vec3 N(0.f, 0.f, 0.f);

	for (auto f : *faces)
	{
		if(f->is)
			N += f->get_fNormal();
	}
	
	normal = glm::normalize(N);

	return normal;
}

void Vertex::normalize()
{
	normal = glm::normalize(normal);
}

void Vertex::set_Q()
{
	glm::mat4 Q_(0.0f);

	for (auto f : *faces)
	{
		if (f->is)
		{
			glm::vec3 N = f->get_fNormal();

			float a = N[0];
			float b = N[1];
			float c = N[2];

			float d = a * pos[0] + b * pos[1] + c * pos[2];
			d = -d;

			// K matrix
			glm::mat4 K(a * a, a * b, a * c, a *d, a * b, b * b, b * c, b * d, a * c, b * c, c * c, c * d, a * d, b * d, c * d, d * d);

			// Addition
			Q_ += K;
		}
	}

	Q = Q_;
}
