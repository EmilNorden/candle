#include "material.h"
#include <FreeImage.h>

Material::Material(size_t idx, FIBITMAP *bitmap, const Vector3d &diffuse, const Vector3d &emissive)
	: m_index(idx), m_diffuse(diffuse), m_emissive(emissive), m_width(0), m_height(0)
{
	if(bitmap)
		set_texture(bitmap);
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

	size_t x = u * (m_width-1);
	size_t y = v * (m_height-1);

	/*RGBQUAD rgb;
	FreeImage_GetPixelColor(bitmap_, x, y, &rgb);

	return Vector3d(rgb.rgbRed / 255.0, rgb.rgbGreen / 255.0, rgb.rgbBlue / 255.0);*/
	const Vector3d &pixel = m_bitmap_d[(y * m_width) + x];
	result.x() = pixel.x();
	result.y() = pixel.y();
	result.z() = pixel.z();
}

void Material::set_emissive(const Vector3d &emissive)
{
	m_emissive = emissive;
}

void Material::set_diffuse(const Vector3d &color)
{
	m_diffuse = color;
}

void Material::set_texture(FIBITMAP *bitmap)
{
	m_width = FreeImage_GetWidth(bitmap);
	m_height = FreeImage_GetHeight(bitmap);
	m_bitmap_d.reset(new Vector3d[m_width * m_height]);
	for(int y = 0 ; y < m_height; ++y) {
		for(int x = 0; x < m_width; ++x) {
			RGBQUAD rgb;
			FreeImage_GetPixelColor(bitmap, x, y, &rgb);

			Vector3d &pixel = m_bitmap_d[(y * m_width) + x];
			pixel.x() = rgb.rgbRed / 255.0;
			pixel.y() = rgb.rgbGreen / 255.0;
			pixel.z() = rgb.rgbBlue / 255.0;
		}
	}
}

int Material::memory_size() const
{
	int size = sizeof(Material);
	size += sizeof(Vector3d) * m_width * m_height;

	return size;
}