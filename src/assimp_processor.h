#ifndef ASSIMP_LOADER_H_
#define ASSIMP_LOADER_H_

#include <memory>
#include "modelprocessor.h"

class Model;
class Texture;
struct aiMaterial;
struct aiMesh;
struct aiString;

class AssimpModelProcessor : public ModelProcessor
{
public:
	virtual std::shared_ptr<Model> load(const std::string &path, size_t vertex_limit, bool allow_cached_materials = true);
	virtual const std::vector<std::string> handled_extensions() const;
private:
	Texture *load_texture(const aiString &path, const std::string &modelPath);
	std::shared_ptr<Material> load_single_material(size_t index, aiMaterial *material, const std::string &model_path, bool allow_cached_materials);
	std::unique_ptr<Mesh> load_single_mesh(aiMesh *mesh, const std::map<size_t, std::shared_ptr<Material>> &materials) const;
};


#endif