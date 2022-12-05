#define NOMINMAX
#include "Model.h"
#include "Application.h"
#include "ModuleTexture.h"
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include "Globals.h"

Model::Model()
{
}

Model::~Model()
{
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
	
}
