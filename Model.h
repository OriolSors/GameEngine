#pragma once
#include <vector>
#include <assimp/scene.h>
#include "Mesh.h"
#include "GL\glew.h"
class Model
{
public:
	Model();
	~Model();

	void Load(const char* file_name);
	void LoadMaterials(const aiScene* scene);
	void LoadMeshes(const aiScene* scene);

private:
	std::vector<Mesh*> mesh;
	std::vector<GLuint> materials;
};

