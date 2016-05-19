#include "material.h"
#include <FreeImage.h>
#include "texture.h"

Material::Material(size_t idx, Texture *texture, const Color &diffuse, const Color &emissive)
	: m_index(idx), m_diffuse(diffuse), m_emissive(emissive), m_diffuse_texture(texture), m_reflectivity(0.0)
{
}

void Material::sample(float u, float v, Color &result) const
{
	// TODO: Ta bort detta sen
	if(u < 0.0f)
		u = 0.0f;
	if(v < 0.0f)
		v = 0.0f;

	if(u > 1.0f)
		u = 1.0f;
	if(v > 1.0f)
		v = 1.0f;

	size_t x = u * (m_diffuse_texture->width()-1);
	size_t y = v * (m_diffuse_texture->height()-1);

	/*RGBQUAD rgb;
	FreeImage_GetPixelColor(bitmap_, x, y, &rgb);

	return Vector3d(rgb.rgbRed / 255.0, rgb.rgbGreen / 255.0, rgb.rgbBlue / 255.0);*/
	m_diffuse_texture->sample(x, y, result);
}

void Material::set_emissive(const Color &emissive)
{
	m_emissive = emissive;
}

void Material::set_diffuse(const Color &color)
{
	m_diffuse = color;
}

void Material::set_diffuse_texture(Texture *texture)
{
	m_diffuse_texture = texture;
}

int Material::memory_size() const
{
	//TODO: fix this later.
	return 0;
}