#ifndef RENDER_CONFIG_H_
#define RENDER_CONFIG_H_

#include <cstddef>

#include "vector.h"

class RenderConfiguration
{
private:
	int m_nthreads;
	size_t resolution_width_;
	size_t resolution_height_;
	size_t samples_;
	Color default_color_;
public:
	RenderConfiguration(int nthreads, size_t res_width, size_t res_height, size_t samples, const Color &default_color)
		: m_nthreads(nthreads), resolution_width_(res_width), resolution_height_(res_height),
			samples_(samples), default_color_(default_color)
	{
	}

	int nthreads() const { return m_nthreads; }
	size_t resolution_height() const { return resolution_height_; }
	size_t resolution_width() const { return resolution_width_; }
	size_t samples() const { return samples_; }
	const Color &default_color() const { return default_color_; }
};

#endif