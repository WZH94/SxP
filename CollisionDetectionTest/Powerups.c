/******************************************************************************
All content © 2017 Digipen Institute of Technology Singapore, all rights reserved

filename			Powerups.c
author				Wong Zhihao
DP email			zhihao.wong@digipen.edu
course				RTIS

Brief Description:
Contents of powerup object of the game, and handles its randomisation, movement
and activation in the game.

******************************************************************************/

#include "stdlib.h"				/* malloc, free */
#include <stdio.h>				/* printf */
#include <time.h>				/* time */
#include <windows.h>			/* Colours */
#include "Powerups.h"			/* Function declarations */
#include "TileMapManager.h"		/* Tile manager struct and functions */
#include "Enemy.h"				/* Enemy struct and functions */
#include "EnemyAI.h"			/* EnemyAI struct and functions */
#include "Ball.h"				/* Ball struct and functions */
#include "Stage.h"				/* Stage struct and functions */
#include "Player.h"				/* Player struct and functions */
#include "BuffsDebuffs.h"		/* Specific powerup functions */
#include "WorldData.h"			/* MISC Functions */

/* Art of different powerups */
const char RANDOM = '?';
const char HP_BUFF = '+';
const char HP_DEBUFF = 196;
const char LENGTH_BUFF = 'L';
const char LENGTH_DEBUFF = 'S';
const char BALL_SPEED_BUFF = 232;
const char BALL_SPEED_DEBUFF = 233;
const char CONTROLS_DEBUFF = 145;
const char PLAYER_SPEED_BUFF = 247;
const char PLAYER_SPEED_DEBUFF = 174;
const char BALL_SWAP = 228;
const char KILL_ENEMY = 235;

/* Create a new Powerup object with the provided data */
Powerup *Powerup_Create(Enemy *enemy, Ball *ball)
{
	/* the new item to be returned */
	Powerup* powerup = NULL;

	/* allocate the memory for a new object */
	powerup = (Powerup*)malloc(sizeof(Powerup));
	if (powerup == NULL)
	{
		return NULL; /* malloc can fail */
	}

	/* Randomise the x and y position from the killed enemy to spawn from */
	powerup->x_pos = enemy->x_Pos + (rand() % 4);
	powerup->y_pos = enemy->y_Pos + (rand() % 3);

	powerup->frames = 220;						    /* Updates every 80 frames AT MOST, WILL NOT CHANGE */
	powerup->base_timer = 0;					    /* Adjusts the speed, ex. if increases to 1, takes 1 less frame to update */
	powerup->timer = powerup->base_timer;		    /* Timer always starts from base_timer's value */
	powerup->type = (rand() % 11);				    /* Randomise the type of up to drop */
	powerup->art = Powerup_ArtSwitcher(powerup);	/* Sets the powerup object art to the appropriate art based on the type */
	powerup->pass_through = 0;

	/* Direction it should fall to based on which player killed it */
	if (ball->ball_type == 1)
		powerup->direction = 1;
	else powerup->direction = -1;

	/* Go to its x and y pos and prints the powerup */
	WorldData_GoToXY(powerup->x_pos, powerup->y_pos);
	printf("%c", powerup->art);

	/* return the new object */
	return powerup;
}

/* Create a new Kill Enemy power with the provided data */
Powerup *Powerup_KillEnemy_Create(Stage *stage, int direction)
{
	/* the new item to be returned */
	Powerup* powerup = NULL;

	/* allocate the memory for a new object */
	powerup = (Powerup*)malloc(sizeof(Powerup));
	if (powerup == NULL)
	{
		return NULL; /* malloc can fail */
	}

	/* Randomise the x position, powerup spawns from the center of stage */
	powerup->x_pos = (rand() % (stage->size_x - 2)) + 1;
	powerup->y_pos = stage->size_y / 2;

	powerup->frames = 220;						    /* Updates every 220 frames AT MOST, WILL NOT CHANGE */
	powerup->base_timer = 50;					    /* Adjusts the speed, ex. if increases to 1, takes 1 less frame to update */
	powerup->timer = powerup->base_timer;		    /* Timer always starts from base_timer's value */
	powerup->type = -1;							    /* Type kills a random enemy */
	powerup->art = KILL_ENEMY;						/* Sets the powerup object art to the Kill Enemy buff */
	powerup->pass_through = 0;						/* Checks if powerup is passing through an enemy */

	/* Direction it should fall to */
	powerup->direction = direction;
	powerup->direction = direction;

	/* Go to its x and y pos and prints the powerup */
	WorldData_GoToXY(powerup->x_pos, powerup->y_pos);
	printf("%c", powerup->art);

	/* return the new object */
	return powerup;
}

/* Determines the art based on its type */
char Powerup_ArtSwitcher(Powerup *powerup)
{
	switch (powerup->type)
	{
		/* Random powerup */
	case 0: return RANDOM;
		break;
		/* HP Buff */
	case 1: return HP_BUFF;
		break;
		/* HP Debuff */
	case 2: return HP_DEBUFF;
		break;
		/* Length Buff */
	case 3: return LENGTH_BUFF;
		break;
		/* Length Debuff */
	case 4: return LENGTH_DEBUFF;
		break;
		/* Ball Speed Buff Slower */
	case 5: return BALL_SPEED_BUFF;
		break;
		/* Ball Speed Debuff Faster */
	case 6: return BALL_SPEED_DEBUFF;
		break;
		/* Player Controls Swap Debuff */
	case 7: return CONTROLS_DEBUFF;
		break;
		/* Player Reset Speed Buff */
	case 8: return PLAYER_SPEED_BUFF;
		break;
		/* Player Slow Down Debuff */
	case 9: return PLAYER_SPEED_DEBUFF;
		break;
		/* Ball Swap Powerup */
	case 10: return BALL_SWAP;
		break;
		/* Random powerup */
	default: return RANDOM;
		break;
	}
}

/* Free the memory associated with a given Powerup object */
void Powerup_Free(Powerup **powerupPtr)
{
	/* safety check on the parameters */
	if ((powerupPtr == NULL) || (*powerupPtr == NULL))
	{
		return;  /* take no action if the parameters are invalid */
	}

	/* free the given item */
	free(*powerupPtr);

	/* clear the provided pointer */
	*powerupPtr = NULL;
}

/* Randomises if powerup should drop when enemy is killed */
void Powerup_Spawner(Powerup *powerup[], Enemy *enemy, Ball *ball)
{
	if (rand() % 7 <= 2)
	{
		int i;

		/* Checks for a free memory */
		for (i = 0; i < 10; ++i)
			if (powerup[i] == NULL)
			{
				/* Creates the powerup */
				powerup[i] = Powerup_Create(enemy, ball);
				return;
			}
	}
}

/* Activates the powerup based on its type*/
void Powerup_Activate(Powerup *powerup, Stage *stage, Player *player, Ball *ball, TileMapManager *tile_manager, Sound *sound,
					  Enemy *enemy1[], Enemy *enemy2[], Enemy_AI *enemy_AI1, Enemy_AI *enemy_AI2, int player_num)
{
	/* Updates last collected powerup on the sdebar */
	Stage_Powerup_Print(powerup, player_num);

	switch (powerup->type)
	{
	case -1: CoOp_Powerup_Kill(stage, enemy1, enemy2, enemy_AI1, enemy_AI2, tile_manager, sound);
		break;
		/* Random powerup, randomises which powerup to give */
	case 0: powerup->type = (rand() % 9) + 1;
		Powerup_Activate(powerup, stage, player, ball, tile_manager, sound, enemy1, enemy2, enemy_AI1, enemy_AI2, player_num);
		break;
		/* HP Buff */
	case 1: Buff_HP(player, sound);
		break;
		/* HP Debuff */
	case 2: Debuff_HP(player, sound);
		break;
		/* Length Buff */
	case 3: Player_LengthBorderCheck(stage, player);
		Player_LengthUpdate(player, 2, tile_manager, sound);
		break;
		/* Length Debuff */
	case 4: Player_LengthUpdate(player, -2, tile_manager, sound);
		break;
		/* Ball speed Buff */
	case 5: Ball_Slow(ball, player, sound);
		break;
		/* Ball speed Debuff */
	case 6: Ball_Fast(ball, player, sound);
		break;
		/* Player Controls Swap Debuff */
	case 7: PlayerControls_Swap(player, sound);
		break;
		/* Player Speed Reset Buff */
	case 8: PlayerSpeed_Fast(player, sound);
		break;
		/* Player Speed Slow Debuff */
	case 9: PlayerSpeed_Slow(player, sound);
		break;
		/* Ball Swap Powerup */
	case 10: Ball_Swap(ball, sound, tile_manager);
		break;
		/* Random powerup, randomises which powerup to give */
	default: powerup->type = (rand() % 9) + 1;
		Powerup_Activate(powerup, stage, player, ball, tile_manager, sound, enemy1, enemy2, enemy_AI1, enemy_AI2, player_num);
		break;
	}
}

/* Detects if powerup hit anything */
int Powerup_CollisionDetection(Powerup *powerup, Player *player_1, Player *player_2, Ball *ball1, Ball *ball2, Stage *stage, TileMapManager *tileManager, 
							   Sound *sound, Enemy *enemy1[], Enemy *enemy2[], Enemy_AI *enemy_AI1, Enemy_AI *enemy_AI2)
{
	int result;	/* What the powerup has collide with */

	/* If powerup is not passing through anything, clear it, otherwise no need since it is already cleared */
	if (powerup->pass_through == 0)
	{
		/* Clear powerup */
		WorldData_GoToXY(powerup->x_pos, powerup->y_pos);
		printf(" ");
	}

	/* Update its position in the tile map */
	TileMapManager_Powerup(powerup, tileManager, 0);

	/* Handles collision detection of powerup */
	result = TileMapManager_Powerup_Collision(powerup, tileManager);

	/* Powerup hit Player 1 */
	if (result == 1)
	{
		/* Activates the powerup for P1 and increase score */
		Powerup_Activate(powerup, stage, player_1, ball1, tileManager, sound, enemy1, enemy2, enemy_AI1, enemy_AI2, 1);
		player_1->score += 50;
		return 1;
	}

	/* Powerup hit Player 2 */
	else if (result == 2)
	{
		/* Activates the powerup for P2 and increase score */
		Powerup_Activate(powerup, stage, player_2, ball2, tileManager, sound, enemy1, enemy2, enemy_AI1, enemy_AI2, 2);
		player_2->score += 50;
		return 1;
	}

	/* Powerup will pass through an enemy */
	else if (result == 3)
	{
		powerup->pass_through = 1;
		return 0;
	}

	/* Powerup hit Player 1 ball */
	else if (result == 4)
	{
		/* Activates the powerup for P1 and increase score */
		Powerup_Activate(powerup, stage, player_1, ball1, tileManager, sound, enemy1, enemy2, enemy_AI1, enemy_AI2, 1);
		player_1->score += 200;
		return 1;
	}

	/* Powerup hit Player 2 ball */
	else if (result == 5)
	{
		/* Activates the powerup for P1 and increase score */
		Powerup_Activate(powerup, stage, player_2, ball2, tileManager, sound, enemy1, enemy2, enemy_AI1, enemy_AI2, 2);
		player_2->score += 200;
		return 1;
	}

	/* Players missed the powerup, nothing happens */
	else if (result == 6)
		return 1;

	/* Return 0, powerup does not clear and continues moving */
	else
	{
		powerup->pass_through = 0;
		return 0;
	}
}

/* Handles movement of powerup */
void Powerup_Move(Powerup *powerup, TileMapManager *tileManager)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	/* Save current attributes */
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;

	/* If powerup is not created, do nothing */
	if (powerup == NULL)
		return;
	else
	{
		/* Moves powerup based on which player it is falling to */
		powerup->y_pos -= powerup->direction;

		/* Updates its positon in the tilemap */
		TileMapManager_Powerup(powerup, tileManager, 7);

		switch (powerup->type)
		{
		case 1:
		case 3:
		case 5:
		case 8: SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			break;
		case 2:
		case 4:
		case 6:
		case 7:
		case 9:SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
			break;
		case 0:
		default: SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			break;
		}
		if (powerup->pass_through == 0)
		{
			/* Prints powerup */
			WorldData_GoToXY(powerup->x_pos, powerup->y_pos);
			printf("%c", powerup->art);
		}
		SetConsoleTextAttribute(hConsole, saved_attributes);
	}
}

/* Updates powerup every frame */
void Powerup_Update(Powerup *powerup[], Player *player_1, Player *player_2, Ball *ball1, Ball *ball2, Stage *stage, TileMapManager *tile_manager, 
					Sound *sound, Enemy *enemy1[], Enemy *enemy2[], Enemy_AI *enemy_AI1, Enemy_AI *enemy_AI2)
{
	int i;		/* loop counter */
	int free;	/* Checks if powerup should be cleared if player has collected or missed it */
	for (i = 0; i < 10; ++i)
	{
		/* If powerup exists */
		if (powerup[i] != NULL)
		{
			if ((powerup[i]->timer += 1 * Delta_Time(stage)) >= powerup[i]->frames)
			{
				powerup[i]->timer = powerup[i]->base_timer;

				/* Check if powerup has collided with player or border and should be cleared */
				free = Powerup_CollisionDetection(powerup[i], player_1, player_2, ball1, ball2, stage, tile_manager, sound, enemy1, enemy2, enemy_AI1, enemy_AI2);
				if (free)
				{
					/* Free the powerup object */
					Powerup_Free(&powerup[i]);
					/* Stop function */
					return;
				}
				/* Powerup has not collided with anything, continue moving through the map */
				Powerup_Move(powerup[i], tile_manager);
			}
		}
	}
}

/* Handles the functions that happen when the ball hits a powerup */
void Ball_Powerup_CollisionDetection(int x_pos, int y_pos, Stage* stage, TileMapManager *tileManager, Sound *sound, Ball *ball, Powerup *powerup[],
									 Enemy *enemy1[], Enemy *enemy2[], Enemy_AI *enemy_AI1, Enemy_AI *enemy_AI2)
{
	int i;	/* Loop counter */

	/* Finds powerups that exist */
	for (i = 0; i < 10; ++i)
	{
		/* If powerup exists */
		if (powerup[i] != NULL)
		{
			/* If powerup location matches with the ball it collided with */
			if (x_pos == powerup[i]->x_pos && y_pos == powerup[i]->y_pos)
			{
				/* Clear powerup */
				WorldData_GoToXY(powerup[i]->x_pos, powerup[i]->y_pos);
				printf(" ");
				/* Activate the powerup for the player's ball who collided with it */
				Powerup_Activate(powerup[i], stage, ball->friendplayer, ball, tileManager, sound, enemy1, enemy2, enemy_AI1, enemy_AI2, ball->ball_type);
				Powerup_Free(&powerup[i]);
				break;
			}
		}
	}
}

/* When player hits the powerup, find which powerup it hit and activates it */
void Player_Powerup_Collision(Powerup *powerup[], Stage *stage, Player *player, Ball *ball, TileMapManager *tile_manager, Sound *sound,
							  Enemy *enemy1[], Enemy *enemy2[], Enemy_AI *enemy_AI1, Enemy_AI *enemy_AI2)
{
	int i;	/* Loop counter */

	/* Finds powerups that exist */
	for (i = 0; i < 10; ++i)
	{
		/* If powerup exists */
		if (powerup[i] != NULL)
		{
			/* If powerup location matches with the player it collided with */
			if (powerup[i]->x_pos == player->x_p && powerup[i]->y_pos == player->y_p)
			{
				/* Activates the powerup for that player */
				Powerup_Activate(powerup[i], stage, player, ball, tile_manager, sound, enemy1, enemy2, enemy_AI1, enemy_AI2, player->id);
				/* Free the powerup object */
				Powerup_Free(&powerup[i]);
				break;
			}
		}
	}
}