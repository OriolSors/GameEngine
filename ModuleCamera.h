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
	update_status PreUpdate(float deltaTime);
	update_status Update(float deltaTime);
	update_status PostUpdate(float deltaTime);
	bool CleanUp();

	float4x4 GetViewMatrix();
	float4x4 GetProjectionMatrix();
	float GetDistanceNearClippingPlane();
	float GetDistanceFarClippingPlane();
	float GetHorizontalFOV();
	float GetAspectRatio();
	float3 GetPos();
	float GetRotationSpeed();
	float GetCameraSpeedMultiplier();

	void Translate(const vec& direction);
	void Rotate(const vec& rotation);
	void LookAt(const vec& point);
	void RotateAround(const float2& rotationAround);

	void SetHorizontalFOV(float hFOV);
	void SetAspectRatio(float aspectRatio);
	void SetPlaneDistances(float distanceNear, float distanceFar);
	void SetPosition(const float3& position);
	void SetRotationSpeed(float rotationSpeed);
	void SetCameraSpeedMultiplier(float cameraSpeedMultiplier);


private:
	float cameraSpeed = 1.0f;
	float cameraSpeedMultiplier = 2.0f;
	float zoomSpeed = 15.0f;
	float rotationSpeed = 0.25f;
	float3 lookAtPoint;

	Frustum frustum;
	
};

