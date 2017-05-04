#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <thread>
#include <conio.h>
#include <string.h>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

using namespace std;
#define MAX_SIZE_SNAKE 100
#define MAX_SIZE_FOOD 2
#define MAX_SPEED 3
#define MAX_SIZE_GATE 5
#define KEYESC VK_ESCAPE
#define MAX_SIZE_LINK 50
#define MAX_SIZE_ENEMY 3

// Du lieu tro choi
extern POINT snake[MAX_SIZE_SNAKE];
extern POINT food[MAX_SIZE_FOOD];
extern POINT gate[MAX_SIZE_GATE];
extern POINT enemy[MAX_SIZE_ENEMY];
extern int CHAR_LOCK;//Huong khong the di chuyen
extern int MOVING;//Huong di chuyen cua snake
extern int SPEED;//Level toc do
extern int HEIGHT_CONSOLE, WIDTH_CONSOLE;
extern int FOOD_INDEX;//So food hien hanh
extern int SIZE_SNAKE;//Kich thuoc snake, luc dau = 6 va toi da =10
extern int STATE;//Trang thai song hay chet
extern int GATE_INDEX;//Trang thai GATE 1:co / 0:khong

void FixConsoleWindow();
void GotoXY(int x, int y);
bool isValid(int x, int y);
void GenerateFood();
void ResetData();
void DrawBoard(int x, int y, int width, int height);
void StartGame();
void ExitGame(HANDLE t);
void PauseGame(HANDLE t);
void Eat();
bool CheckSnakeEatLeft();
bool CheckSnakeEatRight();
bool CheckSnakeEatDown();
bool CheckSnakeEatUp();
void ProcessDead();
void DrawSnake(char* str);
void DrawFood(char* str);
void MoveRight();
void MoveLeft();
void MoveDown();
void MoveUp();
void ThreadFunc();
bool CheckGate();
void GenerateGate();
void DrawGate(char* str);
void SnakeGoToGate();
void ResetAfterGate();
bool SnakeCrashRightGate();
bool SnakeCrashLeftGate();
bool SnakeCrashUpGate();
bool SnakeCrashDownGate();
void DeleteGate();
void SaveGame(char* link);
void SolveSaveGame();
bool LoadGame(char* link);
void SolveLoadGame();
bool CheckEnemy();
void GenerateEnemy();
void DrawEnemy(char* str);
bool EnemyHitSnake();
void EnemyMove();
bool EnemyHitDefence();
void DeleteEnemy();