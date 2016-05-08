#include "model.h"
#include "material.h"
#include "mesh.h"

#include <cfloat>
#include <iostream>
#include <set>

Model::Model(const Matrix &transform, std::vector<std::unique_ptr<Mesh>> &meshes)
	: m_transform(transform), m_transform_is_dirty(true), m_scale(Vector3d(1, 1, 1))
{
	for(auto &mesh : meshes) {	
		m_meshes.push_back(std::move(mesh));
	}
}

Vector3d Model::min_bound() const
{
	Vector3d min(DBL_MAX, DBL_MAX, DBL_MAX);

	for(auto &mesh : m_meshes) {
		for(auto &vertice : mesh->m_vertices) {
			if(vertice.x() < min.x())
				min.x() = vertice.x();
			if(vertice.y() < min.y())
				min.y() = vertice.y();
			if(vertice.z() < min.z())
				min.z() = vertice.z();
		}
	}

	return min;
}
Vector3d Model::max_bound() const
{
	Vector3d max(-DBL_MAX, -DBL_MAX, -DBL_MAX);

	for(auto &mesh : m_meshes) {
		for(auto &vertice : mesh->m_vertices) {
			if(vertice.x() > max.x())
				max.x() = vertice.x();
			if(vertice.y() > max.y())
				max.y() = vertice.y();
			if(vertice.z() > max.z())
				max.z() = vertice.z();
		}
	}

	return max;
}

void Model::set_translation(const Vector3d &translation)
{
	if(translation != m_translation) {
		m_translation = translation;
		m_transform_is_dirty = true;
	}
}

void Model::set_rotation(const Vector3d &rotation)
{
	if(rotation != m_rotation) {
		m_rotation = rotation;
		m_transform_is_dirty = true;
	}
}

void Model::set_scale(const Vector3d &scale)
{
	if(scale != m_scale) {
		m_scale = scale;
		m_transform_is_dirty = true;
	}
}

Vector3d Model::translation() const
{
	return m_translation;
}

Vector3d Model::rotation() const
{
	return m_rotation;
}

Vector3d Model::scale() const
{
	return m_scale;
}

void Model::update_transform(bool force_update)
{
	if(m_transform_is_dirty || force_update) {
		m_transform_is_dirty = false;

		Matrix scale_matrix;
		Matrix rotation_x_matrix;
		Matrix rotation_y_matrix;
		Matrix rotation_z_matrix;
		Matrix translation_matrix;

		Matrix::CreateScale(m_scale.x(), m_scale.y(), m_scale.z(), &scale_matrix);
		Matrix::CreateRotationX(m_rotation.x(), &rotation_x_matrix);
		Matrix::CreateRotationY(m_rotation.y(), &rotation_y_matrix);
		Matrix::CreateRotationZ(m_rotation.z(), &rotation_z_matrix);
		Matrix::CreateTranslation(m_translation, &translation_matrix);

		Matrix rotation_matrix = rotation_x_matrix * rotation_y_matrix * rotation_z_matrix;

		m_transform = scale_matrix * rotation_matrix * translation_matrix;

		for(auto &mesh : m_meshes) {
			mesh->transform_vertices(m_transform, rotation_matrix); // TODO: Hur hanteras skalning med normaler? Är translation det enda som ska ur transformen?
		}
	}
}

void Model::print_memory_usage() const
{
	size_t size = sizeof(Model);
	std::set<Material*> materials;
	size_t materials_size = 0;

	for(auto &mesh : m_meshes) {
		size += mesh->memory_size();
		if(materials.insert(mesh->m_material.get()).second) {
			materials_size += mesh->m_material->memory_size();
		}
	}

	std::cout << "Memory usage: " << size / (1024*1024) << " Mb. Materials: " << materials_size / (1024*1024) << " Mb.\n";
}