#pragma once

#include<list>
#include "Globals.h"
#include "Module.h"
#include "Timer.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleProgram;
class ModuleRenderExercise;
class ModuleDebugDraw;
class ModuleCamera;
class ModuleEditor;
class ModuleTexture;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	void RetrieveHardwareInfo();
	void RetrieveGPUInfo();

public:
	ModuleRender* renderer = nullptr;
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleProgram* program = nullptr;
	ModuleRenderExercise* exercise = nullptr;
	ModuleDebugDraw* debugDraw = nullptr;
	ModuleCamera* camera = nullptr;
	ModuleEditor* editor = nullptr;
	ModuleTexture* texture = nullptr;


	//----- APPLICATION WINDOW INFO -----
	char windowName[25] = "Super Awesome Engine";
	char organization[10] = "UPC";
	int maxFPS = 60;
	bool limFPS = true;

	const char* appName = "Super Awesome Engine";
	const char* description = "Game Engine";
	const char* author = "Oriol Sors";
	const char* libraries = "GLEW, SDL2, MathGeoLib, ImGuI, DirectXTex, assimp";
	const char* license = "MIT";


	//----- HARDWARE WINDOW INFO -----
	char sdlVersion[20];
	
	int numCPU;
	int sizeCache;
	float sizeRam;
	bool caps[11];

	float vramBudget;
	float vramUsage;
	float vramAvailable;
	float vramReserved;


private:

	Timer time;
	float dt;
	std::list<Module*> modules;

};

extern Application* App;
