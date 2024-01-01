/*
		Writen by: Oscar Bergström
		https://github.com/OSCARJFB

		MIT License
		Copyright (c) 2024 Oscar Bergström
*/

#include "Game.h"

static bool calculateFrameTime(void)
{
	static time_t frameCountStart = 0, frameCountEnd = 0;

	if (frameCountStart == 0)
	{
		frameCountStart = time(NULL);
		return false;
	}
	
	frameCountEnd = time(NULL);
	double diff = difftime(frameCountStart, frameCountEnd) * 1000;
	if (diff / CLOCKS_PER_SEC > 1.0)
	{
		return true;
	}

	return false;
}

static bool frames(void)
{
	static int frames = 0;

	// cap game at 60 frames.
	if (frames == 60)
	{
		if (calculateFrameTime())
		{
			frames = 0;
		}
	}
	else
	{
		++frames;
	}

	return frames < 60;
}

static Snake* allocateSnakePart(void)
{
	Snake* part = malloc(sizeof(Snake));
	if (part == NULL)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	return part;
}

static void addPartToSnake(Snake** snake)
{
	Snake* part = allocateSnakePart();
	
	if (*snake == NULL)
	{
		*snake = part;
		return; 
	}

	for (Snake* tail = *snake; tail != NULL; tail = tail->next)
	{
		if (tail->next == NULL)
		{
			tail->next = part;
		}
	}
}

static void printSnake(Snake** snake)
{
	
}

void run(void)
{
	const int ESC = 27;
	Snake* snake = NULL;

	initConsole();
	addPartToSnake(&snake);

	for(int ch = 0; ch = ESC; ch = getchar())
	{
		// FRAME LOCK
		if (!frames())
		{
			continue;
		}

		// READ INPUT

		// CHECK INPUT

		// DRAW

	}
}