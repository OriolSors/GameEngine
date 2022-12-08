#include "Globals.h"

#include <windows.h>
#include <stdio.h>

void _log(const char file[], int line, const char* format, ...)
{
	static char tmp_string[64000];
	static char tmp_string2[64000];
	static va_list  ap;

	va_start(ap, format);
	vsprintf_s(tmp_string, 64000, format, ap);
	va_end(ap);
	sprintf_s(tmp_string2, 64000, "\n%s(%d) : %s", file, line, tmp_string);
	OutputDebugString(tmp_string2);
	logBuffer.append(tmp_string2);
}

std::string logBuffer;