#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow();

	// Destructor
	virtual ~ModuleWindow();

	// Called before quitting
	bool Init();

	// Called before quitting
	bool CleanUp();

	float GetBrightness();
	void SetBrightness(float brightness);

	int GetWidth();
	int GetHeight();

	void SetWidth(int w);
	void SetHeigth(int h);

	int GetRefreshRate();

	bool IsFullscreen();
	void SetFullscreen(bool fullscreen);

	bool IsResizable();
	void SetResizable(bool resizable);

	bool IsBorderless();
	void SetBorderless(bool borderless);

	bool IsFullScreenDesktop();
	void SetFullScreenDesktop(bool fullscreenDesktop);


public:
	//The window we'll be rendering to
	SDL_Window* window = NULL;

	//The surface contained by the window
	SDL_Surface* screen_surface = NULL;

	int width;
	int height;
	bool fullscreen;
	bool resizable;
	bool borderless;
	bool fullscreenDesktop;
};

#endif // __ModuleWindow_H__