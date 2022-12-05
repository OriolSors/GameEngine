#pragma once
#include <assimp/mesh.h>
#include <vector>
class Mesh
{
public:
	Mesh();
	~Mesh();

	void LoadVBO(const aiMesh* mesh);
	void LoadEBO(const aiMesh* mesh);
	void CreateVAO();
	void Draw(const std::vector<unsigned>& model_textures);

private:
	unsigned vbo;
	unsigned ebo;
	unsigned vao;
	unsigned material_index;
	unsigned num_vertices;
	unsigned num_indices;
};

