/*
		Writen by: Oscar Bergström
		https://github.com/OSCARJFB

		MIT License
		Copyright (c) 2024 Oscar Bergström
*/

#include "Console.h"

#ifdef _WIN32

static DWORD dMode = 0;
static HANDLE hInput = NULL;

void restoreConsole(void)
{
	SetConsoleMode(hInput, dMode);
	printf("Terminal was restored successfully");
}

void initConsole(void)
{
	if ((hInput = GetStdHandle(STD_INPUT_HANDLE)) == INVALID_HANDLE_VALUE)
	{
		DWORD dError = GetLastError();
		printf("Error %d while trying to get current handle.", dError);
		exit(dError);
	}

	if (!GetConsoleMode(hInput, &dMode))
	{
		DWORD dError = GetLastError();
		printf("Error %d while trying to get the current console mode.", dError);
		exit(dError);
	}

	atexit(restoreConsole);
	SetConsoleMode(hInput ,dMode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT));
}

#endif
