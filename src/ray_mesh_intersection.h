#ifndef COLLISION_INFO_H_
#define COLLISION_INFO_H_

#include "mesh.h"
#include "vector.h"


class RayMeshIntersection {
public:
	Mesh const * mesh;
	uint16_t index0;
	uint16_t index1;
	uint16_t index2;
	double u, v;
	double distance; // TODO: Can I remove this and only rely on ray.dist?

	RayMeshIntersection(Mesh const * m, uint16_t i0, uint16_t i1, uint16_t i2, double tex_u, double tex_v)
		: mesh(m), index0(i0), index1(i1), index2(i2), u(tex_u), v(tex_v)
	{
	}

	RayMeshIntersection()
		: mesh(nullptr)
	{
	}
};

#endif