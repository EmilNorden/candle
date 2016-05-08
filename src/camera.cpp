#include "camera.h"

#include <algorithm>
#include <random>
#include "ray.h"
#include "vector.h"

#define PI 3.14159265359   

void Camera::update() {
	const int distance = 10;

	double image_plane_height = 2 * distance * tan(m_fov / 2.0);
	double image_plane_width = image_plane_height * m_aspect_ratio;
    
    Vector3d n = m_direction * -1;
    n.normalize();

    m_u = m_up.cross(n);
	m_u.normalize();

	m_v = n.cross(m_u);
	m_v.normalize();

	Vector3d image_plane_center = m_position - n * distance;

	m_image_plane_origin = image_plane_center + (m_u * (image_plane_width / 2.0)) - (m_v * (image_plane_height / 2.0));

	m_pixel_width = image_plane_width / m_resolution.x();
	m_pixel_height = image_plane_height / m_resolution.y();
	m_updated_this_frame = true;
}

void Camera::cast_ray(Ray &ray, size_t x, size_t y) const {
	ray.m_origin = m_position;

	ray.m_direction = (m_image_plane_origin - (m_u * m_pixel_width * (double)x) + (m_v * m_pixel_height * (double)y)) - m_position;
	ray.m_direction.normalize();
	ray.dist = DBL_MAX;
}

void Camera::cast_perturbed_ray(Ray &ray, size_t x, size_t y, std::mt19937 &rand_state) const
{
	cast_ray(ray, x, y);

	Vector3d focus_point = m_position + ray.m_direction * m_focal_length;

	std::uniform_real_distribution<> distribution(0, 1);

	double angle = distribution(rand_state) * PI * 2;
	double length = distribution(rand_state) * m_blur_radius;

	ray.m_origin = m_position + (m_u * sin(angle) * length) + (m_v * cos(angle) * length);
	ray.m_direction = focus_point - ray.m_origin;
	ray.m_direction.normalize();
}

void Camera::reset_update_flag()
{
	m_updated_this_frame = false;
}