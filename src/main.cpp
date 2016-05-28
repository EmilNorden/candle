
#include <FreeImage.h>

#include "camera.h"
#include "renderconfig.h"
#include "raytracer.h"
#include "material.h"
#include "mesh.h"
#include "modelloader.h"
#include "assimp_processor.h"
#include "model.h"
#include "vector.h"
#include "ray.h"
#include "texture.h"
#include "octreescene.h"

#include <iostream>

#include <random>
#include <ctime>

#include <libconfig.h++>

#define PI 3.14159265359

double deg_to_rad(double deg)
{
	return deg * (180.0 / PI);
}

template <typename TScene>
void add_light_ball(std::mt19937 &rand, ModelLoader &loader, TScene &scene, const Vector3d  &pos)
{
	auto ball2 = loader.load("/home/emil/models/sphere/only_quad_sphere.obj", false);
	scene.add_model(ball2);
	//scene.models.push_back(ball2);
	
	ball2->set_scale(Vector3d(10));
	ball2->set_translation(pos);
	
	for(auto &mesh : ball2->m_meshes)
			mesh->m_material->set_emissive(Color(11000, 11000, 11000));
			
	return;

	int color = rand() % 3;
	if(color == 0)
	{
		for(auto &mesh : ball2->m_meshes)
			mesh->m_material->set_emissive(Color(5000, 20000, 5000));
	}
	else if(color == 1)
	{
		for(auto &mesh : ball2->m_meshes)
			mesh->m_material->set_emissive(Color(20000, 5000, 5000));
	}
	else if(color == 2)
	{
		for(auto &mesh : ball2->m_meshes)
			mesh->m_material->set_emissive(Color(5000, 5000, 20000));
	}
}

template <typename TScene>
void add_mushroom_house(std::mt19937 &rand, ModelLoader &loader, TScene &scene, const Vector3d  &pos)
{
	auto mush = loader.load("/home/emil/models/mushroom_house/Mushroom House.obj");
	mush->set_translation(pos);
	mush->set_rotation(Vector3d(0, -(PI / 2.0), 0));
	scene.add_model(mush);
	//scene.models.push_back(mush);

	//add_light_ball(rand, loader, scene, pos);
	//add_light_ball(rand, loader, scene, pos);
}

#ifndef _TEST

Color parse_color(libconfig::Setting &colorSetting)
{
	return Color(colorSetting.lookup("r"), colorSetting.lookup("g"), colorSetting.lookup("b"));
}

RenderConfiguration parse_render_config(libconfig::Config &cfg)
{
	int renderWidth = cfg.lookup("render.width");
	int renderHeight = cfg.lookup("render.height");
	int samples = cfg.lookup("render.samples");
	int nthreads = cfg.lookup("render.nthreads");
	Color backColor = parse_color(cfg.lookup("render.backColor"));
	
	return RenderConfiguration(nthreads, renderWidth, renderHeight, samples,
		backColor);;
}

Camera parse_camera_config(libconfig::Config &cfg, int resolution_width, int resolution_height)
{
	double fov = cfg.lookup("camera.fov");
	double blurRadius = cfg.lookup("camera.blurRadius");
	
	Vector3f position(
		cfg.lookup("camera.position.x"), 
		cfg.lookup("camera.position.y"), 
		cfg.lookup("camera.position.z"));
		
	Vector3f target(
		cfg.lookup("camera.target.x"), 
		cfg.lookup("camera.target.y"), 
		cfg.lookup("camera.target.z"));
	
	Vector3f up(
		cfg.lookup("camera.up.x"),
		cfg.lookup("camera.up.y"),
		cfg.lookup("camera.up.z"));
	
	Vector3f direction = target - position;
	direction.normalize();
	
	return Camera(position, direction, up, fov, 
		(double)resolution_width / resolution_height, 
		Vector2i(resolution_width, resolution_height), 599.041, blurRadius);
}

template <typename TScene>
void parse_scene_config(libconfig::Config &cfg, TScene &scene, ModelLoader &loader)
{
	libconfig::Setting& objects = cfg.lookup("scene.objects");
	
	for(int i = 0; i < objects.getLength(); ++i)
	{
		std::string modelPath = (const char*)objects[i]["modelPath"];
		Vector3d translation(
			objects[i].lookup("translation.x"),
			objects[i].lookup("translation.y"),
			objects[i].lookup("translation.z"));
		Vector3d rotation(
			objects[i].lookup("rotation.x"),
			objects[i].lookup("rotation.y"),
			objects[i].lookup("rotation.z"));
		Vector3d scale(
			objects[i].lookup("scale.x"),
			objects[i].lookup("scale.y"),
			objects[i].lookup("scale.z"));
		
		auto model = loader.load(modelPath);
		model->set_translation(translation);
		model->set_rotation(rotation);
		model->set_scale(scale);
		
		if(objects[i].exists("meshes"))
		{
			libconfig::Setting& meshConfig = objects[i].lookup("meshes");
			for(int j = 0; j < meshConfig.getLength(); ++j)
			{
				std::string meshName = meshConfig[j]["name"];
				
				bool meshFound = false;
				for(auto& mesh : model->m_meshes)
				{
					if(mesh->name() == meshName)
					{
						meshFound = true;
						Texture *diffuseMap = nullptr;
						Texture *normalMap = nullptr;
						Color diffuse;
						Color emission;
						
						if(meshConfig[j].exists("diffuseColor")) {
							
							auto& diffuseColor = meshConfig[j].lookup("diffuseColor");
							diffuse = parse_color(diffuseColor);
							
						}
						else {
							
							//diffuse = mesh->m_material->diffuse();
							
						}
						
						std::string mapPath;
						if(meshConfig[j].lookupValue("diffuseMap", mapPath)) {
							
							std::cout << "Loading texture '" << mapPath << "'.\n";
							diffuseMap = new Texture(mapPath);
							
						}
						else {
							
							diffuseMap = mesh->m_material->diffuse_map();
							
						}
						
						if(meshConfig[j].lookupValue("normalMap", mapPath)) {
							
							std::cout << "Loading texture '" << mapPath << "'.\n";
							normalMap = new Texture(mapPath);
							
						}
						else {
							
							normalMap = mesh->m_material->normal_map();
							
						}

						if(meshConfig[j].exists("emissionColor")) {
							
							double emissionStrength = meshConfig[j].lookup("emissionStrength");
							auto& emissionColor = meshConfig[j].lookup("emissionColor");
							emission = parse_color(emissionColor) * emissionStrength;
							
						}
						else {
							
							emission = mesh->m_material->emissive();
							
						}
						
						mesh->m_material = std::make_shared<Material>(0, diffuse, emission, diffuseMap, normalMap);
						break;
					}
				}
				
				if(!meshFound)
				{
					std::cerr << "Could not override material for mesh '" << meshName << "'. Mesh not found.\n";
				}
			}
		}
		scene.add_model(model);
		//scene.models.push_back(model);
	}
}

int main(int argc, char **argv)
{
	size_t seed = 1439398627; // time(0);
	std::cout << "Using seed: " << seed << "\n";
	std::mt19937 rand(seed);
	FreeImage_Initialise();
	
	ModelLoader loader;
	auto assimp_processor = std::unique_ptr<ModelProcessor>(new AssimpModelProcessor);
	loader.register_processor(assimp_processor);
	
	libconfig::Config cfg;
	
	cfg.readFile("../input/test2.cfg");

	auto renderConfiguration = parse_render_config(cfg);
	auto camera = parse_camera_config(cfg, renderConfiguration.resolution_width(), renderConfiguration.resolution_height());
	
	OctreeScene scene;
	
	parse_scene_config(cfg, scene, loader);
	
	RayTracer tracer;
	
	camera.update();

	scene.build_scene();

	Ray dist_ray;
	camera.cast_ray(dist_ray, renderConfiguration.resolution_width() / 2.0, renderConfiguration.resolution_height() / 2.0);
	
	double dist = DBL_MAX;
	RayMeshIntersection intersection;
	if(scene.propagate(dist_ray, intersection))
		dist = dist_ray.dist;
		
	std::cout << "Distance: " << dist << "\n";
	if(dist > 3000)
		dist = 3000;
	camera.set_focal_length(dist);
	
	//DWORD id = GetCurrentThreadId();

	scene.print_statistics();

	tracer.render(renderConfiguration, camera, scene);
	
	return 0;
}

#endif