#pragma once
#include "stdafx.h"		/* bool */
#include "Player.h"		/* Player objects and data */
#include "Stage.h"		/* Stage objects and data */
#include "FMODInit.h"	/* Sound objects and data */


typedef struct Enemy Enemy;
typedef struct Enemy_AI Enemy_AI;
typedef struct Powerup Powerup;
typedef struct EnemyBullet EnemyBullet;


/*if the ball hits the borders, checks the bounce ball accordingly*/
void Ball_BorderCollision(Sound *sound, Ball* ball, Stage* stage, TileMapManager *tileManager);

/*checks if ball collides with anything using TileMap, returns 1 if no collision*/
int Ball_CollisionManager(Sound *sound, TileMapManager* tilemap, Ball* ball, int current_x_pos, int current_y_pos, int next_x_ballpos, int next_y_ballpos,
	Stage* stage, Enemy* enemy1, Enemy* enemy2, Enemy_AI* enemy_AI1, Enemy_AI* enemy_AI2, Powerup* powerup, EnemyBullet *enemyBullet);
