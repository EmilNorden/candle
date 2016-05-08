#include "octreeold.h"

#include "aabb.h"
#include "model.h"
#include "mesh.h"
#include "ray.h"


CuboidInternal::CuboidInternal()
	: bounds(AABB(Vector3d(DBL_MAX), Vector3d(-DBL_MAX)))
{
}

void OctreeOld::getallmeshes(CuboidInternal &cuboid, std::set<Mesh*> &meshes)
{
	if(cuboid.children) {
		for(int i = 0; i < 8; ++i) {
			getallmeshes(cuboid.children[i], meshes);
		}
	}
	else {
		for(int i = 0; i < cuboid.items.size(); ++i) {
			meshes.insert(cuboid.items[i]);
		}
	}
}

void OctreeOld::meshes(std::set<Mesh*> &meshes)
{
	getallmeshes(root_, meshes);
}


void OctreeOld::build(const std::vector<std::shared_ptr<Model>> &items, size_t limit, size_t max_depth)
{
	root_.bounds = AABB(Vector3d(DBL_MAX), Vector3d(-DBL_MAX));

	for(const auto &model : items) {
		for(const auto &mesh : model->m_meshes) {
			root_.bounds.expand(mesh->m_bounds);
			root_.items.push_back(mesh.get());
		}
	}

	int id = 0;
	build_internal(root_, limit, 1, max_depth, &id);
}

void OctreeOld::build_internal(CuboidInternal &current, size_t limit, size_t current_depth, size_t max_depth, int *cuboid_id)
{
	current.id = *cuboid_id;
	*cuboid_id = (*cuboid_id)+1;

	if(current.items.size() < limit || current_depth >= max_depth)
		return;

	Vector3d child_size = (current.bounds.max() - current.bounds.min()) / 2.0;

	current.children.reset(new CuboidInternal[8]);
	for(int x = 0; x < 2; ++x)
	{
		for(int y = 0; y < 2; ++y)
		{
			for(int z = 0; z < 2; ++z) {
				CuboidInternal &child = current.children[(x * 4) + (y * 2)  + z];

				Vector3d min = current.bounds.min() + (child_size * Vector3d(x , y, z));
				Vector3d max = min + child_size;
				child.bounds = AABB(min, max);
			}
		}
	}

	for(int i = 0; i < 8; ++i) {
		for(auto &item : current.items) {
			if(item->m_bounds.intersects(current.children[i].bounds)) {
				current.children[i].items.push_back(item);
			}
		}

		build_internal(current.children[i], limit, current_depth + 1, max_depth, cuboid_id);
	}

	//current.items.clear();
}

bool OctreeOld::propagate(const Ray &ray, CollisionInfoInternal &collision, const CollisionInfoInternal *ignore) const
{
	return propagate_internal(ray, root_, collision, ignore);
}

static bool should_ignore(const Mesh *mesh, uint16_t i0, uint16_t i1, uint16_t i2, const CollisionInfoInternal *ignore)
{
	return ignore != nullptr && ignore->mesh == mesh && ignore->index0 == i0 && ignore->index1 == i1 && ignore->index2 == i2;
}

//struct distance_from_ray
//{
//	const Ray &ray_;
//	distance_from_ray(const Ray &ray)
//		: ray_(ray)
//	{
//	}
//
//	double score(const CuboidInternal *a) const
//	{
//		Vector3d a_center = (a->bounds.max() - a->bounds.min()) / 2.0;
//
//		Vector3d ray_a_dir = a_center - ray_.m_origin;
//		ray_a_dir.normalize();
//		double a_dot = 2 - ray_a_dir.dot(ray_.m_direction);
//
//		return a_dot * (ray_.m_origin - a_center).length_squared();
//	}
//
//	inline bool operator() (const CuboidInternal *a, const CuboidInternal *b)
//	{
//		return score(a) < score(b);
//	}
//};

inline bool is_within_reach(const Ray &ray, const Vector3d &v1, const Vector3d &v2, const Vector3d &v3, double max_dist)
{
	return (ray.m_origin - v1).length_squared() < max_dist || (ray.m_origin - v2).length_squared() < max_dist || (ray.m_origin - v3).length_squared() < max_dist; 
}

bool OctreeOld::propagate_internal(const Ray &ray, const CuboidInternal &current, CollisionInfoInternal &collision, const CollisionInfoInternal *ignore) const
{
	if(!current.bounds.intersects(ray))
		return false;

	if(current.children) {
		//CuboidInternal *children[8] =  { &current.children[0], &current.children[1], &current.children[2], &current.children[3], &current.children[4], &current.children[5], &current.children[6], &current.children[7] };
		//std::sort(std::begin(children), std::end(children), distance_from_ray(ray));
		double best_dist = DBL_MAX;
		CollisionInfoInternal child_collision;
		bool result = false;
		for(int i = 0; i < 8; ++i) {

			if(propagate_internal(ray, current.children[i], child_collision, ignore) && child_collision.distance < best_dist) {
				best_dist = child_collision.distance;
				collision = child_collision;
				result = true;
			}
		}

		return result;
	}
	else {
		double best_dist = DBL_MAX;
		bool result = false;

		for(const auto &mesh : current.items) {

			if(mesh->m_bounds.intersects(ray)) {
		
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
						mesh->m_vertices[i2], &u, &v, &dist) && dist < best_dist)
					{
						//bool ray_in_cuboid = current.bounds.intersects(ray);
						//bool mesh_in_cuboid = mesh->m_bounds.intersects(current.bounds);
						//bool ray_in_mesh = mesh->m_bounds.intersects(ray);
						//if(!current.bounds.intersects(ray)) {
						//	bool b = current.bounds.intersects(mesh->m_bounds);
						//	int fff = 5;

						//	
						//}

						best_dist = dist;

						collision.mesh = mesh;
						collision.index0 = i0;
						collision.index1 = i1;
						collision.index2 = i2;
						collision.u = u;
						collision.v = v;
						collision.distance = dist;

						result = true;
					}
				}
			}

		}

		return result;
	}

	return false;
}

bool OctreeOld::propagate_distance_internal(const Ray &ray, const CuboidInternal &current, double *dist) const
{
	if(!current.bounds.intersects(ray))
		return false;

	if(current.children) {
		for(int i = 0; i < 8; ++i) {
			if(propagate_distance_internal(ray, current.children[i], dist))
				return true;
		}
	}
	else {
		double best_dist = DBL_MAX;
		bool result = false;

		for(auto &mesh : current.items) {

			if(mesh->m_bounds.intersects(ray)) {
		
				size_t index_count = mesh->m_indices.size();
				for(size_t i = 0; i < index_count; i += 3) {
					uint16_t i0 = mesh->m_indices[i];
					uint16_t i1 = mesh->m_indices[i+1];
					uint16_t i2 = mesh->m_indices[i+2];

					double temp_dist;
					double u, v;
					if(ray.intersects(mesh->m_vertices[i0], 
						mesh->m_vertices[i1],
						mesh->m_vertices[i2], &u, &v, &temp_dist) && temp_dist < best_dist)
					{
						best_dist = temp_dist;

						*dist = best_dist;

						result = true;
					}
				}
			}

		}

		return result;
	}
}

bool OctreeOld::propagate_distance(const Ray &ray, double *dist) const
{
	return propagate_distance_internal(ray, root_, dist);
}

void OctreeOld::find_max_depth(const CuboidInternal &current, size_t current_depth, size_t &max_depth) const
{
	if(current_depth > max_depth)
		max_depth = current_depth;

	if(current.children) {
		for(size_t i = 0; i < 8; ++i) {
			find_max_depth(current.children[i], current_depth + 1, max_depth);
		}
	}
}

size_t OctreeOld::depth() const
{
	size_t depth = 0;
	find_max_depth(root_, 1, depth);
	return depth;
}

void OctreeOld::process_internal(const CuboidInternal &cuboid, const AABB &bounds, std::function<void(const Mesh*)> &callback)
{
	if(cuboid.bounds.intersects(bounds)) {
		if(cuboid.children) {
			for(size_t i = 0; i < 8; ++i) {
				process_internal(cuboid.children[i], bounds, callback);
			}
		}
		else {
			for(size_t i = 0; i < cuboid.items.size(); ++i) {
				if(cuboid.items[i]->m_bounds.intersects(bounds)) {
					callback(cuboid.items[i]);
				}
			}
		}
	}
}

void OctreeOld::process(const AABB &bounds, std::function<void(const Mesh*)> &callback)
{
	process_internal(root_, bounds, callback);
}