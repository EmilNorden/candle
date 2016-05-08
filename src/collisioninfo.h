#ifndef COLLISION_INFO_H_
#define COLLISION_INFO_H_

#include "mesh.h"
#include "vector.h"


class CollisionInfoInternal {
public:
	Mesh const * mesh;
	uint16_t index0;
	uint16_t index1;
	uint16_t index2;
	double u, v;
	double distance;

	CollisionInfoInternal(Mesh const * m, uint16_t i0, uint16_t i1, uint16_t i2, double tex_u, double tex_v)
		: mesh(m), index0(i0), index1(i1), index2(i2), u(tex_u), v(tex_v)
	{
	}

	CollisionInfoInternal()
		: mesh(nullptr)
	{
	}
};

class CollisionInfo
{
private:
	
public:
	Vector3d color_;
	Vector3d color() const { return color_; }
};

#endif