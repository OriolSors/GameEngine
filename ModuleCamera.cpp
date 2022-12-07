#include "Application.h"
#include "ModuleInput.h"
#include "ModuleCamera.h"
#include "ModuleWindow.h"
#include "ModuleRenderExercise.h"
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
	int w, h;
	SDL_GetWindowSize(App->window->window, &w, &h);
	float aspectRatio = float(w) / float(h);
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetViewPlaneDistances(0.15f, 200.0f);
	frustum.SetHorizontalFovAndAspectRatio(DegToRad(90.0f), aspectRatio);
	frustum.SetPos(float3(0.0f, 1.0f, 12.0f));
	frustum.SetFront(-float3::unitZ);
	frustum.SetUp(float3::unitY);
	

	return true;
}

update_status ModuleCamera::PreUpdate(float deltaTime)
{
	return UPDATE_CONTINUE;
}

update_status ModuleCamera::Update(float deltaTime)
{
	cameraSpeed = 1.0f;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT)) cameraSpeed *= cameraSpeedMultiplier;

	
	if (App->input->IsRightClicked()) {

		float3 direction = float3::zero;
		if (App->input->GetKey(SDL_SCANCODE_W)) direction.z = -1;
		if (App->input->GetKey(SDL_SCANCODE_S)) direction.z = 1;
		if (App->input->GetKey(SDL_SCANCODE_A)) direction.x = -1;
		if (App->input->GetKey(SDL_SCANCODE_D)) direction.x = 1;
		if (App->input->GetKey(SDL_SCANCODE_E)) direction.y = -1;
		if (App->input->GetKey(SDL_SCANCODE_Q)) direction.y = 1;

		Translate(direction *  deltaTime);


		float2 mouseRotation(App->input->GetMouseMotion());
		vec rotation(mouseRotation.x, mouseRotation.y, 0.0f);
		/*
		vec rotation(vec::zero);

		if (mouseRotation.x > 0) rotation.x = 1;
		if (mouseRotation.x < 0) rotation.x = -1;
		if (mouseRotation.y > 0) rotation.y = 1;
		if (mouseRotation.y < 0) rotation.y = -1;

		*/

		Rotate(rotation * deltaTime);
	}

	if (App->input->GetKey(SDL_SCANCODE_LALT) && App->input->IsLeftClicked() ) {
		
		RotateAround(App->input->GetMouseMotion()*deltaTime);
		
	}

	if (App->input->GetKey(SDL_SCANCODE_F)) LookAt(App->exercise->GetLookAtPoint());

	
	int wheelValue = App->input->GetWheel();
	vec zoom(vec::zero);
	if (wheelValue > 0) zoom.z = -1;
	if (wheelValue < 0) zoom.z = 1;

	Translate(zoom * zoomSpeed * deltaTime);
	
	
	return UPDATE_CONTINUE;
}

update_status ModuleCamera::PostUpdate(float deltaTime)
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

float ModuleCamera::GetCameraSpeedMultiplier()
{
	return cameraSpeedMultiplier;
}

void ModuleCamera::Translate(const vec& direction) 
{
	frustum.SetPos(frustum.Pos() + frustum.WorldMatrix().Float3x3Part().MulDir(direction * cameraSpeed));

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

void ModuleCamera::LookAt(const vec& point) {
	lookAtPoint = point;
	float3 direction = lookAtPoint - frustum.Pos();

	float3x3 rotation = float3x3::LookAt(frustum.Front(), direction.Normalized(), frustum.Up(), float3::unitY);

	frustum.SetFront(rotation.MulDir(frustum.Front()).Normalized());
	frustum.SetUp(rotation.MulDir(frustum.Up()).Normalized());
}

void ModuleCamera::RotateAround(const float2& rotationAround) {

	LookAt(App->exercise->GetLookAtPoint());

	float3 focus = frustum.Pos() - lookAtPoint;

	Quat qyRotation = Quat::RotateAxisAngle(frustum.Up(), rotationAround.y * rotationSpeed);
	Quat qxRotation = Quat::RotateAxisAngle(frustum.WorldRight(), rotationAround.x * rotationSpeed);
	Quat qRotation = qyRotation.Mul(qxRotation);

	focus = qRotation.Transform(focus);

	frustum.SetPos(lookAtPoint + focus);

	LookAt(lookAtPoint);
}

void ModuleCamera::SetHorizontalFOV(float hFOV)
{
	float hFOVRadians = DegToRad(hFOV);

	float aspect = frustum.AspectRatio();
	frustum.SetHorizontalFovAndAspectRatio(hFOVRadians, aspect);
}

void ModuleCamera::SetAspectRatio(float aspectRatio)
{
	float hFOV = frustum.HorizontalFov();
	frustum.SetHorizontalFovAndAspectRatio(hFOV, aspectRatio);
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

void ModuleCamera::SetCameraSpeedMultiplier(float cameraSpeedMultiplier)
{
	this->cameraSpeedMultiplier = cameraSpeedMultiplier;
}