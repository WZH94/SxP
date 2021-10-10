/******************************************************************************
All content © 2017 Digipen Institute of Technology Singapore, all rights reserved

filename			TileManager.h
author				Ngm Hui Min, Lois
DP email			lois.ngm@digipen.edu
course				RTIS

Brief Description:
Header file for TileMapManager.c which contains the contents of tile map 
object of the game.
******************************************************************************/
#pragma once

/* necessary structs */
typedef struct EnemyBullet EnemyBullet;
typedef struct Powerup Powerup;
typedef struct Player Player;
typedef struct Ball Ball;
typedef struct Enemy Enemy;

/* Contents of a TileMapManager */
typedef struct TileMapManager
{
	char tileManagement[78][80];
} TileMapManager;


/* Create a new TileMapmanager object, using the provided parameters */
TileMapManager* TileMapManager_Create();

/* Create a new TileMapmanager object for the main menu, using the provided parameters */
TileMapManager* TileMapManagerMainMenu_Create();

/* Create a new TileMapmanager object for the main menu when user selects PvP Stage, using the provided parameters */
TileMapManager* TileMapManagerPvPStage_Create();

/* Create a new TileMapmanager object for the main menu when user selects PvP Terrain, using the provided parameters */
TileMapManager* TileMapManagerPvPTerrain_Create();

/* Create a new TileMapmanager object for the main menu when user selects Coop, using the provided parameters */
TileMapManager* TileMapManagerCoOpMenu_Create();

/* Resets the tile manager every frame in Menu */
void TileMapManagerMenu_Update(TileMapManager *tileManager);

/* Resets the tile manager every frame in PvP Stage Menu */
void TileMapManagerPvPStage_Update(TileMapManager *tileManager);

/* Resets the tile manager every frame in PvP Terrain Menu */
void TileMapManagerPvPTerrain_Update(TileMapManager *tileManager);

/* Resets the tile manager every frame in Coop Menu */
void TileMapManagerCoOpMenu_Update(TileMapManager *tileManager);

/* Free the memory associated with a given Tilemanager object */
void TileMapManager_Free(TileMapManager** tileManagerPtr);

/* Handles the movement of player with Tile map */
void TileMapManager_Player(Player *player, TileMapManager *tilemanager, int action);

/* Handles the movement of enemy with Tile map */
void TileMapManager_Enemy(Enemy *enemy, int direction, TileMapManager *tileManager, int action);

/* Handles collision detection of ball */
void TileMapManager_Ball(Ball *ball, TileMapManager *tileManager, int action);

/* Handles collision detection of powerup */
void TileMapManager_Powerup(Powerup *powerup, TileMapManager *tileManager, int action);

/* Handles collision detection of player */
int TileMapManager_Collision_Player(TileMapManager *tileManager, int next_x_player, int next_y_player);

/* Handles collision detection of enemies */
int TileMapManager_Collision_Enemy(TileMapManager *tileManager, Enemy* enemy);

/* Handles collision detection of powerup */
int TileMapManager_Powerup_Collision(Powerup *powerup, TileMapManager *tileManager);

int TileMapManager_Collision_Ball(TileMapManager *tileManager, int next_x_ballpos, int next_y_ballpos);

/* Handles initialising of bullet on Tile Manager */
void TileMapManager_EnemyBullet(EnemyBullet *enemyBullet, TileMapManager *tileManager, int action);

/* Handles collision detection of enemy bullets */
int TileMapManager_EnemyBullet_Collision(EnemyBullet *enemyBullet, TileMapManager *tileManager);

/* Checks if the hydra will be spawning into another enemy */
int TileMapManager_Hydra_Check(int x, int y, TileMapManager *tileManager);