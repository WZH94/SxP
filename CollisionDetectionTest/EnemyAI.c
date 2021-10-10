/******************************************************************************
All content © 2017 Digipen Institute of Technology Singapore, all rights reserved

filename			EnemyAI.c
author				Wong Zhihao
DP email			zhihao.wong@digipen.edu
course				RTIS

Brief Description:
Enemy AI creator and handler, controls the movements and shooting of enemies

******************************************************************************/

#include <stdlib.h>			/* malloc, free */
#include "EnemyAI.h"		/* Function declarations */
#include "FMODInit.h"		/* Sound objects and functions */
#include "Stage.h"			/* Stage objects and functions */
#include "Enemy.h"			/* Enemy objects and functions */
#include "TileMapManager.h" /* Tile manager objects and functions */
#include "EnemyBullets.h"	/* Enemy bullet objects and functions */

/* Create a new Enemy AI object with the provided data */
Enemy_AI* Enemy_AI_Create(Stage *stage, int player, int y_spawn)
{
	Enemy_AI* enemy_AI = NULL; /* the new item to be retrieved */

							   /* allocate the memory for a new object */
	enemy_AI = (Enemy_AI*)malloc(sizeof(Enemy_AI));
	if (enemy_AI == NULL)
	{
		return NULL; /* malloc can fail */
	}

	enemy_AI->forward_trigger = 0;			 /* Triggers enemy to advance */
	enemy_AI->left_trigger = 1;				 /* Triggers enemy to move left (P1 perspective) */
	enemy_AI->right_trigger = 0;			 /* Triggers enemy to move right (P1 perspective) */
	enemy_AI->time_to_move = -2;			 /* Enemy must have moved at least twice in one direction before it can change direction */
	enemy_AI->p_mult = player;				 /* Reverses direction of enemies facing P2 */
	enemy_AI->y_spawn = y_spawn;			 /* y coord of spawn */
	enemy_AI->frames = 5200;				 /* Updates every 5200 frames, WILL NOT CHANGE */

	/* Adjusts the speed of enemy movement. Movement is faster in PvP mode */
	if (stage->play_type == PvP)
		enemy_AI->base_timer = 1700;
	/* Co-Op movement speed is normal */
	else
		enemy_AI->base_timer = 0;

	enemy_AI->timer = enemy_AI->base_timer;	 /* Timer always starts from base_timer's value */

	/* Returns object */
	return enemy_AI;
}

/* Free the memory associated with a given Enemy AI object */
void Enemy_AI_Free(Enemy_AI** enemybehaviourPtr)
{
	/* safety check on the parameters */
	if ((enemybehaviourPtr == NULL) || (*enemybehaviourPtr == NULL))
	{
		return;  /* take no action if the parameters are invalid */
	}

	/* free the given item */
	free(*enemybehaviourPtr);

	/* clear the provided pointer */
	*enemybehaviourPtr = NULL;
}

/* Controls the behaviour of the enemy movements */
int Enemy_Control(Stage *stage, Enemy *enemy[], Enemy_AI *enemy_AI, TileMapManager *tileManager, Ball *ball1, Ball *ball2, Powerup *powerup[], Sound *sound)
{
	int front_enemy;	/* Front most enemy of the enemy group */
	int left_enemy;		/* Left most enemy of the enemy group */
	int right_enemy;	/* Right most enemy of the enemy group */

	/* Finds the front, left and right most enemy */
	front_enemy = Enemy_FrontLine_Check(stage, enemy);
	left_enemy = Enemy_LeftSide_Check(stage, enemy);
	right_enemy = Enemy_RightSide_Check(stage, enemy);

	/* Updates which enemies are able to fire */
	Enemy_FireActive_Update(stage, enemy);

	/* Updates the timer for when the enemy should be moving by the Delta Time */
	enemy_AI->timer += 1 * Delta_Time(stage);

	/* Checks if it is time for the enemy to advance towards player */
	if (enemy_AI->forward_trigger == 1)
	{
		/* If it is time for enemy to move */
		if (enemy_AI->timer >= enemy_AI->frames)
		{
			/* Reset enemy move timer */
			enemy_AI->timer = enemy_AI->base_timer;

			/* Update enemy */
			Enemy_Update(stage, enemy, enemy_AI, 0, -1 * enemy_AI->p_mult, tileManager, ball1, ball2, powerup, sound);

			/* Removes trigger for the enemy to advance towards player */
			enemy_AI->forward_trigger = 0;
		}
	}

	/* Checks if enemy should be moving left (P1 enemies) and right (P2 enemies) */
	else if (enemy_AI->left_trigger == 1)
	{
		/* If it is time for enemy to move */
		if (enemy_AI->timer >= enemy_AI->frames)
		{
			/* Reset enemy move timer */
			enemy_AI->timer = enemy_AI->base_timer;

			/* Check if the left/right most enemy is not at the border */
			if ((enemy[left_enemy]->id == 1 && enemy[left_enemy]->x_Pos >= 3) ||
				(enemy[right_enemy]->id == 2 && enemy[right_enemy]->x_Pos + 3 < stage->size_x - 3))
				/* Update enemy */
				Enemy_Update(stage, enemy, enemy_AI, -1 * enemy_AI->p_mult, 0, tileManager, ball1, ball2, powerup, sound);

			/* Checks if left/right most enemy is at the border */
			if (((enemy[left_enemy]->id == 1 && enemy[left_enemy]->x_Pos <= 2) ||
				(enemy[right_enemy]->id == 2 && enemy[right_enemy]->x_Pos + 3 >= stage->size_x - 3))/* && ++enemy_AI->time_to_move > 0*/)
			{
				/* Minimum number of times it moves in one direction before the direction can change */
				enemy_AI->time_to_move = -2;
				/* Trigger enemy to move right and stop it from moving left (P1 enemies), move left and stop moving right (P2 enemies) */
				enemy_AI->right_trigger = 1;
				enemy_AI->left_trigger = 0;
				/* Since it is at the edge of the screen, enemy should advance toward player, check if enemy is already at player */
				if ((enemy[front_enemy]->id == 1 && enemy[front_enemy]->y_Pos > 6) ||
					(enemy[front_enemy]->id == 2 && enemy[front_enemy]->y_Pos < stage->size_y - 7))
				{
					/* Initialises enemy to advance towards player */
					enemy_AI->forward_trigger = 1;
				}

				/* Enemy has reached player 1 */
				else if (enemy[front_enemy]->id == 1 && enemy[front_enemy]->y_Pos <= 6)
					return 1;

				/* Enemy reached player 2 */
				else if (enemy[front_enemy]->id == 2 && enemy[front_enemy]->y_Pos >= stage->size_y - 7)
					return 2;
			}
		}

	}
	/* Checks if enemy should be moving right (P1 enemies) and left (P2 enemies) */
	else if (enemy_AI->right_trigger == 1)
	{
		/* If it is time for enemy to move */
		if (enemy_AI->timer >= enemy_AI->frames)
		{
			/* Reset enemy move timer */
			enemy_AI->timer = enemy_AI->base_timer;

			/* Check if the left/right most enemy is not at the border */
			if ((enemy[right_enemy]->id == 1 && enemy[right_enemy]->x_Pos + 3 < stage->size_x - 3) ||
				(enemy[left_enemy]->id == 2 && enemy[left_enemy]->x_Pos >= 3))
				/* Update enemy */
				Enemy_Update(stage, enemy, enemy_AI, 1 * enemy_AI->p_mult, 0, tileManager, ball1, ball2, powerup, sound);

			/* Check if the left/right most enemy is already at the border */
			if (((enemy[left_enemy]->id == 2 && enemy[left_enemy]->x_Pos <= 2) ||
				(enemy[right_enemy]->id == 1 && enemy[right_enemy]->x_Pos + 3 >= stage->size_x - 3))/* && ++enemy_AI->time_to_move > 0*/)
			{
				/* Minimum number of times it moves in one direction before the direction can change */
				enemy_AI->time_to_move = -2;

				/* Trigger enemy to move left and stop it from moving right (P1 Perspective) */
				enemy_AI->left_trigger = 1;
				enemy_AI->right_trigger = 0;

				/* Since it is at the edge of the screen, enemy should advance toward player, check if enemy is already at player */
				if ((enemy[front_enemy]->id == 1 && enemy[front_enemy]->y_Pos > 6) ||
					(enemy[front_enemy]->id == 2 && enemy[front_enemy]->y_Pos < stage->size_y - 7))
				{
					/* Initialises enemy to advance towards player */
					enemy_AI->forward_trigger = 1;
				}

				/* Enemy reached player 1 */
				else if (enemy[front_enemy]->id == 1 && enemy[front_enemy]->y_Pos <= 6)
					return 1;

				/* Enemy reached player 2 */
				else if (enemy[front_enemy]->id == 2 && enemy[front_enemy]->y_Pos >= stage->size_y - 7)
					return 2;
			}
		}
	}

	/* Enemy has not reached player, continue game */
	return 0;
}

/* Checks for the nearest alive enemy in y coord to the player */
int Enemy_FrontLine_Check(Stage *stage, Enemy *enemy[])
{
	int i;	/* Loop counter */

	/* If any enemy is alive in the row, returns that position */
	for (i = 0; i < stage->total_enemies; ++i)
	{
		if (enemy[i]->alive == 1)
			return i;
	}
	/* No enemy alive */
	return 0;
}

/* Finds the coordinate of the enemy which is nearest to the left border */
int Enemy_LeftSide_Check(Stage *stage, Enemy *enemy[])
{
	int i;					/* Loop counter */
	int closest_x = 3;		/* Closest any enemy should be at the left edge */
	int closest_enemy = 0;	/* Closest enemy location */

	/* Loops through every enemies including hydra */
	for (i = 0; i < stage->total_and_hydra; ++i)
	{
		/* Checks if enemy exists and is alive and its x position is nearer to left border than current closest x */
		if (enemy[i] != NULL && closest_x > enemy[i]->x_Pos && enemy[i]->alive == 1 && enemy[i]->HP > 0)
		{
			/* Updates closest x and closest location */
			closest_x = enemy[i]->x_Pos;
			closest_enemy = i;
		}
	}

	/* Returns location of the closest enemy to left border */
	return closest_enemy;
}

/* Finds the coordinate of the enemy which is nearest to the right border */
int Enemy_RightSide_Check(Stage *stage, Enemy *enemy[])
{
	int i;													/* Loop counter */
	int closest_x = enemy[stage->num_enemies - 1]->x_Pos;	/* Closest any enemy should be at the right edge */
	int closest_enemy = stage->num_enemies - 1;				/* Closest enemy location */

	/* Loops through every enemies including hydra */
	for (i = stage->num_enemies - 1; i < stage->total_and_hydra; ++i)
	{
		/* Checks if enemy exists and is alive and its x position is nearer to right border than current closest x */
		if (enemy[i] != NULL && closest_x < enemy[i]->x_Pos && enemy[i]->alive == 1 && enemy[i]->HP > 0)
		{
			/* Updates closest x and closest location */
			closest_x = enemy[i]->x_Pos;
			closest_enemy = i;
		}
	}

	/* Returns location of the closest enemy to left border */
	return closest_enemy;
}

//* Randomises whether the enemy shoots in the current frame */
void Enemy_Fire(Stage *stage, Enemy *enemy[], EnemyBullet *enemyBullet[], Sound *sound)
{
	int i, j;	/* Loop variables */

	/* Loops through every enemy in the stage */
	for (i = 0; i < stage->total_enemies; ++i)
	{
		/* Check if the enemy is allowed and ready to fire */
		if (enemy[i]->fireActive == 1 && enemy[i]->fireReloader >= enemy[i]->fireReload && enemy[i]->alive == 1 && enemy[i]->HP > 0)
		{
			/* Randomises a number, if it is below or equal to 2 the enemy fires */
			if (enemy[i]->fireChance <= 2 || (rand() % enemy[i]->fireChance) <= 2)
				/* Checks for a free memory */
				for (j = 0; j < 50; ++j)
				{
					/* Resets the enemys reload time */
					enemy[i]->fireReloader = 0;
					if (enemyBullet[j] == NULL)
					{
						/* If enemy is an arc, create additional bullets from this function */
						if (enemy[i]->type == 5)
						{
							Enemy_Fire_Arc(enemy[i], &enemyBullet[0], sound);
						}

						/* Create one bullet only*/
						else
							enemyBullet[j] = EnemyBullet_Create(sound, enemy[i], 0);

						/* Not more than one enemy shoots per frame */
						return;
					}
				}
		}
	}
}