#include "Timer.h"

Timer::Timer()
{
}

void Timer::Start()
{
	timeStart = SDL_GetTicks();
	
}

Uint32 Timer::DeltaTime()
{
	return SDL_GetTicks() - timeStart;
}

