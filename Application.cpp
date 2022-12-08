#pragma once
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleProgram.h"
#include "ModuleRenderExercise.h"
#include "ModuleDebugDraw.h"
#include "ModuleCamera.h"
#include "ModuleEditor.h"
#include "ModuleTexture.h"

using namespace std;

Application::Application()
{
	// Order matters: they will Init/start/update in this order
	modules.push_back(window = new ModuleWindow());
	modules.push_back(renderer = new ModuleRender());
	modules.push_back(texture = new ModuleTexture());
	modules.push_back(program = new ModuleProgram());
	modules.push_back(input = new ModuleInput());
	modules.push_back(camera = new ModuleCamera());
	modules.push_back(exercise = new ModuleRenderExercise());
	modules.push_back(editor = new ModuleEditor());
	modules.push_back(debugDraw = new ModuleDebugDraw());

}

Application::~Application()
{
	for(list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
    {
        delete *it;
    }
}

bool Application::Init()
{
	RetrieveHardwareInfo();
	bool ret = true;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init();

	return ret;
}

update_status Application::Update()
{
	RetrieveGPUInfo();
	dt = (float)time.DeltaTime() / 1000.0f;
	time.Start();
	
	update_status ret = UPDATE_CONTINUE;

	
	for (list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PreUpdate(dt);

	for (list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->Update(dt);

	for (list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PostUpdate(dt);


	if (limFPS)
	{
		unsigned int frameMS = time.DeltaTime();
		unsigned int minMS = 1000 / maxFPS;
		if (frameMS < minMS) 
		{
			SDL_Delay(minMS - frameMS); //waiting until we reach the minimum ms for the frame
		}
	}

	editor->PlotFPS(1.0f / dt, dt * 1000.0f); //sending the average FPS and ms to Editor
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for(list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		ret = (*it)->CleanUp();

	return ret;
}

void Application::RetrieveHardwareInfo() {
	SDL_version sdlVersion;
	SDL_GetVersion(&sdlVersion);

	sprintf_s(this->sdlVersion, "%i.%i.%i", sdlVersion.major, sdlVersion.minor, sdlVersion.patch);

	numCPU = SDL_GetCPUCount();
	sizeCache = SDL_GetCPUCacheLineSize();
	sizeRam = SDL_GetSystemRAM() / 1000.0f;
	caps[0] = SDL_Has3DNow();
	caps[1] = SDL_HasAltiVec();
	caps[2] = SDL_HasAVX();
	caps[3] = SDL_HasAVX2();
	caps[4] = SDL_HasMMX();
	caps[5] = SDL_HasRDTSC();
	caps[6] = SDL_HasSSE();
	caps[7] = SDL_HasSSE2();
	caps[8] = SDL_HasSSE3();
	caps[9] = SDL_HasSSE41();
	caps[10] = SDL_HasSSE42();
}

void Application::RetrieveGPUInfo()
{
	int vramBudgetKb;
	int vramAvailableKb;
	glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &vramBudgetKb);
	glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &vramAvailableKb);
	vramBudget = vramBudgetKb / 1000.0f;
	vramAvailable = vramAvailableKb / 1000.0f;
	vramUsage = vramBudget - vramAvailable;

}
