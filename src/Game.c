/*
		Writen by: Oscar Bergström
		https://github.com/OSCARJFB

		MIT License
		Copyright (c) 2024 Oscar Bergström
*/

#include "Game.h"

static inline void clearScreen(void)
{
#ifdef _WIN32
	system("cls");
#endif
}

static void getBoardSize(Board* board)
{
	char buffer[3];

	puts("Enter the board size: ");
	fgets(buffer, 3, stdin);

#ifdef _WIN32
	if (sscanf_s(buffer, "%d", &board->length) == EOF)
	{
		puts("Invalid input");
	}
#else
	if (sscanf(buffer, "%d", &board->length) == EOF)
	{
		puts("Invalid input");
	}
#endif

	board->width = board->length;
}

static void setBoardSize(Board* board)
{
	do
	{
		getBoardSize(board);
		if (board->length % 2 != 0)
		{
			printf("Can't set the board size not evenly divisible by 2. %d / 2 = %f\n", 
				board->length, (float)board->length / 2.0f);
		}

	} while (board->length % 2 != 0);
}

static void initBoard(Board* board)
{
	for (unsigned int i = 0; i < board->length; ++i)
	{
		for (unsigned int j = 0; j < board->width; ++j)
		{
			if (j == 0)
			{
				board->grid[i][j] = '#';
			}
			else if (j + 1 == board->width)
			{
				board->grid[i][j] = '#';
			}
			else if (i == 0)
			{
				board->grid[i][j] = '#';
			}
			else if(i + 1 == board->length)
			{ 
				board->grid[i][j] = '#';
			}
			else
			{
				board->grid[i][j] = ' ';
			}
		}
	}
}

static void allocateBoard(Board* board)
{
	setBoardSize(board);
	board->grid = malloc(sizeof(char*) * board->length);
	if (board->grid == NULL)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	for (unsigned int i = 0; i < board->length; ++i)
	{
		board->grid[i] = malloc(sizeof(char) * board->width);
		if (board->grid[i] == NULL)
		{
			perror("malloc");
			exit(EXIT_FAILURE);
		}
	}
	initBoard(board);
}

void deallocateBoard(Board board)
{
	for (unsigned int i = 0; i < board.length; ++i)
	{
		free(board.grid[i]);
		board.grid[i] = NULL;
	}

	free(board.grid);
	board.grid = NULL;
}

static bool framelock(void)
{
	static time_t start = 0, end = 0;

	if (start == 0)
	{
		start = time(NULL);
		return false;
	}
	
	end = time(NULL);
	double diff = difftime(end, start);
	if (diff > 5.0)
	{
		end = end = 0;
		clearScreen();
		return true;
	}

	return false;
}

static Snake* allocateSnakePart(void)
{
	Snake* part = malloc(sizeof(Snake));
	if (part == NULL)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	part->next = NULL;
	part->x = 0;
	part->y = 0;

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

static void printBoard(Snake* snake, Board* board)
{
	for (unsigned int i = 0; i < board->length; ++i)
	{
		for (unsigned int j = 0; j < board->width; ++j)
		{
			printf("%c", board->grid[i][j]);
		}
		printf("\n");
	}
}

static unsigned int updateDirection(char ch)
{
	static unsigned int direction = 0;

	switch (ch)
	{
	case 'w':
		direction = 1;
		break;
	case 'a':
		direction = 2;
		break;
	case 's':
		direction = 3;
		break;
	case 'd':
		direction = 4;
		break;
	default:
		break;
	}

	return direction;
}

static void updateSnakePosition(Snake* snake, unsigned int direction)
{
	switch (direction)
	{
	case 1:
		--snake->y;
		break;
	case 2:
		--snake->x;
		break;
	case 3:
		++snake->y;
		break;
	case 4:
		++snake->x;
		break;
	}
}

static void addSnakeToBoard(Snake* snake, Board* board)
{
	for (Snake* tail = snake; tail != NULL; tail = tail->next)
	{
		if (tail->y < board->length && tail->x < board->width)
		{
			board->grid[tail->y][tail->x] = 'o';
		}
	}
}

void run(void)
{
	const unsigned int ESC = 27;
	Snake* snake = NULL;
	Board board;
	
	allocateBoard(&board);
	addPartToSnake(&snake);
	initConsole();

	snake->x = board.length / 2;
	snake->y = board.width / 2;

	while(1)
	{
		// READ INPUT
		unsigned int direction = updateDirection(_kbhit());

		// FRAME LOCK
		if (!framelock())
		{
			continue;
		}

		// UPDATE
		updateSnakePosition(snake, direction);
		addSnakeToBoard(snake, &board);

		// DRAW
		printBoard(snake, &board);
	}
	
	deallocateBoard(board);
}