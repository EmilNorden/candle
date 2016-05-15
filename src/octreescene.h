#ifndef OCTREESCENE_H_
#define OCTREESCENE_H_

#include "octree.h"
#include "emissive_mesh.h"
#include <memory>
#include <vector>

class Mesh;
class Model;
class Ray;
class RayMeshIntersection;

class OctreeScene
{
public:
	bool propagate(Ray &ray, RayMeshIntersection &intersection, const RayMeshIntersection *ignore = nullptr) const;
    void add_model(const std::shared_ptr<Model> &model);
    void build_scene();
    const std::vector<EmissiveMesh> &emissive_meshes() const { return m_emissive_meshes; }
    void print_statistics() const;
private:
    void find_emissive_meshes();
    void apply_all_transforms() const;

    std::vector<std::shared_ptr<Model>> m_models;
    std::vector<EmissiveMesh> m_emissive_meshes;
    Octree<Mesh*> m_tree;
};

#endif