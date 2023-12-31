/*
		Writen by: Oscar Bergström
		https://github.com/OSCARJFB

		MIT License
		Copyright (c) 2023 Oscar Bergström
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

void run(void)
{
	const int ESC = 27;
	initTerm();

	for(int ch = 0; ch = ESC; ch = getchar())
	{
		if (!frames())
		{
			continue;
		}
	}
}