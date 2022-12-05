#include "Model.h"
#include "Application.h"
#include "ModuleTexture.h"
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include "Globals.h"
#include <vector>

Model::Model()
{
}

Model::~Model()
{
	meshes.clear();
}

void Model::Load(const char* file_name)
{
	
	const aiScene* scene = aiImportFile(file_name, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene)
	{
		LoadMaterials(scene);
		LoadMeshes(scene);
	}
	else
	{
		ENGINE_LOG("Error loading %s: %s", file_name, aiGetErrorString());
	}
}

void Model::LoadMaterials(const aiScene* scene)
{
	aiString file;
	materials.reserve(scene->mNumMaterials);
	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{
		if (scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &file) == AI_SUCCESS)
		{
			App->texture->Load(file.data);
			materials.push_back(App->texture->texture_object);
		}
	}
	
}

void Model::LoadMeshes(const aiScene* scene)
{
	meshes.reserve(scene->mNumMeshes);
	for (unsigned i = 0; i < scene->mNumMeshes; ++i)
	{
		aiMesh* mMesh = scene->mMeshes[i];

		Mesh* mesh = new Mesh();
		mesh->SetMaterialIndex(mMesh->mMaterialIndex);

		mesh->LoadVBO(mMesh);
		mesh->LoadEBO(mMesh);
		mesh->CreateVAO();
		meshes.push_back(mesh);
	}
}

void Model::Draw() {

	std::vector<Mesh*>::iterator iter, end;
	for (iter = meshes.begin(), end = meshes.end(); iter != end; ++iter) {
		(*iter)->Draw(materials);
	}
}
