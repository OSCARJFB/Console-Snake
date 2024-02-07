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
#define MAX 255 

enum Direction
{
	up = 1,
	left,
	down,
	right,
};

typedef struct Snake
{
    int x, y;
    struct Snake* next;
} Snake; 

typedef struct Food
{
	int x, y;
} Food;

typedef struct Board
{
    int size;
    char** grid;
} Board;

void run(void);

#endif
