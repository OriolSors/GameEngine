#include "ModuleCamera.h"
#include "Game/MathGeoLib/Geometry/Frustum.h"
#include "Game/MathGeoLib/Math/float3x3.h"
#include "GL\glew.h"

ModuleCamera::ModuleCamera()
{
}

ModuleCamera::~ModuleCamera()
{
}

bool ModuleCamera::Init()
{
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetViewPlaneDistances(0.1f, 200.0f);
	frustum.SetHorizontalFovAndAspectRatio(DegToRad(90.0f), 1.3f);
	frustum.SetPos(float3(0.0f, 1.0f, -2.0f));
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);

	return true;
}

update_status ModuleCamera::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleCamera::Update()
{
	return UPDATE_CONTINUE;
}

update_status ModuleCamera::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleCamera::CleanUp()
{
	return false;
}

float4x4 ModuleCamera::GetViewMatrix()
{
	return frustum.ViewMatrix();
}

float4x4 ModuleCamera::GetProjectionMatrix()
{
	return frustum.ProjectionMatrix();
}

float4x4 ModuleCamera::GetModelMatrix()
{
	return float4x4();
}
