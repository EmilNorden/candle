#include "material.h"
#include <FreeImage.h>
#include "texture.h"

Material::Material(size_t idx, const Color &diffuse, const Color &emissive, Texture* diffuseMap, Texture* normalMap)
		: m_index(idx), m_diffuse(diffuse), m_emissive(emissive),
		 m_diffuse_map(diffuseMap), m_normal_map(normalMap), m_reflectivity(0.0f)
{
}

void Material::sample_diffuse(float u, float v, Color& result) const
{
	if(u < 0.0f)
		u = 0.0f;
	if(v < 0.0f)
		v = 0.0f;

	if(u > 1.0f)
		u = 1.0f;
	if(v > 1.0f)
		v = 1.0f;

	size_t x = u * (m_diffuse_map->width()-1);
	size_t y = v * (m_diffuse_map->height()-1);
	
	m_diffuse_map->sample(x, y, result);
}

void Material::sample_normal(float u, float v, Color& result) const
{
	if(u < 0.0f)
		u = 0.0f;
	if(v < 0.0f)
		v = 0.0f;

	if(u > 1.0f)
		u = 1.0f;
	if(v > 1.0f)
		v = 1.0f;

	size_t x = u * (m_normal_map->width()-1);
	size_t y = v * (m_normal_map->height()-1);
	
	m_normal_map->sample(x, y, result);
}

void Material::set_emissive(const Color &emissive)
{
	m_emissive = emissive;
}

void Material::set_diffuse(const Color &color)
{
	m_diffuse = color;
}

void Material::set_diffuse_map(Texture* diffuseMap)
{
	m_diffuse_map = diffuseMap;
}

void Material::set_normal_map(Texture* normalMap)
{
	m_normal_map =  normalMap;
}

int Material::memory_size() const
{
	//TODO: fix this later.
	return 0;
}