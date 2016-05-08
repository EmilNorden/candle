#ifndef OCTREE_H_
#define OCTREE_H_

#include "aabb.h"
#include "collisioninfo.h"

#include <functional>
#include <memory>
#include <vector>
#include <boost/shared_array.hpp>
#include <set>

class Model;
class Mesh;

class CuboidInternal
{
public:
	int id;
	AABB bounds;
	std::vector<Mesh*> items;
	boost::shared_array<CuboidInternal> children;

	CuboidInternal();
};

class OctreeOld
{
private:

	void build_internal(CuboidInternal &current, size_t limit, size_t current_depth, size_t max_depth, int *cuboid_id);
	bool propagate_internal(const Ray &ray, const CuboidInternal &current, CollisionInfoInternal &collision, const CollisionInfoInternal *ignore) const;
	bool propagate_distance_internal(const Ray &ray, const CuboidInternal &current, double *dist) const;
	void find_max_depth(const CuboidInternal &current, size_t current_depth, size_t &max_depth) const;
	void process_internal(const CuboidInternal &cuboid, const AABB &bounds, std::function<void(const Mesh*)> &callback);

	CuboidInternal root_;
public:
	void build(const std::vector<std::shared_ptr<Model>> &items, size_t limit, size_t max_depth);

	bool propagate(const Ray &ray, CollisionInfoInternal &collision, const CollisionInfoInternal *ignore = nullptr) const;
	bool propagate_distance(const Ray &ray, double *dist) const;
	size_t depth() const;

	void meshes(std::set<Mesh*> &meshes);
	void getallmeshes(CuboidInternal &cuboid, std::set<Mesh*> &meshes);

	void process(const AABB &bounds, std::function<void(const Mesh*)> &callback);
};

#endif