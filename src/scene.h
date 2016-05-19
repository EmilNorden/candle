/*#ifndef SCENE_H_
#define SCENE_H_

#include "mesh.h"
#include "octree.h"
#include "vector.h"
#include "emissive_mesh.h"

#include <memory>
#include <random>
#include <vector>

class RayMeshInterSection;
class Model;
class Ray;



class Scene
{
private:
	std::vector<EmissiveMesh> emissive_meshes_;
	Octree<Mesh*> tree2_;

	RayMeshIntersection find_scene_intersection(const Ray &ray, const RayMeshIntersection * ignore = nullptr) const;
	Color propagate(Ray &ray, const Color &default_color, std::mt19937 &random, size_t depth, const RayMeshIntersection *ignore = nullptr) const;
	void global_illumination(Ray &ray, std::mt19937 &random, const std::shared_ptr<Material> &material, const Vector3d &intersection_point, const Vector3d &surface_normal, Color &diffuse, Color &color, size_t depth, const Color &default_color) const;
	Color shade(Ray &ray, std::mt19937 &random, const RayMeshIntersection &collision, size_t depth, const Color &default_color) const;
	void apply_all_transforms() const;
	void validate_no_duplicate_models() const;
	void find_emissive_meshes();
public:
	std::vector<std::shared_ptr<Model>> models;
	Color propagate(Ray &ray, const Color &default_color, std::mt19937 &random, size_t depth = 5) const;
	double measure_distance(Ray &ray);
	size_t tree_depth() const;

	void build_scene();
};

class OctreeScene
{
	RayMeshIntersection propagate(Ray &ray, const RayMeshIntersection *ignore = nullptr) const;
};

#endif
*/