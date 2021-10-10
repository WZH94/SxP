#pragma once
#include "stdafx.h"		/* bool */
#include "Player.h"		/* Player objects and data */
#include "Stage.h"		/* Stage objects and data */
#include "FMODInit.h"	/* Sound objects and data */


typedef struct Enemy Enemy;
typedef struct Enemy_AI Enemy_AI;
typedef struct Powerup Powerup;
typedef struct EnemyBullet EnemyBullet;

/* Ball Direction defs */
#define DOWN 13
#define UP 12
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
#define STOP -1

/* Contents of a ball */
typedef struct BallMainMenu
{
	int ball_type;
	char ballchar;
	char firebutton;
	int x_ballpos;
	int y_ballpos;
	int start_y_ballpos;
	int ball_fired_flag;
	int balldirection;
	int frames;			 /* How many often out of this amount of frames it updates for */
	int timer;			 /* Tick tocks every frame */
	int base_timer;		 /* Adjusts how often it updates per frame out of the base timer */
	int ballhitballflag;
	int ballisdeadflag;

} BallMainMenu;

/* Create a new Item object with the provided data */
BallMainMenu* Ball_Create(int name, char ballcharacter, int x_start_pos, int y_start_pos, char fire);

/*Moves ball accordingly, and checks for collision everytime before it moves in a certain direction*/
void BallMainMenu_MovementManager(Sound *sound, BallMainMenu* ball, TileMapManager* tilemap, Stage* stage, Enemy* enemy1[], Enemy* enemy2[], Enemy_AI* enemy_AI1, Enemy_AI* enemy_AI2,
	Powerup* powerup, EnemyBullet *enemyBullet);
/*Updates the ball*/
void BallMainMenu_Update(Sound *sound, BallMainMenu* ball, TileMapManager *tileManager, Stage* stage, Enemy* enemy1[], Enemy* enemy2[], Enemy_AI* enemy_AI1, Enemy_AI* enemy_AI2,
	Powerup* powerup, EnemyBullet *enemyBullet);
/*Free the memory for ball*/
void BallMainMenu_Free(BallMainMenu** Ballptr);
