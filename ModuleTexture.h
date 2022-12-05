#pragma once
#include "Module.h"
#include "Game/DirectXTex/DirectXTex.h"
#include "GL\glew.h"
class ModuleTexture :
    public Module
{
public:

	ModuleTexture();
	~ModuleTexture();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void Load(const char* textureFile);

public:
	GLuint texture_object;

	unsigned internalFormat;
	unsigned format;
	unsigned type;

	DirectX::TexMetadata md;

};

