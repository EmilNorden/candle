#include "modelloader.h"

#include "model.h"
#include "modelprocessor.h"
#include <boost/filesystem.hpp>
#include <stdexcept>
#include <iostream>

using namespace std;

void ModelLoader::register_processor(std::unique_ptr<ModelProcessor> &processor)
{
	m_processors.push_back(std::move(processor));
}

std::shared_ptr<Model> ModelLoader::load(const std::string &path, bool allow_cached_materials, size_t vertex_limit) const
{
	boost::filesystem::path model_path(path);
	std::string extension = model_path.extension().string();

	for(const auto &processor : m_processors) {
		for(const auto &handled_ext : processor->handled_extensions()) {
			if(handled_ext == extension) {
				auto model =  processor->load(path, vertex_limit, allow_cached_materials);
				const Vector3d min_bound = model->min_bound();
				const Vector3d max_bound = model->max_bound();
				cout << "Model bounds: [" << min_bound.x() << ", " << min_bound.y() << ", " << min_bound.z() << "] - [" << max_bound.x() << ", " << max_bound.y() << ", " << max_bound.z() << "]\n";
				return model;
			}
		}
	}

	throw runtime_error("No ModelProcessor exists for the given file.");
}