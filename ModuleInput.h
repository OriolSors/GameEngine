#pragma once
#include "Module.h"
#include "Globals.h"
#include "Game/MathGeoLib/Math/float2.h"
#include "SDL/include/SDL.h"

typedef unsigned __int8 Uint8;

class ModuleInput : public Module
{
public:
	
	ModuleInput();
	~ModuleInput();

	bool Init();
	update_status PreUpdate(float deltaTime);
	update_status Update(float deltaTime);
	bool CleanUp();

	Uint8 GetKey(int key);
	int GetWheel();
	float2 GetMouseMotion();

	bool IsRightClicked();
	bool IsLeftClicked();

private:
	const Uint8 *keyboard = NULL;
	int mouseWheel = 0;
	float2 mouseMotion = float2::zero;

	bool rightClick = false;
	bool leftClick = false;
};