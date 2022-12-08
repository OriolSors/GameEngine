#pragma once
#include <windows.h>
#include <stdio.h>
#include <string>

#define ENGINE_LOG(format, ...) _log(__FILE__, __LINE__, format, __VA_ARGS__);

void _log(const char file[], int line, const char* format, ...);

extern std::string logBuffer;

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Configuration -----------
#define SCREEN_WIDTH 1080
#define SCREEN_HEIGHT 720
#define FULLSCREEN false
#define BORDERLESS false
#define RESIZABLE true
#define FULLSCREEN_DESKTOP false
#define VSYNC true
#define FPS_LOG_SIZE 100
#define TITLE "Super Awesome Engine"
#define GLSL_VERSION "#version 440"