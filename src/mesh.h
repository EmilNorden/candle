#ifndef MESH_H_
#define MESH_H_

#include "aabb.h"
#include "vector.h"
#include <vector>
#include <cstdint>
#include <memory>
#include <string>

class Material;
class Matrix;

class Mesh
{
public:
	
	std::vector<Vector3d> m_vertices;
	std::vector<uint16_t> m_indices;
	std::vector<Vector3d> m_normals;
	std::vector<Vector2d> m_texture_coords;
	std::shared_ptr<Material> m_material;
	AABB m_bounds;

	Mesh(const std::string &name, const std::vector<Vector3d> &vertices, const std::vector<uint16_t> &indices, const std::vector<Vector3d> &normals, const std::vector<Vector2d> &texture_coords, const std::shared_ptr<Material> &material);

	void transform_vertices(const Matrix &transform, const Matrix &normal_transform);

	size_t memory_size() const;

	const AABB &bounds() const { return m_bounds; }
private:
	std::vector<Vector3d> m_original_vertices;
	std::vector<Vector3d> m_original_normals;
    std::string m_name;
	void update_bounds();
};

typedef std::unique_ptr<Mesh> MeshPtr;

#endif