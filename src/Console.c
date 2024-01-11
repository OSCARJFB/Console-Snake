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
#ifdef _DEBUG
	printf("Terminal was restored successfully");
#endif
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

CHAR _kbhit(void)
{
	CHAR cAsciiKey = 0;
	DWORD dEvents = 0;
	INPUT_RECORD inputRecord;

	if (PeekConsoleInput(hInput, &inputRecord, 1, &dEvents) && dEvents > 0)
	{
		ReadConsoleInput(hInput, &inputRecord, 1, &dEvents);
		if (inputRecord.EventType == KEY_EVENT && inputRecord.Event.KeyEvent.bKeyDown)
		{
			cAsciiKey = inputRecord.Event.KeyEvent.uChar.AsciiChar;
		}
	}

	return cAsciiKey;
}

#endif
