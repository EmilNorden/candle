#include "octreescene.h"
#include "model.h"
#include <iostream>

using namespace std;

void OctreeScene::add_model(const std::shared_ptr<Model> &model)
{
    m_models.push_back(model);
}

void OctreeScene::build_scene()
{
	apply_all_transforms();
	find_emissive_meshes();
	
    std::vector<Mesh*> meshes;
	for(auto &model : m_models)
		for(auto &mesh : model->m_meshes)
			meshes.push_back(mesh.get());
    
    m_tree.build(5, 1, meshes.begin(), meshes.end());
}

bool OctreeScene::propagate(Ray &ray, RayMeshIntersection &intersection, const RayMeshIntersection *ignore) const
{   
    return m_tree.propagate(ray, intersection, ignore);
}

void OctreeScene::find_emissive_meshes()
{
	for(const auto &model : m_models) {
		for(const auto &mesh : model->m_meshes) {
			if(mesh->m_material && mesh->m_material->is_emissive()) {
				m_emissive_meshes.push_back(EmissiveMesh(mesh.get()));
			}
		}
	}
}

void OctreeScene::apply_all_transforms() const
{
	for(auto &model : m_models) {
		model->update_transform();
	}
}

void OctreeScene::print_statistics() const
{
   cout << "Scene size: " << m_models.size() << " models.\n";
   cout << "Octree depth: " << m_tree.depth() << "\n";
}