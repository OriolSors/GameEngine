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
	float GetDistanceNearClippingPlane();
	float GetDistanceFarClippingPlane();
	float GetHorizontalFOV();
	float GetAspectRatio();
	float3 GetPos();
	float GetRotationSpeed();

	void Translate(const vec& direction);
	void Rotate(const vec& rotation);

	void SetHorizontalFOV(float hFOV);
	void SetAspectRatio(float aspectRatio);
	void SetPlaneDistances(float distanceNear, float distanceFar);
	void SetPosition(const float3& position);
	void SetRotationSpeed(float rotationSpeed);

public:
	bool move = false;

private:
	float cameraSpeed = 1.0f;
	float zoomSpeed = 100.0f;
	float rotationSpeed = 1.0f;

	Frustum frustum;
	
};

