#include "material.h"
#include <FreeImage.h>
#include "texture.h"

Material::Material(size_t idx, Texture *texture, const Vector3d &diffuse, const Vector3d &emissive)
	: m_index(idx), m_diffuse_texture(texture), m_diffuse(diffuse), m_emissive(emissive)
{
}

void Material::sample(double u, double v, Vector3d &result) const
{
	// TODO: Ta bort detta sen
	if(u < 0.0)
		u = 0.0;
	if(v < 0.0)
		v = 0.0;

	if(u > 1.0)
		u = 1.0;
	if(v > 1.0)
		v = 1.0;

	size_t x = u * (m_diffuse_texture->width()-1);
	size_t y = v * (m_diffuse_texture->height()-1);

	/*RGBQUAD rgb;
	FreeImage_GetPixelColor(bitmap_, x, y, &rgb);

	return Vector3d(rgb.rgbRed / 255.0, rgb.rgbGreen / 255.0, rgb.rgbBlue / 255.0);*/
	m_diffuse_texture->sample(x, y, result);
}

void Material::set_emissive(const Vector3d &emissive)
{
	m_emissive = emissive;
}

void Material::set_diffuse(const Vector3d &color)
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