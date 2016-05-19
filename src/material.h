#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "vector.h"
#include <memory>

struct FIBITMAP;
class Texture;

class Material
{    
public:
	Material(size_t idx, Texture *texture, const Color &diffuse, const Color &emissive);

	void sample(float u, float v, Color &result) const;
	const Color &emissive() const { return m_emissive; }
	void set_emissive(const Color &emissive);
	const Color &diffuse() const { return m_diffuse; }
	void set_diffuse(const Color &color);
	bool is_emissive() const { return !m_emissive.is_zero(); }
	bool has_texture() const { return m_diffuse_texture != nullptr; }
	double reflectivity() const { return m_reflectivity; }
	void set_reflectivity(double refl) { m_reflectivity = refl; }
    int index() const { return m_index; }
	void set_diffuse_texture(Texture *texture);

	int memory_size() const;
private:
    int m_index;
    Color m_diffuse;
    Color m_emissive;
	
	Texture *m_diffuse_texture;
	double m_reflectivity;
};

#endif