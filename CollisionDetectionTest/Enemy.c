/******************************************************************************
All content © 2017 Digipen Institute of Technology Singapore, all rights reserved

filename			Enemy.c
author				Wong Zhihao
DP email			zhihao.wong@digipen.edu
course				RTIS

Brief Description:
Contents of enemy object of the game, and handles the printing, clearing
and changes to enemies in game.

******************************************************************************/

#include "Windows.h"				 /* free, strncpy, malloc */
#include "WorldData.h"				 /* WorldData_GoToXY function */
#include "Stage.h"					 /* Stage object and data */
#include "EnemyAI.h"				 /* Enemy AI object and data */
#include "TileMapManager.h"			 /* Tile Manager object and data */
#include "EnemyTypes.h"				 /* Enemy Types and functions */
#include "Ball.h"					 /* Ball object and functions */
#include "Jebus.h"					 /* Jebus functions */
#include "Ball_CollisionManager.h"	 /* Ball collision functions */
#include "Player.h"					 /* Player object and functions */
#include "FMODInit.h"				 /* Sound object and functions */
#include "Enemy.h"					 /* Enemy object and functions */

/* array to clear the enemy for redrawing */
const char *enemyClear[ENEMY_HEIGHT] = {
	"    ",
	"    ",
	"    "
};

/* Create a new Enemy object with the provided data */
Enemy* Enemy_Create(Stage *stage, int x_pos, int y_pos, int player_mult, int id, TileMapManager *tileManager, int hydra_type)
{
	int i;
	int y;
	int repeat = 1;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	/* Save current attributes */
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;

	Enemy *enemy = NULL; /* the new item to be returned */

	/* allocate the memory for a new object */
	enemy = (Enemy*)malloc(sizeof(Enemy));
	if (enemy == NULL)
	{
		return NULL; /* malloc can fail */
	}

	enemy->x_Pos = x_pos;		/* Initialise the very first enemy's x coord */
	enemy->y_Pos = y_pos;		/* Initialise the very first enemy's y coord */
	enemy->alive = TRUE;		/* Initialise enemy to be alive */
	enemy->id = id;				/* Which player it is facing */
	enemy->fireActive = 0;		/* Initialises enemy to not be fire active */
	enemy->fireReload = 2000;	/* Minimum frames in between shots */
	enemy->fireReloader = 0;	/* Initialises reload time to 0 */
	enemy->full_HP = 1;			/* Flag for whether enemy is at full HP or not */
	enemy->is_hydra = 0;		/* 1 means the enemy is a hydra, -1 means enemy was a hydra and might have a weird position */
	enemy->hydra_link = -1;		/* The other hydra that spawned with this hydra, -1 if there is not any */
	
	/* If enemy is hydra, it already has a type, go straight into initialising it */
	if (hydra_type >= 1)
	{
		/* Enemy type initialised to previous enemy type */
		enemy->type = hydra_type;
		/* Moves on to other initialisation functions */
		EnemyType_Switcher(enemy);
	}

	/* Randomly choose a type for the enemy */
	else
		/* If enemy type is already at the max of the stage, repeat another enemy type to choose */
		while (repeat)
			repeat = EnemyType_Assigner(enemy, stage);

	/* Sets the fire chance of each enemy */
	EnemyFireChance(stage, enemy);

	/* If enemy is not a hydra, sets the normal colours */
	if (enemy->is_hydra == 0)
		switch (enemy->type)
		{
		case 0:
		case 1: SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
			break;
		case 2:
		case 3:
		case 4: SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
			break;
		case 5: SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
			break;
		case 6: SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			break;
		case 7: SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			break;
		default:
			break;
		}

	/* Enemy is hydra, sets it to be white */
	if (enemy->is_hydra == 1)
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

	/* draws the enemy*/
	for (i = 0, y = y_pos; i < ENEMY_HEIGHT; ++i, y += player_mult)
	{
		if (enemy->id == 1)
			TileMapManager_Enemy(enemy, 1, tileManager, 3);
		if (enemy->id == 2)
			TileMapManager_Enemy(enemy, -1, tileManager, 3);
		WorldData_GoToXY(x_pos, y);
		printf("%s", enemy->enemyStyle[i]);
	}
	SetConsoleTextAttribute(hConsole, saved_attributes);
	/* return the new object */
	return enemy;
}

/* Free the memory associated with a given Enemy object */
void Enemy_Free(Enemy** enemyPtr)
{
	/* safety check on the parameters */
	if ((enemyPtr == NULL) || (*enemyPtr == NULL))
	{
		return;  /* take no action if the parameters are invalid */
	}

	/* free the given item */
	free(*enemyPtr);

	/* clear the provided pointer */
	*enemyPtr = NULL;
}

/* Sets the art of the enemy */
void EnemyCopyArt(Enemy *enemy, const char *enemyChar[])
{
	int i;	/* Loop counter */

	/* copy the data from the parameters into the new object */
	for (i = 0; i < ENEMY_HEIGHT; ++i)
		strncpy_s(enemy->enemyStyle[i], ENEMY_WIDTH + 1, enemyChar[i], ENEMY_WIDTH + 1);
}

/* Sets the chance out of fireChance each enemy can fire every frame, higher is lower chance sorry */
void EnemyFireChance(Stage *stage, Enemy *enemy)
{
	/* If play mode is PvP set the fire chance to be more often */
	if (stage->play_type == PvP)
		switch (enemy->type)
		{
			/* Shooter */
		case 2: enemy->fireChance = 26000;
			break;
			/* Big Shot */
		case 3: enemy->fireChance = 32000;
			break;
			/* Pea */
		case 4: enemy->fireChance = 13000;
			break;
			/* Arc */
		case 5: enemy->fireChance = 33000;
			break;
			/* Torpedo */
		case 6: enemy->fireChance = 71000;
			break;
		}

	else
		/* Play mode is Co-Op */
		switch (enemy->type)
		{
			/* Shooter */
		case 2: enemy->fireChance = 36000;
			break;
			/* Big Shot */
		case 3: enemy->fireChance = 46000;
			break;
			/* Pea */
		case 4: enemy->fireChance = 21000;
			break;
			/* Arc */
		case 5: enemy->fireChance = 51000;
			break;
			/* Torpedo */
		case 6: enemy->fireChance = 100000;
			break;
		}
}

/* Initiaises the enemies based on the stage */
void Enemy_Spawn(Stage* stage, Enemy* enemy[], Enemy_AI* enemy_AI, TileMapManager *tileManager, int id, Sound* sound)
{
	int i;			 /* loop variable */
	int x, y;		 /* x and y coordinates of enemy */
	int row_enemies; /* Limits the number of enemies per row to create based on stage data */

	/* Goes through every enemy */
	for (i = 0, row_enemies = 0, x = stage->x_border, y = enemy_AI->y_spawn; i < stage->total_enemies; ++i)
	{
		/* Checks if there are still enemies to create in it's current row */
		if (row_enemies < stage->num_enemies)
		{
			/* Create enemy */
			enemy[i] = Enemy_Create(stage, x, y, enemy_AI->p_mult, id, tileManager, 0);
			/* Moves over to the next enemy's x coord along the row */
			x += ENEMY_WIDTH + stage->horizontal_gap;
			++row_enemies;
		}
		/* All enemies of the row printed, move to next row */
		else
		{
			/* Resets x coord to the start of the new row */
			x = stage->x_border;
			/* Moves over to next row y coord */
			y += enemy_AI->p_mult * (ENEMY_HEIGHT + stage->vertical_gap);
			/* Create enemy */
			enemy[i] = Enemy_Create(stage, x, y, enemy_AI->p_mult, id, tileManager, 0);
			/* 1 enemy has been printed in that row already */
			row_enemies = 1;
			/* Moves over to the next enemy's x coord along the row */
			x += ENEMY_WIDTH + stage->horizontal_gap;
		}
		
		/* Plays a sound when each enemy builds */
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\StageEnemyBuild.wav");
		playSound_Low(sound);
		/* You can see the enemies being build */
		Sleep(30);
	}

	/* Resets the number of enemies spawned for the next player */
	stage->TYPE0_tracker = 0;
	stage->TYPE1_tracker = 0;
	stage->TYPE2_tracker = 0;
	stage->TYPE3_tracker = 0;
	stage->TYPE4_tracker = 0;
	stage->TYPE5_tracker = 0;
	stage->TYPE6_tracker = 0;
	stage->TYPE7_tracker = 0;
	stage->TYPE8_tracker = 0;
}

/* Clears the enemies for redrawing */
void Enemy_Clear(Stage* stage, Enemy* enemy[], Enemy_AI* enemy_AI, TileMapManager *tileManager)
{
	int i, j; /* loop variables */
	int y;	  /* y coordinate of enemy string */
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	/* Save current attributes */
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;


	/* Loops through total number of enemies per stage based on stage data */
	for (j = 0; j < stage->total_and_hydra; ++j)
	{
		if (enemy[j] != NULL)
		{
			/* Loops through the height of the enemy */
			for (i = 0, y = enemy[j]->y_Pos; i < ENEMY_HEIGHT; ++i, y += enemy_AI->p_mult)
			{
				/* Checks if enemy is alive */
				if (enemy[j]->alive == TRUE)
				{

					switch (enemy[j]->type)
					{
					case 0:
					case 1: SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
						break;
					case 2:
					case 3:
					case 4: SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
						break;
					case 5: SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
						break;
					case 6: SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
						break;
					case 7: SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
						break;
					default:
						break;
					}

					
					if (enemy[j]->is_hydra == 1)
						SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
					TileMapManager_Enemy(enemy[j], enemy_AI->p_mult, tileManager, 0);
					WorldData_GoToXY(enemy[j]->x_Pos, y);
					printf("%s", enemyClear[i]);
					SetConsoleTextAttribute(hConsole, saved_attributes);
				}
				/* Enemy is dead, do not print current enemy */
				else break;
			}
		}
	}
}

/* Clears individual enemy */
void Enemy_IndvClear(Enemy* enemy, Enemy_AI* enemy_AI, TileMapManager *tileManager)
{
	int i; /* loop variables */
	int y;	  /* y coordinate of enemy string */
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	/* Save current attributes */
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;

	/* Loops through the height of the enemy and sets their colour */
	for (i = 0, y = enemy->y_Pos; i < ENEMY_HEIGHT; ++i, y += enemy_AI->p_mult)
	{
		switch (enemy->type)
		{
		case 0:
		case 1: SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
			break;
		case 2:
		case 3:
		case 4: SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
			break;
		case 5: SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
			break;
		case 6: SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			break;
		case 7: SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			break;
		default:
			break;
		}

		/* Different colour for hydra */
		if (enemy->is_hydra == 1)
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

		/* Clears the enemy from Tile Manager and play area */
		TileMapManager_Enemy(enemy, enemy_AI->p_mult, tileManager, 0);
		WorldData_GoToXY(enemy->x_Pos, y);
		printf("%s", enemyClear[i]);
		SetConsoleTextAttribute(hConsole, saved_attributes);
	}
}

/* Reprints the enemies in their new positions */
void Enemy_Print(Stage* stage, Enemy* enemy[], Enemy_AI* enemy_AI, TileMapManager *tileManager)
{
	int i, j; /* Loop variables */
	int y;	  /* y coordinate of enemy string */
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	/* Save current attributes */
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;


	/* Loops through total number of enemies per stage based on stage data */
	for (j = 0; j < stage->total_and_hydra; ++j)
	{
		if (enemy[j] != NULL)
		{
			/* Loops through the height of the enemy */
			for (i = 0, y = enemy[j]->y_Pos; i < ENEMY_HEIGHT; ++i, y += enemy_AI->p_mult)
			{
				/* Checks if enemy is alive */
				if (enemy[j]->alive == TRUE)
				{
					/* Sets the colours for each enemy */
					switch (enemy[j]->type)
					{
					case 0:
					case 1: SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
						break;
					case 2:
					case 3:
					case 4: SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
						break;
					case 5: SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
						break;
					case 6: SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
						break;
					case 7: SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
						break;
					default:
						break;
					}

					/* Different colour for hydra */
					if (enemy[j]->is_hydra == 1)
						SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

					/* Prints the enemy */
					TileMapManager_Enemy(enemy[j], enemy_AI->p_mult, tileManager, 3);
					WorldData_GoToXY(enemy[j]->x_Pos, y);
					printf("%s", enemy[j]->enemyStyle[i]);

					SetConsoleTextAttribute(hConsole, saved_attributes);
				}
				/* Enemy is dead, do not print current enemy */
				else break;
			}
		}
	}
}

/* Prints individual enemy */
void Enemy_IndvPrint(Enemy* enemy, Enemy_AI* enemy_AI, TileMapManager *tileManager)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	/* Save current attributes */
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;
	int i; /* loop variables */
	int y; /* y coordinate of enemy string */

	/* Loops through the height of the enemy and sets their colour */
	for (i = 0, y = enemy->y_Pos; i < ENEMY_HEIGHT; ++i, y += enemy_AI->p_mult)
	{
		switch (enemy->type)
		{
		case 0:
		case 1: SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
			break;
		case 2:
		case 3:
		case 4: SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
			break;
		case 5: SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
			break;
		case 6: SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			break;
		case 7: SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			break;
		default:
			break;
		}

		if (enemy->is_hydra == 1)
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

		/* Prints the enemy */
		TileMapManager_Enemy(enemy, enemy_AI->p_mult, tileManager, 3);
		WorldData_GoToXY(enemy->x_Pos, y);
		printf("%s", enemy->enemyStyle[i]);

		SetConsoleTextAttribute(hConsole, saved_attributes);
	}
}

/* Prints individual enemy for Menu */
void Enemy_IndvPrint_Menu(Enemy* enemy)
{
	int i; /* loop variables */
	int y; /* y coordinate of enemy string */

		   /* Loops through the height of the enemy */
	for (i = 0, y = enemy->y_Pos; i < ENEMY_HEIGHT; ++i, ++y)
	{

		WorldData_GoToXY(enemy->x_Pos, y);
		printf("%s", enemy->enemyStyle[i]);
	}
}

/* Moves the enemies together */
void Enemy_Movement(Stage *stage, Enemy *enemy[], Enemy_AI* enemy_AI, int x, int y, TileMapManager *tileManager, Ball *ball1, Ball *ball2,
	Powerup *powerup[], Sound *sound)
{
	int i; /* Loop variable */

	/* Loop through each enemy facing target player and updates their positions accordingly */
	for (i = 0; i < stage->total_and_hydra; i++)
	{
		/* Checks if the enemy exists */
		if (enemy[i] != NULL)
		{
			int result;	/* What it collided with */

			enemy[i]->x_Pos += x;	/* Updates x position of enemy */
			enemy[i]->y_Pos += y;	/* Updates y position of enemy */

			/* Finds what the enemy collided with */
			result = TileMapManager_Collision_Enemy(tileManager, enemy[i]);

			/* Enemy moved into ball 1 */
			if (result == 4)
			{
				/* Handles collision detection of enemy to ball */
				Enemy_Ball_CollisionDetection(stage, enemy[i], enemy_AI, tileManager, sound, ball1, powerup, x, y);
			}

			/* Enemy moved into ball 2 */
			else if (result == 5)
			{
				/* Handles collision detection of enemy to ball */
				Enemy_Ball_CollisionDetection(stage, enemy[i], enemy_AI, tileManager, sound, ball2, powerup, x, y);
			}
		}
	}
}

/* Enemy collides with the ball */
void Enemy_Ball_CollisionDetection(Stage* stage, Enemy* enemy, Enemy_AI* enemy_AI, TileMapManager *tileManager,
	Sound *sound, Ball *ball, Powerup *powerup[], int enemy_x_change, int enemy_y_change)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	/* Save current attributes */
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;

	/*Clears ball at old position*/
	TileMapManager_Ball(ball, tileManager, 0);
	WorldData_GoToXY(ball->x_ballpos, ball->y_ballpos);
	printf(" ");

	/* Handles collision detection of ball to enemy */
	Ball_Enemy_CollisionDetection(ball->x_ballpos, ball->y_ballpos, ball->x_ballpos, ball->y_ballpos, stage, &enemy, enemy_AI, tileManager, sound, ball, powerup);

	/* Updates ball coordinates so it's not stuck inside the enemy */
	ball->x_ballpos += enemy_x_change;
	ball->y_ballpos += enemy_y_change;

	/*Draws ball at new position as well as update ball's position*/
	if (ball->ball_type == 1)
	{
		/* Updates tile manager of the ball */
		TileMapManager_Ball(ball, tileManager, 4);
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	}
	else if (ball->ball_type == 2)
	{
		/* Updates tile manager of the ball */
		TileMapManager_Ball(ball, tileManager, 5);
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	}

	/* Prints ball in its new location */
	WorldData_GoToXY(ball->x_ballpos, ball->y_ballpos);
	printf("%c", ball->ballchar);
	SetConsoleTextAttribute(hConsole, saved_attributes);
}

/* Sets which enemies can fire */
void Enemy_FireActive_Update(Stage *stage, Enemy *enemy[])
{
	int i, j;	/* loop variables */

	/* Goes through every enemy in the first row */
	for (i = 0; i < stage->num_enemies; ++i)
		/* Goes through enemies within that column */
		for (j = i; j < stage->total_enemies; j += stage->num_enemies)
		{
			/* Checks if enemy is alive */
			if (enemy[j]->alive == 1 && enemy[j]->HP > 0)
			{
				/* Checks if that enemy is of the type that can shoot */
				if (enemy[j]->type >= 2 && enemy[j]->type <= 6)
				{
					/* Sets it to be fire active */
					enemy[j]->fireActive = 1;
					/* Enemy is not of the correct type and will not fire */
					break;
				}

				/* Enemy is a type that cannot shoot, move to next column */
				else break;
			}

			/* If enemy is dead, set it to not be able to fire */
			else if (enemy[j]->alive <= 0 || enemy[j]->HP <= 0)
				enemy[j]->fireActive = 0;
		}

	/* Checks if the extra hydra units can fire */
	for (i = stage->total_enemies; i < stage->total_and_hydra; ++i)
	{
		/* Checks if the hydra spawn is alive and has been created */
		if (enemy[i] != NULL && enemy[i]->alive == 1 && enemy[i]->HP > 0 && enemy[i]->is_hydra == -1)
		{
			int link;	/* The linked hydra unit s*/

			/* The main hydra unit that it is beside */
			link = enemy[i]->hydra_link;

			/* Check if the main unit is at the front most row */
			if (link >= stage->num_enemies)
			{
				/* Check if the unit in front of the linked unit is alive and set it to not fire if it is */
				if (enemy[link - stage->num_enemies]->alive == 1 && enemy[link - stage->num_enemies]->HP > 0)
					enemy[i]->fireActive = 0;
				/* Check if the unit in front of the linked unit is dead and set it to fire if it is */
				else if (enemy[link - stage->num_enemies]->alive == 0 || enemy[link - stage->num_enemies]->HP <= 0)
					enemy[i]->fireActive = 1;
			}
		}

		/* Sets dead hydra units to not be fire active */
		else if (enemy[i] != NULL && enemy[i]->alive == 0 && enemy[i]->HP <= 0)
			enemy[i]->fireActive = 0;
	}

	/* Reloads every enemy that are fire active */
	Enemy_Fire_Reload(stage, enemy);
}

/* Reloads the enemies that can fire */
void Enemy_Fire_Reload(Stage *stage, Enemy *enemy[])
{
	int i;	/* Loop counter */

	/* Goes through every enemy including hydra units */
	for (i = 0; i < stage->total_and_hydra; ++i)
	{
		/* Checks if the enemy exists and it can fire */
		if (enemy[i] != NULL && enemy[i]->fireActive == 1)
		{
			/* Reloads the enemy */
			enemy[i]->fireReloader += 1 * Delta_Time(stage);
		}
	}
}

/* Contains all the functions needed to update and redraw the enemy positions */
void Enemy_Update(Stage *stage, Enemy *enemy[], Enemy_AI *enemy_AI, int x, int y, TileMapManager *tileManager, Ball *ball1, Ball *ball2,
	Powerup *powerup[], Sound *sound)
{
	Enemy_Clear(stage, enemy, enemy_AI, tileManager);
	Enemy_Movement(stage, enemy, enemy_AI, x, y, tileManager, ball1, ball2, powerup, sound);
	Enemy_Print(stage, enemy, enemy_AI, tileManager);
	Jebus_AI(stage, enemy, enemy_AI, tileManager, sound);

	/* Plays a sound when the enemy moves */
	loadSound(sound, "..\\CollisionDetectionTest\\SFX\\EnemyMove.wav");
	playSound_Low(sound);
}