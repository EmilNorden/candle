#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "vector.h"
#include <memory>

struct FIBITMAP;

class Material
{    
public:
	
	

	Material(size_t idx, FIBITMAP *bitmap, const Vector3d &diffuse, const Vector3d &emissive);

	void sample(double u, double v, Vector3d &result) const;
	const Vector3d &emissive() const { return m_emissive; }
	void set_emissive(const Vector3d &emissive);
	const Vector3d &diffuse() const { return m_diffuse; }
	void set_diffuse(const Vector3d &color);
	bool is_emissive() const { return !m_emissive.is_zero(); }
	bool has_texture() const { return (bool)m_bitmap_d; }
	double reflectivity() const { return m_reflectivity; }
	void set_reflectivity(double refl) { m_reflectivity = refl; }
    int index() const { return m_index; }

	void set_texture(FIBITMAP *bitmap);

	int memory_size() const;
private:
    int m_index;
    Vector3d m_diffuse;
    Vector3d m_emissive;
	int m_width;
	int m_height;
	
	std::unique_ptr<Vector3d[]> m_bitmap_d;
	double m_reflectivity;
};

#endif