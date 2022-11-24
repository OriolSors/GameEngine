#include "Application.h"
#include "ModuleInput.h"
#include "ModuleCamera.h"
#include "ModuleWindow.h"
#include "Game/MathGeoLib/Math/float3x3.h"
#include "Game/MathGeoLib/Math/Quat.h"
#include "GL\glew.h"
#include "SDL.h"
#include <cstring>
#include <iostream>

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
	frustum.SetPos(float3(0.0f, 1.0f, 12.0f));
	frustum.SetFront(-float3::unitZ);
	frustum.SetUp(float3::unitY);
	

	return true;
}

update_status ModuleCamera::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleCamera::Update()
{
	//----- TRANSLATION -----
	cameraSpeed = 1.0f;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT)) cameraSpeed = 3.0f;

	vec direction(vec::zero);
	if (App->input->GetKey(SDL_SCANCODE_W)) direction.z = -0.01f;
	if (App->input->GetKey(SDL_SCANCODE_S)) direction.z = 0.01f;
	if (App->input->GetKey(SDL_SCANCODE_A)) direction.x = -0.01f;
	if (App->input->GetKey(SDL_SCANCODE_D)) direction.x = 0.01f;
	if (App->input->GetKey(SDL_SCANCODE_E)) direction.y = -0.01f;
	if (App->input->GetKey(SDL_SCANCODE_Q)) direction.y = 0.01f;

	Translate(direction * cameraSpeed);

	//----- ZOOM -----

	vec zoom(vec::zero);
	if (App->input->GetWheel() > 0) zoom.z = -0.01f;
	if (App->input->GetWheel() < 0) zoom.z = 0.01f;

	ENGINE_LOG("%d",App->input->GetWheel());
	Translate(zoom * zoomSpeed);

	//----- ROTATION -----

	vec rotation(vec::zero);
	if (App->input->GetKey(SDL_SCANCODE_UP)) rotation.x = 0.01f;
	if (App->input->GetKey(SDL_SCANCODE_DOWN)) rotation.x = -0.01f;
	if (App->input->GetKey(SDL_SCANCODE_RIGHT)) rotation.y = -0.01f;
	if (App->input->GetKey(SDL_SCANCODE_LEFT)) rotation.y = 0.01f;
	
	Rotate(rotation);

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

float ModuleCamera::GetDistanceNearClippingPlane() {
	return frustum.NearPlaneDistance();
}

float ModuleCamera::GetDistanceFarClippingPlane() {
	return frustum.FarPlaneDistance();
}

float ModuleCamera::GetHorizontalFOV()
{
	return RadToDeg(frustum.HorizontalFov());
}

float ModuleCamera::GetAspectRatio()
{
	return frustum.AspectRatio();
}

float3 ModuleCamera::GetPos()
{
	return frustum.Pos();
}

float ModuleCamera::GetRotationSpeed()
{
	return rotationSpeed;
}

void ModuleCamera::Translate(const vec& direction) 
{
	frustum.SetPos(frustum.Pos() + frustum.WorldMatrix().Float3x3Part().MulDir(direction));

	/*
	vec viewDirection (frustum.WorldRight().Normalized() * direction.x + frustum.Up().Normalized() * direction.y - frustum.Front().Normalized() * direction.z);
	frustum.SetPos(frustum.Pos() + viewDirection);
	*/
}

void ModuleCamera::Rotate(const vec& rotation) {
	
	Quat qyRotation = Quat::RotateY(rotation.y * rotationSpeed);
	Quat qxRotation = Quat::RotateAxisAngle(frustum.WorldRight(), rotation.x * rotationSpeed);
	Quat qRotation = qxRotation.Mul(qyRotation);

	vec oldFront = frustum.Front().Normalized();
	frustum.SetFront(qRotation *oldFront);
	vec oldUp = frustum.Up().Normalized();
	frustum.SetUp(qRotation * oldUp);
}

void ModuleCamera::SetHorizontalFOV(float hFOV)
{
	float hFOVRadians = DegToRad(hFOV);

	int w;
	int h;

	SDL_GetWindowSize(App->window->window, &w, &h);

	float aspect = float(w) / float(h);
	frustum.SetHorizontalFovAndAspectRatio(hFOVRadians, aspect);
}

void ModuleCamera::SetAspectRatio(float aspectRatio)
{
	float hFOV = frustum.HorizontalFov();
	float vFOV = 2.f * atanf(aspectRatio * tanf(hFOV * 0.5f));
	frustum.SetPerspective(hFOV, vFOV);
}

void ModuleCamera::SetPlaneDistances(float distanceNear, float distanceFar)
{
	frustum.SetViewPlaneDistances(distanceNear, distanceFar);
}

void ModuleCamera::SetPosition(const float3& position)
{
	frustum.SetPos(position);
}

void ModuleCamera::SetRotationSpeed(float rotationSpeed)
{
	this->rotationSpeed = rotationSpeed;
}