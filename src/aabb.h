#ifndef AABB_H_
#define AABB_H_

#include "vector.h"

class Ray;

class AABB
{
public:
	AABB();
	AABB(const Vector3f &min, const Vector3f &max);

	void expand(const AABB &other);
	void expand(const Vector3f &point);
	bool intersects(const AABB &other) const;
	bool intersects(const Ray &ray) const;
	bool intersects_dist(const Ray &ray, double &dist) const;
	bool intersects_dist2(const Ray &ray, double &dist) const;
	bool intersects2(const Ray &ray) const;
	const Vector3f &min() const { return m_min; }
	const Vector3f &max() const { return m_max; }
private:
	Vector3f m_min;
	Vector3f m_max;
};

#endif