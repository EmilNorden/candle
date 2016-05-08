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
	Ray(const Vector3d &origin, const Vector3d &direction)
		: m_origin(origin), m_direction(direction)
	{
		m_dir_reciprocal.x() = 1.0 / m_direction.x();
		m_dir_reciprocal.y() = 1.0 / m_direction.y();
		m_dir_reciprocal.z() = 1.0 / m_direction.z();

		dist = DBL_MAX;
	}

	Vector3d m_origin;
	Vector3d m_direction;
	Vector3d m_dir_reciprocal;

	double dist;

	bool intersects(const Vector3d &v1, const Vector3d &v2, const Vector3d &v3, double *u, double *v, double *dist) const;
};

#endif