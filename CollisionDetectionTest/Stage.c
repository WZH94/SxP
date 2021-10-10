/******************************************************************************
All content © 2017 Digipen Institute of Technology Singapore, all rights reserved

filename			Stage.c
author				Wong Zhihao
DP email			zhihao.wong@digipen.edu
course				RTIS

Brief Description:
Contents of stage object of the game and function to print out stage borders.

******************************************************************************/

#include <stdio.h>				/* printf */
#include <Windows.h>			/* Colours */
#include <time.h>				/* Clock and time */
#include "Stage.h"				/* Function declarations */
#include "WorldData.h"			/* Misc Functions */
#include "Enemy.h"				/* Enemy struct and functions */
#include "EnemyAI.h"			/* EnemyAI struct and functions */
#include "Ball.h"				/* Ball struct and functions */
#include "FMODInit.h"			/* Sound struct and functions */
#include "TileMapManager.h"		/* Tile Manager struct and functions */
#include "EnemyTypes.h"			/* Enemy types and functions */
#include "Player.h"				/* Player struct and functions */
#include "Powerups.h"			/* Powerup struct and functions */

#define SIDEBAR_X 75			/* X COORD of the Side Bar texts */
#define SIDEBAR_P1Y_SWAP 4		/* Y COORD of Side Bar text notifying P1 of controls swap */
#define SIDEBAR_P2Y_SWAP 73		/* Y COORD of Side Bar text notifying P2 of controls swap */
#define SIDEBAR_P1Y_POWERUP 9	/* Y COORD of Side Bar text notifying P1 of last powerup collected */
#define SIDEBAR_P2Y_POWERUP 68	/* Y COORD of Side Bar text notifying P2 of last powerup collected */

/* Art of the powerups in game */
const char RANDOM1 = '?';
const char HP_BUFF1 = '+';
const char HP_DEBUFF1 = 196;
const char LENGTH_BUFF1 = 'L';
const char LENGTH_DEBUFF1 = 'S';
const char BALL_SPEED_BUFF1 = 232;
const char BALL_SPEED_DEBUFF1 = 233;
const char CONTROLS_DEBUFF1 = 145;
const char PLAYER_SPEED_BUFF1 = 247;
const char PLAYER_SPEED_DEBUFF1 = 174;
const char BALL_SWAP1 = 228;
const char KILL_ENEMY1 = 235;

/* Create a new Stage object with the provided data */
Stage* Stage_Create(int play_type, float difficulty, int enemies, int rows, int size_x, int size_y, int x_border, int y_border, int h_gap, int v_gap,
	int type1, int type2, int type3, int type4, int type5, int type6, int type7, int type8, int player1_start_posY, int player2_start_posY, int stage_type)
{
	Stage* stage = NULL;

	/* allocate the memory for a new object */
	stage = (Stage*)malloc(sizeof(Stage));
	if (stage == NULL)
	{
		return NULL; /* malloc can fail */
	}
	stage->play_type = play_type;									/* PvP or Co-Op */
	stage->diff_mult = difficulty;								    /* Difficulty of the stage UNUSED */
	stage->difficulty_control = 0;									/* If difficulty should be incremented in current time */
	stage->respawn_control = 0;										/* If enemy should be respawned should be incremented in current time in PvP */
	stage->coop_drop_powerup = 0;									/* If powerup killing enemy should be dropped in current time in Co-Op */
	stage->coop_powerup_control = 0;								/* If powerup killing enemy should be dropped in current time in Co-Op */
	stage->start_time = time(NULL);									/* The start time of the start of game */
	stage->end_time = 0;											/* How long the terrain effects last */
	stage->powerup_time = 0;										/* Interval between powerups dropping that kills enemy in Co-Op */
	stage->num_enemies = enemies;									/* Number of enemies per row */
	stage->num_rows = rows;											/* Number of rows of enemies per player */
	stage->horizontal_gap = h_gap;									/* Horizontal gap between each enemy */
	stage->vertical_gap = v_gap;									/* Vertical gap between each enemy */
	stage->size_x = size_x;											/* The width of the play area */
	stage->size_y = size_y;											/* The height of the playa area */
	stage->x_border = x_border;										/* Where the first enemy spawns on the x-axis */
	stage->y_border = y_border;										/* Where the first enemy spawns on the y-axis */
	stage->total_enemies = enemies * rows;							/* Total number of enemies without hydra */
	stage->player1_start_posY = player1_start_posY /*4*/;			/* Start y position of player 1 */
	stage->player2_start_posY = player2_start_posY /*size_y - 6*/;	/* Start y position of player 2 */
	stage->ball1_start_posY = stage->player1_start_posY + 2;		/* Start y position of ball 1 */
	stage->ball2_start_posY = stage->player2_start_posY - 1;		/* Start y position of ball 2 */
	stage->stage_type = stage_type;									/* The terrarin type of the stage */
	stage->terrain_control = 0;										/* If terrain effect should be activated in current time */

	/* Number of types of each enemy that spawns in the stage */
	stage->num_TYPE0 = stage->total_enemies - type1 - type2 - type3 - type4 - type5 - type6 - type7 - type8;
	stage->num_TYPE1 = type1;
	stage->num_TYPE2 = type2;
	stage->num_TYPE3 = type3;
	stage->num_TYPE4 = type4;
	stage->num_TYPE5 = type5;
	stage->num_TYPE6 = type6;
	stage->num_TYPE7 = type7;
	stage->num_TYPE8 = type8;

	/* Tracks the number of types of each enemy that has spawned for each player in the stage */
	stage->TYPE0_tracker = 0;
	stage->TYPE1_tracker = 0;
	stage->TYPE2_tracker = 0;
	stage->TYPE3_tracker = 0;
	stage->TYPE4_tracker = 0;
	stage->TYPE5_tracker = 0;
	stage->TYPE6_tracker = 0;
	stage->TYPE7_tracker = 0;
	stage->TYPE8_tracker = 0;
	
	/* 10 slots for the additional hydra units */
	stage->total_and_hydra = stage->total_enemies + 10;

	/* return the new object */
	return stage;
}

/* Free the memory associated with a given Stage object */
void Stage_Free(Stage** stagePtr)
{
	/* safety check on the parameters */
	if ((stagePtr == NULL) || (*stagePtr == NULL))
	{
		return;  /* take no action if the parameters are invalid */
	}

	/* free the given item */
	free(*stagePtr);

	/* clear the provided pointer */
	*stagePtr = NULL;
}

/* Prints out the stage borders */
void Stage_Print(Stage* stage)
{
	/* Art of the borders */
	const char side_borders = 186;
	const char player_borders = 205;
	const char left_connect = 204;
	const char right_connect = 185;

	/* Draws the right border */
	int x, y;
	for (x = stage->size_x - 1, y = 0; y < stage->size_y; y++)
	{
		WorldData_GoToXY(x, y);
		printf("%c", side_borders);
	}
	/* Draws the left border*/
	for (x = 0, y = 0; y < stage->size_y; y++)
	{
		WorldData_GoToXY(x, y);
		printf("%c", side_borders);
	}

	/* Draws the top border */
	for (x = 1, y = 3; x < stage->size_x - 1; x++)
	{
		WorldData_GoToXY(x, y);
		printf("%c", player_borders);
	}

	/* Draws the bottom border */
	for (x = 1, y = (stage->size_y - 4); x < stage->size_x - 1; x++)
	{
		WorldData_GoToXY(x, y);
		printf("%c", player_borders);
	}

	/* Draws the connecting borders */
	WorldData_GoToXY(0, 3);
	printf("%c", left_connect);

	WorldData_GoToXY(0, stage->size_y - 4);
	printf("%c", left_connect);

	WorldData_GoToXY(stage->size_x - 1, 3);
	printf("%c", right_connect);

	WorldData_GoToXY(stage->size_x - 1, stage->size_y - 4);
	printf("%c", right_connect);

	/* Colours */
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	/* Save current attributes */
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;

	SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);

	/* Prints the state of the controls for player 1 and player 2 in the sidebar */
	WorldData_GoToXY(SIDEBAR_X, SIDEBAR_P1Y_SWAP);
	printf("%c CONTROLS NORMAL ", CONTROLS_DEBUFF1);

	WorldData_GoToXY(SIDEBAR_X, SIDEBAR_P2Y_SWAP);
	printf("%c CONTROLS NORMAL ", CONTROLS_DEBUFF1);

	SetConsoleTextAttribute(hConsole, saved_attributes);
	
	/* Prints this text for player 1 and 2 in the side bar */
	WorldData_GoToXY(SIDEBAR_X, SIDEBAR_P1Y_POWERUP - 2);
	printf("LAST POWERUP COLLECTED:");

	WorldData_GoToXY(SIDEBAR_X, SIDEBAR_P2Y_POWERUP + 2);
	printf("LAST POWERUP COLLECTED:");
}

/* Prints the sidebar for notifying the player if controls has been swapped and reverted */
void Stage_Control_Swap_Print(int state)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	/* Save current attributes */
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;

	/* Prints that player controls are swapped in the side bar */
	if (state == 1)
	{
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);

		WorldData_GoToXY(SIDEBAR_X, SIDEBAR_P1Y_SWAP);
		printf("%c CONTROLS SWAPPED", CONTROLS_DEBUFF1);
	}

	/* Prints that player controls are normal in the side bar */
	else if (state == 0)
	{
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);

		WorldData_GoToXY(SIDEBAR_X, SIDEBAR_P1Y_SWAP);
		printf("%c CONTROLS NORMAL ", CONTROLS_DEBUFF1);
	}

	SetConsoleTextAttribute(hConsole, saved_attributes);
}

/* Prints the sidebar for notifying the player the last powerup collected */
void Stage_Powerup_Print(Powerup *powerup, int player_num)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	/* Save current attributes */
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;

	/* Which player to print for */
	if (player_num == 1)
		WorldData_GoToXY(SIDEBAR_X, SIDEBAR_P1Y_POWERUP);
	else if (player_num == 2)
		WorldData_GoToXY(SIDEBAR_X, SIDEBAR_P2Y_POWERUP);

	/* Sets the colour of the text to print based on colour of powerup */
	switch (powerup->type)
	{
	case 1: 
	case 3: 
	case 5: 
	case 8: SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	case 2:
	case 4:
	case 6:
	case 7:
	case 9:SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
		break;
	case 0:
	default: SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		break;
	}

	/* The text and art of the powerup to print */
	switch (powerup->type)
	{
		/* Kill enemy powerup*/
	case -1: printf("%c ENEMY KILLED            ", KILL_ENEMY1);
		break;
		/* HP Buff */
	case 1: printf("%c HP UP                   ", HP_BUFF1);
		break;
		/* HP Debuff */
	case 2: printf("%c HP DOWN                 ", HP_DEBUFF1);
		break;
		/* Length Buff */
	case 3: printf("%c PLAYER LENGTH UP        ", LENGTH_BUFF1);
		break;
		/* Length Debuff */
	case 4: printf("%c PLAYER LENGTH DOWN      ", LENGTH_DEBUFF1);
		break;
		/* Ball speed Buff */
	case 5: printf("%c BALL SPEED SLOWED       ", BALL_SPEED_BUFF1);
		break;
		/* Ball speed Debuff */
	case 6: printf("%c BALL SPEED INCREASED    ", BALL_SPEED_DEBUFF1);
		break;
		/* Player Controls Swap Debuff */
	case 7: printf("%c CONTROLS SWAPPED        ", CONTROLS_DEBUFF1);
		break;
		/* Player Speed Reset Buff */
	case 8: printf("%c MOVEMENT SPEED INCREASED", PLAYER_SPEED_BUFF1);
		break;
		/* Player Speed Slow Debuff */
	case 9: printf("%c MOVEMENT SPEED INCREASED", PLAYER_SPEED_DEBUFF1);
		break;
		/* Ball Swap Powerup */
	case 10: printf("%c BALL POSITIONS SWAPPED  ", BALL_SWAP1);
		break;
	}

	SetConsoleTextAttribute(hConsole, saved_attributes);
}

/* Prints the UI for the main menu */
void Stage_MainMenu_Print(Stage* stage)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	int x, y;
	/* Save current attributes */
	const char side_borders = 186;
	const char player_borders = 205;

	/* Draws the right border */
	for (x = stage->size_x - 1, y = 1; y < stage->size_y - 1; y++)
	{
		WorldData_GoToXY(x, y);
		printf("%c", side_borders);
	}
	/* Draws the left border*/
	for (x = 0, y = 1; y < stage->size_y - 1; y++)
	{
		WorldData_GoToXY(x, y);
		printf("%c", side_borders);
	}

	/* Draws the top border */
	for (x = 1, y = 0; x < stage->size_x - 1; x++)
	{
		WorldData_GoToXY(x, y);
		printf("%c", player_borders);
	}
	/* Draws the bottom border */
	for (x = 1, y = stage->size_y - 1; x < stage->size_x - 1; x++)
	{
		WorldData_GoToXY(x, y);
		printf("%c", player_borders);
	}

	SetConsoleTextAttribute(hConsole, saved_attributes);
}

/* Adjusts for low frame rate */
long Delta_Time(Stage *stage)
{
	/* Lower the frame rate, the faster the movements advance */
	if (stage->delta_time <= 0)
		return 1;
	else return stage->delta_time + 1;
}

/* Checks if there are any enemies left alive in the stage in Co-Op mode */
int CoOp_Enemy_Check(Stage *stage, Enemy *enemy1[], Enemy *enemy2[])
{
	int i;			/* Loop counter */
	int num_alive = 0;	/* Number of enemies left alive in game */

	/* Check player 1 enemies */
	for (i = 0; i < stage->total_and_hydra; ++i)
	{
		if (enemy1[i] != NULL && enemy1[i]->alive == 0 && enemy1[i]->HP <= 0);
		else if (enemy1[i] != NULL && enemy1[i]->alive == 1 && enemy1[i]->HP > 0)
			++num_alive;
	}

	/* Check player 2 enemies */
	for (i = 0; i < stage->total_and_hydra; ++i)
	{
		if (enemy2[i] != NULL && enemy2[i]->alive == 0 && enemy2[i]->HP <= 0);
		else if (enemy2[i] != NULL && enemy2[i]->alive == 1 && enemy2[i]->HP > 0)
			++num_alive;
	}

	/* If there are still enemies alive */
	if (num_alive > 0)
	{
		/* If less than 5 enemies alive in the game, activate for the Co-Op enemy killer powerup to start dropping */
		if (num_alive < 5)
			stage->coop_drop_powerup = 1;
		/* Otherwise do not activate that function */
		else
			stage->coop_drop_powerup = 0;
		/* Return 0, game not over yet */
		return 0;
	}
	/* Game won! */
	else
		return 1;
}

/* Keeps track of elapsed game time and handles stage functions that happen on a timer */
void Stage_Timer(Stage *stage, Enemy *enemy1[], Enemy *enemy2[], Enemy_AI *enemy_AI1, Enemy_AI *enemy_AI2, Ball *ball1, Ball *ball2, Player *player_1, Player *player_2,
				 TileMapManager *tileManager, Sound *sound, Powerup *powerup[])
{
	/* Find the current elapsed time in seconds */
	time_t elapsed = time(NULL) - stage->start_time;

	/* Controls how often the difficulty is increased i.e. ball movements, enemy speed etc. Faster in PvP */
	if (elapsed % (6 - stage->play_type) == 0 && stage->difficulty_control == 0)
		/* Enables the calling of Stage_Difficulty function */
		stage->difficulty_control = 1;

	/* Sets the difficutly of the stage based on the difficulty */
	if ((elapsed % (6 - stage->play_type) != 0) && stage->difficulty_control == 1)
		Stage_Difficulty(stage, enemy1, enemy2, enemy_AI1, enemy_AI2, ball1, ball2);

	/* Controls respawning of enemies in PvP mode */
	if (elapsed % 6 == 0 && stage->respawn_control == 0 && stage->play_type == PvP)
		/* Enables the calling of PvP_Respawn function */
		stage->respawn_control = 1;

	/* Respawns enemies in PvP mode */
	if ((elapsed % 6 != 0) && stage->respawn_control == 1 && stage->play_type == PvP)
	{
		PvP_Respawn(stage, enemy1, enemy_AI1, tileManager, sound);
		PvP_Respawn(stage, enemy2, enemy_AI2, tileManager, sound);
	}

	/* Checks if there are less than 5 enemies alive and then drops a powerup that kills a random enemy to both players every 15 seconds, only in Co-Op */
	if (stage->play_type == CoOp && stage->coop_drop_powerup == 1 && stage->coop_powerup_control == 0)
	{
		stage->powerup_time = time(NULL) + 15;
		/* Enables the calling of CoOp_KillEnemy_Dropper function */
		stage->coop_powerup_control = 1;
	}

	/* Drops powerups to both players in Co-Op */
	if (stage->powerup_time == time(NULL) && stage->coop_powerup_control == 1)
		CoOp_KillEnemy_Dropper(stage, powerup);

	/* Handles fire terrain's burn effect */
	if (stage->stage_type == FIRE)
	{
		/* Controls how often the enemies "burn" */
		if (elapsed % (20 - stage->play_type * 2) == 0 && stage->terrain_control == 0)
			/* Enables the calling of Stage_Difficulty_Fire function */
			stage->terrain_control = 1;

		/* Burns the enemies */
		if ((elapsed % (20 - stage->play_type * 2) != 0) && stage->terrain_control == 1)
			Stage_Difficulty_Fire(stage, enemy1, enemy2, enemy_AI1, enemy_AI2, ball1, ball2, sound);

		/* Unburns the enemies and resets them back to normal speed after time is up */
		if (stage->end_time == time(NULL) && stage->terrain_control == -1)
			Stage_Difficulty_Fire_Reset(stage, enemy1, enemy2, enemy_AI1, enemy_AI2, ball1, ball2, sound);
	}

	/* Handles ice terrain's burn effect */
	if (stage->stage_type == ICE)
	{
		/* Controls how often players "Freeze" */
		if (elapsed % (19 - stage->play_type * 2) == 0 && stage->terrain_control == 0)
			/* Enables the calling of Stage_Ice_Freeze function */
			stage->terrain_control = 1;

		/* Freezes the players */
		if ((elapsed % (19 - stage->play_type * 2) != 0) && stage->terrain_control == 1)
			Stage_Ice_Freeze(stage, player_1, player_2, sound);

		/* Unfreezes the player after time is up */
		if (stage->end_time == time(NULL) && stage->terrain_control == -1)
			Stage_Ice_Thaw(stage, player_1, player_2, sound);
	}
}

/* Sets the difficutly of the stage based on in game elapsed time */
void Stage_Difficulty(Stage *stage, Enemy *enemy1[], Enemy *enemy2[], Enemy_AI *enemy_AI1, Enemy_AI *enemy_AI2, Ball *ball1, Ball *ball2)
{

	int i;	/* Loop counter */

	/* Increases current stage difficulty UNUSED */
	stage->diff_mult += 1;

	/* Resets if the difficulty should be increased so it doesnt increase multiple times within the second */
	stage->difficulty_control = 0;

	/* Speeds the ball up every difficulty increment */
	if (ball1->base_timer < 110)
		ball1->base_timer += 2;
	if (ball2->base_timer < 110)
		ball2->base_timer += 2;

	/* Speeds the movement of enemies */
	enemy_AI1->base_timer += 70;
	enemy_AI2->base_timer += 70;

	/* Increases fire rate of enemies */
	for (i = 0; i < stage->total_and_hydra; ++i)
	{
		/* Checks if enemy exists */
		if (enemy1[i] != NULL)
		{
			/* Different increments based on the enemy type */
			switch (enemy1[i]->type)
			{
			case 2: enemy1[i]->fireChance -= 450;
				break;
			case 3: enemy1[i]->fireChance -= 510;
				break;
			case 4: enemy1[i]->fireChance -= 150;
				break;
			case 5: enemy1[i]->fireChance -= 540;
				break;
			case 6: enemy1[i]->fireChance -= 700;
				break;
			}
		}

		/* Checks if enemy exists */
		if (enemy2[i] != NULL)
		{
			/* Different increments based on the enemy type */
			switch (enemy2[i]->type)
			{
			case 2: enemy2[i]->fireChance -= 450;
				break;
			case 3: enemy2[i]->fireChance -= 510;
				break;
			case 4: enemy2[i]->fireChance -= 150;
				break;
			case 5: enemy2[i]->fireChance -= 540;
				break;
			case 6: enemy2[i]->fireChance -= 700;
				break;
			}
		}
	}
}

/* Resets burn time of enemies in Fire terrain */
void Stage_Difficulty_Fire_Reset(Stage *stage, Enemy *enemy1[], Enemy *enemy2[], Enemy_AI *enemy_AI1, Enemy_AI *enemy_AI2, Ball *ball1, Ball *ball2, Sound *sound)
{
	int i;	/* Loop counter */

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	/* Save current attributes */
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;

	/* Prints out stage borders based on provided data */
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
	Stage_Print(stage);
	SetConsoleTextAttribute(hConsole, saved_attributes);

	/* Resets if the terrain effect should be resetted so it doesnt reset multiple times within the second */
	stage->terrain_control = 0;

	/* Reset ball speed to normal */
	ball1->base_timer -= 30;
	ball2->base_timer -= 30;

	/* Different increments in PvP, so different reset amounts based on play type*/
	if (stage->play_type == PvP)
	{
		/* Resets the movement of enemies */
		enemy_AI1->base_timer -= 1300;
		enemy_AI2->base_timer -= 1300;
	}
	else
	{
		/* Resets the movement of enemies */
		enemy_AI1->base_timer -= 3000;
		enemy_AI2->base_timer -= 3000;
	}

	/* Resets fire rate of enemies */
	for (i = 0; i < stage->total_and_hydra; ++i)
	{
		/* Checks if enemy exists and it is a type that can shoot */
		if (enemy1[i] != NULL && enemy1[i]->type >= 2 && enemy1[i]->type <= 6)
		{
			/* Resets reload time */
			enemy1[i]->fireReload += 1500;
			/* Resets fire rate of enemy based on its type */
			switch (enemy1[i]->type)
			{
			case 2: enemy1[i]->fireChance += 12000;
				break;
			case 3: enemy1[i]->fireChance += 20200;
				break;
			case 4: enemy1[i]->fireChance += 10000;
				break;
			case 5: enemy1[i]->fireChance += 18000;
				break;
			case 6: enemy1[i]->fireChance += 40000;
				break;
			}
		}

		/* Checks if enemy exists and it is a type that can shoot */
		if (enemy2[i] != NULL && enemy2[i]->type >= 2 && enemy2[i]->type <= 6)
		{
			/* Resets reload time */
			enemy2[i]->fireReload += 1500;
			/* Resets fire rate of enemy based on its type */
			switch (enemy2[i]->type)
			{
			case 2: enemy2[i]->fireChance += 12000;
				break;
			case 3: enemy2[i]->fireChance += 20200;
				break;
			case 4: enemy2[i]->fireChance += 10000;
				break;
			case 5: enemy2[i]->fireChance += 18000;
				break;
			case 6: enemy2[i]->fireChance += 40000;
				break;
			}
		}
	}

	loadSound(sound, "..\\CollisionDetectionTest\\SFX\\FireUnburn.wav");
	playSound_High(sound);
}

/* Sets burn time of enemies in Fire terrain */
void Stage_Difficulty_Fire(Stage *stage, Enemy *enemy1[], Enemy *enemy2[], Enemy_AI *enemy_AI1, Enemy_AI *enemy_AI2, Ball *ball1, Ball *ball2, Sound *sound)
{
	int i;	/* Loop counter */

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	/* Save current attributes */
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;

	/* Prints out stage borders based on provided data */
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
	Stage_Print(stage);
	SetConsoleTextAttribute(hConsole, saved_attributes);

	/* Sets the time when the burn effect wears off */
	stage->end_time = time(NULL) + 5;

	/* -1 means that the terrain effect has been activated and MUST BE RESETTED */
	stage->terrain_control = -1;

	/* Speeds the ball up during burn time */
	if (ball1->base_timer < 110)
		ball1->base_timer += 30;
	if (ball2->base_timer < 110)
		ball2->base_timer += 30;

	/* Different increment of enemy move speed based on game type */
	if (stage->play_type == PvP)
	{
		/* Speeds the movement of enemies, slower for PvP since base movement is already faster */
		enemy_AI1->base_timer += 1300;
		enemy_AI2->base_timer += 1300;
	}
	else
	{
		/* Speeds the movement of enemies */
		enemy_AI1->base_timer += 3000;
		enemy_AI2->base_timer += 3000;
	}


	/* Increases fire rate of enemies */
	for (i = 0; i < stage->total_and_hydra; ++i)
	{
		/* Checks if enemy exists and it is a type that can shoot */
		if (enemy1[i] != NULL && enemy1[i]->type >= 2 && enemy1[i]->type <= 6)
		{
			/* Enemy reload time becomes shorter */
			enemy1[i]->fireReload -= 1500;

			/* Increases fire rate of enemy based on its type */
			switch (enemy1[i]->type)
			{
			case 2: enemy1[i]->fireChance -= 12000;
				break;
			case 3: enemy1[i]->fireChance -= 20200;
				break;
			case 4: enemy1[i]->fireChance -= 10000;
				break;
			case 5: enemy1[i]->fireChance -= 18000;
				break;
			case 6: enemy1[i]->fireChance -= 40000;
				break;
			}
		}

		/* Checks if enemy exists and it is a type that can shoot */
		if (enemy2[i] != NULL && enemy2[i]->type >= 2 && enemy2[i]->type <= 6)
		{
			/* Enemy reload time becomes shorter */
			enemy2[i]->fireReload -= 1500;

			/* Increases fire rate of enemy based on its type */
			switch (enemy2[i]->type)
			{
			case 2: enemy2[i]->fireChance -= 12000;
				break;
			case 3: enemy2[i]->fireChance -= 20200;
				break;
			case 4: enemy2[i]->fireChance -= 10000;
				break;
			case 5: enemy2[i]->fireChance -= 18000;
				break;
			case 6: enemy2[i]->fireChance -= 40000;
				break;
			}
		}
	}

	loadSound(sound, "..\\CollisionDetectionTest\\SFX\\FireBurn.wav");
	playSound_High(sound);
}

/* Freezes the players by changing their controls to NULL */
void Stage_Ice_Freeze(Stage *stage, Player *player_1, Player *player_2, Sound *sound)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	/* Save current attributes */
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;

	/* Prints out stage borders based on provided data */
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	Stage_Print(stage);
	SetConsoleTextAttribute(hConsole, saved_attributes);

	/* Sets the time where the players get thawed */
	stage->end_time = time(NULL) + 1;

	/* -1 means that the terrain effect has been activated and MUST BE RESETTED */
	stage->terrain_control = -1;

	/* Sets the players left and right controls to NULL */
	player_1->left -= 65;
	player_1->right -= 68;

	player_2->left -= 0x25;
	player_2->right -= 0x27;

	loadSound(sound, "..\\CollisionDetectionTest\\SFX\\IceFreeze.wav");
	playSound_Medium(sound);
}

/* Thaws the players by changing their controls back to normal */
void Stage_Ice_Thaw(Stage *stage, Player *player_1, Player *player_2, Sound *sound)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	/* Save current attributes */
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;

	/* Prints out stage borders based on provided data */
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN);
	Stage_Print(stage);
	SetConsoleTextAttribute(hConsole, saved_attributes);
	
	/* Resets if the terrain effect should be resetted so it doesnt reset multiple times within the second */
	stage->terrain_control = 0;

	/* Adds back the players control from NULL */
	player_1->left += 65;
	player_1->right += 68;

	player_2->left += 0x25;
	player_2->right += 0x27;

	loadSound(sound, "..\\CollisionDetectionTest\\SFX\\IceThaw.wav");
	playSound_Medium(sound);
}

/* Handles the respawning of a random enemy in PvP */
void PvP_Respawn(Stage *stage, Enemy *enemy[], Enemy_AI *enemy_AI, TileMapManager *tileManager, Sound *sound)
{
	int i;										/* Loop counter */
	int enemy_dead_container[30] = { 0 };		/* To store the locations of every dead enemy */
	int *container = enemy_dead_container;		/* The pointer to dead enemy locations */
	int num_dead = 0;							/* Number of dead enemies */
	int chosen_enemy;							/* The location of the dead enemy */
	int choose_enemy;							/* Chooses an enemy to respawn */

	/* Resets if enemies should be respawned so it doesnt respawn multiple times within the second */
	stage->respawn_control = 0;

	/* Finds the number of dead enemies and stores all their locations */
	for (i = 0; i < stage->total_enemies; ++i)
	{
		/* Checks if enemy exists, is alive and is not already past the player and is not past the border at the left and right */
		if (enemy[i]->alive == 0 && enemy[i]->HP <= 0 && enemy[i]->y_Pos > 6 && enemy[i]->y_Pos < stage->size_y - 7 && 
			enemy[i]->x_Pos >= 2 && enemy[i]->x_Pos + 3 < stage->size_x - 2)
		{
			/* Stores the location of the dead enemy inside the array */
			*container = i;
			/* Moves to next memory of array */
			++container;
			/* Increases dead count */
			++num_dead;
		}
	}

	/* Checks if there are any dead enemies in play */
	if (num_dead > 0)
	{
		choose_enemy = rand() % (num_dead + 1);
		/* Randomises a dead enemy from all the dead enemies */
		chosen_enemy = enemy_dead_container[choose_enemy];

		/* Revive enemy and sets HP back to max HP */
		enemy[chosen_enemy]->alive = 1;
		enemy[chosen_enemy]->HP = enemy[chosen_enemy]->max_HP;
		enemy[chosen_enemy]->full_HP = 1;

		/* Clears and reprints the enemy */
		Enemy_IndvClear(enemy[chosen_enemy], enemy_AI, tileManager);
		EnemyTypeHP_ArtSwitch(enemy[chosen_enemy]);
		Enemy_IndvPrint(enemy[chosen_enemy], enemy_AI, tileManager);

		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\EnemyRespawn.wav");
		playSound_Low(sound);
	}
}

/* Handles the killing of random enemies in CoOp when player collects the powerup */
void CoOp_Powerup_Kill(Stage *stage, Enemy *enemy1[], Enemy *enemy2[], Enemy_AI *enemy_AI1, Enemy_AI *enemy_AI2, TileMapManager *tileManager, Sound *sound)
{
	int i;										/* Loop counter */
	int enemy_alive_container[80] = { 0 };		/* To store the locations of every enemy still alive */
	int *container = enemy_alive_container;		/* The pointer to alive enemy locations */
	int num_alive = 0;							/* Number of alive enemies */
	int chosen_enemy;							/* The location of the dead enemy */
	int choose_enemy;							/* Chosen enemy to respawn */

	/* Finds the number of alive P1 enemies and stores all their locations */
	for (i = 0; i < stage->total_and_hydra; ++i)
	{
		/* Checks if enemy exists, is alive and is not already past the player and is not past the border at the left and right */
		if (enemy1[i] != NULL && enemy1[i]->alive == 1 && enemy1[i]->HP > 0)
		{
			/* Stores the location of the dead enemy inside the array */
			*container = i;
			/* Moves to next memory of array */
			++container;
			/* Increases dead count */
			++num_alive;
		}
	}

	/* Finds the number of alive P2 enemies and stores all their locations */
	for (i = 0; i < stage->total_and_hydra; ++i)
	{
		/* Checks if enemy exists, is alive and is not already past the player and is not past the border at the left and right */
		if (enemy2[i] != NULL && enemy2[i]->alive == 1 && enemy2[i]->HP > 0)
		{
			/* Stores the location of the dead enemy inside the array */
			*container = i + 40;
			/* Moves to next memory of array */
			++container;
			/* Increases dead count */
			++num_alive;
		}
	}

	/* Randomises a dead enemy from all the dead enemies */
	choose_enemy = rand() % num_alive;
	/* Finds which enemy it is in the array */
	chosen_enemy = enemy_alive_container[choose_enemy];
	
	/* If less than 40, enemy belongs to P1 */
	if (chosen_enemy < 40)
	{
		/* Revive enemy and sets HP back to max HP */
		enemy1[chosen_enemy]->alive = 0;
		enemy1[chosen_enemy]->HP = 0;
		enemy1[chosen_enemy]->full_HP = 0;

		/* Clears and reprints the enemy */
		Enemy_IndvClear(enemy1[chosen_enemy], enemy_AI1, tileManager);

		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\EnemyDie.wav");
		playSound_Low(sound);
	}

	/* Enemy belongs to P2, - 40 to get the correct value */
	else if (chosen_enemy >= 40)
	{
		chosen_enemy -= 40;
		/* Revive enemy and sets HP back to max HP */
		enemy2[chosen_enemy]->alive = 0;
		enemy2[chosen_enemy]->HP = 0;
		enemy2[chosen_enemy]->full_HP = 0;

		/* Clears and reprints the enemy */
		Enemy_IndvClear(enemy2[chosen_enemy], enemy_AI2, tileManager);

		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\EnemyDie.wav");
		playSound_Low(sound);
	}
}

/* Drops a powerup to both players that kills a random enemy */
void CoOp_KillEnemy_Dropper(Stage *stage, Powerup *powerup[])
{
	int i;
	int time = 50;

	/* Controls this function so that it does not happen multiple times a second */
	stage->coop_powerup_control = 0;

	/* Checks for a free memory */
	for (i = 0; i < 10; ++i)
		if (powerup[i] == NULL)
		{
			/* Creates the powerup for player 1 */
			powerup[i] = Powerup_KillEnemy_Create(stage, 1);
			break;
		}

	/* This is so that the seed is different when it spawns the next powerup */
	while (--time);

	/* Checks for a free memory */
	for (i = 0; i < 10; ++i)
		if (powerup[i] == NULL)
		{
			/* Creates the powerup for player 2 */
			powerup[i] = Powerup_KillEnemy_Create(stage, -1);
			break;
		}
}