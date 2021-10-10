/******************************************************************************
All content © 2017 Digipen Institute of Technology Singapore, all rights reserved

filename			Player.h
author				Ngm Hui Min, Lois
DP email			lois.ngm@digipen.edu
course				RTIS

Brief Description:
Header file for player.c which contains the contents of player object of the game.
******************************************************************************/
#pragma once

/* the maximum height of a player */
#define PLAYER_HEIGHT 2

/* the maximum width of a player */
#define PLAYER_WIDTH 11

/* Player 1 ASCII Shortened Twice */
static const char *player1downdown[2] = {
	"1|1",
	"1-1"
};

/* Player 1 ASCII Shortened */
static const char *player1down[2] = {
	"1-|-1",
	"1---1"
};

/* Player 1 ASCII */
static const char *player1[2] = {
	"1--|--1",
	"1-----1"
};

/* Player 1 ASCII Extended */
static const char *player1up[2] = {
	"1---|---1",
	"1-------1"
};

/* Player 1 ASCII Extended Twice */
static const char *player1upup[2] = {
	"1----|----1",
	"1---------1"
};

/* Player 2 ASCII Shortened Twice */
static const char *player2downdown[2] = {
	"2-2",
	"2|2"
};

/* Player 2 ASCII Shortened */
static const char *player2down[2] = {
	"2---2",
	"2-|-2"
};

/* Player 2 ASCII */
static const char *player2[2] = {
	"2-----2",
	"2--|--2"
};

/* Player 2 ASCII Extended */
static const char *player2up[2] = {
	"2-------2",
	"2---|---2"
};

/* Player 2 ASCII Extended Twice */
static const char *player2upup[2] = {
	"2---------2",
	"2----|----2"
};

/* necessary structs */
typedef struct Stage Stage;
typedef struct TileMapManager TileMapManager;
typedef struct Powerup Powerup;
typedef struct EnemyBullet EnemyBullet;
typedef struct Enemy Enemy;
typedef struct Enemy_AI Enemy_AI;
typedef struct Ball Ball;
typedef struct Sound Sound;

/* Contents of a player */
typedef struct Player
{
	char playerStyle[PLAYER_WIDTH][PLAYER_WIDTH + 1];  /* Maximum dimensions of the player */
	int left;										   /* Left control of player */
	int right;										   /* Right control of player */
	int attack;										   /* Attack control of player */
	int boost;										   /* Boost control of player */
	float boost_amt;								   /* Amount of boost player has */
	int score;										   /* Score of player */
	int health;										   /* Health of player */
	int x_p;										   /* x coord of player */
	int y_p;										   /* y coord of player */
	int x_l;										   /* Length of player */
	int y_sb;										   /* y coord of player stats */
	int x_h;										   /* x coord of player health stat */
	int x_b;										   /* x coord of player boost stat */
	int x_s;										   /* x coord of player score stat */
	int id;											   /* id of  player */
	int invulnerable_time;						       /* Sets the player to be invulnerable after getting damaged */
	int swap_flag;									   /* Swap flag */
	int swap_time;									   /* Swap time */
	int frames;										   /* How many often out of this amount of frames it updates for */
	int timer;										   /* Tick tocks every frame */
	int base_timer;									   /* Adjusts how often it updates per frame out of the base timer */
} Player;


/* Create a new Player object with the provided data */
Player* Player_Create(const char *play[], int l, int r, int a, int b, int x_player, int y_player, int x_length,
	int y_statbar, int x_health, int x_boost, int x_score, int id, TileMapManager *tileManager);

/* Free the memory associated with a given Player object */
void Player_Free(Player **playerPtr);

/* Prints the player on the screen */
void Player_Print(Player *player, TileMapManager *tileManager);

/* Clears the area where the player was previously */
void Player_Clear(Player *player, const char *player_clear[], TileMapManager *tileManager);

/* Selects the size to clear based on the player length */
void Player_Clear_Select(Player *player, TileMapManager *tileManager);

/* Prints the health of the player */
void Player_Print_Health(Player *player);

/* Clears the health of player before reprinting */
void Player_Clear_Health(Player *player);

/* Prins the score of the player */
void Player_Print_Score(Player *player);

/* Updates the player details e.g. movement, health and score */
void Player_Update(Stage *stage, Player *player, Ball *ball1, Ball *ball2, Powerup *powerup[], EnemyBullet *enemyBullet[],
	TileMapManager *tileManager, Sound *sound, Enemy *enemy1[], Enemy *enemy2[], Enemy_AI *enemy_AI1, Enemy_AI *enemy_AI2);

/* Action done when player is to be moved */
void Player_Movement(Stage *stage, Player *player, Ball *ball1, Ball *ball2, Powerup *powerup[], EnemyBullet *enemyBullet[],
	TileMapManager *tileManager, Sound *sound, Enemy *enemy1[], Enemy *enemy2[], Enemy_AI *enemy_AI1, Enemy_AI *enemy_AI2);

/* Detects if player hit anything */
void Player_CollisionDetection(int result, Player *player, int change_x, Stage *stage, Ball *ball1, Ball *ball2, Powerup *powerup[], EnemyBullet *enemyBullet[],
	TileMapManager *tileManager, Sound *sound, Enemy *enemy1[], Enemy *enemy2[], Enemy_AI *enemy_AI1, Enemy_AI *enemy_AI2);

/* Ensures the player length does not exceed borders if its length increases */
void Player_LengthBorderCheck(Stage *stage, Player *player);

/* When player gets damaged */
void minusHP(Player *player, Sound *sound);