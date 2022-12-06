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
	void Draw();
	void Clear();

	std::string GetDirectory(const std::string& fname);
	std::string GetFilename(const std::string& fname);

private:
	std::vector<Mesh*> meshes;
	std::vector<GLuint> materials;

	const char* filePath;
};

