#include "solve.h"

void main()
{
	int temp;
	FixConsoleWindow();
	StartGame();
	thread t1(ThreadFunc);
	HANDLE handle_t1 = t1.native_handle();
	while (1)
	{
		temp = toupper(_getch());
		if (STATE == 1)
		{
			if (temp == 'P')
				PauseGame(handle_t1);
			else if (temp == KEYESC)
			{
				ExitGame(handle_t1);
				exit(0);
			}
			else if (temp == 'L')
			{
				PauseGame(handle_t1);
				SolveSaveGame();
			}
			else if (temp == 'T')
			{
				PauseGame(handle_t1);
				SolveLoadGame();
			}
			else
			{
				ResumeThread(handle_t1);
				if ((temp != CHAR_LOCK) && (temp == 'D' || temp == 'A' || temp == 'S' || temp == 'W'))
				{
					if (temp == 'D') CHAR_LOCK = 'A';
					else if (temp == 'W') CHAR_LOCK = 'S';
					else if (temp == 'S') CHAR_LOCK = 'W';
					else CHAR_LOCK = 'D';
					MOVING = temp;
				}
			}
		}
		else
		{
			if (temp == 'Y')
				StartGame();
			else if (temp == 'N')
			{
				ExitGame(handle_t1);
			}
		}
	}
}