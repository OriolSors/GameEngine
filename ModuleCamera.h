#pragma once
#include "Module.h"
#include "Game/MathGeoLib/Geometry/frustum.h"
#include "Game/MathGeoLib/Math/float4x4.h"

class ModuleCamera :
    public Module
{

public:
	ModuleCamera();
	~ModuleCamera();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	float4x4 GetViewMatrix();
	float4x4 GetProjectionMatrix();

	void Translate(const vec& direction);
	void Rotate(const vec& rotation);

public:
	float cameraSpeed = 1.0f;
	float zoomSpeed = 100.0f;

private:
	Frustum frustum;
};

