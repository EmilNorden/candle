#ifndef MODEL_H_
#define MODEL_H_

#include "matrix.h"
#include "vector.h"

#include <memory>
#include <vector>

class Mesh;
class Material;

class Model
{
public:
	std::vector<std::unique_ptr<Mesh>> m_meshes;

	Model(const Matrix &transform, std::vector<std::unique_ptr<Mesh>> &meshes);

	Vector3d min_bound() const;
	Vector3d max_bound() const;

	void set_translation(const Vector3d &translation);
	void set_rotation(const Vector3d &rotation);
	void set_scale(const Vector3d &scale);

	Vector3d translation() const;
	Vector3d rotation() const;
	Vector3d scale() const;

	void update_transform(bool force_update = false);

	void print_memory_usage() const;
	
protected:
	Matrix m_transform;
	bool m_transform_is_dirty;
	Vector3d m_translation;
	Vector3d m_rotation;
	Vector3d m_scale;
};

#endif