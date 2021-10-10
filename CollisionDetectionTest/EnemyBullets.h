/******************************************************************************
All content © 2017 Digipen Institute of Technology Singapore, all rights reserved

filename    EnemyAI.h
author      Wong Zhihao
DP email    zhihao.wong@digipen.edu
course      RTIS

Brief Description:
This file declares the Enemy Bullet object, and all its functions

******************************************************************************/

#pragma once

/* Struct forward declarations */
typedef struct Stage Stage;
typedef struct Player Player;
typedef struct Sound Sound;
typedef struct Enemy Enemy;
typedef struct TileMapManager TileMapManager;

typedef struct EnemyBullet {
	char art;				/* Art of the bullet */
	int type;				/* Which enemy fired it */
	int x_Pos;				/* X coord of bullet */
	int y_Pos;				/* Y coord of bulllet */
	int left;				/* Left arc bullet moves leftwards */
	int right;				/* Right arc bullet moves rightwards */
	int direction;			/* Which player its moving towards */
	int bullet_type;		/* Which bullet type it is */
	int torp_track_timer;	/* How often torpedo bullet tracks the player */
	int pass_through;		/* Whether enemy is moving through an enemy */
	int frames;			    /* How many often out of this amount of frames it updates for */
	int timer;			    /* Tick tocks every frame */
	int base_timer;		    /* Adjusts how often it updates per frame out of the base timer */
} EnemyBullet;

/* Create a new Enemy Bullet object with the provided data */
EnemyBullet* EnemyBullet_Create(Sound *sound, Enemy *enemy, int arc);

/* Free the memory associated with a given EnemyBullet object */
void EnemyBullet_Free(EnemyBullet **enemyBulletPtr);

/* Initialises enemy bullet based on the enemy that will fire */
void EnemyBullet_Type_Init(EnemyBullet *enemyBullet, Enemy *enemy);

/* Creates the bullets for the Arc enemy */
void Enemy_Fire_Arc(Enemy *enemy, EnemyBullet *enemyBullets[], Sound *sound);

/* Plays the correct sound when enemy bullet is created, aka fired */
void EnemyFire_SFX(Enemy *enemy, Sound *sound);

/* Updates enemyBullet every frame */
void EnemyBullet_Update(EnemyBullet *enemyBullet[], Player *player1, Player *player2, Stage *stage, TileMapManager *tile_manager, Sound *sound);

/* Handles movement of Torpedo bullet */
void EnemyBullet_TorpedoMove(EnemyBullet *enemyBullet, Player *player1, Player *player2);

/* Handles the functions that happen when the ball hits a bullet */
void Ball_Bullet_Collision(int x_pos, int y_pos, TileMapManager *tileManager, Sound *sound, EnemyBullet *enemyBullet[]);

/* Detects if bullet hits player  */
int EnemyBullet_CollisionDetection(EnemyBullet *enemyBullet, Player *player1, Player *player2, TileMapManager *tile_manager, Sound *sound);

/* Player moved into a bullet */
void Player_Bullet_Collision(Player *player, EnemyBullet *enemyBullet[], TileMapManager *tile_manager, Sound *sound);