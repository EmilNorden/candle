#include "aabb.h"

#include <algorithm>
#include <cfloat>
#include "ray.h"



#define INTERSECT_EPSILON 9.99999997475243E-07

AABB::AABB()
	: m_min(Vector3d(0, 0, 0)), m_max(Vector3d(0, 0, 0))
{
}

AABB::AABB(const Vector3d &min, const Vector3d &max)
	: m_min(min), m_max(max)
{
}

void AABB::expand(const AABB &other)
{
	expand(other.m_min);
	expand(other.m_max);
}

void AABB::expand(const Vector3d &point)
{
	if(point.x() < m_min.x())
		m_min.x() = point.x();
	if(point.y() < m_min.y())
		m_min.y() = point.y();
	if(point.z() < m_min.z())
		m_min.z() = point.z();

	if(point.x() > m_max.x())
		m_max.x() = point.x();
	if(point.y() > m_max.y())
		m_max.y() = point.y();
	if(point.z() > m_max.z())
		m_max.z() = point.z();
}

bool AABB::intersects(const Ray &ray) const
{
	double tmin = -DBL_MAX;
	double tmax = DBL_MAX;
	
	if(ray.m_direction.x() != 0.0)
	{
        double tx1 = (m_min.x() - ray.m_origin.x())/ray.m_direction.x();
        double tx2 = (m_max.x() - ray.m_origin.x())/ray.m_direction.x();
 
        tmin = std::max(tmin,std::min(tx1, tx2));
        tmax = std::min(tmax, std::max(tx1, tx2));
	}
	
	if(ray.m_direction.y() != 0.0)
	{
        double ty1 = (m_min.y() - ray.m_origin.y())/ray.m_direction.y();
        double ty2 = (m_max.y() - ray.m_origin.y())/ray.m_direction.y();
 
        tmin = std::max(tmin,std::min(ty1, ty2));
        tmax = std::min(tmax, std::max(ty1, ty2));
	}
	
	if(ray.m_direction.z() != 0.0)
	{
        double tz1 = (m_min.z() - ray.m_origin.z())/ray.m_direction.z();
        double tz2 = (m_max.z() - ray.m_origin.z())/ray.m_direction.z();
 
        tmin = std::max(tmin,std::min(tz1, tz2));
        tmax = std::min(tmax, std::max(tz1, tz2));
	}
	
	return tmax >= tmin;
}

bool AABB::intersects_dist(const Ray &ray, double &dist) const
{
	double tmin = -DBL_MAX;
	double tmax = DBL_MAX;
	
	if(ray.m_direction.x() != 0.0)
	{
        double tx1 = (m_min.x() - ray.m_origin.x())/ray.m_direction.x();
        double tx2 = (m_max.x() - ray.m_origin.x())/ray.m_direction.x();
 
        tmin = std::max(tmin,std::min(tx1, tx2));
        tmax = std::min(tmax, std::max(tx1, tx2));
	}
	
	if(ray.m_direction.y() != 0.0)
	{
        double ty1 = (m_min.y() - ray.m_origin.y())/ray.m_direction.y();
        double ty2 = (m_max.y() - ray.m_origin.y())/ray.m_direction.y();
 
        tmin = std::max(tmin,std::min(ty1, ty2));
        tmax = std::min(tmax, std::max(ty1, ty2));
	}
	
	if(ray.m_direction.z() != 0.0)
	{
        double tz1 = (m_min.z() - ray.m_origin.z())/ray.m_direction.z();
        double tz2 = (m_max.z() - ray.m_origin.z())/ray.m_direction.z();
 
        tmin = std::max(tmin,std::min(tz1, tz2));
        tmax = std::min(tmax, std::max(tz1, tz2));
	}
	
	dist = tmin;
	
	return tmax >= tmin;
}

bool AABB::intersects(const AABB &other) const
{
	//return ((this->m_min.x() >= other.m_min.x() && this->m_min.x() < other.m_max.x() ||
	//	this->m_max.x() >= other.m_min.x() && this->m_max.x() < other.m_max.x()) &&
	//	(this->m_min.y() >= other.m_min.y() && this->m_min.y() < other.m_max.y() ||
	//	this->m_max.y() >= other.m_min.y() && this->m_max.y() < other.m_max.y()) &&
	//	(this->m_min.z() >= other.m_min.z() && this->m_min.z() < other.m_max.z() ||
	//	this->m_max.z() >= other.m_min.z() && this->m_max.z() < other.m_max.z()));

	if(m_max.x() < other.m_min.x()) return false;
	if(m_min.x() > other.m_max.x()) return false;
	if(m_max.y() < other.m_min.y()) return false;
	if(m_min.y() > other.m_max.y()) return false;
	if(m_max.z() < other.m_min.z()) return false;
	if(m_min.z() > other.m_max.z()) return false;

	return true;
}

const Vector3d &AABB::min() const
{
	return m_min;
}
const Vector3d &AABB::max() const
{
	return m_max;
}	

/* Algorithm based on article: http://www.siggraph.org/education/materials/HyperGraph/raytrace/rtinter3.htm */
/*
bool AABB::intersects(const Ray &ray) const
{
	double tNear = DBL_MIN;
	double tFar = DBL_MAX;
	
	if(abs(ray.m_direction.x()) < INTERSECT_EPSILON)
	{
		if(ray.m_origin.x() < this->m_min.x() || ray.m_origin.x() > this->m_max.x())
		{
			return false;
		}
	}
	else
	{
		double T1 = (this->m_min.x() - ray.m_origin.x()) / ray.m_direction.x();
		double T2 = (this->m_max.x() - ray.m_origin.x()) / ray.m_direction.x();
		if(T1 > T2)
		{
			double temp = T1;
			T1 = T2;
			T2 = temp;
		}

		if(T1 > tNear)
		{
			tNear = T1;
		}

		if(T2 < tFar)
		{
			tFar = T2;
		}
			

		if(tNear > tFar || tFar < 0)
		{
			return false;
		}
	}

	if(abs(ray.m_direction.y()) < INTERSECT_EPSILON)
	{
		if(ray.m_origin.y() < this->m_min.y() || ray.m_origin.y() > this->m_max.y())
		{
			return false;
		}
	}
	else
	{
		double T1 = (this->m_min.y() - ray.m_origin.y()) / ray.m_direction.y();
		double T2 = (this->m_max.y() - ray.m_origin.y()) / ray.m_direction.y();
		if(T1 > T2)
		{
			double temp = T1;
			T1 = T2;
			T2 = temp;
		}

		if(T1 > tNear)
		{
			tNear = T1;
		}

		if(T2 < tFar)
		{
			tFar = T2;
		}
			

		if(tNear > tFar || tFar < 0)
		{
			return false;
		}
	}


	if(abs(ray.m_direction.z()) < INTERSECT_EPSILON)
	{
		if(ray.m_origin.z() < this->m_min.z() || ray.m_origin.z() > this->m_max.z())
		{
			return false;
		}
	}
	else
	{
		double T1 = (this->m_min.z() - ray.m_origin.z()) / ray.m_direction.z();
		double T2 = (this->m_max.z() - ray.m_origin.z()) / ray.m_direction.z();
		if(T1 > T2)
		{
			double temp = T1;
			T1 = T2;
			T2 = temp;
		}

		if(T1 > tNear)
		{
			tNear = T1;
		}

		if(T2 < tFar)
		{
			tFar = T2;
		}
			

		if(tNear > tFar || tFar < 0)
		{
			return false;
		}
	}

	return true;
}
*/
/* Algorithm based on article: http://www.siggraph.org/education/materials/HyperGraph/raytrace/rtinter3.htm */
/*
bool AABB::intersects_dist(const Ray &ray, double &dist) const
{
	return intersects_dist2(ray, dist);
	
	double tNear = DBL_MIN;
	double tFar = DBL_MAX;

	if(abs(ray.m_direction.x()) < INTERSECT_EPSILON)
	{
		if(ray.m_origin.x() < this->m_min.x() || ray.m_origin.x() > this->m_max.x())
		{
			return false;
		}
	}
	else
	{
		double T1 = (this->m_min.x() - ray.m_origin.x()) / ray.m_direction.x();
		double T2 = (this->m_max.x() - ray.m_origin.x()) / ray.m_direction.x();
		if(T1 > T2)
		{
			double temp = T1;
			T1 = T2;
			T2 = temp;
		}

		if(T1 > tNear)
		{
			tNear = T1;
		}

		if(T2 < tFar)
		{
			tFar = T2;
		}
			

		if(tNear > tFar || tFar < 0)
		{
			return false;
		}
	}

	if(abs(ray.m_direction.y()) < INTERSECT_EPSILON)
	{
		if(ray.m_origin.y() < this->m_min.y() || ray.m_origin.y() > this->m_max.y())
		{
			return false;
		}
	}
	else
	{
		double T1 = (this->m_min.y() - ray.m_origin.y()) / ray.m_direction.y();
		double T2 = (this->m_max.y() - ray.m_origin.y()) / ray.m_direction.y();
		if(T1 > T2)
		{
			double temp = T1;
			T1 = T2;
			T2 = temp;
		}

		if(T1 > tNear)
		{
			tNear = T1;
		}

		if(T2 < tFar)
		{
			tFar = T2;
		}
			

		if(tNear > tFar || tFar < 0)
		{
			return false;
		}
	}


	if(abs(ray.m_direction.z()) < INTERSECT_EPSILON)
	{
		if(ray.m_origin.z() < this->m_min.z() || ray.m_origin.z() > this->m_max.z())
		{
			return false;
		}
	}
	else
	{
		double T1 = (this->m_min.z() - ray.m_origin.z()) / ray.m_direction.z();
		double T2 = (this->m_max.z() - ray.m_origin.z()) / ray.m_direction.z();
		if(T1 > T2)
		{
			double temp = T1;
			T1 = T2;
			T2 = temp;
		}

		if(T1 > tNear)
		{
			tNear = T1;
		}

		if(T2 < tFar)
		{
			tFar = T2;
		}
			

		if(tNear > tFar || tFar < 0)
		{
			return false;
		}
	}

	dist = tNear;

	return true;
}
*/