/*#in
clude "scene.h"

#include "material.h"
#include "model.h"
#include "ray.h"
#include "ray_mesh_intersection.h"
#include <iostream>

Color Scene::propagate(Ray &ray, const Color &default_color, std::mt19937 &random, size_t depth) const
{
	return propagate(ray, default_color, random, depth, nullptr);
}

bool ignore_mesh_triangle(const RayMeshIntersection *ignore, const std::unique_ptr<Mesh> &mesh, uint16_t i0, uint16_t i1, uint16_t i2)
{
	return ignore != nullptr &&
		(mesh.get() == ignore->mesh && i0 == ignore->index0 && i1 == ignore->index1 && i2 == ignore->index2);
}

RayMeshIntersection Scene::find_scene_intersection(const Ray &ray, const RayMeshIntersection * ignore) const
{
	RayMeshIntersection result;
	double best_dist = DBL_MAX;

	for(auto &model : models)
	{
		for(auto &mesh : model->m_meshes)
		{
			for(size_t i = 0; i < mesh->m_indices.size(); i += 3)
			{
				uint16_t i0 = mesh->m_indices.at(i);
				uint16_t i1 = mesh->m_indices.at(i+1);
				uint16_t i2 = mesh->m_indices.at(i+2);

				if(ignore_mesh_triangle(ignore, mesh, i0, i1, i2))
					continue;

				double dist;
				double u, v;
				if(ray.intersects(mesh->m_vertices[i0], 
					mesh->m_vertices[i1],
					mesh->m_vertices[i2], &u, &v, &dist) && dist < best_dist)
				{
					best_dist = dist;

					result = RayMeshIntersection(mesh.get(), i0, i1, i2, u, v);
				}
			}
		}
	}

	return result;
}

void Scene::global_illumination(Ray &ray, std::mt19937 &random, const std::shared_ptr<Material> &material, const Vector3d &intersection_point, const Vector3d &surface_normal, Color &diffuse, Color &color, size_t depth, const Color &default_color) const
{
	Vector3d random_dir = Vector3d::rand_unit_in_hemisphere(surface_normal, random);
	

	Vector3d reflected_direction = ray.m_direction - surface_normal * 2.0 * ray.m_direction.dot(surface_normal);
	reflected_direction.normalize();

	Vector3d final_dir = Vector3d::lerp(random_dir, reflected_direction, material->reflectivity());
	final_dir.normalize();

	Ray random_ray(intersection_point, final_dir);

	RayMeshIntersection random_collision;
	if(tree2_.propagate(random_ray, random_collision)) {
		Vector3d viewer = ray.m_direction * -1;
		Vector3d halfway = (random_dir + viewer);
		halfway.normalize();

		Color random_color = shade(random_ray, random, random_collision, depth-1, default_color);
		color += diffuse * random_color;
	}
	
}

Color Scene::shade(Ray &ray, std::mt19937 &random, const RayMeshIntersection &collision, size_t depth, const Color &default_color) const
{
	if(depth == 0)
		return Color(0, 0, 0);

	const std::shared_ptr<Material> &mat = collision.mesh->m_material;

	const Vector3d &v1 = collision.mesh->m_vertices[collision.index1] - collision.mesh->m_vertices[collision.index0];
	const Vector3d &v2 = collision.mesh->m_vertices[collision.index2] - collision.mesh->m_vertices[collision.index0];
	const Vector3d &intersection_point = collision.mesh->m_vertices[collision.index0] + (v1 * collision.u) + (v2 * collision.v);

	const Vector3d &n1 = collision.mesh->m_normals[collision.index1] - collision.mesh->m_normals[collision.index0];
	const Vector3d &n2 = collision.mesh->m_normals[collision.index2] - collision.mesh->m_normals[collision.index0];
	Vector3d interpolated_normal = collision.mesh->m_normals[collision.index0] + (n1 * collision.u) + (n2 * collision.v);
	interpolated_normal.normalize();

	Color diffuse_color;
	if(mat->has_texture()) {
		const Vector2d &tex1 = collision.mesh->m_texture_coords[collision.index1] - collision.mesh->m_texture_coords[collision.index0];
		const Vector2d &tex2 = collision.mesh->m_texture_coords[collision.index2] - collision.mesh->m_texture_coords[collision.index0];
		const Vector2d &interpolated_uv = collision.mesh->m_texture_coords[collision.index0] + (tex1 * collision.u) + (tex2 * collision.v);

		Color tex_sample;
		mat->sample(interpolated_uv.x(), interpolated_uv.y(), tex_sample);
		diffuse_color = tex_sample;
	}
	else
	{
		diffuse_color = mat->diffuse();
	}

	Color result = mat->emissive();

	global_illumination(ray, random, mat, intersection_point, interpolated_normal, diffuse_color, result, depth, default_color);

	size_t emissive_mesh_count = emissive_meshes_.size();
	for(size_t i = 0; i < emissive_mesh_count; ++i) {
		const Mesh *mesh = emissive_meshes_[i].mesh;
		double squared_distance = (emissive_meshes_[i].center - intersection_point).length_squared();
		if( mesh != collision.mesh) {
			Ray shadow_ray;
			shadow_ray.m_origin = intersection_point;
			shadow_ray.m_direction = emissive_meshes_[i].center - intersection_point; // TODO: Pre-calculate center of mesh?
			shadow_ray.m_direction.normalize();

			RayMeshIntersection shadow_collision;
			if(tree2_.propagate(shadow_ray, shadow_collision, &collision) && shadow_collision.mesh == mesh) {
				// No obstruction between our mesh and the emissive mesh
				double inv_square_length = 1.0 / squared_distance;
				double dot = shadow_ray.m_direction.dot(interpolated_normal);
				if(dot < 0)
					dot = 0.0;

				result += mesh->m_material->emissive() * diffuse_color * dot * inv_square_length;
			}
		}
}

	return result;
}

//static bool should_ignore(const Mesh *mesh, uint16_t i0, uint16_t i1, uint16_t i2, const RayMeshIntersection *ignore)
//{
//	return ignore != nullptr && ignore->mesh == mesh && ignore->index0 == i0 && ignore->index1 == i1 && ignore->index2 == i2;
//}

Color Scene::propagate(Ray &ray, const Color &default_color, std::mt19937 &random, size_t depth, const RayMeshIntersection *ignore) const
{
	Color result = default_color;

	RayMeshIntersection collision;
	
	if(tree2_.propagate(ray, collision)) {
		result = shade(ray, random, collision, depth, default_color);
	}
	
#ifdef DEBUG_BOUNDINGBOXES
	Color box_col(1);
	for(auto &model : models) {
		for(auto &mesh : model->m_meshes) {
				double size = 20;
			if(AABB(Vector3d(-size, -size, -size), Vector3d(size, size, size)).intersects(ray)) {
				result = (result * 0.8) + (box_col * 0.2);
			}
		}
	}
#endif

	return result;
}

void Scene::build_scene()
{
	validate_no_duplicate_models();
	apply_all_transforms();
	find_emissive_meshes();

	std::vector<Mesh*> meshes;
	for(auto &model : models)
		for(auto &mesh : model->m_meshes)
			meshes.push_back(mesh.get());

	tree2_.build(5, 5, meshes.begin(), meshes.end());
}

double Scene::measure_distance(Ray &ray)
{
	double dist = DBL_MAX;
	RayMeshIntersection collision;
	if(tree2_.propagate(ray, collision))
		dist = ray.dist;
	//tree_.propagate_distance(ray, &dist);

	return dist;
}

void Scene::apply_all_transforms() const
{
	for(auto &model : models) {
		model->update_transform();
	}
}

void Scene::validate_no_duplicate_models() const
{
	size_t model_count = models.size();
	for(size_t i = 0; i < model_count-1; ++i) {
		for(size_t j = i+1; j < model_count; ++j) {
			if(models[i].get() == models[j].get())
				throw std::runtime_error("Scene contains multiple identical models.");
		}
	}
}

size_t Scene::tree_depth() const
{
	return tree2_.depth();
}

void Scene::find_emissive_meshes()
{
	for(const auto &model : models) {
		for(const auto &mesh : model->m_meshes) {
			if(mesh->m_material && mesh->m_material->is_emissive()) {
				emissive_meshes_.push_back(EmissiveMesh(mesh.get()));
			}
		}
	}
}

*/