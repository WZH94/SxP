/******************************************************************************
All content © 2017 Digipen Institute of Technology Singapore, all rights reserved

filename    Stage.h
author      Wong Zhihao
DP email    zhihao.wong@digipen.edu
course      RTIS

Brief Description:
This file declares the Stage object and its function declarations.

******************************************************************************/
#pragma once
#include <time.h>

#define PvP 1
#define CoOp 0

#define NORMAL 1
#define ICE 2
#define FIRE 3

/* Forward declarations of structs */
typedef struct Enemy Enemy;
typedef struct Enemy_AI Enemy_AI;
typedef struct Ball Ball;
typedef struct Sound Sound;
typedef struct TileMapManager TileMapManager;
typedef struct Player Player;
typedef struct Powerup Powerup;

/* Forward declaration for the Stage type */
typedef struct Stage
{
	clock_t delta_time;			/* Number of ms per frame */
	clock_t current_time;		/* The last time since checking for delta time */
	float diff_mult;			/* Current difficulty of the match */
	int play_type;				/* PvP or Co-op match */
	int stage_type;				/* type of stage e.g. normal, fire, ice, etc. */
	time_t start_time;			/* The start time of the match */
	time_t end_time;			/* Controls the duration of Terrain effects */
	time_t powerup_time;		/* Controls time it takes for powerup to drop */
	int difficulty_control;		/* Controls if difficulty changes */
	int respawn_control;		/* Controls if enemies should respawn */
	int terrain_control;		/* Controls if terrain effect should happen */
	int coop_drop_powerup;		/* Determines if powerups should begin dropping in Co-Op when there are less than 6 enemies in play */
	int coop_powerup_control;	/* Controls activation of powerup drops */
	int num_enemies;			/* Enemies to spawn per row */
	int num_rows;				/* Number of rows to spawn */
	int horizontal_gap;			/* Horizontal gap between each enemy */
	int vertical_gap;			/* Vertical gap between each enemy */
	int total_enemies;			/* Total number of enemies in this stage */
	int total_and_hydra;		/* Total number of enemies holdable in the stage */
	int size_x;					/* Width of whole game area */
	int size_y;					/* Height of whole game area*/
	int x_border;				/* Where to spawn the first enemy on x-axis */
	int y_border;				/* Where to spawn the first enemy on y-axis */
	int player1_start_posY;		/* Starting coordinate of player 1 on y-axis */
	int player2_start_posY;		/* Starting coordinate of player 2 on y-axis */
	int ball1_start_posY;		/* Starting coordinate of ball 1 on y-axis */
	int ball2_start_posY;		/* Starting coordinate of ball 2 on y-axis */

	/* The number of the specific type of enemy to spawn per player */
	int num_TYPE0;
	int num_TYPE1;
	int num_TYPE2;
	int num_TYPE3;
	int num_TYPE4;
	int num_TYPE5;
	int num_TYPE6;
	int num_TYPE7;
	int num_TYPE8;
	int num_TYPE9;
	int num_TYPE10;
	/* Keeps track of how many of the specific type of enemy has spawned for the player */
	int TYPE0_tracker;
	int TYPE1_tracker;
	int TYPE2_tracker;
	int TYPE3_tracker;
	int TYPE4_tracker;
	int TYPE5_tracker;
	int TYPE6_tracker;
	int TYPE7_tracker;
	int TYPE8_tracker;
	int TYPE9_tracker;
	int TYPE10_tracker;
} Stage;

/* Create a new Stage object with the provided data */
Stage* Stage_Create(int play_type, float difficulty, int enemies, int rows, int size_x, int size_y, int x_border, int y_border, int h_gap, int v_gap,
	int type1, int type2, int type3, int type4, int type5, int type6, int type7, int type8, int player1_start_pos, int player2_start_pos, int stage_type);

/* Free the memory associated with a given Stage object */
void Stage_Free(Stage** stagePtr);

/* Prints out the stage borders */
void Stage_Print(Stage* stage);

/* Prints the sidebar for notifying the player if controls has been swapped and reverted */
void Stage_Control_Swap_Print(int state);

/* Prints the sidebar for notifying the player the last powerup collected */
void Stage_Powerup_Print(Powerup *powerup, int player_num);

/* Prints the UI for the main menu */
void Stage_MainMenu_Print(Stage* stage);

/* Adjusts for low frame rate */
long Delta_Time(Stage *stage);

/* Checks if there are any enemies left alive in the stage in Co-Op mode */
int CoOp_Enemy_Check(Stage *stage, Enemy *enemy1[], Enemy *enemy2[]);

/* Keeps track of elapsed game time */
void Stage_Timer(Stage *stage, Enemy *enemy1[], Enemy *enemy2[], Enemy_AI *enemy_AI1, Enemy_AI *enemy_AI2, Ball *ball1, Ball *ball2, Player *player1, Player *player2,
	TileMapManager *tileManager, Sound *sound, Powerup *powerup[]);

/* Sets the difficutly of the stage based on in game elapsed time */
void Stage_Difficulty(Stage *stage, Enemy *enemy1[], Enemy *enemy2[], Enemy_AI *enemy_AI1, Enemy_AI *enemy_AI2, Ball *ball1, Ball *ball2);

/* Handles the respawning of a random enemy in PvP */
void PvP_Respawn(Stage *stage, Enemy *enemy[], Enemy_AI *enemy_AI, TileMapManager *tileManager, Sound *sound);

/* Resets burn time of enemies in Fire terrain */
void Stage_Difficulty_Fire_Reset(Stage *stage, Enemy *enemy1[], Enemy *enemy2[], Enemy_AI *enemy_AI1, Enemy_AI *enemy_AI2, Ball *ball1, Ball *ball2, Sound *sound);

/* Sets the difficutly of the stage based on in game elapsed time for fire stage effect */
void Stage_Difficulty_Fire(Stage *stage, Enemy *enemy1[], Enemy *enemy2[], Enemy_AI *enemy_AI1, Enemy_AI *enemy_AI2, Ball *ball1, Ball *ball2, Sound *sound);

/* Freezes the players by changing their controls to NULL */
void Stage_Ice_Freeze(Stage *stage, Player *player1, Player *player2, Sound *sound);

/* Thaws the players by changing their controls back to normal */
void Stage_Ice_Thaw(Stage *stage, Player *player1, Player *player2, Sound *sound);

/* Handles the killing of random enemy in CoOp when player collects the powerup */
void CoOp_Powerup_Kill(Stage *stage, Enemy *enemy1[], Enemy *enemy2[], Enemy_AI *enemy_AI1, Enemy_AI *enemy_AI2, TileMapManager *tileManager, Sound *sound);

/* Drops a powerup to both players that kills a random enemy */
void CoOp_KillEnemy_Dropper(Stage *stage, Powerup *powerup[]);