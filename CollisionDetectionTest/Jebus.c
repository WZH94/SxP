/******************************************************************************
All content © 2017 Digipen Institute of Technology Singapore, all rights reserved

filename			Jebus.c
author				Wong Zhihao
DP email			zhihao.wong@digipen.edu
course				RTIS

Brief Description:
Handles the heal and revive functions of the Jebus enemy when it moves

******************************************************************************/

#include <stdlib.h>		/* rand */
#include "Jebus.h"		/* Function declarations */
#include "Enemy.h"		/* Enemy object and functions */
#include "EnemyTypes.h"	/* Enemy Types and functions */
#include "Stage.h"		/* Stage object and functions */
#include "FMODInit.h"	/* Sound object and functions */

/* Finds for the appropriate unit in Jebus' surroundings and returns that unit's position in the array */
int Jebus_CheckSurrounding(Stage *stage, Enemy *jebus, Enemy *enemy[], int check_x, int check_y)
{	
	int i;	/* Loop counter */

	/* Loops through every enemy */
	for (i = 0; i < stage->total_and_hydra; ++i)
	{
		/* Check if the enemy exists, mainly for hydra units */
		if (enemy[i] != NULL)
		{
			/* Is checking for enemies on the right (x + 1) */
			if (check_x > 0 && (enemy[i]->x_Pos <= jebus->x_Pos + check_x) && (enemy[i]->x_Pos > jebus->x_Pos))
				return i;
			/* Is checking for enemies on the left (x - 1) */
			else if (check_x < 0 && (enemy[i]->x_Pos >= jebus->x_Pos + check_x) && (enemy[i]->x_Pos < jebus->x_Pos))
				return i;
			/* Is checking for enemies on the top (y - num_enemies) */
			else if (enemy[i]->id == 1 && check_y < 0 && (enemy[i]->y_Pos >= jebus->y_Pos + check_y) && (enemy[i]->y_Pos < jebus->y_Pos))
				return i;
			/* Is checking for enemies on the bottom (y + num_enemies) */
			else if (enemy[i]->id == 1 && check_y > 0 && (enemy[i]->y_Pos <= jebus->y_Pos + check_y) && (enemy[i]->y_Pos > jebus->y_Pos))
				return i;
		}
	}
	/* No enemy to heal or revive */
	return 0;
}

/* Handles the AI of Jebus */
void Jebus_AI(Stage *stage, Enemy *enemy[], Enemy_AI *enemy_AI, TileMapManager *tileManager, Sound *sound)
{
	int i;					/* Loop counter */
	int enemy_location;		/* Location of enemy to heal or revive */

	/* Goes through every enemy */
	for (i = 0; i < stage->total_enemies; ++i)
	{
		/* Checks for Jebuses within the ranks of the enemy */
		if (enemy[i]->type == 7 && enemy[i]->alive == 1 && enemy[i]->HP > 0)
		{
			/* Randomises if Jebus will take an action in the current move */
			if ((rand() % 6) == 0)
			{
				/* Randomises whether to heal or to revive, if passes, heals every injured enemy around it */
				if ((rand() % 6 < 4))
				{
					/* Checks for enemies on the right */
					enemy_location = Jebus_CheckSurrounding(stage, enemy[i], &enemy[0], stage->horizontal_gap + ENEMY_WIDTH, 0);
					if (enemy_location)
						/* Heals specified enemy */
						Jebus_Heal(enemy[enemy_location], enemy_AI, tileManager, sound);

					/* Checks for enemies on the left */
					enemy_location = Jebus_CheckSurrounding(stage, enemy[i], &enemy[0], -(stage->horizontal_gap + ENEMY_WIDTH), 0);
					if (enemy_location)
						/* Heals specified enemy */
						Jebus_Heal(enemy[enemy_location], enemy_AI, tileManager, sound);

					/* Checks for enemies on the top */
					enemy_location = Jebus_CheckSurrounding(stage, enemy[i], &enemy[0], 0, -(stage->vertical_gap + ENEMY_HEIGHT));
					if (enemy_location)
						/* Heals specified enemy */
						Jebus_Heal(enemy[enemy_location], enemy_AI, tileManager, sound);

					/* Checks for enemies on the bottom */
					enemy_location = Jebus_CheckSurrounding(stage, enemy[i], &enemy[0], 0, stage->vertical_gap + ENEMY_HEIGHT);
					if (enemy_location)
						/* Heals specified enemy */
						Jebus_Heal(enemy[enemy_location], enemy_AI, tileManager, sound);
				}

				/* Revives ONE random enemy around it if any are dead */
				else
				{
					int choose_enemy;		/* Randomises an enemy to revive */
					int rechoose_enemy = 1; /* If chosen enemy was alive, rechoose another enemy */

					/* Handles if every particular enemy has been checked so it does not go into infinite loop */
					int e1, e2, e3, e4;
					/* Initialises to not checked yet */
					e1 = e2 = e3 = e4 = 0;

					/* Makes it choose a random dead enemy to revive until it finishes checking all of them */
					while (rechoose_enemy)
					{
						/* Randomly chooses an enemy on its side to revive */
						choose_enemy = (rand() % 4);

						/* Tells code that these enemies have already been checked */
						switch (choose_enemy)
						{
							/* e1 enemy has been checked etc.. */
						case 0: ++e1;
							break;
						case 1: ++e2;
							break;
						case 2: ++e3;
							break;
						case 3: ++e4;
							break;
						}

						/* Checks for enemies on the right (e1) */
						enemy_location = Jebus_CheckSurrounding(stage, enemy[i], &enemy[0], stage->horizontal_gap + ENEMY_WIDTH, 0);
						if (enemy_location && choose_enemy == 0 && rechoose_enemy)
							rechoose_enemy = Jebus_Revive(enemy[enemy_location], enemy_AI, tileManager, sound);

						/* Checks for enemies on the left (e2) */
						enemy_location = Jebus_CheckSurrounding(stage, enemy[i], &enemy[0], -(stage->horizontal_gap + ENEMY_WIDTH), 0);
						if (enemy_location && choose_enemy == 1 && rechoose_enemy)
							rechoose_enemy = Jebus_Revive(enemy[enemy_location], enemy_AI, tileManager, sound);

						/* Checks for enemies on the top (e3) */
						enemy_location = Jebus_CheckSurrounding(stage, enemy[i], &enemy[0], 0, -(stage->vertical_gap + ENEMY_HEIGHT));
						if (enemy_location && choose_enemy == 2 && rechoose_enemy)
							rechoose_enemy = Jebus_Revive(enemy[enemy_location], enemy_AI, tileManager, sound);

						/* Checks for enemies on the bottom (e4) */
						enemy_location = Jebus_CheckSurrounding(stage, enemy[i], &enemy[0], 0, -(stage->vertical_gap + ENEMY_HEIGHT));
						if (enemy_location && choose_enemy == 3 && rechoose_enemy)
							rechoose_enemy = Jebus_Revive(enemy[enemy_location], enemy_AI, tileManager, sound);

						/* If every enemy has been checked or an enemy has already been revived, break out of loop */
						if ((e1 && e2 && e3 && e4) || rechoose_enemy == 0)
							break;
					}
				}
			}
		}
	}
}

/* Function to heal enemies around Jebus */
void Jebus_Heal(Enemy *enemy, Enemy_AI *enemy_AI, TileMapManager *tileManager, Sound *sound)
{
	/* Checks this enemy if it is alive and not at full HP */
	if (enemy->alive == 1 && enemy->full_HP != 1)
	{
		/* Increases enemy HP */
		++enemy->HP;

		/* Clears and reprints the enemy */
		Enemy_IndvClear(enemy, enemy_AI, tileManager);
		EnemyTypeHP_ArtSwitch(enemy);
		Enemy_IndvPrint(enemy, enemy_AI, tileManager);

		/* If enemy at full HP, set the full HP flag to 1 */
		if (enemy->HP == enemy->max_HP)
			enemy->full_HP = 1;

		/* Plays a sound for this action */
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\JebusHeal.wav");
		playSound_Medium(sound);
	}
}

/* Function to revive enemies around Jebus */
int Jebus_Revive(Enemy *enemy, Enemy_AI *enemy_AI, TileMapManager *tileManager, Sound *sound)
{
	/* Checks this enemy if it is dead */
	if (enemy->alive == 0 && enemy->HP <= 0)
	{
		/* Revive enemy and sets HP back to max HP */
		enemy->alive = 1;
		enemy->HP = enemy->max_HP;
		enemy->full_HP = 1;

		/* Clears and reprints the enemy */
		Enemy_IndvClear(enemy, enemy_AI, tileManager);
		EnemyTypeHP_ArtSwitch(enemy);
		Enemy_IndvPrint(enemy, enemy_AI, tileManager);

		/* Plays a sound for this action */
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\JebusRevive.wav");
		playSound_Medium(sound);

		return 0;
	}

	else return 1;
}