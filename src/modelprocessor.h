#ifndef MESH_LOADER_H_
#define MESH_LOADER_H_

#include "weak_cache.h"

#include <map>
#include <memory>
#include <string>
#include <vector>

class Material;
class Mesh;
class Model;
struct aiMaterial;
struct aiMesh;

class ModelProcessor
{
public:
	virtual std::shared_ptr<Model> load(const std::string &path, size_t vertex_limit, bool allow_cached_materials = true) = 0;
	virtual const std::vector<std::string> handled_extensions() const = 0;
protected:
	WeakCache<Material> m_material_cache;
//private:
//	std::shared_ptr<Material> load_single_material(size_t index, aiMaterial *material, const std::string &model_path) const;
//	std::unique_ptr<Mesh> load_single_mesh(aiMesh *mesh, const std::map<size_t, std::shared_ptr<Material>> &materials) const;
};

#endif