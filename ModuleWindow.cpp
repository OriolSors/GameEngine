#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h" 

ModuleWindow::ModuleWindow()
{
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	ENGINE_LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		ENGINE_LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		SDL_Rect* rect = new SDL_Rect;
		SDL_GetDisplayUsableBounds(0, rect);

		width = rect->w;
		height = rect->h;

		Uint32 flags = SDL_WINDOW_SHOWN |  SDL_WINDOW_OPENGL;

		if(FULLSCREEN == true)
		{
			fullscreen = true;
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if (BORDERLESS == true) {
			borderless = true;
			flags |= SDL_WINDOW_BORDERLESS;
		}
		if (RESIZABLE == true) {
			resizable = true;
			flags |= SDL_WINDOW_RESIZABLE;
		}
		if (FULLSCREEN_DESKTOP == true) {
			fullscreenDesktop = true;
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			ENGINE_LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	ENGINE_LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

float ModuleWindow::GetBrightness()
{
	return SDL_GetWindowBrightness(window);
}

void ModuleWindow::SetBrightness(float brightness)
{
	SDL_SetWindowBrightness(window, brightness);
}

int ModuleWindow::GetWidth()
{
	return width;
}

int ModuleWindow::GetHeight()
{
	return height;
}

void ModuleWindow::SetWidth(int w)
{
	this->width = w;
	SDL_SetWindowSize(window, width, height);
}

void ModuleWindow::SetHeigth(int h)
{
	this->height = h;
	SDL_SetWindowSize(window, width, height);
}

int ModuleWindow::GetRefreshRate()
{
	int ret = 0;

	SDL_DisplayMode dm;
	if (SDL_GetDesktopDisplayMode(0, &dm) != 0) {
		ENGINE_LOG("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
	}
		
	else
		ret = dm.refresh_rate;

	return ret;
}

bool ModuleWindow::IsFullscreen()
{
	return fullscreen;
}

void ModuleWindow::SetFullscreen(bool fullscreen)
{
	if (fullscreen != this->fullscreen)
	{
		this->fullscreen = fullscreen;
		if (this->fullscreen == true)
		{
			if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN) != 0)
				ENGINE_LOG("Could not switch to fullscreen desktop: %s\n", SDL_GetError());
			fullscreen = false;
		}
		else
		{
			if (SDL_SetWindowFullscreen(window, 0) != 0)
				ENGINE_LOG("Could not switch to windowed: %s\n", SDL_GetError());
		}
	}
}

bool ModuleWindow::IsResizable()
{
	return resizable;
}

void ModuleWindow::SetResizable(bool resizable)
{
	this->resizable = resizable;
}

bool ModuleWindow::IsBorderless()
{
	return borderless;
}

void ModuleWindow::SetBorderless(bool borderless)
{
	if (borderless != this->borderless && fullscreen == false && fullscreenDesktop == false)
	{
		this->borderless = borderless;
		SDL_SetWindowBordered(window, (SDL_bool)!this->borderless);
	}
}

bool ModuleWindow::IsFullScreenDesktop()
{
	return fullscreenDesktop;
}

void ModuleWindow::SetFullScreenDesktop(bool fullscreenDesktop)
{
	
	if (fullscreenDesktop != this->fullscreenDesktop)
	{
		this->fullscreenDesktop = fullscreenDesktop;
		if (this->fullscreenDesktop == true)
		{
			if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP) != 0)
				ENGINE_LOG("Could not switch to fullscreen desktop: %s\n", SDL_GetError());
			fullscreen = false;
		}
		else
		{
			if (SDL_SetWindowFullscreen(window, 0) != 0)
				ENGINE_LOG("Could not switch to windowed: %s\n", SDL_GetError());
		}
	}
}

