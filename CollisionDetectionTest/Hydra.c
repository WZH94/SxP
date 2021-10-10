/******************************************************************************
All content © 2017 Digipen Institute of Technology Singapore, all rights reserved

filename			Hydra.c
author				Wong Zhihao
DP email			zhihao.wong@digipen.edu
course				RTIS

Brief Description:
Handles the functions of the Hydra enemy when it spawns and dies

******************************************************************************/
		
#include <stdlib.h>				/* rand */	
#include "Hydra.h"				/* Function declarations */
#include "Stage.h"				/* Stage object and functions */
#include "Enemy.h"				/* Enemy object and functions */
#include "EnemyTypes.h"			/* Enemy Types and functions */
#include "EnemyAI.h"			/* Enemy AI object and functions */
#include "TileMapManager.h"		/* Tile Manager object and functions */
#include "FMODInit.h"			/* Sound objects */

/* Initialises the hydra unit to take on one of the shooter's form */
void Hydra_Init(Enemy *enemy)
{
	/* Mark the enemy as a hydra so it splits when it dies */
	enemy->is_hydra = 1;

	/* Randomises between the shooter types */
	enemy->type = (rand() % 5) + 2;

	/* Shooter, Player 1 */
	if (enemy->id == 1 && enemy->type == 2)
		EnemyType_Init(enemy, enemyShooter_P1);
	/* Shooter, Player 2 */
	else if (enemy->id == 2 && enemy->type == 2)
		EnemyType_Init(enemy, enemyShooter_P2);
	/* Big Shot, Player 1 */
	else if (enemy->id == 1 && enemy->type == 3)
		EnemyType_Init(enemy, enemyBigShot_P1_2);
	/* Big Shot, Player 2 */
	else if (enemy->id == 2 && enemy->type == 3)
		EnemyType_Init(enemy, enemyBigShot_P2_2);
	/* Pea, Player 1 */
	else if (enemy->id == 1 && enemy->type == 4)
		EnemyType_Init(enemy, enemyPea_P1);
	/* Pea, Player 2 */
	else if (enemy->id == 2 && enemy->type == 4)
		EnemyType_Init(enemy, enemyPea_P2);
	/* Arc, Player 1 */
	else if (enemy->id == 1 && enemy->type == 5)
		EnemyType_Init(enemy, enemyArc_P1_3);
	/* Arc, Player 2 */
	else if (enemy->id == 2 && enemy->type == 5)
		EnemyType_Init(enemy, enemyArc_P2_3);
	/* Torpedo */
	else if (enemy->type == 6)
		EnemyType_Init(enemy, enemyTorpedo_3);
}

/* Causes the hydra to split when it dies */
void Hydra_Die(Stage *stage, Enemy *enemyContainer[], Enemy_AI *enemy_AI, TileMapManager *tileManager, int hydra_location, Sound *sound)
{
	int hydra_x;											/* Hydra's x position when it died */
	int hydra_y = enemyContainer[hydra_location]->y_Pos;	/* Hydra's y position when it died */
	int hydra_id = enemyContainer[hydra_location]->id;		/* Which player Hydra is facing */
	int hydra_type = enemyContainer[hydra_location]->type;	/* Hydra's shooter type */
	int limit_spawn = 0;									/* Whether to limit spawning the second hydra unit */
	int i;													/* Loop Counter */

	/* Plays a sound when the hydra dies */
	loadSound(sound, "..\\CollisionDetectionTest\\SFX\\HydraDie.wav");
	playSound_Low(sound);

	/* Checks if Hydra is at the left edge of the map */
	if (Hydra_Position(stage, enemyContainer[hydra_location]) == 1)
	{
		/* Limits the spawn to one hydra unit only */
		limit_spawn = 1;

		/* Hydra spawns at the original units location */
		hydra_x = enemyContainer[hydra_location]->x_Pos;
	}

	/* Checks if Hydra is at the right edge of the map */
	else if (Hydra_Position(stage, enemyContainer[hydra_location]) == -1)
	{
		/* Limits the spawn to one hydra unit only */
		limit_spawn = 1;

		/* Hydra spawns at the original units location */
		hydra_x = enemyContainer[hydra_location]->x_Pos;
	}

	else
	{
		/* Moves it to the left of the dead hydra so second hydra can spawn on its right */
		hydra_x = enemyContainer[hydra_location]->x_Pos - 3;

		/* Checks if this new location will overlap any unit */
		limit_spawn += TileMapManager_Hydra_Check(hydra_x, hydra_y, tileManager);
		/* Checks if second hydra's location will overlap any unit */
		limit_spawn += TileMapManager_Hydra_Check(hydra_x + ENEMY_WIDTH + ENEMY_WIDTH - 1, hydra_y, tileManager);

		/* If it overlaps, unit will only spawn one hydra unit, and moves location back to original unit's location */
		if (limit_spawn)
			hydra_x = enemyContainer[hydra_location]->x_Pos;
	}

	/* Frees the memory of the hydra that died */
	Enemy_Free(&enemyContainer[hydra_location]);

	/* Creates a new object of the hydra unit */
	enemyContainer[hydra_location] = Enemy_Create(stage, hydra_x, hydra_y, enemy_AI->p_mult, hydra_id, tileManager, hydra_type);

	/* If not limited to spawning only one unit */
	if (limit_spawn == 0)
		for (i = 0; i < stage->total_and_hydra; ++i)
		{
			/* Checks if the array location is free */
			if (enemyContainer[i] == NULL)
			{
				/* Creates a new object with the second hydra unit */
				enemyContainer[i] = Enemy_Create(stage, hydra_x + ENEMY_WIDTH + 1, hydra_y, enemy_AI->p_mult, hydra_id, tileManager, hydra_type);
				/* Links the two hydra unit locations together for other functions that may need it */
				enemyContainer[i]->hydra_link = hydra_location;
				enemyContainer[hydra_location]->hydra_link = i;
				/* -1 for WAS a hydra */
				enemyContainer[hydra_location]->is_hydra = -1;
				enemyContainer[i]->is_hydra = -1;

				/* Stop spawning */
				break;
			}
		}
	/* No need for a special check on this enemy's location */
	else if (limit_spawn > 0) enemyContainer[hydra_location]->is_hydra = 0;
}

/* Checks if the dead hydra is at the edges of the map */
int Hydra_Position(Stage *stage, Enemy *enemy)
{
	/* Checks left edge */
	if (enemy->x_Pos <= 3)
		return 1;

	/* Checks right edge */
	else if (enemy->x_Pos >= stage->size_x - ENEMY_WIDTH - 3)
		return -1;

	/* Hydra won't spawn into borders, go ahead */
	else return 0;
}