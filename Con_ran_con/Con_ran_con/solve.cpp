#include "solve.h"

// Du lieu tro choi
POINT snake[MAX_SIZE_SNAKE];
POINT food[MAX_SIZE_FOOD];
POINT gate[MAX_SIZE_GATE];
POINT enemy[MAX_SIZE_ENEMY];
int CHAR_LOCK;//Huong khong the di chuyen
int MOVING;//Huong di chuyen cua snake
int SPEED;//Level toc do
int HEIGHT_CONSOLE, WIDTH_CONSOLE;
int FOOD_INDEX;//So food hien hanh
int SIZE_SNAKE;//Kich thuoc snake, luc dau = 6 va toi da =10
int STATE;//Trang thai song hay chet
int GATE_INDEX;//Trang thai GATE 1:co / 0:khong
int ENEMY_INDEX;//Trang thai ENEMY 1:co / 0:khong
char MSSV[8] = "1612628";

//Lay thoi gian
clock_t duration = clock();
//Co dinh man hinh console
void FixConsoleWindow()
{
	HWND consoleWindow = GetConsoleWindow();
	LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
	style = style&~(WS_MAXIMIZEBOX)&~(WS_THICKFRAME);
	SetWindowLong(consoleWindow, GWL_STYLE, style);
}
void GotoXY(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
//Thay doi mau
WORD GetConsoleTextAttribute(HANDLE hcon)
{
	CONSOLE_SCREEN_BUFFER_INFO con_info;
	GetConsoleScreenBufferInfo(hcon, &con_info);
	return con_info.wAttributes;
}
void ChangeTextColor(char str)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	const int saved_colors(GetConsoleTextAttribute(hConsole));

	SetConsoleTextAttribute(hConsole, 11 | FOREGROUND_INTENSITY | BACKGROUND_INTENSITY);
	cout << str;
	SetConsoleTextAttribute(hConsole, saved_colors);
}
//Xay dung ham ResetData
bool isValid(int x, int y)
{
	for (int i = 0; i < SIZE_SNAKE; i++)
	{
		if (snake[i].x == x && snake[i].y == y)
		{
			return false;
		}
	}
	return true;
}
void GenerateFood()
{
	int x, y;
	srand(time(NULL));
	for (int i = 0; i < MAX_SIZE_FOOD; i++)
	{
		do
		{
			x = rand() % (WIDTH_CONSOLE - 1) + 1;
			y = rand() % (HEIGHT_CONSOLE - 1) + 1;
		} while (isValid(x, y) == false);
		food[i] = { x, y };
	}
}
void ResetData()
{
	CHAR_LOCK = 'A', MOVING = 'D', SPEED = 1;
	FOOD_INDEX = 0, WIDTH_CONSOLE = 100, HEIGHT_CONSOLE = 25, SIZE_SNAKE = 7;
	// khoi tao gia tri mac dinh

	DeleteGate();
	snake[0] = { 10,5 }; snake[1] = { 11,5 };
	snake[2] = { 12,5 }; snake[3] = { 13,5 };
	snake[4] = { 14,5 }; snake[5] = { 15,5 };
	snake[6] = { 16,5 };
	GenerateFood();
}
void DrawBoard(int x, int y, int width, int height)
{
	int curPosX = 0, curPosY = 0;
	GotoXY(x, y);

	for (int i = 0; i <= width; i++) cout << '=';

	for (int i = 1; i < height + y; i++)
	{
		GotoXY(x, i); cout << "X";
		GotoXY(x + width, i); cout << "X";
	}
	GotoXY(0, height);
	for (int i = 0; i <= width; i++)cout << '=';
	GotoXY(curPosX, curPosY);
}
void StartGame()
{
	system("cls");
	ResetData();//Khoi tao game
	DrawBoard(0, 0, WIDTH_CONSOLE, HEIGHT_CONSOLE);
	system("color 3E");
	STATE = 1;
}
void ExitGame(HANDLE t)
{
	system("cls");
	TerminateThread(t, 0);
	exit(0);
}
void PauseGame(HANDLE t)
{
	SuspendThread(t);
}
void Eat()
{
	snake[SIZE_SNAKE] = food[FOOD_INDEX];

	if (FOOD_INDEX == MAX_SIZE_FOOD - 1)
	{
		FOOD_INDEX = MAX_SIZE_FOOD;
		SIZE_SNAKE++;
		GenerateFood();
	}
	else
	{
		FOOD_INDEX++;
		SIZE_SNAKE++;
	}
}
//Check Snake eat itseft
bool CheckSnakeEatLeft()
{
	for (int i = 0; i < SIZE_SNAKE - 1; i++)
	{
		if (snake[SIZE_SNAKE - 1].x - 1 == snake[i].x && snake[SIZE_SNAKE - 1].y == snake[i].y)
			return true;
	}
	return false;
}
bool CheckSnakeEatRight()
{
	for (int i = 0; i < SIZE_SNAKE - 1; i++)
	{
		if (snake[SIZE_SNAKE - 1].x + 1 == snake[i].x && snake[SIZE_SNAKE - 1].y == snake[i].y)
			return true;
	}
	return false;
}
bool CheckSnakeEatDown()
{
	for (int i = 0; i < SIZE_SNAKE - 1; i++)
	{
		if (snake[SIZE_SNAKE - 1].x == snake[i].x && snake[SIZE_SNAKE - 1].y + 1 == snake[i].y)
			return true;
	}
	return false;
}
bool CheckSnakeEatUp()
{
	for (int i = 0; i < SIZE_SNAKE - 1; i++)
	{
		if (snake[SIZE_SNAKE - 1].x == snake[i].x && snake[SIZE_SNAKE - 1].y - 1 == snake[i].y)
			return true;
	}
	return false;
}

void ProcessDead()
{
	STATE = 0;
	DeleteGate();
	DeleteEnemy();
	Beep(700, 500);
	PlaySound(TEXT("tf_nemesis.wav"), NULL, SND_ASYNC);

	GotoXY(0, HEIGHT_CONSOLE + 2);
	printf("You are Dead, type Y to continue or N to exit.");
}

void DrawSnake(char* str)
{
	if (str == " ")
	{
		for (int i = 0; i < SIZE_SNAKE; i++)
		{
			GotoXY(snake[i].x, snake[i].y);
			printf(str);
		}
	}
	else
	{
		for (int i = 0; i < SIZE_SNAKE; i++)
		{
			GotoXY(snake[i].x, snake[i].y);
			ChangeTextColor(MSSV[i % 7]);
		}
	}
}
void DrawFood(char* str)
{
	GotoXY(food[FOOD_INDEX].x, food[FOOD_INDEX].y);
	printf(str);
}
//Kiem tra ran dung vao cong
bool SnakeCrashLeftGate()
{
	if ((snake[SIZE_SNAKE - 1].x + 1 == gate[3].x && snake[SIZE_SNAKE - 1].y == gate[3].y)
		|| (snake[SIZE_SNAKE - 1].x + 1 == gate[4].x&&snake[SIZE_SNAKE - 1].y == gate[4].y))
	{
		return true;
	}
	return false;
}
bool SnakeCrashRightGate()
{
	if ((snake[SIZE_SNAKE - 1].x - 1 == gate[0].x && snake[SIZE_SNAKE - 1].y == gate[0].y)
		|| (snake[SIZE_SNAKE - 1].x - 1 == gate[1].x&&snake[SIZE_SNAKE - 1].y == gate[1].y))
	{
		return true;
	}
	return false;
}
bool SnakeCrashUpGate()
{
	if ((snake[SIZE_SNAKE - 1].x == gate[1].x && snake[SIZE_SNAKE - 1].y + 1 == gate[1].y)
		|| (snake[SIZE_SNAKE - 1].x == gate[2].x&&snake[SIZE_SNAKE - 1].y + 1 == gate[2].y)
		|| (snake[SIZE_SNAKE - 1].x == gate[3].x&&snake[SIZE_SNAKE - 1].y + 1 == gate[3].y))
	{
		return true;
	}
	return false;
}
bool SnakeCrashDownGate()
{
	if ((snake[SIZE_SNAKE - 1].x == gate[0].x && snake[SIZE_SNAKE - 1].y - 1 == gate[0].y)
		|| (snake[SIZE_SNAKE - 1].x == gate[4].x && snake[SIZE_SNAKE - 1].y - 1 == gate[4].y))
	{
		return true;
	}
	return false;
}
//Ran di chuyen
void MoveRight()
{
	if (snake[SIZE_SNAKE - 1].x + 1 == WIDTH_CONSOLE || SnakeCrashLeftGate() || CheckSnakeEatRight() || EnemyHitSnake())
	{
		ProcessDead();
	}
	else
	{
		if (snake[SIZE_SNAKE - 1].x + 1 == food[FOOD_INDEX].x && snake[SIZE_SNAKE - 1].y == food[FOOD_INDEX].y)
		{
			Eat();
		}
		for (int i = 0; i < SIZE_SNAKE - 1; i++)
		{
			snake[i].x = snake[i + 1].x;
			snake[i].y = snake[i + 1].y;
		}
		snake[SIZE_SNAKE - 1].x++;
		if (FOOD_INDEX == MAX_SIZE_FOOD)
		{
			FOOD_INDEX = 0;
			GATE_INDEX = 1;
			GenerateGate();
		}
	}
}
void MoveLeft()
{
	if (snake[SIZE_SNAKE - 1].x - 1 == 0 || SnakeCrashRightGate() || CheckSnakeEatLeft() || EnemyHitSnake())
	{
		ProcessDead();
	}
	else
	{
		if (snake[SIZE_SNAKE - 1].x - 1 == food[FOOD_INDEX].x && snake[SIZE_SNAKE - 1].y == food[FOOD_INDEX].y)
		{
			Eat();
		}
		for (int i = 0; i < SIZE_SNAKE - 1; i++)
		{
			snake[i].x = snake[i + 1].x;
			snake[i].y = snake[i + 1].y;
		}
		snake[SIZE_SNAKE - 1].x--;
		if (FOOD_INDEX == MAX_SIZE_FOOD)
		{
			FOOD_INDEX = 0;
			GATE_INDEX = 1;
			GenerateGate();
		}
	}
}
void MoveDown()
{
	if (snake[SIZE_SNAKE - 1].y + 1 == HEIGHT_CONSOLE || SnakeCrashUpGate() || CheckSnakeEatDown() || EnemyHitSnake())
	{
		ProcessDead();
	}
	else
	{
		if (snake[SIZE_SNAKE - 1].y + 1 == food[FOOD_INDEX].y&&snake[SIZE_SNAKE - 1].x == food[FOOD_INDEX].x)
		{
			Eat();
		}
		for (int i = 0; i < SIZE_SNAKE - 1; i++)
		{
			snake[i].x = snake[i + 1].x;
			snake[i].y = snake[i + 1].y;
		}
		snake[SIZE_SNAKE - 1].y++;
		if (FOOD_INDEX == MAX_SIZE_FOOD)
		{
			FOOD_INDEX = 0;
			GATE_INDEX = 1;
			GenerateGate();
		}
	}
}
void MoveUp()
{
	if (snake[SIZE_SNAKE - 1].y - 1 == 0 || SnakeCrashDownGate() || CheckSnakeEatUp() || EnemyHitSnake())
	{
		ProcessDead();
	}
	else
	{
		if (snake[SIZE_SNAKE - 1].y - 1 == food[FOOD_INDEX].y && snake[SIZE_SNAKE - 1].x == food[FOOD_INDEX].x)
		{
			Eat();
		}
		for (int i = 0; i < SIZE_SNAKE - 1; i++)
		{
			snake[i].x = snake[i + 1].x;
			snake[i].y = snake[i + 1].y;
		}
		snake[SIZE_SNAKE - 1].y--;
		if (snake[SIZE_SNAKE - 1].y - 1 == gate[2].y && snake[SIZE_SNAKE - 1].x == gate[2].x)
		{
			SnakeGoToGate();
			ResetAfterGate();
		}
		else if (FOOD_INDEX == MAX_SIZE_FOOD)
		{
			FOOD_INDEX = 0;
			GATE_INDEX = 1;
			GenerateGate();
		}
	}
}
// Ham dinh nghia chuyen dong
void ThreadFunc()
{
	while (1)
	{
		if (!((duration * 1000000000) % 5000))
		{
			ENEMY_INDEX = 1;
			GenerateEnemy();
		}
		if (STATE == 1)
		{
			if (GATE_INDEX == 1)
			{
				DrawGate(" ");
			}
			else
			{
				DrawFood(" ");
			}
			if (ENEMY_INDEX == 1)
			{
				DrawEnemy(" ");
				EnemyMove();
			}
			DrawSnake(" ");
			switch (MOVING)
			{
			case'A':
				MoveLeft();
				break;
			case'D':
				MoveRight();
				break;
			case 'S':
				MoveDown();
				break;
			case'W':
				MoveUp();
				break;
			}
			if (GATE_INDEX == 1)
			{
				DrawGate("A");
			}
			else
			{
				DrawFood("0");
			}
			DrawSnake("1");
			if (ENEMY_INDEX == 1)
			{
				DrawEnemy("1");
				EnemyMove();
			}
			Sleep(150 / (SPEED));//sleep theo toc do SPEED
			if (STATE == 0)
			{
				DrawSnake(" ");
				DrawFood(" ");
				GotoXY(WIDTH_CONSOLE / 3, HEIGHT_CONSOLE / 2);
				cout << "GAME OVER";
			}
			if (EnemyHitDefence())
			{
				DeleteEnemy();
			}
		}
	}
}
//Tao ra canh cong
bool CheckGate()
{
	for (int i = 0; i < SIZE_SNAKE; i++)
	{
		for (int j = 0; j < MAX_SIZE_GATE; j++)
		{
			if (snake[i].x == gate[j].x && snake[i].y == gate[j].y)
				return true;
		}
	}
	return false;
}
void GenerateGate()
{
	int x, y;
	srand(time(NULL));
	do
	{
		x = rand() % (WIDTH_CONSOLE - 2) + 2;
		y = rand() % (HEIGHT_CONSOLE / 2 - 2) + 2;
		gate[0] = { x + 1,y + 1 };
		gate[1] = { x + 1,y };
		gate[2] = { x,y };
		gate[3] = { x - 1,y };
		gate[4] = { x - 1,y + 1 };
	} while (!CheckGate);
}
void DrawGate(char* str)
{
	for (int i = 0; i < MAX_SIZE_GATE; i++)
	{
		GotoXY(gate[i].x, gate[i].y);
		cout << str;
	}
}
void SnakeGoToGate()
{
	GATE_INDEX = 0;
}
void ResetAfterGate()
{
	system("cls");
	CHAR_LOCK = 'A', MOVING = 'D';
	FOOD_INDEX = 0, WIDTH_CONSOLE = 100, HEIGHT_CONSOLE = 25;
	// khoi tao gia tri mac dinh
	DeleteGate();
	for (int i = 0; i < SIZE_SNAKE; i++)
	{
		snake[i] = { 10 + i,10 };
	}
	DrawBoard(0, 0, WIDTH_CONSOLE, HEIGHT_CONSOLE);
	system("color 3E");
	if (SPEED == MAX_SPEED) SPEED = 1;
	else SPEED++;
}
void DeleteGate()
{
	GATE_INDEX = 0;
	for (int i = 0; i < MAX_SIZE_GATE; i++)
	{
		gate[i] = { WIDTH_CONSOLE + 4 ,0 };
	}
	DrawGate(" ");
}
//Ham luu lai trang thai con ran con
void SaveGame(char* link)
{
	FILE* FileSave = fopen(link, "w+b");
	fprintf(FileSave, "%d %d %d %d %d %d %d %d ", SPEED, FOOD_INDEX, food[FOOD_INDEX].x, food[FOOD_INDEX].y, MOVING, CHAR_LOCK, SIZE_SNAKE, GATE_INDEX);

	for (int i = 0; i < SIZE_SNAKE; i++)
	{
		fprintf(FileSave, "%d %d ", snake[i].x, snake[i].y);
	}
	if (GATE_INDEX == 1)
	{
		for (int i = 0; i < MAX_SIZE_GATE; i++)
		{
			fprintf(FileSave, "%d %d ", gate[i].x, gate[i].y);
		}
	}
	fclose(FileSave);
}
void SolveSaveGame()
{
	char s[MAX_SIZE_LINK];
	GotoXY(20, 10);
	cout << "Nhap vao duong dan ban muon luu game: ";
	fgets(s, MAX_SIZE_LINK, stdin);
	strtok(s, "\n");
	SaveGame(s);
	system("cls");
	DrawBoard(0, 0, WIDTH_CONSOLE, HEIGHT_CONSOLE);
}
bool LoadGame(char* link)
{
	FILE* FileLoad = fopen(link, "r+b");
	if (FileLoad == NULL) { return false; }
	else
	{
		fscanf(FileLoad, "%d %d %d %d %d %d %d %d ", &SPEED, &FOOD_INDEX, &food[FOOD_INDEX].x, &food[FOOD_INDEX].y, &MOVING, &CHAR_LOCK, &SIZE_SNAKE, &GATE_INDEX);
		for (int i = 0; i < SIZE_SNAKE; i++)
		{
			fscanf(FileLoad, "%d %d ", &snake[i].x, &snake[i].y);
		}
		if (GATE_INDEX == 1)
		{
			for (int i = 0; i < MAX_SIZE_GATE; i++)
			{
				fscanf(FileLoad, "%d %d ", &gate[i].x, &gate[i].y);
			}
		}
		fclose(FileLoad);
	}
	return true;
}
void SolveLoadGame()
{
	char s[MAX_SIZE_LINK];
	DrawSnake(" ");
	DrawFood(" ");
	DeleteGate();
	GotoXY(20, 10);
	cout << "Nhap vao duong dan ban muon load game: ";
	fgets(s, MAX_SIZE_LINK, stdin);
	strtok(s, "\n");
	system("cls");
	DrawBoard(0, 0, WIDTH_CONSOLE, HEIGHT_CONSOLE);
	if (!LoadGame(s))
	{
		GotoXY(20, 10);
		cout << "Khong the mo file de load game";
		system("cls");
		DrawBoard(0, 0, WIDTH_CONSOLE, HEIGHT_CONSOLE);
		return;
	}
}
//tao ra Enemy
bool CheckEnemy()
{
	for (int i = 0; i < MAX_SIZE_ENEMY; i++)
	{
		for (int j = 0; j < MAX_SIZE_GATE; j++)
		{
			if (GATE_INDEX == 0 && enemy[i].x == gate[j].x && enemy[i].y == enemy[j].y)
				return true;
		}
	}
	return false;
}
void GenerateEnemy()
{
	int x, y;
	srand(time(NULL));
	do
	{
		x = WIDTH_CONSOLE - 3;
		y = rand() % (HEIGHT_CONSOLE - 2) + 2;
		enemy[0] = { x,y };
		enemy[1] = { x - 1,y };
		enemy[2] = { x - 2,y };
	} while (!CheckEnemy());
}
void DrawEnemy(char* str)
{
	if (str != " ")
	{
		GotoXY(enemy[0].x, enemy[0].y);
		cout << "<";
		for (int i = 1; i < MAX_SIZE_ENEMY; i++)
		{
			GotoXY(enemy[i].x, enemy[i].y);
			cout << str;
		}
	}
	for (int i = 1; i < MAX_SIZE_ENEMY; i++)
	{
		GotoXY(enemy[i].x, enemy[i].y);
		cout << " ";
	}
}
bool EnemyHitSnake()
{
	for (int i = 0; i < SIZE_SNAKE; i++)
	{
		if (enemy[0].x == snake[i].x && enemy[0].y == snake[i].y)
		{
			return true;
		}
	}
	return false;
}
void EnemyMove()
{
	for (int i = 0; i < MAX_SIZE_ENEMY; i++)
	{
		enemy[i].x = enemy[i].x - 1;
		enemy[i].y = enemy[i].y;
	}
}
bool EnemyHitDefence()
{
	if (enemy[0].x + 1 == 0)
		return true;
	return false;
}
void DeleteEnemy()
{
	ENEMY_INDEX = 0;
	for (int i = 0; i < MAX_SIZE_ENEMY; i++)
	{
		enemy[i] = { WIDTH_CONSOLE + 4,0 };
	}
	DrawEnemy(" ");
}