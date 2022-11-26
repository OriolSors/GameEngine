#pragma once
#include "Module.h"
#include "Globals.h"

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

private:
	const Uint8 *keyboard = NULL;
	int mouse = NULL;
};