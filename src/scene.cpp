#include "scene.h"

#include "material.h"
#include "model.h"
#include "ray.h"
#include <iostream>

Vector3d Scene::propagate(Ray &ray, const Vector3d &default_color, std::mt19937 &random, size_t depth) const
{
	return propagate(ray, default_color, random, depth, nullptr);
}

bool ignore_mesh_triangle(const CollisionInfoInternal *ignore, const std::unique_ptr<Mesh> &mesh, uint16_t i0, uint16_t i1, uint16_t i2)
{
	return ignore != nullptr &&
		(mesh.get() == ignore->mesh && i0 == ignore->index0 && i1 == ignore->index1 && i2 == ignore->index2);
}

CollisionInfoInternal Scene::find_scene_intersection(const Ray &ray, const CollisionInfoInternal * ignore) const
{
	CollisionInfoInternal result;
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

					result = CollisionInfoInternal(mesh.get(), i0, i1, i2, u, v);
				}
			}
		}
	}

	return result;
}

void Scene::global_illumination(Ray &ray, std::mt19937 &random, const std::shared_ptr<Material> &material, const Vector3d &intersection_point, const Vector3d &surface_normal, Vector3d &diffuse, Vector3d &color, size_t depth, const Vector3d &default_color) const
{
	Vector3d random_dir = Vector3d::rand_unit_in_hemisphere(surface_normal, random);
	

	Vector3d reflected_direction = ray.m_direction - surface_normal * 2.0 * ray.m_direction.dot(surface_normal);
	reflected_direction.normalize();

	Vector3d final_dir = Vector3d::lerp(random_dir, reflected_direction, material->reflectivity());
	final_dir.normalize();

	Ray random_ray(intersection_point, final_dir);

	CollisionInfoInternal random_collision;
	if(tree2_.propagate(random_ray, random_collision)) {
		Vector3d viewer = ray.m_direction * -1;
		Vector3d halfway = (random_dir + viewer);
		halfway.normalize();

		Vector3d random_color = shade(random_ray, random, random_collision, depth-1, default_color);
		// TODO: Calculate all these things in CollisionInfoInternal

		/*Vector3d &v1 = random_collision.mesh->m_vertices[random_collision.index1] - random_collision.mesh->m_vertices[random_collision.index0];
		Vector3d &v2 = random_collision.mesh->m_vertices[random_collision.index2] - random_collision.mesh->m_vertices[random_collision.index0];
		Vector3d &random_point = random_collision.mesh->m_vertices[random_collision.index0] + (v1 * random_collision.u) + (v2 * random_collision.v);
		Vector3d dir = random_point - intersection_point;*/

		color += diffuse * random_color;
	}
	
}

Vector3d Scene::shade(Ray &ray, std::mt19937 &random, const CollisionInfoInternal &collision, size_t depth, const Vector3d &default_color) const
{
	if(depth == 0)
		return Vector3d(0, 0, 0);

	const std::shared_ptr<Material> &mat = collision.mesh->m_material;

	const Vector3d &v1 = collision.mesh->m_vertices[collision.index1] - collision.mesh->m_vertices[collision.index0];
	const Vector3d &v2 = collision.mesh->m_vertices[collision.index2] - collision.mesh->m_vertices[collision.index0];
	const Vector3d &intersection_point = collision.mesh->m_vertices[collision.index0] + (v1 * collision.u) + (v2 * collision.v);

	const Vector3d &n1 = collision.mesh->m_normals[collision.index1] - collision.mesh->m_normals[collision.index0];
	const Vector3d &n2 = collision.mesh->m_normals[collision.index2] - collision.mesh->m_normals[collision.index0];
	Vector3d interpolated_normal = collision.mesh->m_normals[collision.index0] + (n1 * collision.u) + (n2 * collision.v);
	interpolated_normal.normalize();

	Vector3d diffuse_color;
	if(mat->has_texture()) {
		const Vector2d &tex1 = collision.mesh->m_texture_coords[collision.index1] - collision.mesh->m_texture_coords[collision.index0];
		const Vector2d &tex2 = collision.mesh->m_texture_coords[collision.index2] - collision.mesh->m_texture_coords[collision.index0];
		const Vector2d &interpolated_uv = collision.mesh->m_texture_coords[collision.index0] + (tex1 * collision.u) + (tex2 * collision.v);

		//diffuse_color = mat->diffuse(interpolated_uv.x(), interpolated_uv.y());
		Vector3d tex_sample;
		mat->sample(interpolated_uv.x(), interpolated_uv.y(), tex_sample);
		diffuse_color = tex_sample;
	}
	else
	{
		diffuse_color = mat->diffuse();
	}

	Vector3d result = mat->emissive();

	global_illumination(ray, random, mat, intersection_point, interpolated_normal, diffuse_color, result, depth, default_color);

	// diffuse
	size_t emissive_mesh_count = emissive_meshes_.size();
	for(size_t i = 0; i < emissive_mesh_count; ++i) {
		const Mesh *mesh = emissive_meshes_[i].mesh;
		double squared_distance = (emissive_meshes_[i].center - intersection_point).length_squared();
		if(/*squared_distance <= emissive_meshes_[i].effective_radius &&*/ mesh != collision.mesh) {
			Ray shadow_ray;
			shadow_ray.m_origin = intersection_point;
			shadow_ray.m_direction = emissive_meshes_[i].center - intersection_point; // TODO: Pre-calculate center of mesh?
			shadow_ray.m_direction.normalize();

			CollisionInfoInternal shadow_collision;
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
	
	//if(mat->reflectivity() > 0) {
	//	result = Vector3d(1, 0, 0);
	//	//Vector3d reflected_direction = ray.m_direction - interpolated_normal * 2.0 * ray.m_direction.dot(interpolated_normal);
	//	//reflected_direction.normalize();
	//	//Ray reflected_ray(intersection_point, reflected_direction);

	//	//result = result*(1-mat->reflectivity()) + propagate(reflected_ray, default_color, random, depth-1, &collision)*mat->reflectivity();
	//}

	/*Vector3d light(1, 1, 0);
	light.normalize();
	double dot = light.dot(interpolated_normal);
	if(dot < 0)
		dot = 0.2;
	result = diffuse_color * dot;*/
	return result;
}

//static bool should_ignore(const Mesh *mesh, uint16_t i0, uint16_t i1, uint16_t i2, const CollisionInfoInternal *ignore)
//{
//	return ignore != nullptr && ignore->mesh == mesh && ignore->index0 == i0 && ignore->index1 == i1 && ignore->index2 == i2;
//}

Vector3d Scene::propagate(Ray &ray, const Vector3d &default_color, std::mt19937 &random, size_t depth, const CollisionInfoInternal *ignore) const
{
	Vector3d result = default_color;

	CollisionInfoInternal collision;
	/*
	for(auto& model : models)
	{
		for(auto &mesh : model->m_meshes)
		{
			if(mesh->m_bounds.intersects2(ray))
			{
				for(size_t i = 0; i < mesh->m_indices.size(); i += 3)
				{
					uint16_t i0 = mesh->m_indices[i];
					uint16_t i1 = mesh->m_indices[i+1];
					uint16_t i2 = mesh->m_indices[i+2];
					double temp_dist;
					double u, v;
					if(ray.intersects(mesh->m_vertices[i0], 
						mesh->m_vertices[i1],
						mesh->m_vertices[i2], &u, &v, &temp_dist))
					{
						result = Vector3d(1, 0, 0);
					}
				}
			}
		}
	}
	*/
	if(tree2_.propagate(ray, collision)) {
		//result = Vector3d(1, 0, 0);
		result = shade(ray, random, collision, depth, default_color);
	}
	
#ifdef DEBUG_BOUNDINGBOXES
	Vector3d box_col(1);
	for(auto &model : models) {
		for(auto &mesh : model->m_meshes) {
			//if(mesh->m_bounds.intersects(ray)) {
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

	tree_.build(models, 5, 5);

	std::vector<Mesh*> meshes;
	for(auto &model : models)
		for(auto &mesh : model->m_meshes)
			meshes.push_back(mesh.get());

	tree2_.build(5, 5, meshes.begin(), meshes.end());
}

double Scene::measure_distance(Ray &ray)
{
	double dist = DBL_MAX;
	CollisionInfoInternal collision;
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

EmissiveMesh::EmissiveMesh(Mesh *m)
	: mesh(m)
{
	double strength = m->m_material->emissive().length_squared();
	effective_radius = strength * 100;
	center = (m->bounds().min() + m->bounds().max()) / 2.0;
}