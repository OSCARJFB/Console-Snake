/*
        Writen by: Oscar Bergstr�m
        https://github.com/OSCARJFB

        MIT License
        Copyright (c) 2023 Oscar Bergstr�m
*/

#ifndef GAME_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

#include "Console.h"

typedef struct Snake
{
    int x, y;
    struct Snake* next;
} Snake; 

void run(void);

#endif
