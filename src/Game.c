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

#define MAX 255 
static void getBoardSize(Board* board)
{
	char buffer[MAX - 1];
	const char* msg =
		"################## BOARD SETUP ##################\n"
		"The board size of the game must follow 3 rules,\n"
		"The size can't be lesser than 10\n"
		"The size can't be greater than 40\n"
		"The size must be evenly divisible by 2\n"
		"Please enter the board size: ";

	clearScreen();
	printf(msg);
	fgets(buffer, MAX - 1, stdin);

#ifdef _WIN32
	(void)sscanf_s(buffer, "%d", &board->size);
#else
	(void)sscanf(buffer, "%d", &board->size);
#endif
}

static void setBoardSize(Board* board)
{
	while (board->size % 2 != 0 || board->size < 10 || board->size > 40)
	{
		getBoardSize(board);
	}
}

static void initBoard(Board* board)
{
	for (unsigned int i = 0; i < board->size; ++i)
	{
		for (unsigned int j = 0; j < board->size; ++j)
		{
			if (j == 0)
			{
				board->grid[i][j] = '#';
			}
			else if (j + 1 == board->size)
			{
				board->grid[i][j] = '#';
			}
			else if (i == 0)
			{
				board->grid[i][j] = '#';
			}
			else if(i + 1 == board->size)
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
	board->grid = malloc(sizeof(char*) * board->size);
	if (board->grid == NULL)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	for (unsigned int i = 0; i < board->size; ++i)
	{
		board->grid[i] = malloc(sizeof(char) * board->size);
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
	for (unsigned int i = 0; i < board.size; ++i)
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
	if (diff >= 1.0)
	{
		start = end = 0;
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
			break;
		}
	}
}

static void printBoard(Snake* snake, Board* board)
{
	for (unsigned int i = 0; i < board->size; ++i)
	{
		for (unsigned int j = 0; j < board->size; ++j)
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
		direction = up;
		break;
	case 'a':
		direction = left;
		break;
	case 's':
		direction = right;
		break;
	case 'd':
		direction = down;
		break;
	default:
		break;
	}

	return direction;
}

static void updateSnakeHead(Snake* snake, unsigned int direction)
{
	switch (direction)
	{
	case up:
		--snake->y;
		break;
	case left:
		--snake->x;
		break;
	case right:
		++snake->y;
		break;
	case down:
		++snake->x;
		break;
	}
}

static void setOldSnakeHeadPosition(unsigned int* oldX, unsigned int* oldY, unsigned int direction)
{
	switch (direction)
	{
	case up:
		++*oldY;
		break;
	case left:
		++*oldX;
		break;
	case right:
		--*oldY;
		break;
	case down:
		--*oldX;
		break;
	}
}

static void updateSnake(Snake* snake, unsigned int direction)
{
	updateSnakeHead(snake, direction);
	
	if (snake->next == NULL)
	{
		return;
	}

	unsigned int oldX = snake->x, oldY = snake->y;
	setOldSnakeHeadPosition(&oldX, &oldY, direction);

	for (Snake* tail = snake->next; tail != NULL; tail = tail->next)
	{
		unsigned int tempX = tail->x;
		unsigned int tempY = tail->y;

		tail->x = oldX;
		tail->y = oldY;

		oldX = tempX;
		oldY = tempY;
	}
}

static void addSnakeToBoard(Snake* snake, Board* board)
{
	for (Snake* tail = snake; tail != NULL; tail = tail->next)
	{
		if (tail->y < board->size && tail->x < board->size)
		{
			board->grid[tail->y][tail->x] = 'O';
		}
	}
}

/*
static bool isCollision(Snake* snake, Board* board)
{

}
*/

static void spawnFood(Food* food, Board* board)
{
	food->x = rand() % board->size + 1;
	food->y = rand() % board->size + 1;
}

static inline bool eatFood(Food* food, Snake* snake)
{
	return (snake->x == food->x && snake->y == food->y);
}

static void foodController(Food* food, Snake* snake, Board* board)
{
	if (eatFood(food, snake))
	{
		spawnFood(food, board);
	}
}

static void addFoodToBoard(Food* food, Board* board)
{
	for (unsigned int i = 0; i < board->size; ++i)
	{
		for (unsigned int j = 0; j < board->size; ++j)
		{
			if (food->y == i && food->x == j)
			{
				board->grid[i][j] = 'X'; // x is food.
				break;
			}
		}
	}
}

void run(void)
{
	const unsigned int ESC = 27;
	unsigned int snakeSize = 0;
	Snake* snake = NULL;
	Board board;
	Food food;
	
	allocateBoard(&board);
	addPartToSnake(&snake);
	spawnFood(&food, &board);
	initConsole();

	// Snake base position.
	snake->x = board.size / 2;
	snake->y = board.size / 2;

	while(1)
	{
		// READ INPUT.
		unsigned int direction = updateDirection(_kbhit());

		// FRAME LOCK.
		if (!framelock())
		{
			continue;
		}

		// UPDATE.
		initBoard(&board);
		addFoodToBoard(&food, &board);
		updateSnake(snake, direction);
		addSnakeToBoard(snake, &board);
		foodController(&food, snake, &board);

		// REFRESH AND DRAW.
		clearScreen();
		printBoard(snake, &board);
	}
	
	deallocateBoard(board);
}