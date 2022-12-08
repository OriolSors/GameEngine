#include "Model.h"
#include "Application.h"
#include "ModuleTexture.h"
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include "Globals.h"
#include <vector>
#include <string>

Model::Model()
{
}

Model::~Model()
{
	meshes.clear();
}

void Model::Load(const char* file_name)
{
	ENGINE_LOG("Loading scene FBX: %s", file_name);
	const aiScene* scene = aiImportFile(file_name, aiProcessPreset_TargetRealtime_MaxQuality);
	filePath = file_name;
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

	std::string directory = GetDirectory(filePath);
	std::string textureDir("Assets/Textures/"); 

	materials.reserve(scene->mNumMaterials);
	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{
		if (scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &file) == AI_SUCCESS)
		{
			ENGINE_LOG("Default path for FBX texture: %s", file.data);

			std::string fileName = GetFilename(file.data); 
			ENGINE_LOG("Filename for FBX texture: %s", fileName.c_str());

			directory.append(fileName);
			textureDir.append(fileName);
			
			if (App->texture->Load(file.data)) 
			{ 
				ENGINE_LOG("Loading texture in path: %s", file.data);
				materials.push_back(App->texture->texture_object);
			}
			else if (App->texture->Load(directory.c_str())) 
			{ 
				ENGINE_LOG("Loading texture in path: %s", directory.c_str());
				materials.push_back(App->texture->texture_object);
			}
			else if (App->texture->Load(textureDir.c_str()))
			{ 
				ENGINE_LOG("Loading texture in path: %s", textureDir.c_str());
				materials.push_back(App->texture->texture_object); 
			}
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

		boundingBox.SetNegativeInfinity();

		vec* vertices = new vec[mMesh->mNumVertices];

		for (unsigned j = 0; j < mMesh->mNumVertices; ++j) {
			float x = mMesh->mVertices[j].x;
			float y = mMesh->mVertices[j].y;
			float z = mMesh->mVertices[j].z;

			vertices[j] = vec(x, y, z);
		}
		boundingBox.Enclose(vertices, mMesh->mNumVertices);
		
		meshes.push_back(mesh);
	}
}

void Model::Draw() {

	std::vector<Mesh*>::iterator iter, end;
	for (iter = meshes.begin(), end = meshes.end(); iter != end; ++iter) {
		(*iter)->Draw(materials);
	}
}

void Model::Clear()
{
	meshes.clear();
	materials.clear();
}

std::string Model::GetDirectory(const std::string& fname)
{
	size_t pos = fname.find_last_of("\\/");
	return (std::string::npos == pos)
		? ""
		: fname.substr(0, pos+1);
}

std::string Model::GetFilename(const std::string& fname)
{
	size_t pos = fname.find_last_of("\\/");
	return (std::string::npos == pos)
		? fname
		: fname.substr(pos + 1, fname.size()-1);
}

float3 Model::GetCentreAABB()
{
	return boundingBox.CenterPoint();
}

float Model::GetDiagonalAABB() {
	return boundingBox.Diagonal().Length();
}
