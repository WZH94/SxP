/******************************************************************************
All content © 2017 Digipen Institute of Technology Singapore, all rights reserved

filename			EnemyBullets.c
author				Wong Zhihao
DP email			zhihao.wong@digipen.edu
course				RTIS

Brief Description:
Contents of enemy bullet object of the game, and handles their movements,
initialisations and collisions with objects in the game.

******************************************************************************/

#include <stdlib.h>				/* rand */
#include <stdio.h>				/* printf */
#include "EnemyBullets.h"		/* Function declarations */
#include "TileMapManager.h"		/* Tile manager objects and functions */
#include "FMODInit.h"			/* Sound objects and functions */
#include "Enemy.h"				/* Enemy objects and functions */
#include "Player.h"				/* Player objects and functions */
#include "Stage.h"				/* Stage objects and functions */
#include "WorldData.h"			/* Misc functions */

/* Art of every bullet type in game */
const char normalBullet = '|';
const char leftBullet = '\\';
const char rightBullet = '/';
const char bigBullet_P1 = 239;
const char bigBullet_P2 = 'U';
const char peaBullet = 248;
const char torpedoBullet = 254;

/* Create a new Enemy Bullet object with the provided data */
EnemyBullet* EnemyBullet_Create(Sound *sound, Enemy *enemy, int arc)
{
	/* the new object to be returned */
	EnemyBullet *enemyBullet;

	/* allocate the memory for a new object */
	enemyBullet = (EnemyBullet*)malloc(sizeof(EnemyBullet));
	if (enemyBullet == NULL)
	{
		return NULL; /* malloc can fail */
	}

	/* Sets which direction bullet should travel depending on which player it faces */
	if (enemy->id == 1)
		enemyBullet->direction = 1;
	else enemyBullet->direction = -1;

	/* Bullet should not be moving left or rightwards */
	enemyBullet->left = enemyBullet->right = 0;

	/* ..except for Arc bullets */
	if (enemy->type == 5)
	{
		/* First bullet created on this x position etc... */
		enemyBullet->x_Pos = enemy->x_Pos + arc;

		/* First bullet moves left ward */
		if (arc == 0)
			enemyBullet->left = 1;
		/* Last bullet moves rightward */
		else if (arc == 3)
			enemyBullet->right = 1;
	}
	else
		/* Randomise bullet to spawn anywhere between enemy */
		enemyBullet->x_Pos = enemy->x_Pos + ((rand() % 1) + 1);

	/* Randomise bullet to spawn in front of enemy */
	enemyBullet->y_Pos = enemy->y_Pos - enemyBullet->direction;
	/* Updates every 80 frames AT MOST, WILL NOT CHANGE */
	enemyBullet->frames = 150;
	/* Adjusts the speed, ex. if increases to 1, takes 1 less frame to update */
	enemyBullet->base_timer = 0;
	/* Timer always starts from base_timer's value */
	enemyBullet->timer = enemyBullet->base_timer;
	/* Whether bullet is passing through an enemy and should not be printed, initialised as not passing through anything */
	enemyBullet->pass_through = 0;
	/* Initialises the bullet based on current data */
	EnemyBullet_Type_Init(enemyBullet, enemy);

	/* Go to its x and y pos and prints the bullet */
	WorldData_GoToXY(enemyBullet->x_Pos, enemyBullet->y_Pos);
	printf("%c", enemyBullet->art);

	if (sound != NULL)
		EnemyFire_SFX(enemy, sound);

	/* Returns the object */
	return enemyBullet;
}

/* Free the memory associated with a given EnemyBullet object */
void EnemyBullet_Free(EnemyBullet **enemyBulletPtr)
{
	/* safety check on the parameters */
	if ((enemyBulletPtr == NULL) || (*enemyBulletPtr == NULL))
	{
		return;  /* take no action if the parameters are invalid */
	}

	/* free the given item */
	free(*enemyBulletPtr);

	/* clear the provided pointer */
	*enemyBulletPtr = NULL;
}

/* Initialises enemy bullet based on the enemy that will fire */
void EnemyBullet_Type_Init(EnemyBullet *enemyBullet, Enemy *enemy)
{
	/* Shooter */
	if (enemy->type == 2)
	{
		enemyBullet->art = normalBullet;
		enemyBullet->bullet_type = 2;
	}

	/* BigShot */
	else if (enemy->type == 3)
	{
		enemyBullet->bullet_type = 3;

		if (enemy->id == 1)
			enemyBullet->art = bigBullet_P1;
		else enemyBullet->art = bigBullet_P2;
	}

	/* Pea */
	else if (enemy->type == 4)
	{
		enemyBullet->art = peaBullet;
		enemyBullet->bullet_type = 4;

		/* Pea's bullets move slightly faster */
		enemyBullet->base_timer = 40;
	}

	/* Arc */
	else if (enemy->type == 5)
	{
		if (enemyBullet->left == 0 && enemyBullet->right == 0)
		{
			enemyBullet->art = normalBullet;
		}

		/* Different art for bullets moving left or rightwarwd */
		else if (enemyBullet->left == 1)
		{
			if (enemy->id == 1)
				enemyBullet->art = leftBullet;
			else if (enemy->id == 2)
				enemyBullet->art = rightBullet;
		}

		else if (enemyBullet->right == 1)
		{
			if (enemy->id == 1)
				enemyBullet->art = rightBullet;
			else if (enemy->id == 2)
				enemyBullet->art = leftBullet;
		}

		enemyBullet->bullet_type = 5;
	}

	/* Torpedo */
	else if (enemy->type == 6)
	{
		enemyBullet->art = torpedoBullet;
		enemyBullet->bullet_type = 6;

		/* Torpedo's bullets move slower */
		enemyBullet->base_timer = -50;

		enemyBullet->torp_track_timer = 0;
	}
}

/* Creates the bullets for the Arc enemy */
void Enemy_Fire_Arc(Enemy *enemy, EnemyBullet *enemyBullets[], Sound *sound)
{
	int i;			/* Loop counter */
	int four = 0;	/* Total of four bullets being spawned */

	/* Finds a free memory */
	for (i = 0; i < 50; ++i)
	{
		/* If memory is free and there are still bullets to spawn */
		if (enemyBullets[i] == NULL && four <= 3)
		{
			enemyBullets[i] = EnemyBullet_Create(sound, enemy, four);
			++four;
		}

		/* Max number of bullets spawned, break out */
		else if (four >= 4)
			break;
	}
}

/* Plays the correct sound when enemy bullet is created, aka fired */
void EnemyFire_SFX(Enemy *enemy, Sound *sound)
{
	/* Shooter/Biden fired */
	if (enemy->type == 2 || enemy->type == 5)
	{
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\ShooterShoot.wav");
		playSound_Low(sound);
	}

	/* BigShot fired */
	else if (enemy->type == 3)
	{
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\BigShotShoot.wav");
		playSound_Low(sound);
	}

	/* Pea fired */
	else if (enemy->type == 4)
	{
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\PeaShoot.wav");
		playSound_Low(sound);
	}

	/* Torpedo fired */
	else if (enemy->type == 6)
	{
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\TorpedoShoot.wav");
		playSound_Low(sound);
	}
}

/* Detects bullet collision detection  */
int EnemyBullet_CollisionDetection(EnemyBullet *enemyBullet, Player *player_1, Player *player_2, TileMapManager *tile_manager, Sound *sound)
{
	int result;	/* What the bullet collided with */

	/* If not passing through anything, clear the bullet, otherwise no need to clear as it is already cleared */
	if (enemyBullet->pass_through == 0)
	{
		/* Clear enemyBullet */
		WorldData_GoToXY(enemyBullet->x_Pos, enemyBullet->y_Pos);
		printf(" ");
	}

	/* Clears bullet in tile manager */
	TileMapManager_EnemyBullet(enemyBullet, tile_manager, 0);

	/* Homes the bullet for torpedo bullets */
	if (enemyBullet->bullet_type == 6)
		EnemyBullet_TorpedoMove(enemyBullet, player_1, player_2);

	/* What the bullet collided with */
	result = TileMapManager_EnemyBullet_Collision(enemyBullet, tile_manager);

	/* Bullet hit Player 1 */
	if (result == 1)
	{
		/* Minus HP of player 1 */
		minusHP(player_1, sound);

		/* If Big Shot Bullet, minus again */
		if (enemyBullet->bullet_type == 3)
		{
			player_1->invulnerable_time = 0;
			minusHP(player_1, sound);
		}
		/* 1 for collision detected */
		return 1;
	}

	/* Bullet hit Player 2 */
	else if (result == 2)
	{
		/* Minus HP of player 1 */
		minusHP(player_2, sound);

		/* If Big Shot Bullet, minus again */
		if (enemyBullet->bullet_type == 3)
		{
			player_2->invulnerable_time = 0;
			minusHP(player_2, sound);
		}
		/* 1 for collision detected */
		return 1;
	}

	/* Bullet hit enemy */
	else if (result == 3)
	{
		/* Sets the bullet to not be printed while its travelling through enemy */
		enemyBullet->pass_through = 1;
		/* 0 for no collision */
		return 0;
	}

	/* Bullet hit Player 1 ball, gets destroyed */
	else if (result == 4)
	{
		/* Increases player score for this spectecular event */
		player_1->score += 50;

		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\BallDestroyBullet.wav");
		playSound_Low(sound);

		/* 1 for collision detected */
		return 1;
	}

	/* Bullet hit Player 2 ball, gets destroyed */
	else if (result == 5)
	{
		/* Increases player score for this spectecular event */
		player_2->score += 50;

		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\BallDestroyBullet.wav");
		playSound_Low(sound);

		/* 1 for collision detected */
		return 1;
	}

	/* Bullet hit border */
	else if (result == 6)
		return 1;

	/* Return 0, bullet does not clear and continues moving */
	enemyBullet->pass_through = 0;
	return 0;
}

/* Handles movement of enemyBullet */
void EnemyBullet_Move(EnemyBullet *enemyBullet, TileMapManager *tile_manager)
{
	/* If enemyBullet is not created, do nothing */
	if (enemyBullet == NULL)
		return;
	else
	{
		/* Moves enemyBullet based on which player it is falling to */
		enemyBullet->y_Pos -= enemyBullet->direction;

		/* Adjusts x position of bullet if its moving left or right */
		if (enemyBullet->left == 1)
			--enemyBullet->x_Pos;
		else if (enemyBullet->right == 1)
			++enemyBullet->x_Pos;

		/* Updates bullet position in tile manager */
		TileMapManager_EnemyBullet(enemyBullet, tile_manager, 1);

		/* If not passing through any object, print bullet */
		if (enemyBullet->pass_through == 0)
		{
			/* Prints enemyBullet */
			WorldData_GoToXY(enemyBullet->x_Pos, enemyBullet->y_Pos);
			printf("%c", enemyBullet->art);
		}
	}
}

/* Handles movement of Torpedo bullet */
void EnemyBullet_TorpedoMove(EnemyBullet *enemyBullet, Player *player_1, Player *player_2)
{
	/* If enemyBullet is not created, do nothing */
	if (enemyBullet == NULL)
		return;
	else
	{
		/* Controls how often it tracks the player */
		if (++enemyBullet->torp_track_timer == 2)
		{
			/* Which player to track, tracks player 1 */
			if (enemyBullet->direction == 1)
			{
				/* Checks if centre of player 1 is to the left of the torpedo */
				if ((player_1->x_p + (player_1->x_l / 2)) < enemyBullet->x_Pos)
					--enemyBullet->x_Pos;
				/* Checks if centre of player 1 is to the right of the torpedo */
				else if ((player_1->x_p + (player_1->x_l / 2)) > enemyBullet->x_Pos)
					++enemyBullet->x_Pos;
				/* Torpedo is already heading towards player 1 */
				else;

				/* Resets track timer */
				enemyBullet->torp_track_timer = 0;
			}

			/* Which player to track, tracks player 2 */
			if (enemyBullet->direction == -1)
			{
				/* Checks if centre of player 2 is to the left of the torpedo */
				if ((player_2->x_p + (player_2->x_l / 2)) < enemyBullet->x_Pos)
					--enemyBullet->x_Pos;
				/* Checks if centre of player 2 is to the right of the torpedo */
				else if ((player_2->x_p + (player_2->x_l / 2)) > enemyBullet->x_Pos)
					++enemyBullet->x_Pos;
				/* Torpedo is already heading towards player 2 */
				else;

				/* Resets track timer */
				enemyBullet->torp_track_timer = 0;
			}
		}
	}
}

/* Updates enemyBullet every frame */
void EnemyBullet_Update(EnemyBullet *enemyBullet[], Player *player_1, Player *player_2, Stage *stage, TileMapManager *tile_manager, Sound *sound)
{
	int i;		/* loop counter */
	int free;	/* Checks if enemy bullet should be cleared if it has hit player or gone out of border */

	/* Finds bullets that exists */
	for (i = 0; i < 50; ++i)
	{
		/* If enemyBullet exists */
		if (enemyBullet[i] != NULL)
		{
			/* Checks if it is time for the bullet to move */
			if ((enemyBullet[i]->timer += 1 * Delta_Time(stage)) >= enemyBullet[i]->frames)
			{
				/* Resets the timer */
				enemyBullet[i]->timer = enemyBullet[i]->base_timer;

				/* Check if enemy bullet has hit player or gone out of border and should be cleared */
				free = EnemyBullet_CollisionDetection(enemyBullet[i], player_1, player_2, tile_manager, sound);
				if (free)
				{
					TileMapManager_EnemyBullet(enemyBullet[i], tile_manager, 0);
					/* Free the enemyBullet object */
					EnemyBullet_Free(&enemyBullet[i]);
					/* Move to update next bullet */
					continue;
				}

				/* Enemy bullet has not collided with anything, continue moving through the map */
				EnemyBullet_Move(enemyBullet[i], tile_manager);
			}
		}
	}
}

/* Handles the functions that happen when the ball hits a bullet */
void Ball_Bullet_Collision(int x_pos, int y_pos, TileMapManager *tileManager, Sound *sound, EnemyBullet *enemyBullet[])
{
	int i;	/* Loop counter */

	/* Finds bullet that exists */
	for (i = 0; i < 50; ++i)
	{
		/* If bullet exists */
		if (enemyBullet[i] != NULL)
		{
			/* If position matches with the ball it is colliding with */
			if (x_pos == enemyBullet[i]->x_Pos && y_pos == enemyBullet[i]->y_Pos)
			{
				/* Removes the bullet from the game */
				TileMapManager_EnemyBullet(enemyBullet[i], tileManager, 0);
				WorldData_GoToXY(enemyBullet[i]->x_Pos, enemyBullet[i]->y_Pos);
				printf(" ");
				EnemyBullet_Free(&enemyBullet[i]);

				loadSound(sound, "..\\CollisionDetectionTest\\SFX\\BallDestroyBullet.wav");
				playSound_Low(sound);
				break;
			}
		}
	}
}

/* Player moved into a bullet */
void Player_Bullet_Collision(Player *player, EnemyBullet *enemyBullet[], TileMapManager *tile_manager, Sound *sound)
{
	int i;	/* Loop counter */

	/* Finds every */
	for (i = 0; i < 50; ++i)
	{
		/* If bullet exists */
		if (enemyBullet[i] != NULL)
		{
			/* If position matches with the player it is colliding with */
			if (enemyBullet[i]->x_Pos == player->x_p && enemyBullet[i]->y_Pos == player->y_p)
			{
				/* Removes the bullet from the game */
				TileMapManager_EnemyBullet(enemyBullet[i], tile_manager, 0);
				WorldData_GoToXY(enemyBullet[i]->x_Pos, enemyBullet[i]->y_Pos);
				printf(" ");

				/* Damages the player */
				minusHP(player, sound);

				/* Damages player again if it is a Big Shot bullet */
				if (enemyBullet[i]->bullet_type == 3)
				{
					player->invulnerable_time = 0;
					minusHP(player, sound);
				}
				/* Free the bullet object */
				EnemyBullet_Free(&enemyBullet[i]);
				break;
			}
		}
	}
}