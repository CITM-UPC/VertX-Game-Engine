#include "Log.h"
#include "Application.h"
#include "Globals.h"
#include "Module.h"

void Log(const char file[], int line, const char* format, ...)
{
	static char tmpString1[4096];
	static char tmpString2[4096];
	static va_list ap;

	// Construct the string from variable arguments
	va_start(ap, format);
	vsprintf_s(tmpString1, 4096, format, ap);
	va_end(ap);
	sprintf_s(tmpString2, 4096, "\n%s(%d) : %s", file, line, tmpString1);

	OutputDebugString(tmpString2);

	if (App != nullptr) {
		sprintf_s(tmpString2, BUFFER_SIZE, "%s", tmpString1);
		App->AddConsoleLog(tmpString2);
	}
}