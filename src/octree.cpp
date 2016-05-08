#include "octree.h"

#include "aabb.h"
#include "model.h"
#include "mesh.h"
#include "ray.h"
/*

Octree::CuboidInternal::CuboidInternal()
	: bounds(AABB(Vector3d(DBL_MAX), Vector3d(-DBL_MAX)))
{
}


void Octree::build(const std::vector<std::shared_ptr<Model>> &items, size_t limit)
{
	root_.bounds = AABB(Vector3d(DBL_MAX), Vector3d(-DBL_MAX));

	for(const auto &model : items) {
		for(const auto &mesh : model->meshes_) {
			root_.bounds.expand(mesh->bounds_);
			root_.items.push_back(mesh.get());
		}
	}


}

void Octree::build_internal(CuboidInternal &current, size_t limit)
{
	if(current.items.size() < limit)
		return;

	Vector3d child_size = (current.bounds.max() - current.bounds.min()) / 2.0;

	current.children.reset(new CuboidInternal[8]);
	for(int z = 0; z < 2; ++z) {
		for(int y = 0; y < 2; ++y) {
			for(int x = 0; x < 2; ++x) {
				Vector3d min = current.bounds.min() + (child_size * Vector3d(x , y, z));
				Vector3d max = min + child_size;
				current.children[(z * 4) + (y * 2) + x].bounds = AABB(min, max);
			}
		}
	}

	for(int i = 0; i < 8; ++i) {
		for(auto &item : current.items) {
			if(item->bounds_.intersects(current.children[i].bounds)) {
				current.children[i].items.push_back(item);
			}
		}

		build_internal(current.children[i], limit);
	}

	current.items.clear();
}

bool Octree::propagate(const Ray &ray, CollisionInfoInternal &collision) const
{
	return propagate_internal(ray, root_, collision);
}

bool Octree::propagate_internal(const Ray &ray, const CuboidInternal &current, CollisionInfoInternal &collision) const
{
	if(!current.bounds.intersects(ray))
		return false;

	if(current.children) {
		for(int i = 0; i < 8; ++i) {
			if(propagate_internal(ray, current.children[i], collision))
				return true;
		}
	}
	else {
		double best_dist = DBL_MAX;
		bool result = false;

		for(auto &mesh : current.items) {

			if(mesh->bounds_.intersects(ray)) {
		
				size_t index_count = mesh->indices_.size();
				for(size_t i = 0; i < index_count; i += 3) {
					uint16_t i0 = mesh->indices_[i];
					uint16_t i1 = mesh->indices_[i+1];
					uint16_t i2 = mesh->indices_[i+2];

					//if(ignore_mesh_triangle(ignore, mesh, i0, i1, i2))
					//	continue;

					double dist;
					double u, v;
					if(ray.intersects(mesh->vertices_[i0], 
						mesh->vertices_[i1],
						mesh->vertices_[i2], &u, &v, &dist) && dist < best_dist)
					{
						best_dist = dist;

						collision.mesh = mesh;
						collision.index0 = i0;
						collision.index1 = i1;
						collision.index2 = i2;
						collision.u = u;
						collision.v = v;

						result = true;
					}
				}
			}

		}

		return result;
	}

	return false;
}

*/