
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
#include "texture.h"

#include <iostream>

#include <random>
#include <ctime>

#include <libconfig.h++>

#define PI 3.14159265359

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
	
	for(auto &mesh : ball2->m_meshes)
			mesh->m_material->set_emissive(Vector3d(11000, 11000, 11000));
			
	return;

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

Vector3d parse_color(libconfig::Setting &colorSetting)
{
	return Vector3d(colorSetting.lookup("r"), colorSetting.lookup("g"), colorSetting.lookup("b"));
}

RenderConfiguration parse_render_config(libconfig::Config &cfg)
{
	int renderWidth = cfg.lookup("render.width");
	int renderHeight = cfg.lookup("render.height");
	int samples = cfg.lookup("render.samples");
	int nthreads = cfg.lookup("render.nthreads");
	Vector3d backColor = parse_color(cfg.lookup("render.backColor"));
	
	return RenderConfiguration(nthreads, renderWidth, renderHeight, samples,
		backColor);;
}

Camera parse_camera_config(libconfig::Config &cfg, int resolution_width, int resolution_height)
{
	double fov = cfg.lookup("camera.fov");
	double blurRadius = cfg.lookup("camera.blurRadius");
	
	Vector3d position(
		cfg.lookup("camera.position.x"), 
		cfg.lookup("camera.position.y"), 
		cfg.lookup("camera.position.z"));
		
	Vector3d target(
		cfg.lookup("camera.target.x"), 
		cfg.lookup("camera.target.y"), 
		cfg.lookup("camera.target.z"));
	
	Vector3d up(
		cfg.lookup("camera.up.x"),
		cfg.lookup("camera.up.y"),
		cfg.lookup("camera.up.z"));
	
	Vector3d direction = target - position;
	direction.normalize();
	
	return Camera(position, direction, up, fov, 
		(double)resolution_width / resolution_height, 
		Vector2i(resolution_width, resolution_height), 599.041, blurRadius);
}

void parse_scene_config(libconfig::Config &cfg, Scene &scene, ModelLoader &loader)
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
						Texture *texture = nullptr;
						Vector3d diffuse;
						Vector3d emission;
						
						if(meshConfig[j].exists("diffuseColor"))
						{
							auto& diffuseColor = meshConfig[j].lookup("diffuseColor");
							diffuse = parse_color(diffuseColor);
						}
						
						std::string texturePath;
						if(meshConfig[j].lookupValue("texture", texturePath))
						{
							std::cout << "Loading texture '" << texturePath << "'.\n";
							texture = new Texture(texturePath);
						}

						if(meshConfig[j].exists("emissionColor"))
						{
							double emissionStrength = meshConfig[j].lookup("emissionStrength");
							auto& emissionColor = meshConfig[j].lookup("emissionColor");
							emission = parse_color(emissionColor) * emissionStrength;
						}
						
						mesh->m_material = std::make_shared<Material>(0, texture, diffuse, emission);
						break;
					}
				}
				
				if(!meshFound)
				{
					std::cerr << "Could not override material for mesh '" << meshName << "'. Mesh not found.\n";
				}
			}
		}
		scene.models.push_back(model);
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
	
	cfg.readFile("../input/test1.cfg");

	auto renderConfiguration = parse_render_config(cfg);
	auto camera = parse_camera_config(cfg, renderConfiguration.resolution_width(), renderConfiguration.resolution_height());
	Scene scene;
	
	parse_scene_config(cfg, scene, loader);
	
	RayTracer tracer;

	
	
	//add_mushroom_house(rand, loader, scene, Vector3d(0, 0, 0));
	//add_light_ball(rand, loader, scene, Vector3d(-40, 300, 0));
	//add_light_ball(rand, loader, scene, Vector3d(40, 100, 300));
	
	camera.update();

	scene.build_scene();

	Ray dist_ray;
	camera.cast_ray(dist_ray, renderConfiguration.resolution_width() / 2.0, renderConfiguration.resolution_height() / 2.0);
	double dist = scene.measure_distance(dist_ray);
	std::cout << "Distance: " << dist << "\n";
	camera.set_focal_length(dist);
	
	//DWORD id = GetCurrentThreadId();

	std::cout << "Scene size: " << scene.models.size() << " models.\n";
	std::cout << "Octree depth: " << scene.tree_depth() << "\n";

	tracer.render(renderConfiguration, camera, scene);
	
	return 0;
}

#endif