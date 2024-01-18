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

char kbhit(void)
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

	return (char)cAsciiKey;
}

void clearScreen(void)
{
	system("cls");
}

#elif __linux__

static struct termios original_term, raw_term;

void restoreConsole(void)
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_term);
#ifdef _DEBUG
	printf("Terminal was restored successfully");
#endif
}

void initConsole(void)
{
	if(tcgetattr(STDIN_FILENO, &original_term) == -1)
	{
		perror("tcgetattr");
		exit(-1);
	}

	raw_term = original_term;
	raw_term.c_lflag &= ~(ICANON | ECHO);

	if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw_term) == -1)
	{
		perror("tcsetattr");
		exit(-1);
	}

	atexit(restoreConsole);
}

char kbhit(void)
{
	long bytes = 0;
	char cr = 0;
	ioctl(STDIN_FILENO, FIONREAD, &bytes);
	if (bytes > 0)
	{
		read(STDIN_FILENO, &cr, sizeof(char));
	}
	fflush(stdout);
	return cr;
}

inline void clearScreen(void)
{
	system("clear");
}

#endif





















