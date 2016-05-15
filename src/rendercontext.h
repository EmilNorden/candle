#include <atomic>
#include <memory>
#include <random>

class RenderContext
{
public:
	std::atomic_int scanline;
	const RenderConfiguration &configuration;
	std::unique_ptr<Color[]> image_buffer;
	std::mt19937 &random;

	RenderContext(const RenderConfiguration &config, std::mt19937 &rand)
		: configuration(config), random(rand)
	{
		scanline = -1;

		image_buffer.reset(new Color[config.resolution_height() * config.resolution_width()]);
	}

	bool get_next_scanline(size_t *line) { return (*line = ++scanline) < configuration.resolution_height(); }
};