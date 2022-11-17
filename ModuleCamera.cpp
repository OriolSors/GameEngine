#include "ModuleCamera.h"
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
	/*
	float aspect = float(SCREEN_WIDTH) / float(SCREEN_HEIGHT);
	frustum.SetKind(FrustumProjectiveSpace::FrustumSpaceGL, FrustumHandedness::FrustumRightHanded);
	frustum.SetPos(float3(0.0f, 2.0f, 10.0f));
	frustum.SetFront(-float3::unitZ);
	frustum.SetUp(float3::unitY);
	frustum.SetViewPlaneDistances(0.1f, 100.0f);
	frustum.SetPerspective(2.f * atanf(tanf(VFOV * 0.5f) * aspect), VFOV);
	
	*/
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetViewPlaneDistances(0.1f, 200.0f);
	frustum.SetHorizontalFovAndAspectRatio(DegToRad(90.0f), 1.3f);
	frustum.SetPos(float3(0.0f, 1.0f, -12.0f));
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
	return float4x4(frustum.ViewMatrix());
}

float4x4 ModuleCamera::GetProjectionMatrix()
{
	return float4x4(frustum.ProjectionMatrix());
}

void ModuleCamera::Translate(vec direction) {
	frustum.SetPos(frustum.Pos() + GetViewMatrix().Float3x3Part().MulDir(direction));
}


void ModuleCamera::Rotate() {
	float3x3 rotationDeltaMatrix;

	vec oldFront = frustum.Front().Normalized();
	frustum.SetFront(rotationDeltaMatrix.MulDir(oldFront));
	vec oldUp = frustum.Up().Normalized();
	frustum.SetUp(rotationDeltaMatrix.MulDir(oldUp));
}
