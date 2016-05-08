#ifndef MODEL_LOADER_H_
#define MODEL_LOADER_H_

#include <string>
#include <memory>
#include <vector>

class Model;
class ModelProcessor;

class ModelLoader
{
public:
	void register_processor(std::unique_ptr<ModelProcessor> &processor);
	std::shared_ptr<Model> load(const std::string &path, bool allow_cached_materials = true, size_t vertex_limit = 1000000) const;
private:
	std::vector<std::unique_ptr<ModelProcessor>> m_processors;
};


#endif