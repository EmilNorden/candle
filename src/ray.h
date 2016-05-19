#ifndef RAY_H_
#define RAY_H_

#include <cfloat>
#include "vector.h"

class Ray
{
public:
	Ray()
	{
		dist = DBL_MAX;
	}
	Ray(const Vector3f &origin, const Vector3f &direction)
		: m_origin(origin), m_direction(direction)
	{
		m_dir_reciprocal.x() = 1.0 / m_direction.x();
		m_dir_reciprocal.y() = 1.0 / m_direction.y();
		m_dir_reciprocal.z() = 1.0 / m_direction.z();

		dist = DBL_MAX;
	}

	Vector3f m_origin;
	Vector3f m_direction;
	Vector3f m_dir_reciprocal;

	double dist;

	bool intersects(const Vector3f &v1, const Vector3f &v2, const Vector3f &v3, double *u, double *v, double *dist) const;
};

#endif