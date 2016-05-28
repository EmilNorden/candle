#include "assimp_processor.h"


#include <cstdint>
#include <iostream>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <boost/filesystem.hpp>
#include <FreeImage.h>
#include "material.h"
#include "mesh.h"
#include "model.h"
#include "output_util.h"
#include "vector.h"
#include "texture.h"

using namespace std;

size_t total_vertices(const vector<unique_ptr<Mesh>> &meshes)
{
	size_t total = 0;
	for(const auto &mesh : meshes) {
		total += mesh->m_vertices.size();
	}

	return total;
}

void print_model_metrics(const shared_ptr<Model> &model)
{
	size_t vertex_count = total_vertices(model->m_meshes);
	cout << "Total vertices: " << vertex_count << ". Total meshes: " << model->m_meshes.size() << ". ";
	model->print_memory_usage();
}

shared_ptr<Model> AssimpModelProcessor::load(const string &path, size_t vertex_limit, bool allow_cached_materials)
{
	cout << "=== Loading model: '" << path << "' === \n";

	Assimp::Importer importer;

	importer.SetPropertyInteger(AI_CONFIG_PP_SLM_VERTEX_LIMIT, vertex_limit);

	const aiScene *scene = importer.ReadFile(path,
		aiProcess_CalcTangentSpace			|
		aiProcess_Triangulate				|
		aiProcess_GenSmoothNormals			|
		aiProcess_SplitLargeMeshes			|
		aiProcess_ImproveCacheLocality		|
		aiProcess_RemoveRedundantMaterials	|
		aiProcess_OptimizeMeshes			|
		aiProcess_FixInfacingNormals		|
		aiProcess_FindInvalidData			|
		//aiProcess_OptimizeGraph				|
		aiProcess_JoinIdenticalVertices		|
		aiProcess_SortByPType);

	if(!scene) {
		cerr << "Failed to load model!\n" << importer.GetErrorString() << "\n";
		return nullptr;
	}

	cout << "Loading materials...";
	map<size_t, shared_ptr<Material>> materials;
	for(size_t i = 0; i < scene->mNumMaterials; ++i)
	{
		//display_progress((i+1) / (float)scene->mNumMaterials);
		const shared_ptr<Material> &material = load_single_material(i, scene->mMaterials[i], path, allow_cached_materials);
		if(material)
			materials.emplace(material->index(), material);
	}
	cout << materials.size() << " loaded.\n";

	cout << "Loading meshes...";
	vector<unique_ptr<Mesh>> meshes;
	for(size_t i = 0; i < scene->mNumMeshes; ++i)
	{
		//display_progress((i+1) / (float)scene->mNumMeshes);
		meshes.push_back(load_single_mesh(scene->mMeshes[i], materials));
	}
	cout << meshes.size() << " loaded.\n";
	
	cout << "Done.\n\n";
	//
	
	Matrix transform;
	transform.make_identity();
	auto model = make_shared<Model>(transform, meshes);
	print_model_metrics(model);

	return model;
}

Texture* AssimpModelProcessor::load_texture(const aiString &path, const string &modelPath)
{
	boost::filesystem::path texturePath(path.C_Str());

	if(texturePath.is_relative()) {
		boost::filesystem::path modelDirectory(modelPath);
		modelDirectory = modelDirectory.parent_path();

		texturePath = boost::filesystem::canonical(texturePath, modelDirectory);
	}

	FREE_IMAGE_FORMAT type = FreeImage_GetFileType(texturePath.string().c_str());
	if(type == FIF_UNKNOWN) {
		type = FreeImage_GetFIFFromFilename(texturePath.string().c_str());

		if(type == FIF_UNKNOWN)
			throw runtime_error("Unable to determine texture format.");
	}

	FIBITMAP *bitmap = FreeImage_Load(type, texturePath.string().c_str());
	Texture *texture = new Texture(bitmap);
	FreeImage_Unload(bitmap);
	
	return texture;
}

shared_ptr<Material> AssimpModelProcessor::load_single_material(size_t index, aiMaterial *material, const string &model_path, bool allow_cached_materials)
{
	aiString name; 
	aiGetMaterialString(material, AI_MATKEY_NAME, &name);

	string cache_key = model_path + "_" + string(name.C_Str());

	if(allow_cached_materials) {

		auto cached_material = m_material_cache.get(cache_key);
		if(cached_material)
			return cached_material;

	}

	// material->Get does NOT return colors as their documentation states. aiGetMaterialColor does however. Perhaps the same problem exists with Textures?

	aiColor4D fooo;
	aiGetMaterialColor(material,AI_MATKEY_COLOR_DIFFUSE, &fooo);

	float refracti;
	aiGetMaterialFloat(material, AI_MATKEY_REFRACTI, &refracti);
	//material->Get<float>(AI_MATKEY_REFRACTI, refracti);

	float opacity;
	aiGetMaterialFloat(material, AI_MATKEY_OPACITY, &opacity);
	//material->Get<float>(AI_MATKEY_OPACITY, opacity);

	float shininess;
	aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &shininess);
	//material->Get<float>(AI_MATKEY_SHININESS, shininess);

	int shading_model;
	aiGetMaterialInteger(material, AI_MATKEY_SHADING_MODEL, &shading_model);
	//material->Get<int>(AI_MATKEY_SHADING_MODEL, shading_model);

	aiColor4D ambient_color;
	aiGetMaterialColor(material,AI_MATKEY_COLOR_AMBIENT, &ambient_color);

	aiColor4D diffuse_color;
	aiGetMaterialColor(material,AI_MATKEY_COLOR_DIFFUSE, &diffuse_color);

	aiColor4D emissive_color;
	aiGetMaterialColor(material,AI_MATKEY_COLOR_EMISSIVE, &emissive_color);

	aiColor4D specular_color;
	aiGetMaterialColor(material,AI_MATKEY_COLOR_SPECULAR, &specular_color);

	aiString p;
	unsigned int texCount = 0;
	for(int i = 0; i < aiTextureType_UNKNOWN; ++i) {
		texCount += material->GetTextureCount((aiTextureType)i);
	}
	
	Texture* diffuseTexture = nullptr;
	if(material->GetTexture(aiTextureType_DIFFUSE, 0, &p) == AI_SUCCESS) {
		diffuseTexture = load_texture(p, model_path);
		
	}
	
	Texture* normalTexture = nullptr;
	if(material->GetTexture(aiTextureType_NORMALS, 0, &p) == AI_SUCCESS) {
		normalTexture = load_texture(p, model_path);
	}

	//return shared_ptr<Material>(nullptr);
	auto mat = make_shared<Material>(index,
		Color(diffuse_color.r, diffuse_color.g, diffuse_color.b),
		Color(emissive_color.r, emissive_color.g, emissive_color.b),
		diffuseTexture, normalTexture);

	m_material_cache.insert(cache_key, mat);

	return mat;
}

unique_ptr<Mesh> AssimpModelProcessor::load_single_mesh(aiMesh *mesh, const map<size_t, shared_ptr<Material>> &materials) const
{
	vector<Vector3f> vertices;
	vector<uint16_t> indices;
	vector<Vector3f> normals;
	vector<Vector3f> tangents;
	vector<Vector3f> bitangents;
	vector<Vector2f> texture_coords;

	for(size_t i = 0; i < mesh->mNumVertices; ++i)
	{
		aiVector3D &vertice = mesh->mVertices[i];
		vertices.push_back(Vector3f(vertice.x, vertice.y, vertice.z));

		aiVector3D &normal = mesh->mNormals[i];
		normals.push_back(Vector3f(normal.x, normal.y, normal.z));
		
		if(mesh->HasTangentsAndBitangents()) {
			
			aiVector3D &tangent = mesh->mTangents[i];
			tangents.push_back(Vector3f(tangent.x, tangent.y, tangent.z));
		
			aiVector3D &bitangent = mesh->mBitangents[i];
			bitangents.push_back(Vector3f(bitangent.x, bitangent.y, bitangent.z));
			
		}
		if(mesh->HasTextureCoords(0)) {
			aiVector3D &texture_coord = mesh->mTextureCoords[0][i];
			texture_coords.push_back(Vector2f(texture_coord.x, texture_coord.y));
		}
	}

	for(size_t i = 0; i < mesh->mNumFaces; ++i)
	{
		indices.push_back(mesh->mFaces[i].mIndices[0]);
		indices.push_back(mesh->mFaces[i].mIndices[1]);
		indices.push_back(mesh->mFaces[i].mIndices[2]);
	}

	auto iterator = materials.find(mesh->mMaterialIndex);

	shared_ptr<Material> material(nullptr);

	if(iterator != materials.end())	
		material = iterator->second;
		
	return unique_ptr<Mesh>(new Mesh(mesh->mName.C_Str(), vertices, indices, normals, tangents, 
		bitangents, texture_coords, material));
}

const vector<string> AssimpModelProcessor::handled_extensions() const
{
	string ext[] = {".dae", ".blend", ".3ds", ".ase", ".obj", ".ifc", ".xgl",
		".zgl", ".ply", ".dxf", ".lwo", ".lws", ".lxo", ".stl", ".x", ".ac", ".ms3d", ".cob", ".scn",
	".bvh", ".csm", ".xml", ".irrmesh", ".irr", ".mdl", ".md2", ".md3", ".pk3", ".mdc", ".md5", ".smd",
	".vta", ".m3", ".3d", ".b3d", ".q3d", ".q3s", ".nff", ".off", ".raw", ".ter", ".hmp", ".ndo", ".fbx" };

	return vector<string>(begin(ext), end(ext));
}