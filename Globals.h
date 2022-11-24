#pragma once
#include <windows.h>
#include <stdio.h>

#define ENGINE_LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Configuration -----------
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 700
#define FULLSCREEN false
#define BORDERLESS false
#define RESIZABLE true
#define FULLSCREEN_DESKTOP false
#define VSYNC true
#define TITLE "Super Awesome Engine"
#define GLSL_VERSION "#version 440"