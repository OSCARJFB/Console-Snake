/*
		Writen by: Oscar Bergstr�m
		https://github.com/OSCARJFB

		MIT License
		Copyright (c) 2024 Oscar Bergstr�m
*/

#include "Game.h"

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
	printf("%s", msg);
	fgets(buffer, MAX - 1, stdin);

#ifdef _WIN32
	(void)sscanf_s(buffer, "%u", &board->size);
#else
	(void)sscanf(buffer, "%u", &board->size);
#endif
}

static void setBoardSize(Board* board)
{
	while (board->size % 2 != 0 || board->size < MIN_BOARD_SIZE || board->size > MAX_BOARD_SIZE)
	{
		getBoardSize(board);
	}
}

static void initBoard(Board* board)
{
	for (int i = 0; i < board->size; ++i)
	{
		for (int j = 0; j < board->size; ++j)
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

	for (int i = 0; i < board->size; ++i)
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
	for (int i = 0; i < board.size; ++i)
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

static void printBoard(Board* board)
{
	for (int i = 0; i < board->size; ++i)
	{
		for (int j = 0; j < board->size; ++j)
		{
			printf("%c", board->grid[i][j]);
		}
		printf("\n");
	}
}

static int updateDirection(char ch)
{
	static int direction = 0;

	switch (ch)
	{
	case 'w':
		direction = direction != down ? up : down;
		break;
	case 'a':
		direction = direction != right ? left : right;
		break;
	case 's':
		direction = direction != up ? down : up;
		break;
	case 'd':
		direction = direction != left ? right : left;
		break;
	default:
		break;
	}

	return direction;
}

static void updateSnakeHead(Snake* snake, int direction)
{
	switch (direction)
	{
	case up:
		--snake->y;
		break;
	case left:
		--snake->x;
		break;
	case down:
		++snake->y;
		break;
	case right:
		++snake->x;
		break;
	}
}

static void setOldSnakeHeadPosition(int* oldX, int* oldY, int direction)
{
	switch (direction)
	{
	case up:
		++*oldY;
		break;
	case left:
		++*oldX;
		break;
	case down:
		--*oldY;
		break;
	case right:
		--*oldX;
		break;
	}
}

static void updateSnake(Snake* snake, int direction)
{
	updateSnakeHead(snake, direction);
	
	if (snake->next == NULL)
	{
		return;
	}

	int oldX = snake->x, oldY = snake->y;
	setOldSnakeHeadPosition(&oldX, &oldY, direction);

	for (Snake* tail = snake->next; tail != NULL; tail = tail->next)
	{
		int tempX = tail->x;
		int tempY = tail->y;

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

static void spawnFood(Food* food, Board* board)
{
	int slots = 0, spawnPosition = 0;
	int x[MAX_BOARD_SIZE * MAX_BOARD_SIZE], y[MAX_BOARD_SIZE * MAX_BOARD_SIZE];
	
	// Find all free slots, where we can place food.
	for (int i = 0; i < board->size; ++i)
	{
		for (int j = 0; j < board->size; ++j)
		{
			if (board->grid[i][j] != BORDER && board->grid[i][j] != SNAKE)
			{
				x[slots] = j;
				y[slots] = i;
				++slots;
			}
		}
	}

	// Select a random free position and place the food.
	if (slots > 0)
	{
		spawnPosition = rand() % (slots - 1) + 1;
		food->x = x[spawnPosition];
		food->y = y[spawnPosition];
	}
}

static inline bool eatFood(Food* food, Snake* snake)
{
	return (snake->x == food->x && snake->y == food->y);
}

static bool foodController(Food* food, Snake** snake, Board* board)
{
	if (eatFood(food, *snake))
	{
		spawnFood(food, board);
		addPartToSnake(snake);
		return true;
	}

	return false;
}

static void addFoodToBoard(Food* food, Board* board)
{
	for (int i = 0; i < board->size; ++i)
	{
		for (int j = 0; j < board->size; ++j)
		{
			if (food->y == i && food->x == j)
			{
				board->grid[i][j] = FOOD; // x is food.
				break;
			}
		}
	}
}


static bool isCollision(Snake* snake, Board* board)
{
	bool cFlag = false; 

	// Check if the head has collided with the boarder!
	if (snake->x == 0 || snake->x == board->size - 1)
	{
		cFlag = true;
	}
	else if (snake->y == 0 || snake->y == board->size - 1)
	{
		cFlag = true;
	}

	if (snake->next == NULL)
	{
		return cFlag;
	}

	// Else check if the head is collding with the body.
	int x = snake->x, y = snake->y;
	for (Snake* tail = snake->next; tail != NULL; tail = tail->next)
	{
		if (tail->x == x && tail->y == y)
		{
			cFlag = true;
		}
	}

	return cFlag;
}

static void displayScore(bool* isScore)
{
	static int score = 0;
	
	if (*isScore)
	{
		++score;
		*isScore = false;
	}

	printf("Score: %u", score);
}

void run(void)
{
	bool isScore = false;
	Snake* snake = NULL;
	Board board;
	Food food;

	srand((unsigned int)time(NULL));
	allocateBoard(&board);
	addPartToSnake(&snake);
	spawnFood(&food, &board);
	initConsole();

	// Snake base position.
	snake->x = board.size / 2;
	snake->y = board.size / 2;

	while(!isCollision(snake, &board))
	{
		// READ INPUT.
		int direction = updateDirection(kbhit());

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
		isScore = foodController(&food, &snake, &board);

		// REFRESH AND DRAW.
		clearScreen();
		printBoard(&board);
		displayScore(&isScore);
	}
	
	deallocateBoard(board);
}
