#pragma once
#include "Globals.h"
#include "SDL\include\SDL.h"

class Timer
{
public:

	Timer();

	void Start();
	Uint32 DeltaTime();

private:

	Uint32 timeStart;
};

