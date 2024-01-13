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

#define MIN_BOARD_SIZE 10
#define MAX_BOARD_SIZE 40
#define SNAKE 'O'
#define FOOD 'X'
#define BORDER '#'

enum Direction
{
	up = 1,
	left,
	down,
	right,
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
