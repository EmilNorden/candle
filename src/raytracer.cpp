#include "raytracer.h"

#include "output_util.h"
#include "camera.h"
#include "collisioninfo.h"
#include "ray.h"
#include "renderconfig.h"
#include "scene.h"
#include <FreeImage.h>
#include <iostream>
#include <atomic>
#include <chrono>
#include <random>
#include <thread>
#include <memory>

#ifdef _DEBUG
#define NO_THREADS 5
#else
#define NO_THREADS 1
#endif

class RenderContext
{
public:
	std::atomic_int scanline;
	const RenderConfiguration &configuration;
	std::unique_ptr<Vector3d[]> image_buffer;
	std::mt19937 &random;

	RenderContext(const RenderConfiguration &config, std::mt19937 &rand)
		: configuration(config), random(rand)
	{
		scanline = -1;

		image_buffer.reset(new Vector3d[config.resolution_height() * config.resolution_width()]);
	}

	bool get_next_scanline(size_t *line) { return (*line = ++scanline) < configuration.resolution_height(); }
};

void Render(RenderContext &context, const Camera &camera, const Scene &scene, bool reports_progress)
{
	Ray ray;
	double sample_importance = 1.0 / context.configuration.samples();
	size_t current_scanline;
	while(context.get_next_scanline(&current_scanline)) {
		for(size_t x = 0; x < context.configuration.resolution_width(); ++x) {
			for(size_t sample = 0; sample < context.configuration.samples(); ++sample) {

				camera.cast_perturbed_ray(ray, x, current_scanline, context.random);
				//camera.cast_ray(ray, x, current_scanline);

				Vector3d result = scene.propagate(ray, context.configuration.default_color(), context.random);

				result.clamp(Vector3d(0, 0, 0), Vector3d(1, 1, 1));

				Vector3d &pixel = context.image_buffer[(current_scanline * context.configuration.resolution_width()) + x];
				result.multiply(sample_importance);
				pixel += result;
				
				//pixel.multiply(sample);
				//pixel += result;
				//pixel.divide(sample + 1);
				
			}
		}

		if(reports_progress) {
			float completion = current_scanline / (float)(context.configuration.resolution_height()-1);
			display_progress(completion);
		}
	}
}

void RenderHotspots(RenderContext &context, const Camera &camera, const Scene &scene, bool reports_progress)
{
	std::chrono::high_resolution_clock clock;
	double sample_importance = 1.0 / context.configuration.samples();
	size_t current_scanline;
	while(context.get_next_scanline(&current_scanline)) {
		for(size_t x = 0; x < context.configuration.resolution_width(); ++x) {
			for(size_t sample = 0; sample < context.configuration.samples(); ++sample) {

				Ray ray;
				camera.cast_perturbed_ray(ray, x, current_scanline, context.random);

				auto t1 = clock.now();
				Vector3d result = scene.propagate(ray, context.configuration.default_color(), context.random);
				auto t2 = clock.now();
				auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

				Vector3d &pixel = context.image_buffer[(current_scanline * context.configuration.resolution_width()) + x];
				pixel.x() = milliseconds;
			}
		}

		if(reports_progress) {
			float completion = current_scanline / (float)(context.configuration.resolution_height()-1);
			display_progress(completion);
		}
	}
}

void RayTracer::render(RenderConfiguration &configuration, Camera &camera, Scene &scene)
{
	std::mt19937 random(time(0));
	RenderContext context(configuration, random);
	
	int nthreads = configuration.nthreads();
	
	if(nthreads == 0)
		nthreads = NO_THREADS;

	std::unique_ptr<std::thread[]> threads(new std::thread[nthreads]);

	std::cout << "Starting render...\n";
	std::cout << "Threads: " << nthreads << "\n";

	std::chrono::high_resolution_clock clock;
	auto render_start = clock.now();
	
	for(int i = 0; i < nthreads; ++i) {
		bool report_progress = i == 0;
		threads[i] = std::thread(Render, std::ref(context), camera, scene, report_progress);
	}

	for(int i = 0; i < nthreads; ++i) {
		threads[i].join();
	}

	auto render_end = clock.now();

	display_progress(1.0);

	std::cout << "done. Time taken: " << std::chrono::duration_cast<std::chrono::seconds>(render_end - render_start).count() << " seconds.\n";

	FIBITMAP *bmp = FreeImage_Allocate(configuration.resolution_width(), configuration.resolution_height(), 24);

	for(size_t y = 0; y < configuration.resolution_height(); ++y)
	{
		for(size_t x = 0; x < configuration.resolution_width(); ++x)
		{
			Vector3d &color = context.image_buffer[(y * configuration.resolution_width()) + x];
			RGBQUAD rgb;
			rgb.rgbRed = color.x() * 255;
			rgb.rgbGreen = color.y() * 255;
			rgb.rgbBlue = color.z() * 255;
			rgb.rgbReserved = 255;
			FreeImage_SetPixelColor(bmp, x, y, &rgb);
		}
	}

	FreeImage_Save(FIF_PNG, bmp, "file.png");

	FreeImage_Unload(bmp);
}

void RayTracer::render_hotspots(RenderConfiguration &configuration, Camera &camera, Scene &scene)
{
	std::mt19937 random(time(0));
	RenderContext context(configuration, random);
	
	int nthreads = configuration.nthreads();
	
	if(nthreads == 0)
		nthreads = NO_THREADS;

	std::unique_ptr<std::thread[]> threads(new std::thread[nthreads]);

	std::cout << "Starting render...\n";
	std::cout << "Threads: " << nthreads << "\n";

	std::chrono::high_resolution_clock clock;
	auto render_start = clock.now();
	
	for(int i = 0; i < nthreads; ++i) {
		bool report_progress = i == 0;
		threads[i] = std::thread(RenderHotspots, std::ref(context), camera, scene, report_progress);
	}

	for(int i = 0; i < nthreads; ++i) {
		threads[i].join();
	}

	auto render_end = clock.now();

	display_progress(1.0);

	std::cout << "done. Time taken: " << std::chrono::duration_cast<std::chrono::seconds>(render_end - render_start).count() << " seconds.\n";

	double max_time = -DBL_MAX;
	for(size_t i = 0; i < configuration.resolution_height() * configuration.resolution_width(); ++i) {
		if(context.image_buffer[i].x() > max_time)
			max_time = context.image_buffer[i].x();
	}

	FIBITMAP *bmp = FreeImage_Allocate(configuration.resolution_width(), configuration.resolution_height(), 24);

	for(size_t y = 0; y < configuration.resolution_height(); ++y)
	{
		for(size_t x = 0; x < configuration.resolution_width(); ++x)
		{
			double time = context.image_buffer[(y * configuration.resolution_width()) + x].x();
			RGBQUAD rgb;
			rgb.rgbRed = (time / max_time) * 255;
			rgb.rgbGreen = (time / max_time) * 255;
			rgb.rgbBlue = (time / max_time) * 255;
			rgb.rgbReserved = 255;
			FreeImage_SetPixelColor(bmp, x, y, &rgb);
		}
	}

	FreeImage_Save(FIF_PNG, bmp, "C:\\temp\\bray\\file_hotspot.png");

	FreeImage_Unload(bmp);
}