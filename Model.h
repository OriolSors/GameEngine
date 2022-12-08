#pragma once
#include <vector>
#include <assimp/scene.h>
#include "Mesh.h"
#include "GL\glew.h"
#include "Game/MathGeoLib/Geometry/AABB.h"
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

	float3 GetCentreAABB();
	float GetDiagonalAABB();

	const std::vector<Mesh*>& GetMeshes() { return meshes; }
	const std::vector<GLuint>& GetMaterials() { return materials; }
	const std::vector<int>& GetWidthMaterials() { return materialsWidth; }
	const std::vector<int>& GetHeightMaterials() { return materialsHeight; }

private:
	std::vector<Mesh*> meshes;
	std::vector<GLuint> materials;
	std::vector<int> materialsWidth;
	std::vector<int> materialsHeight;

	AABB boundingBox;

	const char* filePath;
};

