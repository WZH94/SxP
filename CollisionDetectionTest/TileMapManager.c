/******************************************************************************
All content © 2017 Digipen Institute of Technology Singapore, all rights reserved

filename			TileMapManager.c
author				Ngm Hui Min, Lois
DP email			lois.ngm@digipen.edu
course				RTIS

Brief Description:
This file contains the algorithmn to check collisions between objects and updates
accordingly as the game runs.
******************************************************************************/
#include "stdafx.h" 
#include <stdlib.h> /* NULL, malloc/free */
#include "EnemyTypes.h" /* EnemyTypes object */
#include "Enemy.h" /* Enemy object */
#include "Ball.h" /* Ball object */
#include "Player.h" /* Player object*/
#include "Powerups.h" /* Powerup object */
#include "EnemyBullets.h" /* EnemyBullets object */
#include "TileMapManager.h" /* Function declarations */

/* powerup movements*/
#define TOPLEFT20 11
#define TOPRIGHT20 10
#define BOTTOMLEFT20 9
#define BOTTOMRIGHT20 8
#define TOPLEFT60 7
#define TOPRIGHT60 6
#define BOTTOMLEFT60 5
#define BOTTOMRIGHT60 4
#define TOPLEFT45 3
#define TOPRIGHT45 2
#define BOTTOMLEFT45 1
#define BOTTOMRIGHT45 0

/* Create a new TileMapManager object for stages, using the provided parameters */
TileMapManager* TileMapManager_Create()
{
	TileMapManager* tileManager; /* The new TileMapManager object for stages */

	/* allocate the memory for the new TileMapManager object */
	tileManager = (TileMapManager*)malloc(sizeof(TileMapManager));
	if (tileManager == NULL)
	{
		return NULL; /* malloc can fail! */
	}
	int i = 0, j = 0; /* loop variables */
	int c; /* to store the character in the tile */
	FILE *file; /* read file */
	errno_t tmp;
	tmp = fopen_s(&file, "tilemap.txt", "r"); /* reads the file and assign to file */
	while ((c = getc(file)) != EOF) /* check if the character is not the end of the file */ 
	{
		/* if it is the end of the line */
		if (c == '\n')
		{
			/* increment height */
			i++;
			/* reset width */
			j = 0;
		}
		else if (c != ' ')
		{
			/* the char into the array of array of tileManagement */
			tileManager->tileManagement[i][j] = (char)c;
			/* next x position */
			j++;
		}
	}
	/* when complete, close the file */
	fclose(file);
	/* return the new object */
	return tileManager;
}

/* Create a new TileMapManager object for the main menu, using the provided parameters */
TileMapManager* TileMapManagerMainMenu_Create()
{
	TileMapManager* tileManager; /* The new TileMapManager object for main menu */

								 /* allocate the memory for the new WorldData object */
	tileManager = (TileMapManager*)malloc(sizeof(TileMapManager));
	if (tileManager == NULL)
	{
		return NULL; /* malloc can fail! */
	}
	int i = 0, j = 0; /* looping variables */
	int c; /* to store the character in the tile */
	FILE *file; /* read file */
	errno_t tmp;
	tmp = fopen_s(&file, "tilemapmainmenu.txt", "r"); /* reads the file and assign to file */
	while ((c = getc(file)) != EOF) /* check if the character is not the end of the file */ 
	{
		if (c == '\n') /* if it is the end of the line */
		{
			i++; /* increment height */
			/* reset width */
			j = 0;
		}
		else if (c != ' ')
		{
			/* the char into the array of array of tileManagement */
			tileManager->tileManagement[i][j] = (char)c;
			/* next x position */
			j++;
		}
		
	}
	/* when complete, close file */
	fclose(file);
	/* return the new object */
	return tileManager;
}

/* Create a new TileMapManager object for MainMenu when player selects the PvP stage page, using the provided parameters */
TileMapManager* TileMapManagerPvPStage_Create()
{
	TileMapManager* tileManager; /* The new TileMapManager object, for MainMenu when player selects the PvP page */

	/* allocate the memory for the new TileMapManager object, for MainMenu when player selects the PvP page */
	tileManager = (TileMapManager*)malloc(sizeof(TileMapManager));
	if (tileManager == NULL)
	{
		return NULL; /* malloc can fail! */
	}
	int i = 0, j = 0; /* looping variables */
	int c; /* to store the character in the tile */
	FILE *file; /* read file */
	errno_t tmp;
	tmp = fopen_s(&file, "tilemappvpstagemenu.txt", "r"); /* reads the file and assign to file */
	while ((c = getc(file)) != EOF) /* check if the character is not the end of the file */ 
	{
		if (c == '\n') /* if it is the end of the line */
		{
			/* increase height */
			i++;
			/* reset width */
			j = 0;
		}
		else if (c != ' ')
		{
			/* the char into the array of array of tileManagement */
			tileManager->tileManagement[i][j] = (char)c;
			/* go to next x pos */
			j++;
		}

	}
	/* When completed, close file */
	fclose(file);
	/* return the new object */
	return tileManager;
}

/* Create a new TileMapManager object for MainMenu when player selects the PvP terrain page, using the provided parameters */
TileMapManager* TileMapManagerPvPTerrain_Create()
{
	TileMapManager* tileManager; /* The new TileMapManager object for MainMenu when player selects the Coop page */

	/* allocate the memory for the new TileMapManager object for MainMenu when player selects the Coop page */
	tileManager = (TileMapManager*)malloc(sizeof(TileMapManager));
	if (tileManager == NULL)
	{
		return NULL; /* malloc can fail! */
	}
	int i = 0, j = 0; /* looping variable */
	int c; /* to store the character in the file */
	FILE *file; /* read file */
	errno_t tmp;
	tmp = fopen_s(&file, "tilemappvpterrainmenu.txt", "r"); /* reads the file and assign to file*/
	while ((c = getc(file)) != EOF) /* checks if the character is not the end of the file */
	{
		if (c == '\n') /* if it is the end of the line */
		{
			/* increase height */
			i++;
			/* reset width */
			j = 0;
		}
		else if (c != ' ') 
		{
			/* thec char into the array of array of tileManagement */
			tileManager->tileManagement[i][j] = (char)c;
			/* go to the next x pos */
			j++;
		}

	}
	/* When completed, close file */
	fclose(file);
	/* return the new object */
	return tileManager;
}

/* Create a new TileMapManager object for MainMenu when player selects the Coop page, using the provided parameters */
TileMapManager* TileMapManagerCoOpMenu_Create()
{
	TileMapManager* tileManager; /* The new TileMapManager object for MainMenu when player selects the Coop page */

	 /* allocate the memory for the TileMapManager object for MainMenu when player selects the Coop page */
	tileManager = (TileMapManager*)malloc(sizeof(TileMapManager));
	if (tileManager == NULL)
	{
		return NULL; /* malloc can fail! */
	}
	int i = 0, j = 0; /* looping variables */
	int c; /* to store the character in the file */
	FILE *file; /* read file */
	errno_t tmp;
	tmp = fopen_s(&file, "tilemapcoopmenu.txt", "r"); /* reads the file and assigns to file */
	while ((c = getc(file)) != EOF) /* checks if the character is not the end of the file */
	{
		if (c == '\n') /* if it is the end of the line */
		{
			i++;/* increase height */
			j = 0; /* reset width */
		}
		else if (c != ' ')
		{
			tileManager->tileManagement[i][j] = (char)c; /* the char into the aray of array of tileManagement */
			j++; /* go to the next x pos */
		}

	}
	/* When completed, close file */
	fclose(file);
	/* return the new object */
	return tileManager;
}


/* Resets the tile manager every frame in Menu */
void TileMapManagerMenu_Update(TileMapManager *tileManager)
{
	int i = 0, j = 0; /* looping variable */
	int c; /* to store the charcter in the file */
	FILE *file; /* read file */
	errno_t tmp;
	tmp = fopen_s(&file, "tilemapmainmenu.txt", "r"); /* reads the file and assigns to file */

	while ((c = getc(file)) != EOF) /* checks if the character is not the end of the file */
	{
		if (c == '\n') /* if it is the end of the line */
		{
			i++; /* increase height */
			j = 0; /* reset width */
		}
		else if (c != ' ') 
		{
			tileManager->tileManagement[i][j] = (char)c; /* the char into the array of array of tileManagement*/
			j++; /* go to the next x pos */
		}
	}
	/* when completed close the file */
	fclose(file);
}

/* Resets the tile manager every frame in PvP Stage Menu */
void TileMapManagerPvPStage_Update(TileMapManager *tileManager)
{
	int i = 0, j = 0; /* looping variable */
	int c; /* to store the character in the file */
	FILE *file; /* read file */
	errno_t tmp;
	tmp = fopen_s(&file, "tilemappvpstagemenu.txt", "r"); /* reads the file and assigns to file */

	while ((c = getc(file)) != EOF) /* checks  if it is the end of the file */
	{
		if (c == '\n') /* if it is the end of the line */
		{
			i++; /* increase height */
			j = 0; /* reset width */
		}
		else if (c != ' ')
		{
			tileManager->tileManagement[i][j] = (char)c; /* the char into the array of array of tileManagement */
			j++; /* go to the next x pos */
		}
	}
	/* When completed close the file */
	fclose(file);

}

/* Resets the tile manager every frame in PvP Terrain Menu */
void TileMapManagerPvPTerrain_Update(TileMapManager *tileManager)
{
	int i = 0, j = 0; /* looping variable */
	int c; /* to store the character in the file */
	FILE *file; /* read file */
	errno_t tmp;
	tmp = fopen_s(&file, "tilemappvpterrainmenu.txt", "r"); /* reads the file and assigns to file */

	while ((c = getc(file)) != EOF) /* checks if it is the end of the file */
	{
		if (c == '\n') /* checks if it is the end of the line */
		{
			i++; /* increase height */
			j = 0; /* reset width */
		}
		else if (c != ' ')
		{
			tileManager->tileManagement[i][j] = (char)c; /* the char into the array of array of tileManagement */
			j++; /* go to the next x pos */
		}
	}
	/* When completed, close the file */
	fclose(file);

}

/* Resets the tile manager every frame in Co-Op Menu */
void TileMapManagerCoOpMenu_Update(TileMapManager *tileManager)
{
	int i = 0, j = 0; /* looping variable */
	int c; /* to store the character in the file */
	FILE *file; /* read file */
	errno_t tmp;
	tmp = fopen_s(&file, "tilemapcoopmenu.txt", "r"); /* reads teh file and assigns to file */

	while ((c = getc(file)) != EOF) /* checks if it is the end of the file */
	{
		if (c == '\n') /* checks if it is the end of the line */
		{
			i++; /* increase height */
			j = 0; /* resets width */
		}
		else if (c != ' ')
		{
			tileManager->tileManagement[i][j] = (char)c; /* the char into array of array into of tileManagement*/
			j++; /* go to the next x pos */
		}
	}
	/* When completed, close file */
	fclose(file);

}

/* Free the memory associated with a given Tilemanager object */
void TileMapManager_Free(TileMapManager** tileManagerPtr)
{
	/* safety check on the parameters */
	if ((tileManagerPtr == NULL) || (*tileManagerPtr == NULL))
	{
		return;  /* take no action if the parameters are invalid */
	}

	/* free the given item */
	free(*tileManagerPtr);

	/* clear the provided pointer */
	*tileManagerPtr = NULL;

}
/**************************************************
Movement of objects about the Tile Map
***************************************************/
/* Movement of objects about the Tile for Player*/
void TileMapManager_Player(Player *player, TileMapManager *tileManager, int action)
{
	int i, j; /* looping variablae */
	/* loop from player height */
	for (i = player->y_p; i < player->y_p + 2; ++i)
	{
		/* loop from player width */
		for (j = player->x_p; j < player->x_p + player->x_l; ++j)
		{
			switch (action)
			{
				/* to clear */
			case 0:
				tileManager->tileManagement[i][j] = '0';
				break;
				/* if is player 1 */
			case 1:
				tileManager->tileManagement[i][j] = '1';
				break;
				/* if is player 2 */
			case 2:
				tileManager->tileManagement[i][j] = '2';
				break;
			default:
				break;
			}
		}
	}
}

/* Movement of objects about the Tile for Enemy */
void TileMapManager_Enemy(Enemy* enemy, int direction, TileMapManager *tileManager, int action)
{
	int i, j; /* looping variable */
	int y; /* depends on where the enemy is facing, 1 for player 1, -1 for player 2 */
	/* loops the enemy height */
	for (i = 0, y=enemy->y_Pos; i < ENEMY_HEIGHT;  ++i, y += direction)
	{
		/* loops the enemy width */
		for (j = enemy->x_Pos; j < enemy->x_Pos + ENEMY_WIDTH; ++j)
		{
			switch (action)
			{
				/* to clear */
			case 0:
				tileManager->tileManagement[y][j] = '0';
				break;
				/* if is enemy */
			case 3:
				tileManager->tileManagement[y][j] = '3';
			default:
				break;
			}
		}
	}
}
/* Movement of objects about the Tile for Ball */
void TileMapManager_Ball(Ball *ball, TileMapManager *tileManager, int action)
{
	switch (action)
	{
		/* to clear */
	case 0:
		ball->y_ballpos;
		tileManager->tileManagement[ball->y_ballpos][ball->x_ballpos] = '0';
		break;
		/* if is player 1's ball */
	case 4:
		tileManager->tileManagement[ball->y_ballpos][ball->x_ballpos] = '4';
		break;
		/* if is player 2's ball */
	case 5:
		tileManager->tileManagement[ball->y_ballpos][ball->x_ballpos] = '5';
		break;
	default:
		break;
	}
}

/* Movement of objects about the Tile for Poewrup */
void TileMapManager_Powerup(Powerup *powerup, TileMapManager *tileManager, int action)
{
	switch (action)
	{
		/* to clear */
	case 0:
		tileManager->tileManagement[powerup->y_pos][powerup->x_pos] = '0';
		break;
		/* if is power up */
	case 7:
		tileManager->tileManagement[powerup->y_pos][powerup->x_pos] = '7';
		break;
	default:
		break;
	}
}

/* Movement of objects about the Tile for Enemy Bullets */
void TileMapManager_EnemyBullet(EnemyBullet *enemyBullet, TileMapManager *tileManager, int action)
{
	switch (action)
	{
		/* to clear */
	case 0:
		tileManager->tileManagement[enemyBullet->y_Pos][enemyBullet->x_Pos] = '0';
		break;
		/* if is an enemy bullet */
	case 1:
		tileManager->tileManagement[enemyBullet->y_Pos][enemyBullet->x_Pos] = '|';
		break;
	default:
		break;
	}
}

/**************************************************
 Collision Detection of objects about the Tile Map
***************************************************/

/* Handles collision detection of player */
int TileMapManager_Collision_Player(TileMapManager *tileManager, int next_x_player, int next_y_player)
{
	switch (tileManager->tileManagement[next_y_player][next_x_player])
	{
	case '4': return 4; /* Player hit ball 1 */
		break;
	case '5': return 5; /* Player hit ball 2 */
		break;
	case '7': return 7; /* Player hit powerup */
		break;
	case '8': return 8; /* Player hit bullet */
		break;
	default: return 0;
	}
}

/* Handles collision detection of enemies */
int TileMapManager_Collision_Enemy(TileMapManager *tileManager, Enemy* enemy)
{
	switch (tileManager->tileManagement[enemy->y_Pos][enemy->x_Pos])
	{
	case '0': return 0; /* Enemy is moving into empty space */
		break;
	case '4': return 4; /* Enemy is moving into ball 1 */
		break;
	case '5': return 5; /* Enemy is moving into ball 2 */
		break;
	default: return 0;
	}
}

/* Handles collision detection of ball */
int TileMapManager_Collision_Ball(TileMapManager *tileManager, int next_x_ballpos, int next_y_ballpos)
{
	switch (tileManager->tileManagement[next_y_ballpos][next_x_ballpos])
	{
	case '0': return 0; /* Next space is a free space */
		break;
	case '1': return 1; /* Next space is Player 1 */
		break;
	case '2': return 2; /* Next space is Player 2 */
		break;
	case '3': return 3; /* Next space is an enemy */
		break;
	case '4': return 4; /* Next space is Ball 1 */
		break;
	case '5': return 5; /* Next space is Ball 2 */
		break;
	case '6': return 6; /* Next space is the border */
		break;
	case '7': return 7; /* Next space is a powerup */
		break;
	case '|': return 8; /* Next space is a bullet */
		break;
	case '9': return 9; /* Next space is text */
		break;
	default: return 0;
	}
}

/* Handles collision detection of powerup */
int TileMapManager_Powerup_Collision(Powerup *powerup, TileMapManager *tileManager)
{
	int next_y;

	next_y = powerup->y_pos - powerup->direction;

	/* Powerup hit Player 1 */
	if (tileManager->tileManagement[next_y][powerup->x_pos] == '1')
		return 1;

	/* Powerup hit Player 2 */
	else if (tileManager->tileManagement[next_y][powerup->x_pos] == '2')
		return 2;

	/* Powerup passing through enemy */
	else if (tileManager->tileManagement[next_y][powerup->x_pos] == '3')
		return 3;

	/* Powerup hit Player 1 ball */
	else if (tileManager->tileManagement[next_y][powerup->x_pos] == '4')
		return 4;

	/* Powerup hit Player 2 ball */
	else if (tileManager->tileManagement[next_y][powerup->x_pos] == '5')
		return 5;

	/* Powerup hit border */
	else if (tileManager->tileManagement[next_y][powerup->x_pos] == '6')
		return 6;

	/* Nothing happened */
	else return 0;
}

/* Handles collision detection of enemy bullets */
int TileMapManager_EnemyBullet_Collision(EnemyBullet *enemyBullet, TileMapManager *tileManager)
{
	/* future positions of the enemy bullets */
	int next_y;
	int next_x = enemyBullet->x_Pos;

	/* if it shooting upwards or downwards */
	next_y = enemyBullet->y_Pos - enemyBullet->direction;

	/* from the enemy bullet's direction, determine the future x pos of the bullet */
	if (enemyBullet->left == 1)
		--next_x;
	else if (enemyBullet->right == 1)
		++next_x;

	/* Bullet hit Player 1 */
	if (tileManager->tileManagement[next_y][next_x] == '1')
		return 1;

	/* Bullet hit Player 2 */
	else if (tileManager->tileManagement[next_y][next_x] == '2')
		return 2;

	/* Bullet hit enemy */
	else if (tileManager->tileManagement[next_y][next_x] == '3')
		return 3;

	/* Bullet hit Player 1 ball */
	else if (tileManager->tileManagement[next_y][next_x] == '4')
		return 4;

	/* Bullet hit Player 2 ball */
	else if (tileManager->tileManagement[next_y][next_x] == '5')
		return 5;

	/* Powerup hit border */
	else if (tileManager->tileManagement[next_y][next_x] == '6')
	{
		return 6;
	}

	/* Nothing happened, carry on... */
	else return 0;
}

/* Checks if the hydra will be spawning into another enemy */
int TileMapManager_Hydra_Check(int x, int y, TileMapManager *tileManager)
{
	if (tileManager->tileManagement[y][x] == '3')
		return 1;

	else return 0;
}