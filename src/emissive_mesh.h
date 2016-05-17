#ifndef EMISSIVE_MESH_H_
#define EMISSIVE_MESH_H_

#include "material.h"
#include "mesh.h"

class EmissiveMesh
{
public:
	Mesh *mesh;
	double effective_radius;
	Vector3d center;

	EmissiveMesh(Mesh *m)
        : mesh(m)
    {
        double strength = m->m_material->emissive().length_squared();
        effective_radius = strength * 100;
        center = (m->bounds().min() + m->bounds().max()) / 2.0;
    }
};

#endif