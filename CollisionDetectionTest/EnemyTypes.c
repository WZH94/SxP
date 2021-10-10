/******************************************************************************
All content © 2017 Digipen Institute of Technology Singapore, all rights reserved

filename			EnemyTypes.c
author				Wong Zhihao
DP email			zhihao.wong@digipen.edu
course				RTIS

Brief Description:
Handles the assigning and initialising of the various enemy types when they are
spawned.

******************************************************************************/

#include <time.h>	     /* time */
#include <stdlib.h>		 /* srand */
#include <string.h>		 /* strncpy */
#include "EnemyTypes.h"	 /* Function declarations */
#include "Hydra.h"		 /* Hydra functions */
#include "Stage.h"		 /* Stage struct and functions */
#include "Enemy.h"		 /* Enemy struct and functions */

/* Randomises the type of enemy to assign it to */
int EnemyType_Assigner(Enemy *enemy, Stage *stage)
{
	int random;				/* Random number */
	random = rand() % 9;	/* Randomises number between all enemy types */

	/* Checks if there are any more BB units to spawn then initialises to that unit */
	if (random == 0 && stage->TYPE0_tracker < stage->num_TYPE0)
	{
		enemy->type = 0;
		++stage->TYPE0_tracker;
	}

	/* Checks if there are any more Wall units to spawn then initialises to that unit */
	else if (random == 1 && stage->TYPE1_tracker < stage->num_TYPE1)
	{
		enemy->type = 1;
		++stage->TYPE1_tracker;
	}

	/* Checks if there are any more Shooter units to spawn then initialises to that unit */
	else if (random == 2 && stage->TYPE2_tracker < stage->num_TYPE2)
	{
		enemy->type = 2;
		++stage->TYPE2_tracker;
	}

	/* Checks if there are any more Big Shot units to spawn then initialises to that unit */
	else if (random == 3 && stage->TYPE3_tracker < stage->num_TYPE3)
	{
		enemy->type = 3;
		++stage->TYPE3_tracker;
	}

	/* Checks if there are any more Pea units to spawn then initialises to that unit */
	else if (random == 4 && stage->TYPE4_tracker < stage->num_TYPE4)
	{
		enemy->type = 4;
		++stage->TYPE4_tracker;
	}

	/* Checks if there are any more Arc units to spawn then initialises to that unit */
	else if (random == 5 && stage->TYPE5_tracker < stage->num_TYPE5)
	{
		enemy->type = 5;
		++stage->TYPE5_tracker;
	}

	/* Checks if there are any more Torpedo units to spawn then initialises to that unit */
	else if (random == 6 && stage->TYPE6_tracker < stage->num_TYPE6)
	{
		enemy->type = 6;
		++stage->TYPE6_tracker;
	}

	/* Checks if there are any more Jebus units to spawn then initialises to that unit */
	else if (random == 7 && stage->TYPE7_tracker < stage->num_TYPE7)
	{
		enemy->type = 7;
		++stage->TYPE7_tracker;
	}

	/* Checks if there are any more Hydra units to spawn then initialises to that unit */
	else if (random == 8 && stage->TYPE8_tracker < stage->num_TYPE8)
	{
		enemy->type = 8;
		++stage->TYPE8_tracker;
	}

	/* Return 1 to choose another unit type */
	else return 1;
	
	/* Unit type chosen, switches its art and initialises it */
	EnemyType_Switcher(enemy);

	/* No need to repick another unit type */
	return 0;
}

/* Chooses which data to send for initialisation based on its type */
void EnemyType_Switcher(Enemy *enemy)
{
	switch (enemy->type)
	{
		/* Enemy is BB */
	case 0: EnemyType_Init(enemy, enemyBB);
		break;
		/* Enemy is The Wall */
	case 1: EnemyType_Init(enemy, enemyWall_4);
		break;
		/* Enemy is Shooter */
	case 2: if (enemy->id == 1) EnemyType_Init(enemy, enemyShooter_P1);
			else EnemyType_Init(enemy, enemyShooter_P2);
			break;
			/* Enemy is BigShot */
	case 3: if (enemy->id == 1) EnemyType_Init(enemy, enemyBigShot_P1_2);
			else EnemyType_Init(enemy, enemyBigShot_P2_2);
			break;
			/* Enemy is Pea */
	case 4: if (enemy->id == 1) EnemyType_Init(enemy, enemyPea_P1);
			else EnemyType_Init(enemy, enemyPea_P2);
			break;
			/* Enemy is Arc */
	case 5: if (enemy->id == 1) EnemyType_Init(enemy, enemyArc_P1_3);
			else EnemyType_Init(enemy, enemyArc_P2_3);
			break;
			/* Enemy is Torpedo */
	case 6: EnemyType_Init(enemy, enemyTorpedo_3);
		break;
		/* Enemy is Jebus */
	case 7: EnemyType_Init(enemy, enemyJebus_2);
		break;
		/* Enemy is Hydra */
	case 8: Hydra_Init(enemy);
		break;
	}
}

/* Initialises the unit based on its type and given data */
void EnemyType_Init(Enemy *enemy, const char *enemyChar[])
{
	switch (enemy->type)
	{
		/* Enemy is BB */
	case 0: enemy->max_HP = enemy->HP = 1;
		break;
		/* Enemy is The Wall */
	case 1: enemy->max_HP = enemy->HP = 4;
		break;
		/* Enemy is Shooter */
	case 2: enemy->max_HP = enemy->HP = 1;
		break;
		/* Enemy is Big Shot */
	case 3: enemy->max_HP = enemy->HP = 2;
		break;
		/* Enemy is Pea */
	case 4: enemy->max_HP = enemy->HP = 1;
		break;
		/* Enemy is Arc */
	case 5: enemy->max_HP = enemy->HP = 3;
		break;
		/* Enemy is Torpedo */
	case 6: enemy->max_HP = enemy->HP = 3;
		break;
		/* Enemy is Jebus */
	case 7: enemy->max_HP = enemy->HP = 2;
		break;
	}

	EnemyCopyArt(enemy, enemyChar);
}

/* Changes art of enemy based on HP when it gets hit */
void EnemyTypeHP_ArtSwitch(Enemy *enemy)
{
	switch (enemy->type)
	{
		/* Change art of The Wall based on HP */
	case 1:
		if (enemy->HP == 4)
			EnemyCopyArt(enemy, enemyWall_4);
		else if (enemy->HP == 3)
			EnemyCopyArt(enemy, enemyWall_3);
		else if (enemy->HP == 2)
			EnemyCopyArt(enemy, enemyWall_2);
		else if (enemy->HP == 1)
			EnemyCopyArt(enemy, enemyWall_1);
		break;

		/* Change art of BigShot based on HP */
	case 3:
		if (enemy->id == 1)
		{
			if (enemy->HP == 2)
				EnemyCopyArt(enemy, enemyBigShot_P1_2);
			else if (enemy->HP == 1)
				EnemyCopyArt(enemy, enemyBigShot_P1_1);
		}

		else if (enemy->id == 2)
		{
			if (enemy->HP == 2)
				EnemyCopyArt(enemy, enemyBigShot_P2_2);
			else if (enemy->HP == 1)
				EnemyCopyArt(enemy, enemyBigShot_P2_1);
		}
		break;

		/* Change art of Arc based on HP */
	case 5:
		if (enemy->id == 1)
		{
			if (enemy->HP == 3)
				EnemyCopyArt(enemy, enemyArc_P1_3);
			else if (enemy->HP == 2)
				EnemyCopyArt(enemy, enemyArc_P1_2);
			else if (enemy->HP == 1)
				EnemyCopyArt(enemy, enemyArc_P1_1);
		}
		else if (enemy->id == 2)
		{
			if (enemy->HP == 3)
				EnemyCopyArt(enemy, enemyArc_P2_3);
			else if (enemy->HP == 2)
				EnemyCopyArt(enemy, enemyArc_P2_2);
			else if (enemy->HP == 1)
				EnemyCopyArt(enemy, enemyArc_P2_1);
		}
		break;

		/* Change art of Torpedo based on HP */
	case 6:
		if (enemy->HP == 3)
			EnemyCopyArt(enemy, enemyTorpedo_3);
		else if (enemy->HP == 2)
			EnemyCopyArt(enemy, enemyTorpedo_2);
		else if (enemy->HP == 1)
			EnemyCopyArt(enemy, enemyTorpedo_1);
		break;

		/* Change art of Jebus based on HP */
	case 7:
		if (enemy->HP == 2)
			EnemyCopyArt(enemy, enemyJebus_2);
		else if (enemy->HP == 1)
			EnemyCopyArt(enemy, enemyJebus_1);
		break;
	}
}
