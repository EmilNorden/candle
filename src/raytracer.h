#ifndef RAYTRACER_H_
#define RAYTRACER_H_

#include "vector.h"
#include <FreeImage.h>
#include <iostream>
#include <memory>
#include <thread>

#include "camera.h"
#include "material.h"
#include "output_util.h"
#include "ray.h"
#include "ray_mesh_intersection.h"
#include "renderconfig.h"
#include "rendercontext.h"

class Camera;
class RenderConfiguration;
class Scene;


#ifdef _DEBUG
#define NO_THREADS 5
#else
#define NO_THREADS 1
#endif

class RayTracer
{
private:
	template <typename TScene>
	void render_internal(RenderContext &context, const Camera &camera, const TScene &scene, bool reports_progress);
	
	template <typename TScene>
	void global_illumination(Ray &ray, const TScene &scene, std::mt19937 &random, const std::shared_ptr<Material> &material, const Vector3d &intersection_point, const Vector3d &surface_normal, Color &diffuse, Color &color, int depth) const;
	
	template <typename TScene>
	Color shade(Ray &ray, const TScene &scene, std::mt19937 &random, const RayMeshIntersection &intersection, int depth) const;
public:
	template <typename TScene>
	void render(RenderConfiguration &configuration, Camera &camera, TScene &scene);
	
	template <typename TScene>
	void render_hotspots(RenderConfiguration &configuration, Camera &camera, TScene &scene);
};

template <typename TScene>
void RenderHotspots(RenderContext &context, const Camera &camera, const TScene &scene, bool reports_progress)
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
				Color result = scene.propagate(ray, context.configuration.default_color(), context.random);
				auto t2 = clock.now();
				auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

				Color &pixel = context.image_buffer[(current_scanline * context.configuration.resolution_width()) + x];
				pixel.x() = milliseconds;
			}
		}

		if(reports_progress) {
			float completion = current_scanline / (float)(context.configuration.resolution_height()-1);
			display_progress(completion);
		}
	}
}

template <typename TScene>
void RayTracer::global_illumination(Ray &ray, const TScene &scene,  std::mt19937 &random, const std::shared_ptr<Material> &material, const Vector3d &intersection_point, const Vector3d &surface_normal, Color &diffuse, Color &color, int depth) const
{
	Vector3d random_dir = Vector3d::rand_unit_in_hemisphere(surface_normal, random);
	

	Vector3d reflected_direction = ray.m_direction - surface_normal * 2.0 * ray.m_direction.dot(surface_normal);
	reflected_direction.normalize();

	Vector3d final_dir = Vector3d::lerp(random_dir, reflected_direction, material->reflectivity());
	final_dir.normalize();

	Ray random_ray(intersection_point, final_dir);

	RayMeshIntersection random_collision;
	if(scene.propagate(random_ray, random_collision)) {
		Vector3d viewer = ray.m_direction * -1;
		Vector3d halfway = (random_dir + viewer);
		halfway.normalize();

		Color random_color = shade(random_ray, scene, random, random_collision, depth-1);
		// TODO: Calculate all these things in RayMeshIntersection

		/*Vector3d &v1 = random_collision.mesh->m_vertices[random_collision.index1] - random_collision.mesh->m_vertices[random_collision.index0];
		Vector3d &v2 = random_collision.mesh->m_vertices[random_collision.index2] - random_collision.mesh->m_vertices[random_collision.index0];
		Vector3d &random_point = random_collision.mesh->m_vertices[random_collision.index0] + (v1 * random_collision.u) + (v2 * random_collision.v);
		Vector3d dir = random_point - intersection_point;*/

		color += diffuse * random_color;
	}
	
}

template <typename TScene>
Color RayTracer::shade(Ray &ray, const TScene &scene, std::mt19937 &random, const RayMeshIntersection &intersection, int depth) const
{
	if(depth == 0)
		return Color(0, 0, 0);

	const std::shared_ptr<Material> &mat = intersection.mesh->m_material;
	// X == 330 && current_scanline == 370
	const Vector3d &v1 = intersection.mesh->m_vertices[intersection.index1] - intersection.mesh->m_vertices[intersection.index0];
	const Vector3d &v2 = intersection.mesh->m_vertices[intersection.index2] - intersection.mesh->m_vertices[intersection.index0];
	const Vector3d &intersection_point = intersection.mesh->m_vertices[intersection.index0] + (v1 * intersection.u) + (v2 * intersection.v);

	const Vector3d &n1 = intersection.mesh->m_normals[intersection.index1] - intersection.mesh->m_normals[intersection.index0];
	const Vector3d &n2 = intersection.mesh->m_normals[intersection.index2] - intersection.mesh->m_normals[intersection.index0];
	Vector3d interpolated_normal = intersection.mesh->m_normals[intersection.index0] + (n1 * intersection.u) + (n2 * intersection.v);
	interpolated_normal.normalize();

	Color diffuse_color;
	if(mat->has_texture()) {
		const Vector2d &tex1 = intersection.mesh->m_texture_coords[intersection.index1] - intersection.mesh->m_texture_coords[intersection.index0];
		const Vector2d &tex2 = intersection.mesh->m_texture_coords[intersection.index2] - intersection.mesh->m_texture_coords[intersection.index0];
		const Vector2d &interpolated_uv = intersection.mesh->m_texture_coords[intersection.index0] + (tex1 * intersection.u) + (tex2 * intersection.v);
		
		Color tex_sample;
		mat->sample(interpolated_uv.x(), interpolated_uv.y(), tex_sample);
		diffuse_color = tex_sample;
	}
	else {
		diffuse_color = mat->diffuse();
	}

	Color result = mat->emissive();
	
	global_illumination(ray, scene, random, mat, intersection_point, interpolated_normal, diffuse_color, result, depth);

	// diffuse
	auto& emissive_meshes = scene.emissive_meshes();
	size_t emissive_mesh_count = emissive_meshes.size();
	for(size_t i = 0; i < emissive_mesh_count; ++i) {
		const Mesh *mesh = emissive_meshes[i].mesh;
		double squared_distance = (emissive_meshes[i].center - intersection_point).length_squared();
		if(/*squared_distance <= emissive_meshes[i].effective_radius &&*/ mesh != intersection.mesh) {
			Ray shadow_ray;
			shadow_ray.m_origin = intersection_point;
			shadow_ray.m_direction = emissive_meshes[i].center - intersection_point; // TODO: Pre-calculate center of mesh?
			shadow_ray.m_direction.normalize();

			RayMeshIntersection shadow_intersection;
			if(scene.propagate(shadow_ray, shadow_intersection, &intersection)) { 
				if(shadow_intersection.mesh == mesh) {
					// No obstruction between our mesh and the emissive mesh
					double inv_square_length = 1.0 / squared_distance;
					double dot = shadow_ray.m_direction.dot(interpolated_normal);
					if(dot < 0.0)
						dot = 0.0;

					result += mesh->m_material->emissive() * diffuse_color * dot * inv_square_length;
				}
			}
		}
	}
	
		//if(mat->reflectivity() > 0) {
	//	result = Vector3d(1, 0, 0);
	//	//Vector3d reflected_direction = ray.m_direction - interpolated_normal * 2.0 * ray.m_direction.dot(interpolated_normal);
	//	//reflected_direction.normalize();
	//	//Ray reflected_ray(intersection_point, reflected_direction);

	//	//result = result*(1-mat->reflectivity()) + propagate(reflected_ray, default_color, random, depth-1, &collision)*mat->reflectivity();
	//}

	/*Vector3d light(1, 1, 0);
	light.normalize();
	double dot = light.dot(interpolated_normal);
	if(dot < 0)
		dot = 0.2;
	result = diffuse_color * dot;*/
	return result;
}


template <typename TScene>
void RayTracer::render_internal(RenderContext &context, const Camera &camera, const TScene &scene, bool reports_progress)
{
	Ray ray;
	RayMeshIntersection intersection;
	double sample_importance = 1.0 / context.configuration.samples();
	size_t current_scanline;
	while(context.get_next_scanline(&current_scanline)) {
		for(size_t x = 0; x < context.configuration.resolution_width(); ++x) {
			for(size_t sample = 0; sample < context.configuration.samples(); ++sample) {

				camera.cast_perturbed_ray(ray, x, current_scanline, context.random);
				//camera.cast_ray(ray, x, current_scanline);
				
				//Color result = scene.propagate(ray, context.configuration.default_color(), context.random);
				RayMeshIntersection intersection;
				Color result = context.configuration.default_color();
				if(scene.propagate(ray, intersection, nullptr)) {
					
					result = shade(ray, scene, context.random, intersection, 5);
				}

				result.clamp(Color(0, 0, 0), Color(1, 1, 1));

				Color &pixel = context.image_buffer[(current_scanline * context.configuration.resolution_width()) + x];
				result.multiply(sample_importance);
				pixel += result;
			}
		}

		if(reports_progress) {
			float completion = current_scanline / (float)(context.configuration.resolution_height()-1);
			display_progress(completion);
		}
	}
}


template <typename TScene>
void RayTracer::render(RenderConfiguration &configuration, Camera &camera, TScene &scene)
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
		threads[i] = std::thread(&RayTracer::render_internal<TScene>, this, std::ref(context), camera, scene, report_progress);
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
			Color &color = context.image_buffer[(y * configuration.resolution_width()) + x];
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

template <typename TScene>
void RayTracer::render_hotspots(RenderConfiguration &configuration, Camera &camera, TScene &scene)
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
		threads[i] = std::thread(RenderHotspots<TScene>, std::ref(context), camera, scene, report_progress);
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

	FreeImage_Save(FIF_PNG, bmp, "file_hotspots.png");

	FreeImage_Unload(bmp);
}



#endif