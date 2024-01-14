/*
		Writen by: Oscar Bergström
		https://github.com/OSCARJFB

		MIT License
		Copyright (c) 2024 Oscar Bergström
*/

#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32

#include <Windows.h>

#elif __linux__

#include <unistd.h>
#include <termios.h>
#include <string.h>

#endif


#ifdef _WINDOWS_

void initConsole(void);
CHAR _kbhit(void);

#elif __linux__

void initConsole(void);
char _kbhit(void);

#endif 

#endif
