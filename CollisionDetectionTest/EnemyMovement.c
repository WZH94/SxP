#include "GameInitialise.h"

void MoveEnemyLeft(int iteration)
{
	/* Clear enemy field to update position */
	for (y = PLAYER_HEIGHT + 2; y < SIZE_Y - PLAYER_HEIGHT - 2; ++y)
	{
		for (x = 0; x < SIZE_X; ++x)
		{
			if (World[y][x] != empty)
				World_Update[y][x] = empty;
		}
	}
	for (y = 0; y < SIZE_Y; ++y)
	{
		for (x = 0; x < SIZE_X; ++x)
		{
			if (y > ENEMY_BOUNDARY_TOP && y < ENEMY_BOUNDARY_BOTTOM && x >(3 - iteration) && x < (SIZE_X - 3 - iteration)
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
							World_Update[tempy][tempx] = enemy1P1[b][a];
						else
							World_Update[tempy][tempx] = enemy1P2[b][a];
					}

					tempx = x;
					++tempy;
					++b;
				}

				x += ENEMY_WIDTH + 2;

				if (x > SIZE_X - 3 - iteration)
					y += ENEMY_HEIGHT;
			}

		}
	}
}

void MoveEnemyRight(int iteration)
{
	/* Clear enemy field to update position */
	for (y = PLAYER_HEIGHT + 2; y < SIZE_Y - PLAYER_HEIGHT - 2; ++y)
	{
		for (x = 0; x < SIZE_X; ++x)
		{
			if (World[y][x] != empty)
				World_Update[y][x] = empty;
		}
	}
	for (y = 0; y < SIZE_Y; ++y)
	{
		for (x = 0; x < SIZE_X; ++x)
		{
			if (y > ENEMY_BOUNDARY_TOP && y < ENEMY_BOUNDARY_BOTTOM && x >(3 - iteration) && x < (SIZE_X - 3 - iteration)
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
							World_Update[tempy][tempx] = enemy1P1[b][a];
						else
							World_Update[tempy][tempx] = enemy1P2[b][a];
					}

					tempx = x;
					++tempy;
					++b;
				}

				x += ENEMY_WIDTH + 2;

				if (x > SIZE_X - 3 - iteration)
					y += ENEMY_HEIGHT;
			}

		}
	}
}
