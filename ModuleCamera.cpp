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
	frustum.SetViewPlaneDistances(0.4f, 200.0f);
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

	vec front(frustum.Front().Normalized());
	vec up(frustum.Up().Normalized());
	vec right(frustum.WorldRight().Normalized());
	
	if (App->input->IsRightClicked()) {

		vec direction = vec::zero;

		if (App->input->GetKey(SDL_SCANCODE_W)) direction += front;
		if (App->input->GetKey(SDL_SCANCODE_S)) direction -= front;
		if (App->input->GetKey(SDL_SCANCODE_A)) direction -= right;
		if (App->input->GetKey(SDL_SCANCODE_D)) direction += right;
		if (App->input->GetKey(SDL_SCANCODE_E)) direction -= up;
		if (App->input->GetKey(SDL_SCANCODE_Q)) direction += up;

		Translate(direction *  deltaTime * cameraSpeed);

		vec rotation = vec::zero;
		float2 mouseRotation(App->input->GetMouseMotion());
		rotation = vec (mouseRotation.x, mouseRotation.y, 0.0f);
		

		Rotate(rotation * deltaTime * rotationSpeed);
	}


	if (App->input->GetKey(SDL_SCANCODE_LALT) && App->input->IsLeftClicked() ) {
		
		RotateAround(App->input->GetMouseMotion()*deltaTime * rotationSpeed);
		
	}

	if (App->input->GetKey(SDL_SCANCODE_F)) LookAt(lookAtPoint);

	
	int wheelValue = App->input->GetWheel();
	Translate(wheelValue * front * zoomSpeed * deltaTime * cameraSpeed);
	
	
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
	frustum.SetPos(frustum.Pos() + direction);
}

void ModuleCamera::Rotate(const vec& rotation) {
	
	Quat qyRotation = Quat::RotateY(rotation.y);
	Quat qxRotation = Quat::RotateAxisAngle(frustum.WorldRight(), rotation.x );
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

void ModuleCamera::Focus(Model* model) {
	frustum.SetPos(float3(0.0f, 1.0f, 12.0f)); //fix the camera in a default Position
	LookAt(model->GetCentreAABB());
	float3 currentPos = frustum.Pos();

	Translate(vec(0.0f, lookAtPoint.y - currentPos.y, 0.0f)); //put the camera in the same height as the model
	LookAt(lookAtPoint);

	currentPos = frustum.Pos();

	float distanceToCenter = (lookAtPoint - currentPos).Length();
	float modelDiagonal = model->GetDiagonalAABB();

	float ratio = distanceToCenter / modelDiagonal;
	float lambda = 1.0f / ratio; //we get closer if the distance to the center is bigger than the model diagonal; otherwise if diagonal is bigger
	if (ratio > 1.0f) {
		Translate(frustum.Front().Normalized() * lambda* distanceToCenter);
	}
	else {
		Translate(-frustum.Front().Normalized() * lambda* distanceToCenter);
	}

}

void ModuleCamera::RotateAround(const float2& rotationAround) {

	LookAt(lookAtPoint);

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