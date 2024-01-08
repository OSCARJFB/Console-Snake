/*
		Writen by: Oscar Bergström
		https://github.com/OSCARJFB

		MIT License
		Copyright (c) 2024 Oscar Bergström
*/

#ifndef GAME_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

#include "Console.h"

enum Direction
{
	up = 1,
	left,
	right,
	down,
};

typedef struct Snake
{
    unsigned int x, y;
    struct Snake* next;
} Snake; 

typedef struct Food
{
	unsigned int x, y;
} Food;

typedef struct Board
{
    unsigned int size;
    char** grid;
} Board;

void run(void);

#endif
