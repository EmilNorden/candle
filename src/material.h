#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "vector.h"
#include <memory>

struct FIBITMAP;
class Texture;

class Material
{    
public:
	Material(size_t idx, const Color &diffuse, const Color &emissive, Texture* diffuseMap, Texture* normalMap);

	void sample_diffuse(float u, float v, Color& result) const;
	void sample_normal(float u, float v, Color& result) const;
	const Color &emissive() const { return m_emissive; }
	void set_emissive(const Color &emissive);
	const Color &diffuse() const { return m_diffuse; }
	void set_diffuse(const Color &color);
	bool is_emissive() const { return !m_emissive.is_zero(); }
	bool has_diffuse_map() const { return m_diffuse_map != nullptr; }
	bool has_normal_map() const { return m_normal_map != nullptr; }
	float reflectivity() const { return m_reflectivity; }
	void set_reflectivity(float refl) { m_reflectivity = refl; }
    int index() const { return m_index; }
	void set_diffuse_map(Texture* diffuseMap);
	Texture *diffuse_map() { return m_diffuse_map; }
	void set_normal_map(Texture* normalMap);
	Texture *normal_map() { return m_normal_map; }
	

	int memory_size() const;
private:
    int m_index;
    Color m_diffuse;
    Color m_emissive;
	
	Texture *m_diffuse_map;
	Texture *m_normal_map;
	float m_reflectivity;
};

#endif