#include "ray.h"

#define EPSILON 9.99999997475243E-07
//#define EPSILON 0.000001

bool Ray::intersects(const Vector3f &v1, const Vector3f &v2, const Vector3f &v3, double *result_u, double *result_v, double *dist) const
{
	Vector3f e1, e2;  //Edge1, Edge2
	Vector3f P, Q, T;
	double det, inv_det, u, v;
	double t;

	//Find vectors for two edges sharing V1
	e1 = v2 - v1;
	e2 = v3 - v1;
	//Begin calculating determinant - also used to calculate u parameter
	P = m_direction.cross(e2);
	//if determinant is near zero, ray lies in plane of triangle

	det = e1.dot(P);
	
	if(det > -EPSILON && det < EPSILON) return false;
	// BACK-FACE CULLING
	if(det < EPSILON) return false;
	inv_det = 1.0 / det;

	//calculate distance from V1 to ray origin
	T = m_origin - v1;

	//Calculate u parameter and test bound
	u = T.dot(P) * inv_det;
	//The intersection lies outside of the triangle
	if(u < 0.f || u > 1.f) return 0;

	//Prepare to test v parameter
	Q = T.cross(e1);

	//Calculate V parameter and test bound
	v = m_direction.dot(Q) * inv_det;
	//The intersection lies outside of the triangle
	if(v < 0.0 || u + v  > 1.0) return false;

	t = e2.dot(Q) * inv_det;

	if(t > EPSILON) { //ray intersection
		*dist = t;
		*result_u = u;
		*result_v = v;
		return true;
	}

	// No hit, no win
	return false;
}