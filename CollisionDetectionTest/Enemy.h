/******************************************************************************
All content © 2017 Digipen Institute of Technology Singapore, all rights reserved

filename    Enemy.h
author      Wong Zhihao
DP email    zhihao.wong@digipen.edu
course      RTIS

Brief Description:
This file declares the Enemy object, the enemy height and width as it never
changes, and function declarations

******************************************************************************/
#pragma once

/* Struct forward declarations */
typedef struct Stage Stage;
typedef struct Enemy_AI Enemy_AI;
typedef struct Ball Ball;
typedef struct TileMapManager TileMapManager;
typedef struct Sound Sound;
typedef struct Powerup Powerup;
typedef struct Player Player;

/* the maximum height of an enemy */
#define ENEMY_HEIGHT 3

/* the maximum height of an enemy */
#define ENEMY_WIDTH 4

/* Forward declaration for the Item type */
typedef struct Enemy
{
	char enemyStyle[ENEMY_HEIGHT][ENEMY_WIDTH + 1]; /* Dimensions of enemy */
	int x_Pos;										/* x coord of enemy */
	int y_Pos;										/* y coord of enemy */
	int alive;										/* Status of enemy */
	int type;										/* Type of enemy */
	int is_hydra;									/* Check if the enemy is a hydra */
	int hydra_link;									/* Links the extra hydra unit with the original one for easier coding :> */
	int HP;											/* HP of enemy */
	int max_HP;										/* Max possible HP of enemy */
	int full_HP;									/* Whether enemy is at full HP or not, for Jebus functions */
	int id;											/* Which player it is facing */
	int fireActive;									/* Whether enemy can fire */
	int fireReload;									/* Max reload time for every enemy */
	int fireReloader;								/* Tracks reload time */
	int fireChance;									/* Handles how often it fires */
} Enemy;

/* Create a new Enemy object with the provided data */
Enemy* Enemy_Create(Stage *stage, int x_pos, int y_pos, int player_mult, int id, TileMapManager *tileManager, int hydra_type);

/* Free the memory associated with a given Enemy object */
void Enemy_Free(Enemy **enemyPtr);

/* Sets the art of the enemy */
void EnemyCopyArt(Enemy *enemy, const char *enemyChar[]);

/* Sets how often each enemy fires */
void EnemyFireChance(Stage *stage, Enemy *enemy);

/* Reprints the enemies in their new positions */
void Enemy_Print(Stage* stage, Enemy* enemy[], Enemy_AI* enemy_AI, TileMapManager *tileManager);

/* Prints individual enemy */
void Enemy_IndvPrint(Enemy* enemy, Enemy_AI* enemy_AI, TileMapManager *tileManager);

/* Prints individual enemy for Menu */
void Enemy_IndvPrint_Menu(Enemy* enemy);

/* Initiaises the enemies based on the stage */
void Enemy_Spawn(Stage *stage, Enemy* enemy[], Enemy_AI *enemy_AI, TileMapManager *tileManager, int id, Sound* sound);

/* Sets which enemies can fire */
void Enemy_FireActive_Update(Stage *stage, Enemy *enemy[]);

/* Reloads the enemies that can fire */
void Enemy_Fire_Reload(Stage *stage, Enemy *enemy[]);

/* Contains all the functions needed to update and redraw the enemy positions */
void Enemy_Update(Stage *stage, Enemy *enemy[], Enemy_AI *enemy_AI, int x, int y, TileMapManager *tileManager, Ball *ball1, Ball *ball2,
				  Powerup *powerup[], Sound *sound);

/* Enemy collides with the ball */
void Enemy_Ball_CollisionDetection(Stage* stage, Enemy* enemy, Enemy_AI* enemy_AI, TileMapManager *tileManager,
								   Sound *sound, Ball *ball, Powerup *powerup[], int enemy_x_change, int enemy_y_change);

/* Clears individual enemy */
void Enemy_IndvClear(Enemy* enemy, Enemy_AI* enemy_behaviour, TileMapManager *tileManager);
