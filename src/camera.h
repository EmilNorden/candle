#ifndef CAMERA_H_
#define CAMERA_H_

#include "vector.h"
#include <stdexcept>
#include <random>

class Ray;

class Camera
{    
public:

	Camera(const Vector3d &pos, const Vector3d &dir, const Vector3d &up, const double fov, const double aspect_ratio,
		const Vector2i &resolution, double focal_length, double blur_radius)
		: m_position(pos), m_direction(dir), m_up(up), m_fov(fov), m_aspect_ratio(aspect_ratio), m_focal_length(focal_length),
			m_resolution(resolution), m_blur_radius(blur_radius) {

			if(aspect_ratio <= 0)
				throw std::runtime_error("aspect_ratio must be greater than 0");
	}

	void set_position(const Vector3d &pos) {
		m_position = pos;
	}

	const Vector3d position() const {
		return m_position;
	}

	void set_direction(const Vector3d &dir) {
		m_direction = dir;
	}

	const Vector3d direction() const {
		return m_direction;
	}

	double blur_radius() const {
		return m_blur_radius;
	}

	void set_blur_radius(const double value) {
		m_blur_radius = value;
	}

	double focal_length() const {
		return m_focal_length;
	}

	void set_focal_length(double value) {
		m_focal_length = value;
	}

	bool updated_this_frame() const {
		return m_updated_this_frame;
	}

	void cast_ray(Ray &ray, size_t x, size_t y) const;

	void cast_perturbed_ray(Ray &ray, size_t x, size_t y, std::mt19937 &rand_state) const;

	void update();

	void reset_update_flag();
private:
    Vector3d m_position;
	Vector3d m_direction;
	Vector3d m_up;
	Vector3d m_u;
	Vector3d m_v;
    Vector3d m_image_plane_origin;
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