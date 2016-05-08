
#include <FreeImage.h>

#include "camera.h"
#include "renderconfig.h"
#include "raytracer.h"
#include "material.h"
#include "mesh.h"
#include "modelloader.h"
#include "assimp_processor.h"
#include "model.h"
#include "scene.h"
#include "vector.h"
#include "ray.h"

#include <iostream>

#include <random>
#include <ctime>

#define PI 3.14159265359

#define WIDTH	640
#define HEIGHT 480


double deg_to_rad(double deg)
{
	return deg * (180.0 / PI);
}

void add_light_ball(std::mt19937 &rand, ModelLoader &loader, Scene &scene, const Vector3d  &pos)
{
	auto ball2 = loader.load("/home/emil/models/sphere/only_quad_sphere.obj", false);
	scene.models.push_back(ball2);
	ball2->set_scale(Vector3d(10));
	ball2->set_translation(pos);

	int color = rand() % 3;
	if(color == 0)
	{
		for(auto &mesh : ball2->m_meshes)
		mesh->m_material->set_emissive(Vector3d(5000, 20000, 5000));
	}
	else if(color == 1)
	{
		for(auto &mesh : ball2->m_meshes)
			mesh->m_material->set_emissive(Vector3d(20000, 5000, 5000));
	}
	else if(color == 2)
	{
		for(auto &mesh : ball2->m_meshes)
			mesh->m_material->set_emissive(Vector3d(5000, 5000, 20000));
	}
}


void add_mushroom_house(std::mt19937 &rand, ModelLoader &loader, Scene &scene, const Vector3d  &pos)
{
	auto mush = loader.load("/home/emil/models/mushroom_house/Mushroom House.obj");
	mush->set_translation(pos);
	mush->set_rotation(Vector3d(0, -(PI / 2.0), 0));
	scene.models.push_back(mush);

	//add_light_ball(rand, loader, scene, pos);
	//add_light_ball(rand, loader, scene, pos);
}

#ifndef _TEST

int main(int argc, char **argv)
{
	size_t seed = 1439398627; // time(0);
	std::cout << "Using seed: " << seed << "\n";
	std::mt19937 rand(seed);
	FreeImage_Initialise();

	RenderConfiguration configuration(WIDTH, HEIGHT, 1, Vector3d(0, 0, 0));

	RayTracer tracer;

	Scene scene;

	ModelLoader loader;
	auto assimp_processor = std::unique_ptr<ModelProcessor>(new AssimpModelProcessor);
	loader.register_processor(assimp_processor);
	
	auto cube = loader.load("/home/emil/models/cube/cube.fbx");
	scene.models.push_back(cube);
	
	Vector3d pos(0, 0, 10);

	Vector3d target(0, 0, 0);

	Vector3d dir = target - pos;
	dir.normalize();
	
	Camera camera(pos, dir, Vector3d(0, 1, 0), PI / 4, (double)WIDTH / HEIGHT, Vector2i(WIDTH, HEIGHT), 599.041);
	camera.set_blur_radius(0);
	//camera.set_blur_radius(0.005);
	camera.update();

	scene.build_scene();

	Ray dist_ray;
	camera.cast_ray(dist_ray, WIDTH / 2.0, HEIGHT / 2.0);
	double dist = scene.measure_distance(dist_ray);
	std::cout << "Distance: " << dist << "\n";
	camera.set_focal_length(dist);
	
	//DWORD id = GetCurrentThreadId();

	std::cout << "Scene size: " << scene.models.size() << " models.\n";
	std::cout << "Octree depth: " << scene.tree_depth() << "\n";

	tracer.render(configuration, camera, scene);
	
	return 0;
}

#endif