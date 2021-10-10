/******************************************************************************
All content © 2017 Digipen Institute of Technology Singapore, all rights reserved

filename			Ball_CollisionManager.h
author				Enriquez, Audry May dela Paz
DP email			a.enriquez@digipen.edu
course				BSGD

Brief Description:
Header file for Ball_CollisionManager.c which contains functions for 
collision detection for ball

******************************************************************************/
#pragma once
#include "stdafx.h"		/* bool */

typedef struct Sound Sound;
typedef struct Stage Stage;
typedef struct Player Player;
typedef struct Enemy Enemy;
typedef struct Enemy_AI Enemy_AI;
typedef struct Powerup Powerup;
typedef struct EnemyBullet EnemyBullet;
typedef struct TileMapManager TileMapManager;
typedef struct Ball Ball;

/*If ball has hit the player, function does the appropriate actions*/
void Ball_PlayerCollision(Sound *sound, Ball* ball, Player* player, Stage* stage, TileMapManager *tileManager);

/* Handles the functions that happen when the ball hits an enemy */
void Ball_Enemy_CollisionDetection(int current_x_pos, int current_y_pos, int x_pos, int y_pos, Stage* stage, Enemy* enemy[], Enemy_AI* enemy_AI,
	TileMapManager *tileManager, Sound *sound, Ball *ball, Powerup *powerup[]);
/*if the ball hits the borders, checks the bounce ball accordingly*/
void Ball_BorderCollision(Sound *sound, Ball* ball, Stage* stage);

/*checks if ball collides with anything using TileMap, returns 1 if no collision*/
int Ball_CollisionManager(Sound *sound, TileMapManager* tilemap, Ball* ball, int current_x_pos, int current_y_pos, int next_x_ballpos, int next_y_ballpos,
	Stage* stage, Enemy* enemy1[], Enemy* enemy2[], Enemy_AI* enemy_AI1, Enemy_AI* enemy_AI2, Powerup* powerup[], EnemyBullet *enemyBullet[]);

/*if ball hits another ball,  function does the apporiate actions*/
void Ball_BallCollision(Ball *ball, Sound *sound);

/* If player hit the ball during the player's update*/
void Player_BallCollision(Player *player, int change_x, Stage *stage, Ball *ball, TileMapManager *tileManager, Sound *sound);

/*If player has missed the ball*/
void Ball_isdeadPlayerMissed(Ball* ball, Stage* stage, Sound* sound);

/*If ball goes out of bounds, redraws ball back to player -- in case the balls glitches. 
 returns 1 if ball is within, return 0 if ball is out of bounds*/
int Ball_isOutofBounds(Ball *ball, Stage *stage, Sound* sound, TileMapManager *tilemanager);