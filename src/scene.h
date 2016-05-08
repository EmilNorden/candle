#ifndef SCENE_H_
#define SCENE_H_

#include "collisioninfo.h"
#include "mesh.h"
#include "octreeold.h"
#include "octree.h"
#include "vector.h"

#include <memory>
#include <random>
#include <vector>

class CollisionInfoInternal;
class Model;
class Ray;

class EmissiveMesh
{
public:
	Mesh *mesh;
	double effective_radius;
	Vector3d center;

	EmissiveMesh(Mesh *m);
};

class Scene
{
private:
	std::vector<EmissiveMesh> emissive_meshes_;
	OctreeOld tree_;
	Octree<Mesh*> tree2_;

	CollisionInfoInternal find_scene_intersection(const Ray &ray, const CollisionInfoInternal * ignore = nullptr) const;
	Vector3d propagate(Ray &ray, const Vector3d &default_color, std::mt19937 &random, size_t depth, const CollisionInfoInternal *ignore = nullptr) const;
	void global_illumination(Ray &ray, std::mt19937 &random, const std::shared_ptr<Material> &material, const Vector3d &intersection_point, const Vector3d &surface_normal, Vector3d &diffuse, Vector3d &color, size_t depth, const Vector3d &default_color) const;
	Vector3d shade(Ray &ray, std::mt19937 &random, const CollisionInfoInternal &collision, size_t depth, const Vector3d &default_color) const;
	void apply_all_transforms() const;
	void validate_no_duplicate_models() const;
	void find_emissive_meshes();
public:
	std::vector<std::shared_ptr<Model>> models;
	Vector3d propagate(Ray &ray, const Vector3d &default_color, std::mt19937 &random, size_t depth = 5) const;
	double measure_distance(Ray &ray);
	size_t tree_depth() const;

	void build_scene();
};

#endif