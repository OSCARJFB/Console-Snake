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
#include <sys/ioctl.h>
#endif

void clearScreen(void);
void initConsole(void);
char kbhit(void);

#endif
