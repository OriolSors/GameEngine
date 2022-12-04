#pragma once
#include <vector>
#include <assimp/scene.h>
#include "Mesh.h"
#include "Material.h"
class Model
{
public:
	Model();
	~Model();

	void Load(const char* file_name);
	void LoadMaterials(const aiScene* scene, aiString& file);
	void LoadMeshes(const aiScene* scene, aiString& file);

private:
	std::vector<Mesh*> mesh;
	std::vector<unsigned int> materials;
};

