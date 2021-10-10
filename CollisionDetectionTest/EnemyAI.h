/******************************************************************************
All content © 2017 Digipen Institute of Technology Singapore, all rights reserved

filename    EnemyAI.h
author      Wong Zhihao
DP email    zhihao.wong@digipen.edu
course      RTIS

Brief Description:
This file declares the Enemy_AI object, and all its functions

******************************************************************************/

#pragma once

/* Struct forward declarations */
typedef struct EnemyBullet EnemyBullet;
typedef struct Ball Ball;
typedef struct TileMapManager TileMapManager;
typedef struct Stage Stage;
typedef struct Enemy Enemy;
typedef struct Sound Sound;
typedef struct Powerup Powerup;

/* Contents of Enemy AI */
typedef struct Enemy_AI
{
	int forward_trigger; /* Triggers the enemy to move towards the player */
	int left_trigger;    /* Triggers the enemy to move left (P1 perspective) */
	int right_trigger;	 /* Triggers the enemy to move right (P1 perspective) */
	int time_to_move;	 /* Stops the enemy from moving back the other direction right after it has changed */
	int dist_player;     /* Controls how much it can move towards the player */
	int p_mult;			 /* Reverses direction of enemies facing P2 */
	int y_spawn;		 /* Adjusts spawn location based on which player it is spawning for */
	int frames;			 /* How many often out of this amount of frames it updates for */
	int timer;			 /* Tick tocks every frame */
	int base_timer;		 /* Adjusts how often it updates per frame out of the base timer */
} Enemy_AI;

/* Create a new Enemy AI object with the provided data */
Enemy_AI* Enemy_AI_Create(Stage *stage, int player, int y_spawn);

/* Free the memory associated with a given Enemy AI object */
void Enemy_AI_Free(Enemy_AI** enemybehaviourPtr);

/* Controls the behaviour of the enemy */
int Enemy_Control(Stage *stage, Enemy *enemy[], Enemy_AI *enemy_AI, TileMapManager *tileManager, Ball *ball1, Ball *ball2, Powerup *powerup[], Sound *sound);

/* Checks for the nearest alive enemy in y coord to the player */
int Enemy_FrontLine_Check(Stage *stage, Enemy *enemy[]);

/* Finds the coordinate of the enemy which is nearest to the left border */
int Enemy_LeftSide_Check(Stage *stage, Enemy *enemy[]);

/* Finds the coordinate of the enemy which is nearest to the right border */
int Enemy_RightSide_Check(Stage *stage, Enemy *enemy[]);

/* Randomises whether the enemy shoots in the current frame */
void Enemy_Fire(Stage *stage, Enemy *enemy[], EnemyBullet *enemyBullet[], Sound *sound);