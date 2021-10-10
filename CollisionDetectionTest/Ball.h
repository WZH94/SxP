/******************************************************************************
All content © 2017 Digipen Institute of Technology Singapore, all rights reserved

filename			Ball.h
author				Enriquez, Audry May dela Paz
DP email			a.enriquez@digipen.edu
course				BSGD

Brief Description:
Header file for ball.c which contains the contents of ball object of the game.
******************************************************************************/

#pragma once
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

/*To check a group of ball directions*/
#define General_TOPRIGHT -2			/*Checks all topright directions if any angle*/
#define General_TOPLEFT -3			/*Checks all topleft directions if any angle*/
#define General_BOTTOMRIGHT -4		/*Checks all bottomright directions if any angle*/
#define General_BOTTOMLEFT -5		/*Checks all bottomleft directions if any angle*/
#define General_UPWARDS -6			/*Checks all directions going up*/
#define General_DOWNWARDS -7		/*Checks all directions going down*/
#define General_UPWARDLEFT -8		/*Checks all directiosn going up and topleft*/
#define General_DOWNWARDLEFT -9		/*Checks all directions going down and bottomleft*/
#define General_UPWARDRIGHT -10		/*Checks all directions going up and topright*/
#define General_DOWNWARDRIGHT -11	/*Checks all directions going down and bottomright*/
#define General_Angle45 -12			/*Checks all directions that have an angle of 45*/
#define General_Angle60 -13			/*Checks all directions that have an angle of 60*/
#define General_Angle20 -14			/*Checks all directions that have an angle of 20*/
#define General_AllDirections -15	/*Checks all directions*/

/*Checks whether to change the angle of the ball or not*/
#define ChangeAngle 1
#define DoNotChangeAngle 0

typedef struct Sound Sound;
typedef struct Player Player;
typedef struct Stage Stage;
typedef struct Enemy Enemy;
typedef struct Enemy_AI Enemy_AI;
typedef struct Powerup Powerup;
typedef struct EnemyBullet EnemyBullet;
typedef struct TileMapManager TileMapManager;

/* Contents of a ball */
typedef struct Ball
{
	char ballchar;				 /*Character of the ball*/
	char firebutton;			 /*Button for player to press*/
	time_t shoot_time;			 /* Max time the player can hold on to the ball */
	int auto_shoot_control;		/* Controls if ball will auto shoot once player has held on to it for too long */
	int ball_type;				/*Type of ball. Type 1 for player 1, Type 2 for player 2, Type 3 for main menu ball*/
	int x_ballpos;				 /*Current X coordinate of ball*/
	int y_ballpos;				 /*Current Y coordinate of ball*/
	int start_y_ballpos;		 /*Stores the Initial Y-coordinate of ball to reprint it when it dies*/
	int ball_fired_flag;		 /*Flag to check if the ball has been fired. 0 is not fired, 1 is fired*/
	int balldirection;			 /*Checks the direction of the ball*/
	int frames;					 /* How many often out of this amount of frames it updates for */
	int timer;					 /* Tick tocks every frame */
	int base_timer;				 /* Adjusts how often it updates per frame out of the base timer */
	int ballhitballflag;		 /*Flag to check if ball has been hit by another ball before. 0 if not hit. 1 is hit*/
	int ballisdeadflag;			 /*Checks to see if the ball was missed by the player. 1 if dead, 0 if not dead*/
	int timetorevival;			 /*If the ball has been missed, wait for a few moments before allowing player to use it again*/
	Player *friendplayer;		 /*Pointer to the ball's friendly player*/
	Player *enemyplayer;		/*Pointer to the ball's enemy player - to be used in PVP*/
	struct Ball *enemyball;		/*Pointer to other ball in the game, to be used for ball vs ball collision detection*/

} Ball;

/* Create a new Item object with the provided data */
Ball* Ball_Create(Stage *stage, int name, char ballcharacter, int x_start_pos, int y_start_pos, char fire, Player *friendlyplayer, Player* enemyplayer);

/*If ball has not been shot, move the ball with the player*/
void Ball_MoveBallWithPlayer(Ball* ball, TileMapManager *tileManager);

/*Moves ball accordingly, and checks for collision everytime before it moves in a certain direction*/
void Ball_MovementManager(Sound *sound, Ball* ball, TileMapManager* tilemap, Stage* stage, Enemy* enemy1[], Enemy* enemy2[],
	Enemy_AI* enemy_AI1, Enemy_AI* enemy_AI2, Powerup* powerup[], EnemyBullet *enemyBullet[]);
/*Updates the ball*/
void Ball_Update(Sound *sound, Ball* ball, TileMapManager *tileManager, Stage* stage, Enemy* enemy1[], Enemy* enemy2[], Enemy_AI* enemy_AI1, Enemy_AI* enemy_AI2,
	Powerup* powerup[], EnemyBullet *enemyBullet[]);

/*Free the memory for ball*/
void Ball_Free(Ball** Ballptr);

/*Updates ball's enemy ball*/
void Ball_UpdateEnemyBall(Ball* ball1, Ball* ball2);

/*Function to check the ball's direction. Returns the direction in the correct angle it should change to the direction of the ball is same as direction player is checking*/
int Ball_DirectionChanger(Ball* ball, int CheckDirection, int DirectionChange, int ChangeAngleLater, Sound* sound);

/*Function to change the player's health and score*/
void Ball_AffectPlayerHealthandScore(Player* healthplayer1, int health1, Player* healthplayer2, int health2, Player* scoreplayer1, int score1, Player* scoreplayer2, int score2);