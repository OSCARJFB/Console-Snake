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
#endif

void initConsole(void);

#ifdef _WINDOWS_
CHAR _kbhit(void);
#elif __linux__
char _kbhit(void);
#endif 

#endif
