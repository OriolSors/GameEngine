#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"
#include "ModuleRenderExercise.h"
#include "SDL/include/SDL.h"
#include "Game/ImGui/imgui_impl_sdl.h"

ModuleInput::ModuleInput()
{}

// Destructor
ModuleInput::~ModuleInput()
{}

// Called before render is available
bool ModuleInput::Init()
{
	ENGINE_LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		ENGINE_LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

update_status ModuleInput::PreUpdate(float deltaTime)
{
    keyboard = SDL_GetKeyboardState(NULL);
    
    return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleInput::Update(float deltaTime)
{
    SDL_Event sdlEvent;

    mouseWheel = 0;
    mouseMotion = float2::zero;

    while (SDL_PollEvent(&sdlEvent) != 0)
    {

        ImGui_ImplSDL2_ProcessEvent(&sdlEvent);

        switch (sdlEvent.type)
        {
        case SDL_QUIT:
            return UPDATE_STOP;
        case SDL_WINDOWEVENT:
            if (sdlEvent.window.event == SDL_WINDOWEVENT_RESIZED || sdlEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                App->renderer->WindowResized(sdlEvent.window.data1, sdlEvent.window.data2);
            break;

        case SDL_MOUSEWHEEL:
            mouseWheel = sdlEvent.wheel.y;
            break;
            
        case SDL_MOUSEBUTTONDOWN:
            if (sdlEvent.button.button == SDL_BUTTON_RIGHT)rightClick = true;
            else if (sdlEvent.button.button == SDL_BUTTON_LEFT)leftClick = true;
            break;

        case SDL_MOUSEBUTTONUP:
            if (sdlEvent.button.button == SDL_BUTTON_RIGHT)rightClick = false;
            else if (sdlEvent.button.button == SDL_BUTTON_LEFT)leftClick = false;
            break;

           
        case SDL_MOUSEMOTION:
            mouseMotion.x = -sdlEvent.motion.yrel;
            mouseMotion.y = -sdlEvent.motion.xrel;
            break;
        case SDL_DROPFILE:
            App->exercise->SetModel(sdlEvent.drop.file);
            break;
        }
    }

    return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	ENGINE_LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

Uint8 ModuleInput::GetKey(int key)
{
    return keyboard[key];
}

int ModuleInput::GetWheel()
{
    return mouseWheel;
}

float2 ModuleInput::GetMouseMotion() {
    return mouseMotion;
}

bool ModuleInput::IsRightClicked()
{
    return rightClick;
}

bool ModuleInput::IsLeftClicked()
{
    return leftClick;
}


