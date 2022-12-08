#include "Globals.h"

#include <windows.h>
#include <stdio.h>

void _engineLog(const char file[], int line, const char* format, ...)
{
	/*Every call to ENGINE_LOG will fill the external buffer logBuffer*/

	static char strBufferInput[5000];
	static char strBufferLOG[5000];
	static va_list  ap;

	//getting the list of arguments and adding to a buffer
	va_start(ap, format);
	vsprintf_s(strBufferInput, 5000, format, ap);
	va_end(ap);

	//fill the output buffer for logging into Editor
	sprintf_s(strBufferLOG, 5000, "\n%s(%d) : %s", file, line, strBufferInput);
	OutputDebugString(strBufferLOG);
	logBuffer.append(strBufferLOG);
}

std::string logBuffer;