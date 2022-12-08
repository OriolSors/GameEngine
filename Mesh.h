#pragma once
#include <assimp/mesh.h>
#include <vector>
#include "GL\glew.h"
class Mesh
{
public:
	Mesh();
	~Mesh();

	void LoadVBO(const aiMesh* mesh);
	void LoadEBO(const aiMesh* mesh);
	void CreateVAO();
	void Draw(const std::vector<GLuint>& model_textures);

	void SetMaterialIndex(GLuint material_index);
	void SetNumVertices(unsigned num_vertices);
	void SetNumIndices(unsigned num_indices);

	unsigned GetMaterialIndex() { return material_index; }
	unsigned GetNumVertices() { return num_vertices; }
	unsigned GetNumIndices() { return num_indices; }

private:
	unsigned vbo;
	unsigned ebo;
	unsigned vao;
	GLuint material_index;
	unsigned num_vertices;
	unsigned num_indices;
};

