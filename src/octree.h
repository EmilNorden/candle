#ifndef OCTREE2_H_
#define OCTREE2_H_

#include "aabb.h"
#include "ray.h"
#include "collisioninfo.h"
#include <vector>
#include <functional>
#include <memory>

template <typename T>
class Octree
{
private:
	template <typename TCuboid>
	struct Cuboid
	{
	public:
		AABB bounds;
		std::vector<TCuboid> objects;
		std::shared_ptr<Cuboid<TCuboid>> cuboids;
	
		Cuboid() { }
	};


	Cuboid<T> root_;

	void buildCuboid(size_t treshold, size_t maxDepth, size_t depth, Cuboid<T> &cuboid);
	void processTreeInternal(const Cuboid<T> &cuboid, const AABB &bounds, std::function<void(const T&)> &callback) const;
	void processTreeInternal(const Cuboid<T> &cuboid, const Ray &ray, const std::function<void(const T&)> &callback) const;
	void find_max_depth(const Cuboid<T> &current, size_t current_depth, size_t &max_depth) const;

	bool propagate_internal(Ray &ray, const Cuboid<T> &cuboid, CollisionInfoInternal &collision, const CollisionInfoInternal *ignore) const;
public:
	Octree() { }

	template <typename Iter>
	void build(size_t treshold, size_t maxDepth, Iter a, Iter b, typename Iter::iterator_category *p = 0);
	void processTree(const AABB &bounds, std::function<void(const T&)> &callback) const;
	void processTree(const Ray &ray, const std::function<void(const T&)> &callback) const;

	bool propagate(Ray &ray, CollisionInfoInternal &collision, const CollisionInfoInternal *ignore = nullptr) const;

	size_t depth() const;
};

template <typename T>
template <typename Iter>
void Octree<T>::build(size_t treshold, size_t maxDepth, Iter a, Iter b, typename Iter::iterator_category *p)
{
	root_.objects.clear();
	for(auto it = a; it != b; ++it)
	{
		root_.bounds.expand((*it)->bounds());
		root_.objects.push_back(*it);
		//root_.objects.push_back((*it).get());
	}

	buildCuboid(treshold, maxDepth, 1, root_);
}

template <typename T>
void Octree<T>::buildCuboid(size_t treshold, size_t maxDepth, size_t depth, Cuboid<T> &cuboid)
{
	if(cuboid.objects.size() <= treshold || depth >= maxDepth)
		return;
	Vector3d splitSize = (cuboid.bounds.max() - cuboid.bounds.min()) / 2.0;
	cuboid.cuboids.reset(new Cuboid<T>[8], [](Cuboid<T> *p) { delete[] p; });
	
	for(int x = 0; x < 2; ++x)
	{
		for(int y = 0; y < 2; ++y)
		{
			for(int z = 0; z < 2; ++z) {
				Cuboid<T> &child = cuboid.cuboids.get()[(x * 4) + (y * 2)  + z];
				Vector3d child_min = cuboid.bounds.min() + (splitSize * Vector3d(x, y, z));
				Vector3d child_max = child_min + splitSize;
				child.bounds = AABB(child_min, child_max);
				
				for(int obj_i = cuboid.objects.size() - 1; obj_i >= 0; --obj_i)
				{
					if(child.bounds.intersects(cuboid.objects[obj_i]->bounds()))
					{
						child.objects.push_back(cuboid.objects[obj_i]);
					}
				}
				buildCuboid(treshold, maxDepth, depth + 1, child);
			}
		}
	}

	cuboid.objects.clear();
}

template <typename T>
void Octree<T>::processTreeInternal(const Cuboid<T> &cuboid, const AABB &bounds, std::function<void(const T&)> &callback) const
{
	if(cuboid.bounds.intersects(bounds))
	{
		if(cuboid.cuboids)
		{
			for(int i = 0; i < 8; ++i)
				processTreeInternal(cuboid.cuboids.get()[i], bounds, callback);
		}
		else
		{
			for(size_t i = 0; i < cuboid.objects.size(); ++i)
			{
				if(cuboid.objects[i]->bounds().intersects(bounds))
				{
					callback(cuboid.objects[i]);
				}
			}
		}
	}
}

template <typename T>
void Octree<T>::processTreeInternal(const Cuboid<T> &cuboid, const Ray &ray, const std::function<void(const T&)> &callback) const
{
	if(cuboid.bounds.intersects(ray))
	{
		if(cuboid.cuboids)
		{
			for(int i = 0; i < 8; ++i)
				processTreeInternal(cuboid.cuboids.get()[i], ray, callback);
		}
		else
		{
			for(size_t i = 0; i < cuboid.objects.size(); ++i)
			{
				if(cuboid.objects[i]->bounds().intersects(ray))
				{
					callback(cuboid.objects[i]);
				}
			}
		}
	}
}

template <typename T>
void Octree<T>::processTree(const AABB &bounds, std::function<void(const T&)> &callback) const
{
	processTreeInternal(root_, bounds, callback);
}

template <typename T>
void Octree<T>::processTree(const Ray &ray, const std::function<void(const T&)> &callback) const
{
	processTreeInternal(root_, ray, callback);
}

template <typename T>
size_t Octree<T>::depth() const
{
	size_t depth = 0;
	find_max_depth(root_, 1, depth);
	return depth;
}

template <typename T>
void Octree<T>::find_max_depth(const Cuboid<T> &current, size_t current_depth, size_t &max_depth) const
{
	if(current_depth > max_depth)
		max_depth = current_depth;

	if(current.cuboids) {
		for(size_t i = 0; i < 8; ++i) {
			find_max_depth(current.cuboids.get()[i], current_depth + 1, max_depth);
		}
	}
}

static bool should_ignore(const Mesh *mesh, uint16_t i0, uint16_t i1, uint16_t i2, const CollisionInfoInternal *ignore)
{
	return ignore != nullptr && ignore->mesh == mesh && ignore->index0 == i0 && ignore->index1 == i1 && ignore->index2 == i2;
}

template <typename T>
bool Octree<T>::propagate_internal(Ray &ray, const Cuboid<T> &cuboid, CollisionInfoInternal &collision, const CollisionInfoInternal *ignore) const
{
	double cuboid_dist;
	if(!cuboid.bounds.intersects_dist(ray, cuboid_dist))
		return false;

	if(cuboid_dist > ray.dist)
		return false;

	bool success = false;

	if(cuboid.cuboids) {
		for(size_t i = 0; i < 8; ++i) {
			if(propagate_internal(ray, cuboid.cuboids.get()[i], collision, ignore))
				success = true;
		}
	}
	else {
		size_t object_count = cuboid.objects.size();
		for(size_t i = 0; i < object_count; ++i) {
			double obj_dist;
			const Mesh* mesh = cuboid.objects[i];
			if(mesh->bounds().intersects_dist(ray, obj_dist)) {

				if(obj_dist < ray.dist) {

					size_t index_count = mesh->m_indices.size();
					for(size_t i = 0; i < index_count; i += 3) {
						uint16_t i0 = mesh->m_indices[i];
						uint16_t i1 = mesh->m_indices[i+1];
						uint16_t i2 = mesh->m_indices[i+2];

						//if(ignore_mesh_triangle(ignore, mesh, i0, i1, i2))
						//	continue;

						double dist;
						double u, v;
						if(!should_ignore(mesh, i0, i1, i2, ignore) && ray.intersects(mesh->m_vertices[i0], 
							mesh->m_vertices[i1],
							mesh->m_vertices[i2], &u, &v, &dist) && dist < ray.dist)
						{
							ray.dist = dist;

							collision.mesh = mesh;
							collision.index0 = i0;
							collision.index1 = i1;
							collision.index2 = i2;
							collision.u = u;
							collision.v = v;
							collision.distance = dist;
							success = true;
						}
					}

				}
			}
		}
	}
	
	return success;
}

template <typename T>
bool Octree<T>::propagate(Ray &ray, CollisionInfoInternal &collision, const CollisionInfoInternal *ignore) const
{
	return propagate_internal(ray, root_, collision, ignore);
}

#endif