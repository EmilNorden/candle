#ifndef CAMERA_H_
#define CAMERA_H_

#include "vector.h"
#include <stdexcept>
#include <random>

class Ray;

class Camera
{    
public:

	Camera(const Vector3f &pos, const Vector3f &dir, const Vector3f &up, const double fov, const double aspect_ratio,
		const Vector2i &resolution, double focal_length, double blur_radius);

	void set_position(const Vector3f &pos) 
	{
		m_position = pos;
	}

	const Vector3f position() const
	{
		return m_position;
	}

	void set_direction(const Vector3f &dir) 
	{
		m_direction = dir;
	}

	const Vector3f direction() const 
	{
		return m_direction;
	}

	double blur_radius() const 
	{
		return m_blur_radius;
	}

	void set_blur_radius(const double value) 
	{
		m_blur_radius = value;
	}

	double focal_length() const {
		return m_focal_length;
	}

	void set_focal_length(double value) 
	{
		m_focal_length = value;
	}

	bool updated_this_frame() const 
	{
		return m_updated_this_frame;
	}

	void cast_ray(Ray &ray, size_t x, size_t y) const;

	void cast_perturbed_ray(Ray &ray, size_t x, size_t y, std::mt19937 &rand_state) const;

	void update();

	void reset_update_flag();
private:
    Vector3f m_position;
	Vector3f m_direction;
	Vector3f m_up;
	Vector3f m_u;
	Vector3f m_v;
    Vector3f m_image_plane_origin;
	double m_blur_radius;
	double m_fov;
	double m_aspect_ratio;
    double m_focal_length;
	double m_pixel_width;
	double m_pixel_height;
    Vector2i m_resolution;
    bool m_updated_this_frame;
};


#endif