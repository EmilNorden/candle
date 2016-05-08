#include "mesh.h"
#include <cfloat>
#include "matrix.h"
#include "material.h"

Mesh::Mesh(const std::string &name, const std::vector<Vector3d> &vertices, const std::vector<uint16_t> &indices, const std::vector<Vector3d> &normals, const std::vector<Vector2d> &texture_coords, const std::shared_ptr<Material> &material)
	: m_name(name), m_original_vertices(vertices), m_vertices(vertices), m_indices(indices), m_original_normals(normals), m_normals(normals), m_texture_coords(texture_coords), m_material(material), m_bounds(AABB(Vector3d(DBL_MAX), Vector3d(-DBL_MAX)))
{
	update_bounds();
}

void Mesh::transform_vertices(const Matrix &transform, const Matrix &normal_transform)
{
	for(size_t i = 0; i < m_vertices.size(); ++i) {
		Matrix::transform(m_original_vertices[i], transform, m_vertices[i]);
	}

	for(size_t i = 0; i < m_normals.size(); ++i) {
		Matrix::transform(m_original_normals[i], normal_transform, m_normals[i]);
		m_normals[i].normalize();
	}

	update_bounds();
}

void Mesh::update_bounds()
{
	m_bounds = AABB(Vector3d(DBL_MAX), Vector3d(-DBL_MAX));

	for(size_t i = 0; i < m_vertices.size(); ++i) {
		m_bounds.expand(m_vertices[i]);
	}
}

size_t Mesh::memory_size() const
{
	size_t size = sizeof(Mesh);
	size += sizeof(Vector3d) * (m_original_vertices.size() + m_vertices.size());
	size += sizeof(uint16_t) * m_vertices.size();
	size += sizeof(Vector3d) * m_normals.size();
	size += sizeof(Vector2d) * m_texture_coords.size();

	return size;
}