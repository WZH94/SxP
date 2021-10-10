#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define ENEMY_HEIGHT 3
#define ENEMY_WIDTH 4
#define PLAYER_HEIGHT 2
#define PLAYER_WIDTH 5
#define ENEMY_BOUNDARY_TOP 21
#define ENEMY_BOUNDARY_BOTTOM 53
#define CENTER_BOUNDARY_TOP 36
#define CENTER_BOUNDARY_BOTTOM 38
#define sizex 68
#define sizey 74

char World[74][68];
char Life1[5] = { 221, 221, 221, 221, 221 };
char Life2[5] = { 221, 221, 221, 221, 221 };
int Score1 = 0, Score2 = 0;
int Count;
char empty = ' ';
char playerLine = '_';
int x, y;
int a, b, buffer;

const char *enemy1P1[3][4] = {
	{'/', '~', '~', '\\'},
	{'|', '.', '.', '|'},
	{'\\', '-', '-', '/'}
};

const char *enemy1P2[3][4] = {
	{ '/', '-', '-', '\\' },
	{ '|', '.', '.', '|' },
	{ '\\', '~', '~', '/' }
};

const char player1[2][5] = {
	{'o', '-', '|', '-', 'o'},
	{'o', '-', '-', '-', 'o'}
};

const char player2[2][5] = {
	{ 'o', '-', '-', '-', 'o' },
	{ 'o', '-', '|', '-', 'o' }
};

/* Update the World Map */
void WorldDraw()
{
	printf("\n     LIFE: %s                                Highscore: %d   \n", Life1, Score1);
	for (y = 0; y < sizey; ++y) {
		printf("|");
		for (x = 0; x < sizex; ++x) {
			printf("%c", World[y][x]);
		}

		printf("|");
		printf("\n");
	}
	printf("\n     Highscore: %d                                     LIFE: %s   \n", Score2, Life2);
}

int main()
{
	while (1)
	{
		/* Initialise the world UI */
		for (y = 0; y < sizey; ++y) {
			for (x = 0; x < sizex; ++x)
			{
				/* Spawn Player 1 and Player 2 */
				if (y == 2 && x == (sizex / 2))
				{
					for (b = 0; b < PLAYER_HEIGHT; ++b, ++y, x = sizex / 2)
					{
						for (a = 0; a < PLAYER_WIDTH; ++a, ++x)
						{
							World[y][x] = player1[b][a];
						}

					}
				}

				/* Spawn Player 2 */
				if (y == (sizey - 3) && x == (sizex / 2))
				{

					for (b = 0, a = 0; a < PLAYER_WIDTH; ++a, ++x)
						World[y][x] = player2[b][a];

					++y;
					x = sizex / 2;

					for (b = 1, a = 0; a < PLAYER_WIDTH; ++a, ++x)
						World[y][x] = player2[b][a];
				}

				/* Spawn Enemies */
				if (y > ENEMY_BOUNDARY_TOP && y < ENEMY_BOUNDARY_BOTTOM && x > 3 && x < (sizex - 3)
					&& (y < CENTER_BOUNDARY_TOP || y > CENTER_BOUNDARY_BOTTOM))
				{
					int tempx = x;
					int tempy = y;
					b = 0;

					while (b < ENEMY_HEIGHT)
					{
						for (a = 0; a < ENEMY_WIDTH; ++a, ++tempx)
						{
							if (y < CENTER_BOUNDARY_TOP)
								World[tempy][tempx] = enemy1P1[b][a];
							else
								World[tempy][tempx] = enemy1P2[b][a];
						}

						tempx = x;
						++tempy;
						++b;
					}

					x += ENEMY_WIDTH + 2;

					if (x > (sizex - 3))
						y += ENEMY_HEIGHT;
				}

				/* creates world edge line */
				if (y == 0 || y == sizey - 1)
				{
					World[y][x] = playerLine;
				}

				else
					World[y][x] = empty;
			}
		}

		/* Draw out the world UI */
		system("cls");
		Sleep(100);
		
		WorldDraw();
		getch();
	}
}