#ifndef AABB_H_
#define AABB_H_

#include "vector.h"

class Ray;

class AABB
{
public:
	AABB();
	AABB(const Vector3d &min, const Vector3d &max);

	void expand(const AABB &other);
	void expand(const Vector3d &point);
	bool intersects(const AABB &other) const;
	bool intersects(const Ray &ray) const;
	bool intersects_dist(const Ray &ray, double &dist) const;
	bool intersects_dist2(const Ray &ray, double &dist) const;
	bool intersects2(const Ray &ray) const;
	const Vector3d &min() const;
	const Vector3d &max() const;
private:
	Vector3d m_min;
	Vector3d m_max;
};

#endif