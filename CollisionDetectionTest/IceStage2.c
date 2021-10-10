/******************************************************************************
All content © 2017 Digipen Institute of Technology Singapore, all rights reserved

filename			IceStage2.c
author				Enriquez, Audry May dela Paz (25%) | Wong Zhihao (25%)
					Ngm Hui Min, Lois (25%)			   | Ong Tze Ang (25%)
DP email			a.enriquez@digipen.edu			   | zhihao.wong@digipen.edu
					lois.ngm@digipen.edu			   | tzeang.ong@digipen.edu
course				BSGD							   | RTIS
					RTIS							   | RTIS

Brief Description:
Contents of Ice Stage 2 object of the game.
******************************************************************************/
/* Load Libraries */
#include "stdafx.h"
#include <windows.h>			/* COORD, HANDLE etc */
#include <stdio.h>				/* Printf */
#include <conio.h>				/* TO BE REMOVED WHEN DONE TESTING for kbhit */
#include <time.h>				/* Delta Time and current time */
#include "Player.h"				/* Player object and data */
#include "Enemy.h"				/* Enemy object and data */
#include "Stage.h"				/* Stage object and data */
#include "EnemyAI.h"			/* Enemy AI object and data */
#include "FMODInit.h"			/* Sound object and data */
#include "Ball.h"				/* Ball object and data */
#include "TileMapManager.h"		/* Tile manager object and data */
#include "Powerups.h"			/* Powerup object and data */
#include "EnemyTypes.h"			/* Enemy Types */
#include "EnemyBullets.h"		/* Enemy Bullets */
#include "GameStateMainMenu.h"	/* Main Menu */
#include "WorldData.h"			/* WorldData_GoToXY(), hidecursor() */
#include "IceStage2.h"			/* function declaration */
#include "FireStage1.h"			/* firestage_1() */
#include "StageData.h"			/* common functions of stages */
#include "Highscore.h"			/* Highscore_Check(), Highscore_Display_Stage() */

/* number of enemies in a row */
#define NUM_ENEMIES 7
/* number of rows of enemies */
#define NUM_ROWS 3

/* Size of screen */
#define SIZE_X 70
#define SIZE_Y 78
#define xBorder 6
#define yBorder 23

/* Enemies for Player 1 */
#define ENEMY1_START_POSY 22
/* Enemies for Player 2 */
#define ENEMY2_START_POSY 54

/* Details of enemies (starting positions and gap between them */
#define ENEMY_START_POSX 5
#define ENEMY_END_POSX 66
#define HORIZONTAL_GAP 5
#define VERTICAL_GAP 2	

/* Ice Terrain Stage 2 */
void iceStage_2(Sound *sound, int play_type, int terrain)
{
	/* colour */
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	/* Save current attributes of the original colour */
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;
	/* set up for truly random */
	srand((unsigned)clock());

	/* Hides cursor */
	hidecursor();

	/* Initialize the Tile Manager */
	TileMapManager *tile_manager = NULL;

	/* loop variables */
	int i, j;
	/* position of which the player is in the highscore if applicable */
	int position;

	/* Initialises the player objects */
	Player *player_1 = NULL;
	Player *player_2 = NULL;

	/*Initialise the ball objects*/
	Ball *ball_1 = NULL;
	Ball *ball_2 = NULL;

	/* Initialises the enemy behaviours */
	Enemy_AI *enemy_AI_P1 = NULL;
	Enemy_AI *enemy_AI_P2 = NULL;
	/* Initialises the enemies */
	Enemy *enemyContainer_P1[(NUM_ENEMIES * NUM_ROWS) + 10] = { NULL };
	Enemy *enemyContainer_P2[(NUM_ENEMIES * NUM_ROWS) + 10] = { NULL };

	/* Name of current stage */
	Stage* iceStage;

	/* Creates powerups, there can only be 10 powerups max in the map at a time */
	Powerup *powerup[10] = { NULL };

	/* Creates enemy bullets, there will only be 50 enemy bullets in the map at a time */
	EnemyBullet *enemyBullet[50] = { NULL };

	/* Create the tile manager */
	tile_manager = TileMapManager_Create();

	/* Creates the current stage based on provided data */
	iceStage = Stage_Create(play_type, 1, NUM_ENEMIES, NUM_ROWS, SIZE_X, SIZE_Y, xBorder, yBorder, HORIZONTAL_GAP, VERTICAL_GAP,
		5, 0, 5, 4, 1, 2, 1, 3, PLAYER1_START_POSY, PLAYER2_START_POSY, terrain);

	/* Sets the colour of the stage borders based on terrain type */
	if (iceStage->stage_type == NORMAL);
	else if (iceStage->stage_type == ICE) SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN);
	else if (iceStage->stage_type == FIRE) SetConsoleTextAttribute(hConsole, FOREGROUND_RED);

	/* Prints out stage borders based on provided data */
	Stage_Print(iceStage);
	SetConsoleTextAttribute(hConsole, saved_attributes);

	/* Creates the players */
	player_1 = Player_Create(player1, PLAYER1_LEFT, PLAYER1_RIGHT, PLAYER1_ATTACK, PLAYER1_BOOST, PLAYER1_START_POSX,
		PLAYER1_START_POSY, 7, PLAYER1_STATBAR, PLAYER_HEALTH, PLAYER_BOOST, PLAYER_SCORE, 1, tile_manager);
	player_2 = Player_Create(player2, PLAYER2_LEFT, PLAYER2_RIGHT, PLAYER2_ATTACK, PLAYER2_BOOST, PLAYER2_START_POSX,
		PLAYER2_START_POSY, 7, PLAYER2_STATBAR, PLAYER_HEALTH, PLAYER_BOOST, PLAYER_SCORE, 2, tile_manager);

	/* Creates the enemy behaviours based on the player it faces */
	enemy_AI_P1 = Enemy_AI_Create(iceStage, 1, iceStage->y_border);
	enemy_AI_P2 = Enemy_AI_Create(iceStage, -1, iceStage->size_y - iceStage->y_border - 2);

	/* Spawns the enemies based on stage data and enemy behaviours */
	Enemy_Spawn(iceStage, enemyContainer_P1, enemy_AI_P1, tile_manager, 1, sound);
	Enemy_Spawn(iceStage, enemyContainer_P2, enemy_AI_P2, tile_manager, 2, sound);

	/* Creates the balls */
	ball_1 = Ball_Create(iceStage, 1, '@', PLAYER1_START_POSX + (PLAYER_WIDTH / 2), PLAYER1_START_POSY + PLAYER_HEIGHT, PLAYER1_FIREBUTTON_SPACEBAR, player_1, player_2);
	ball_2 = Ball_Create(iceStage, 2, 'O', PLAYER2_START_POSX + (PLAYER_WIDTH / 2), PLAYER2_START_POSY - 1, PLAYER2_FIREBUTTON_ENTER, player_2, player_1);
	Ball_UpdateEnemyBall(ball_1, ball_2);

	/* intiialize the fireStage current_time */
	iceStage->current_time = clock();

	int select;

	while (1)
	{
		/* get char of user input while game loops */
		int input;

		/* Hides cursor every loop */
		hidecursor();

		/* update the current stage time */
		iceStage->delta_time = clock() - iceStage->current_time;
		iceStage->current_time = clock();

		/* Keep track of elapsed time of the game */
		Stage_Timer(iceStage, enemyContainer_P1, enemyContainer_P2, enemy_AI_P1, enemy_AI_P2, ball_1, ball_2, player_1, player_2,
			tile_manager, sound, powerup);

		/* Updates player data every loop */
		Player_Update(iceStage, player_1, ball_1, ball_2, powerup, enemyBullet, tile_manager, sound,
			enemyContainer_P1, enemyContainer_P2, enemy_AI_P1, enemy_AI_P2);
		Player_Update(iceStage, player_2, ball_1, ball_2, powerup, enemyBullet, tile_manager, sound,
			enemyContainer_P1, enemyContainer_P2, enemy_AI_P1, enemy_AI_P2);

		/* Updates Powerup data every loop */
		Powerup_Update(powerup, player_1, player_2, ball_1, ball_2, iceStage, tile_manager, sound,
			enemyContainer_P1, enemyContainer_P2, enemy_AI_P1, enemy_AI_P2);

		/* Updates enemy bullets data every loop */
		EnemyBullet_Update(enemyBullet, player_1, player_2, iceStage, tile_manager, sound);

		/* Updates the balls every loop */
		Ball_Update(sound, ball_1, tile_manager, iceStage, enemyContainer_P1, enemyContainer_P2, enemy_AI_P1, enemy_AI_P2, powerup, enemyBullet);
		Ball_Update(sound, ball_2, tile_manager, iceStage, enemyContainer_P1, enemyContainer_P2, enemy_AI_P1, enemy_AI_P2, powerup, enemyBullet);

		/************************
		Controls enemy movements
		*************************/
		/* checks if the enemy has reached player 2 */
		if (Enemy_Control(iceStage, enemyContainer_P2, enemy_AI_P2, tile_manager, ball_1, ball_2, powerup, sound))
		{
			WorldData_GoToXY(13, 36);

			if (iceStage->play_type == PvP)
				printf("THE ENEMY HAS REACHED PLAYER 2! PLAYER 1 WINS!");
			else if (iceStage->play_type == CoOp)
				printf("THE ENEMY HAS REACHED YOU! YOU HAVE LOST!");
			break;
		}
		/* checks if the enemy has reached player 1 */
		if (Enemy_Control(iceStage, enemyContainer_P1, enemy_AI_P1, tile_manager, ball_1, ball_2, powerup, sound))
		{
			WorldData_GoToXY(13, 36);

			if (iceStage->play_type == PvP)
				printf("THE ENEMY HAS REACHED PLAYER 1! PLAYER 2 WINS!");
			else if (iceStage->play_type == CoOp)
				printf("THE ENEMY HAS REACHED YOU! YOU HAVE LOST!");
			break;
		}

		/* Goes through enemies in the first row */
		for (i = 0; i < iceStage->num_enemies; ++i)
			/* Goes through enemies within that column */
			for (j = i; j < iceStage->total_enemies; j += iceStage->num_enemies)
			{
				/* Checks if enemy is alive for player 1 area */
				if (enemyContainer_P1[j]->alive == 1 && enemyContainer_P1[j]->HP > 0)
				{
					/* if enemy is close to player, show warning */
					if (enemyContainer_P1[j]->y_Pos == 12)
					{
						WorldData_GoToXY(89, 20);
						printf("!!   DANGER   !!");
						WorldData_GoToXY(89, 21);
						printf("!! ENEMY NEAR !!");
					}
				}
				/* Checks if enemy is alive for player 2 area */
				if (enemyContainer_P2[j]->alive == 1 && enemyContainer_P2[j]->HP > 0)
				{
					/* if enemy is close to player, show warning */
					if (enemyContainer_P2[j]->y_Pos == 64)
					{
						WorldData_GoToXY(89, 60);
						printf("!!   DANGER   !!");
						WorldData_GoToXY(89, 61);
						printf("!! ENEMY NEAR !!");
					}
				}
			}

		/* Randomises whether the enemy shoots in the current frame */
		Enemy_Fire(iceStage, enemyContainer_P1, enemyBullet, sound); /* for P1 */
		Enemy_Fire(iceStage, enemyContainer_P2, enemyBullet, sound); /* for P2 */

		/* Update sound */
		updateFMOD(sound);

		/* if player hits certain keys */
		if (_kbhit())
		{
			input = _getch();

			/* if player hits pause */
			if (input == 'p')
			{
				/* play for pause menu */
				loadSound(sound, "..\\CollisionDetectionTest\\SFX\\MenuSelect.wav");
				playSound_Medium(sound);

				/* pause the stage time */
				time_t pauseclock;
				pauseclock = time(NULL);

				/* prints the pause menu */
				optionmenu_Print(iceStage, 1, 1);
				select = _getch();

				/* if player selects 1, 2 or 3 in pause menu */
				while (select)
				{
					if (select == '1' || select == '2' || select == '3')
						break;

					printf("");
					select = _getch();
				}
				switch (select)
				{
					/* RESUME */
				case '1':
					/* resume the game time */
					iceStage->start_time += time(NULL) - pauseclock;
					iceStage->end_time += time(NULL) - pauseclock;
					iceStage->powerup_time += time(NULL) - pauseclock;
					/* play sound */
					loadSound(sound, "..\\CollisionDetectionTest\\SFX\\MenuSelect.wav");
					playSound_Medium(sound);
					/* clear away the pause menu */
					optionmenu_Clear();
					break;
					/* MAIN MENU */
				case '2':
					/* Exiting the stage, frees the stage */
					Stages_Free(iceStage, player_1, player_2, enemyContainer_P1, enemyContainer_P2, enemyBullet, enemy_AI_P1, enemy_AI_P2,
					powerup, ball_1, ball_2, tile_manager);
					/* play sound */
					loadSound(sound, "..\\CollisionDetectionTest\\SFX\\MenuSelect.wav");
					playSound_Medium(sound);
					/* reset the windows size for main menu */
					SetConsoleWindowSize(80, 25);
					/* stop the fire stage background music */
					stopMusic(sound->Battle);
					loadMusic_Menu(sound, "..\\CollisionDetectionTest\\Musics\\MainMenuMusic.ogg");
					playMusic_Menu(sound);
					/* Create and display MAIN MENU */
					MainMenuStageCreate(sound);
					break;
					/* QUIT */
				case '3':
					/* quits the game, frees the stage */
					Stages_Free(iceStage, player_1, player_2, enemyContainer_P1, enemyContainer_P2, enemyBullet, enemy_AI_P1, enemy_AI_P2,
					powerup, ball_1, ball_2, tile_manager);
					/* Free sound */
					Sound_Free(&sound);
					/* close game */
					exit(0);
					break;
				}

			}
		}

		/* Win Condition */
		/* if either player's health depletes, end stage */
		if (player_1->health <= 0 || player_2->health <= 0)
			break;

		/* if all enemies are dead, end stage */
		if (iceStage->play_type == CoOp && CoOp_Enemy_Check(iceStage, enemyContainer_P1, enemyContainer_P2))
			break;

	}

	/* stop the fire stage background music */
	stopMusic(sound->Battle);

	/*****
	PvP
	*****/
	/* if both players health are less than equal to 0 */
	if (player_1->health <= 0 && player_2->health <= 0 && iceStage->play_type == PvP)
	{
		WorldData_GoToXY(28, 36);
		printf("Y'all dead.");
	}
	/* if player 1's health is less than equal to 0 */
	if (player_1->health <= 0 && iceStage->play_type <= PvP)
	{
		WorldData_GoToXY(28, 36);
		printf("PLAYER 2 WINS!");

		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\PvPWin.wav");
		playSound_High(sound);
	}
	/* if player 2's health is less than equal to 0 */
	if (player_2->health <= 0 && iceStage->play_type <= PvP)
	{
		WorldData_GoToXY(28, 36);
		printf("PLAYER 1 WINS!");

		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\PvPWin.wav");
		playSound_High(sound);
	}
	/**********
	CoOp
	***********/
	/* if either player's health is less than equal to zero, LOSE */
	if (player_1->health <= 0 || player_2->health <= 0 && iceStage->play_type == CoOp)
	{
		WorldData_GoToXY(28, 36);
		printf("YOU HAVE LOST!");

		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\CoOpDefeat.wav");
		playSound_High(sound);
	}
	/* if all enemies are cleared, WIN */
	if (iceStage->play_type == CoOp && CoOp_Enemy_Check(iceStage, enemyContainer_P1, enemyContainer_P2))
	{
		/* Congradulatory message */
		WorldData_GoToXY(18, 36);
		printf("YOU HAVE WON! MOVE ON TO THE NEXT ROUND!");

		/* Play win sound */
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\CoOpWin.wav");
		playSound_High(sound);

		Sleep(2000);
		select = _getch();
		/* checks if the player is in the highscore */
		position = Highscore_Check(player_1, player_2, 4);
		system("cls");
		iceStage->size_x = 80;
		iceStage->size_y = 25;
		/* display the highscore to player */
		Highscore_Display_Stage(iceStage, 4, position);
		/* free stage */
		Stages_Free(iceStage, player_1, player_2, enemyContainer_P1, enemyContainer_P2, enemyBullet, enemy_AI_P1, enemy_AI_P2,
			powerup, ball_1, ball_2, tile_manager);

		/* reset the console size */
		SetConsoleWindowSize(80, 70);
		WorldData_GoToXY(33, 35);

		/* Transition to next stage */
		printf("Fire Stage 1");
		WorldData_GoToXY(36, 37);
		printf("Co-Op");
		Sleep(2000);
		system("cls");
		WorldData_GoToXY(37, 35);
		printf("Ready?");
		Sleep(1000);
		system("cls");
		SetConsoleWindowSize(107, 80);
		/* Play fire stage 1 background music */
		loadMusic_Battle(sound, "..\\CollisionDetectionTest\\Musics\\CombatMusic.ogg");
		playMusic_Battle(sound);
		/* Go to Fire Stage 1 */
		fireStage_1(sound, CoOp, FIRE);	}

	else
	{
		Sleep(2000);
		select = _getch();
		/* Reset console size to menu */
		SetConsoleWindowSize(80, 25);
		/* Free the stage */
		Stages_Free(iceStage, player_1, player_2, enemyContainer_P1, enemyContainer_P2, enemyBullet, enemy_AI_P1, enemy_AI_P2,
			powerup, ball_1, ball_2, tile_manager);
		/* Play main menu */
		loadMusic_Menu(sound, "..\\CollisionDetectionTest\\Musics\\MainMenuMusic.ogg");
		playMusic_Menu(sound);
		/* Create MAIN MENU */
		MainMenuStageCreate(sound);
	}
}