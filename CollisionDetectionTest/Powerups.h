/******************************************************************************
All content © 2017 Digipen Institute of Technology Singapore, all rights reserved

filename    Powerups.h
author      Wong Zhihao
DP email    zhihao.wong@digipen.edu
course      RTIS

Brief Description:
This file declares the Powerup object and its functions.

******************************************************************************/

#pragma once

/* Struct forward declarations */
typedef struct Enemy Enemy;
typedef struct Enemy_AI Enemy_AI;
typedef struct Ball Ball;
typedef struct Stage Stage;
typedef struct Player Player;
typedef struct TileMapManager TileMapManager;
typedef struct Sound Sound;

typedef struct Powerup {
	char art;			/* The art of the powerup */
	int type;			/* The type of powerup */
	int x_pos;			/* The x coord of the powerup */
	int y_pos;			/* The y coord of the powerup */
	int direction;		/* Which player it is dropping to */
	int pass_through;	/* If powerup is passing through an enemy */
	int frames;			/* How many often out of this amount of frames it updates for */
	int timer;			/* Tick tocks every frame */
	int base_timer;		/* Adjusts how often it updates per frame out of the base timer */
} Powerup;

/* Create a new Powerup object with the provided data */
Powerup *Powerup_Create(Enemy *enemy, Ball *ball);

/* Create a new Kill Enemy power with the provided data */
Powerup *Powerup_KillEnemy_Create(Stage *stage, int direction);

/* Determines the art based on its type */
char Powerup_ArtSwitcher(Powerup *powerup);

/* Free the memory associated with a given Powerup object */
void Powerup_Free(Powerup **powerupPtr);

/* Randomises if powerup should drop when enemy is killed */
void Powerup_Spawner(Powerup *powerup[], Enemy *enemy, Ball *ball);

/* Activates the powerup based on its type*/
void Powerup_Activate(Powerup *powerup, Stage *stage, Player *player, Ball *ball, TileMapManager *tile_manager, Sound *sound,
					  Enemy *enemy1[], Enemy *enemy2[], Enemy_AI *enemy_AI1, Enemy_AI *enemy_AI2, int player_num);

/* Detects if powerup hit anything */
int Powerup_CollisionDetection(Powerup *powerup, Player *player1, Player *player2, Ball *ball1, Ball *ball2, Stage *stage, TileMapManager *tile_manager, 
							   Sound *sound, Enemy *enemy1[], Enemy *enemy2[], Enemy_AI *enemy_AI1, Enemy_AI *enemy_AI2);

/* Handles movement of powerup */
void Powerup_Move(Powerup *powerup, TileMapManager *tile_manager);

/* Updates powerup every frame */
void Powerup_Update(Powerup *powerup[], Player *player1, Player *player2, Ball *ball1, Ball *ball2, Stage *stage, TileMapManager *tile_manager, 
					Sound *sound, Enemy *enemy1[], Enemy *enemy2[], Enemy_AI *enemy_AI1, Enemy_AI *enemy_AI2);

/* Handles the functions that happen when the ball hits a powerup */
void Ball_Powerup_CollisionDetection(int x_pos, int y_pos, Stage* stage, TileMapManager *tileManager, Sound *sound, Ball *ball, Powerup *powerup[],
									 Enemy *enemy1[], Enemy *enemy2[], Enemy_AI *enemy_AI1, Enemy_AI *enemy_AI2);

/* When player hits the powerup, find which powerup it hit and activates it */
void Player_Powerup_Collision(Powerup *powerup[], Stage *stage, Player *player, Ball *ball, TileMapManager *tile_manager, Sound *sound, 
							  Enemy *enemy1[], Enemy *enemy2[], Enemy_AI *enemy_AI1, Enemy_AI *enemy_AI2);