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

typedef struct Snake
{
    unsigned int x, y;
    struct Snake* next;
} Snake; 

typedef struct Board
{
    unsigned int length, width;
    char** grid;
} Board;

void run(void);

#endif
