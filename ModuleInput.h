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
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();

	Uint8 GetKey(int key);
	int GetWheel();
	float2 GetMouseMotion();

private:
	const Uint8 *keyboard = NULL;
	int mouseWheel = NULL;
	float2 mouseMotion;

public:
	bool right = false;
};