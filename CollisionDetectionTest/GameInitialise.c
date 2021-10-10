/******************************************************************************
All content © 2017 Digipen Institute of Technology Singapore, all rights reserved

filename			GameInitialise.c
author				Enriquez, Audry May dela Paz (25%) | Wong Zhihao (25%)
					Ngm Hui Min, Lois (25%)			   | Ong Tze Ang (25%)
DP email			a.enriquez@digipen.edu			   | zhihao.wong@digipen.edu
					lois.ngm@digipen.edu			   | tzeang.ong@digipen.edu
course				BSGD							   | RTIS
					RTIS							   | RTIS

Brief Description:
Contents of GameInitialize object of the game. (Debug)
******************************************************************************/
/* Load Libraries */
#include "stdafx.h"
#include "GameInitialise.h"		/* Function declarations */
#include <windows.h>			/* COORD, HANDLE etc */
#include <stdio.h>				/* Printf */
#include <conio.h>				/* TO BE REMOVED WHEN DONE TESTING for kbhit */
#include "Player.h"				/* Player object and data */
#include "Enemy.h"				/* Enemy object and data */
#include "Stage.h"				/* Stage object and data */
#include "EnemyAI.h"			/* Enemy AI object and data */
#include "FMODInit.h"			/* Sound object and data */
#include "Ball.h"				/* Ball object and data */
#include "TileMapManager.h"		/* Tile manager object and data */
#include "Powerups.h"			/* Powerup object and data */
#include <time.h>				/* Clock FOR DEBUGGING */
#include "EnemyTypes.h"			/* Enemy Types */
#include "EnemyBullets.h"		/* Enemy bullet object */
#include "GameStateMainMenu.h"	/* Main Menu */
#include "WorldData.h"			/* WorldData_GoToXY(), hidecursor() */
#include "StageData.h"			/* common functions of stages */

/* Enemies for Player 1 */
#define ENEMY1_START_POSY 22
#define ENEMY2_START_POSY 54
#define ENEMY_START_POSX 5
#define ENEMY_END_POSX 66

/* number of enemies in a row */
#define NUM_ENEMIES 9
/* number of rows of enemies*/
#define NUM_ROWS 3

/* Size of screen */
#define SIZE_X 70
#define SIZE_Y 78
#define xBorder 5
#define yBorder 24

/* gap between enemies */
#define HORIZONTAL_GAP 3
#define VERTICAL_GAP 2	

/* Debugger for Player ball directions */
void DebugDirectionNamer(Ball* ball1, int balltype)
{
	switch (ball1->balldirection)
	{
	case -1: printf("Player %d ball direction : STOP                  ", balltype);
		break;
	case 0: printf("Player %d ball direction : BOTTOMRIGHT 45degrees  ", balltype);
		break;
	case 1: printf("Player %d ball direction : BOTTOMLEFT 45degrees   ", balltype);
		break;
	case 2: printf("Player %d ball direction : TOPRIGHT 45degrees     ", balltype);
		break;
	case 3: printf("Player %d ball direction : TOPLEFT 45degrees      ", balltype);
		break;
	case 4: printf("Player %d ball direction : BOTTOMRIGHT 60degrees  ", balltype);
		break;
	case 5: printf("Player %d ball direction : BOTTOMLEFT 60degrees   ", balltype);
		break;
	case 6: printf("Player %d ball direction : TOPRIGHT 60degrees     ", balltype);
		break;
	case 7: printf("Player %d ball direction : TOPLEFT 60degrees      ", balltype);
		break;
	case 8: printf("Player %d ball direction : BOTTOMRIGHT 20degrees  ", balltype);
		break;
	case 9: printf("Player %d ball direction : BOTTOMLEFT 20degrees   ", balltype);
		break;
	case 10: printf("Player %d ball direction : TOPRIGHT 20degrees    ", balltype);
		break;
	case 11: printf("Player %d ball direction : TOPLEFT 20degrees     ", balltype);
		break;
	case 12: printf("Player %d ball direction : UP                    ", balltype);
		break;
	case 13: printf("Player %d ball direction : DOWN                  ", balltype);
		break;
	}
}

/* Debug Menu */
void Debug(Stage* stage, Player* player_1, Player *player_2, Enemy* enemy1[], Enemy* enemy2[], Enemy_AI* enemy_AI,
	Ball* ball1, Ball* ball2)
{
	/* Displays delta time */
	WorldData_GoToXY(141, 3);
	printf("%3ld", stage->delta_time);
	/* Displays player 1's positions */
	WorldData_GoToXY(141, 3);
	printf("Player 1 x,y : (%2d,%2d)", player_1->x_p, player_1->y_p);
	WorldData_GoToXY(141, 5);

	DebugDirectionNamer(ball1, 1);
	WorldData_GoToXY(141, 6);
	/* Displays player 1's health */
	printf("Player 1 HP : %3d", player_1->health);
	WorldData_GoToXY(141, 7);
	/* Displays player 1's boost value */
	printf("Player 1 boost value : %.2f", player_1->boost_amt);
	WorldData_GoToXY(141, 8);
	/* Displays player 1's paddle length */
	printf("Player 1 length : %d", player_1->x_l);
	WorldData_GoToXY(141, 9);
	/* Displays player 1's first enemy position */
	printf("Enemy P1 x,y : (%2d,%2d)", enemy1[0]->x_Pos, enemy1[0]->y_Pos);
	WorldData_GoToXY(141, 10);
	/* Displays player 1's last enemy position */
	printf("Enemy P1 edge x,y : (%2d,%2d)", enemy1[stage->num_enemies - 1]->x_Pos, enemy1[stage->num_enemies - 1]->y_Pos);

	/* Display the direction of which the enemies are moving */
	WorldData_GoToXY(141, 14);
	printf("Enemy L / R : %3d / %3d", enemy_AI->left_trigger, enemy_AI->right_trigger);
	WorldData_GoToXY(141, 15);
	/* displays the distance between the enemy and players */
	printf("Enemy distance to player P1 / P2: %2d | 17 / %2d | 17", enemy1[0]->y_Pos - player_1->y_p - 2, player_2->y_p - enemy2[0]->y_Pos - 1);
	WorldData_GoToXY(141, 16);
	/* Displays the elapsed time */
	printf("Elasped Time : %.15ld", clock());
	WorldData_GoToXY(72, 17);
	/* Displays the elapsed time of the stage */
	printf("Elasped Time (STAGE) : %2lld", time(NULL) - stage->start_time);

	WorldData_GoToXY(141, 19);
	/* Displays the position of player 1's ball */
	printf("Ball1 x,y : (%2d,%2d)", ball1->x_ballpos, ball1->y_ballpos);
	/* Displays the speed of player 1's ball */
	WorldData_GoToXY(141, 20);
	printf("Ball1 speed: %d", ball1->base_timer);
	WorldData_GoToXY(141, 21);
	/* Displays the position of player 2's ball */
	printf("Ball2 x,y : (%2d,%2d)", ball2->x_ballpos, ball2->y_ballpos);
	WorldData_GoToXY(141, 22);
	/* Displays the speed of player 2's ball */
	printf("Ball1 speed: %d", ball2->base_timer);
	WorldData_GoToXY(141, 23);
	DebugDirectionNamer(ball1, 1);
	WorldData_GoToXY(141, 24);
	DebugDirectionNamer(ball2,2);

	/* Display the difficulty multiplier */
	WorldData_GoToXY(141, 25);
	printf("Difficulty: %3.1f", stage->diff_mult);

	/* Display the speed of player 1's enemies */
	WorldData_GoToXY(141, 27);
	printf("Enemy 1 move speed: %3d", enemy_AI->base_timer);
}

/* Debugger to check if enemy has been killed */
void Debug_KillEnemy(Stage *stage, Enemy *enemy[], Enemy_AI *enemy_AI, TileMapManager *tilemanager)
{
	int i;

	for (i = 0; i < stage->total_enemies; ++i)
		if (enemy[i]->alive == 1)
		{
			enemy[i]->alive = 0;
			Enemy_IndvClear(enemy[i], enemy_AI, tilemanager);
			break;
		}
}
/* Debugger to check the respawn of enemies */
void Debug_RespawnEnemy(Stage *stage, Enemy *enemy[], Enemy_AI *enemy_AI, TileMapManager *tilemanager)
{
	int check = 1;
	int num;

	while (check)
	{
		num = rand() % stage->total_enemies;

		if (enemy[num]->alive == 1);
		else
		{
			enemy[num]->alive = 1;
			Enemy_IndvPrint(enemy[num], enemy_AI, tilemanager);
			check = 0;
		}
	}
}


/* Frees the data of this object */
void testStage_Free(Stage *stage, Player *player_1, Player *player_2, Enemy *enemyP1[], Enemy *enemyP2[], EnemyBullet *enemyBullet[],
	Enemy_AI *enemy_AI_P1, Enemy_AI *enemy_AI_P2, Powerup *powerup[], Ball *ball_1, Ball *ball_2, TileMapManager *tileManager)
{
	int i; /* loop variable */
	/* frees players */
	Player_Free(&player_1);
	Player_Free(&player_2);
	for (i = 0; i < stage->total_and_hydra; ++i)
	{
		/* Free enemy */
		Enemy_Free(&enemyP1[i]);
		Enemy_Free(&enemyP2[i]);
	}

	for (i = 0; i < 10; ++i)
	{
		/* Free power up*/
		Powerup_Free(&powerup[i]);
	}

	for (i = 0; i < 50; ++i)
	{
		/* Free enemy bullets */
		EnemyBullet_Free(&enemyBullet[i]);
	}
	/* Free Enemy AIs */
	Enemy_AI_Free(&enemy_AI_P1);
	Enemy_AI_Free(&enemy_AI_P2);

	/* Free Balls */
	Ball_Free(&ball_1);
	Ball_Free(&ball_2);

	/* Free Tile Map Manager */
	TileMapManager_Free(&tileManager);

	/* Free the Stage */
	Stage_Free(&stage);
}

/* Testing Stage before normal, fire and ice */
void testStage(Sound *sound, int terrain)
{
	/* colour */
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	/* Save current attributes of the original colour */
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;
	/* set truly random */
	srand((unsigned)clock());

	/* Hides cursor */
	hidecursor();
	/* Initialize the Tile Manager */
	TileMapManager *tile_manager = NULL;

	/* loop variables */
	int i, j;

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
	Stage* testStage;

	/* Creates powerups, there can only be 10 powerups max in the map at a time */
	Powerup *powerup[10] = { NULL };

	/* Creates enemy bullets, there will only be 50 enemy bullets in the map at a time */
	EnemyBullet *enemyBullet[50] = { NULL };

	/* Create the tile manager */
	tile_manager = TileMapManager_Create();

	/* Creates the current stage based on provided data */
	testStage = Stage_Create(PvP, 1, NUM_ENEMIES, NUM_ROWS, SIZE_X, SIZE_Y, xBorder, yBorder, HORIZONTAL_GAP, VERTICAL_GAP, 
							 1, 2, 3, 4, 2, 5, 1, 10, PLAYER1_START_POSY, PLAYER2_START_POSY, terrain);

	/* Prints out stage borders based on provided data */
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	Stage_Print(testStage);
	SetConsoleTextAttribute(hConsole, saved_attributes);

	/* Creates the players */
	player_1 = Player_Create(player1, PLAYER1_LEFT, PLAYER1_RIGHT, PLAYER1_ATTACK, PLAYER1_BOOST, PLAYER1_START_POSX,
		PLAYER1_START_POSY, 7, PLAYER1_STATBAR, PLAYER_HEALTH, PLAYER_BOOST, PLAYER_SCORE, 1, tile_manager);
	player_2 = Player_Create(player2, PLAYER2_LEFT, PLAYER2_RIGHT, PLAYER2_ATTACK, PLAYER2_BOOST, PLAYER2_START_POSX,
		PLAYER2_START_POSY, 7, PLAYER2_STATBAR, PLAYER_HEALTH, PLAYER_BOOST, PLAYER_SCORE, 2, tile_manager);

	/* Creates the enemy behaviours based on the player it faces */
	enemy_AI_P1 = Enemy_AI_Create(testStage, 1, testStage->y_border);
	enemy_AI_P2 = Enemy_AI_Create(testStage, -1, testStage->size_y - testStage->y_border - 2);

	/* Spawns the enemies based on stage data and enemy behaviours */
	Enemy_Spawn(testStage, enemyContainer_P1, enemy_AI_P1, tile_manager, 1, sound);
	Enemy_Spawn(testStage, enemyContainer_P2, enemy_AI_P2, tile_manager, 2, sound);

	/* Creates the balls */
	ball_1 = Ball_Create(testStage, 1, '@', PLAYER1_START_POSX + (PLAYER_WIDTH / 2), PLAYER1_START_POSY + PLAYER_HEIGHT, PLAYER1_FIREBUTTON_SPACEBAR, player_1, player_2);
	ball_2 = Ball_Create(testStage, 2, 'O', PLAYER2_START_POSX + (PLAYER_WIDTH / 2), PLAYER2_START_POSY - 1, PLAYER2_FIREBUTTON_ENTER, player_2, player_1);
	Ball_UpdateEnemyBall(ball_1, ball_2);

	/* intialize the current time of stage */
	testStage->current_time = clock();

	int select;

	while (1)
	{
		int input;
		/* Debug menu */
		Debug(testStage, player_1, player_2, enemyContainer_P1, enemyContainer_P2, enemy_AI_P1, ball_1, ball_2);

		/* Update the stage time */
		testStage->delta_time = clock() - testStage->current_time;
		testStage->current_time = clock();

		/* Hides cursor */
		hidecursor();

		/* Updates player data every loop */
		Player_Update(testStage, player_1, ball_1, ball_2, powerup, enemyBullet, tile_manager, sound, 
					  enemyContainer_P1, enemyContainer_P2, enemy_AI_P1, enemy_AI_P2);
		Player_Update(testStage, player_2, ball_1, ball_2, powerup, enemyBullet, tile_manager, sound, 
					  enemyContainer_P1, enemyContainer_P2, enemy_AI_P1, enemy_AI_P2);

		/* Updates the powerup data every loop*/
		Powerup_Update(powerup, player_1, player_2, ball_1, ball_2, testStage, tile_manager, sound,
					   enemyContainer_P1, enemyContainer_P2, enemy_AI_P1, enemy_AI_P2);
		/* Updates the Enemy Bullet every loop */
		EnemyBullet_Update(enemyBullet, player_1, player_2, testStage, tile_manager, sound);

		/* Updates the balls every loop */
		Ball_Update(sound, ball_1, tile_manager, testStage, enemyContainer_P1, enemyContainer_P2, enemy_AI_P1, enemy_AI_P2, powerup, enemyBullet);
		Ball_Update(sound, ball_2, tile_manager, testStage, enemyContainer_P1, enemyContainer_P2, enemy_AI_P1, enemy_AI_P2, powerup, enemyBullet);

		/* Controls enemy movements */
		if (Enemy_Control(testStage, enemyContainer_P2, enemy_AI_P2, tile_manager, ball_1, ball_2, powerup, sound))
		{
			WorldData_GoToXY(13, 36);
			printf("THE ENEMY HAS REACHED PLAYER 2! PLAYER 1 WINS!");
			break;
		}

		if (Enemy_Control(testStage, enemyContainer_P1, enemy_AI_P1, tile_manager, ball_1, ball_2, powerup, sound))
		{
			WorldData_GoToXY(13, 36);
			printf("THE ENEMY HAS REACHED PLAYER 1! PLAYER 2 WINS!");
			break;
		}

		/* Goes through enemies in the first row */
		for (i = 0; i < testStage->num_enemies; ++i)
			/* Goes through enemies within that column */
			for (j = i; j < testStage->total_enemies; j += testStage->num_enemies)
			{
				/* Checks if enemy is alive for player 1 */
				if (enemyContainer_P1[j]->alive == 1 && enemyContainer_P1[j]->HP > 0)
				{
					/* Display warning */
					if (enemyContainer_P1[j]->y_Pos == 12)
					{
						WorldData_GoToXY(89, 20);
						printf("!! DANGER !!");
						WorldData_GoToXY(89, 21);
						printf("!! ENEMY  !!");
					}
				}
				/* Checks if enemy is alive for player 2 */
				if (enemyContainer_P2[j]->alive == 1 && enemyContainer_P2[j]->HP > 0)
				{
					/* Display warning */
					if (enemyContainer_P2[j]->y_Pos == 64)
					{
						WorldData_GoToXY(89, 60);
						printf("!! DANGER !!");
						WorldData_GoToXY(89, 61);
						printf("!! ENEMY  !!");
					}
				}
			}

		/* Randomises whether the enemy shoots in the current frame */
		Enemy_Fire(testStage, enemyContainer_P1, enemyBullet, sound);
		Enemy_Fire(testStage, enemyContainer_P2, enemyBullet, sound);

		/* Keeps track of elapsed game time */
		Stage_Timer(testStage, enemyContainer_P1, enemyContainer_P2, enemy_AI_P1, enemy_AI_P2, ball_1, ball_2, player_1, player_2, 
					tile_manager, sound, powerup);

		updateFMOD(sound);

		/* Testing features */
		if (_kbhit())
		{
			input = _getch();
			/* Test player paddle sizes */
			//if (input == 'h' && length < 9)
			//{
			//	length += 2;
			//	switch (length)
			//	{
			//	case 3:
			//		Player_LengthBorderCheck(testStage, player_1);
			//		Player_LengthUpdate(player_1, player1down, 3, tile_manager);
			//		break;
			//	case 5:
			//		Player_LengthBorderCheck(testStage, player_1);
			//		Player_LengthUpdate(player_1, player1, 5, tile_manager);
			//		break;
			//	case 7:
			//		Player_LengthBorderCheck(testStage, player_1);
			//		Player_LengthUpdate(player_1, player1up, 7, tile_manager);
			//		break;
			//	case 9:
			//		Player_LengthBorderCheck(testStage, player_1);
			//		Player_LengthUpdate(player_1, player1upup, 9, tile_manager);
			//		break;
			//	}
			//}

			//if (input == 'g' && length > 1)
			//{
			//	length -= 2;
			//	switch (length)
			//	{
			//	case 1: Player_LengthUpdate(player_1, player1downdown, 1, tile_manager);
			//		break;
			//	case 3: Player_LengthUpdate(player_1, player1down, 3, tile_manager);
			//		break;
			//	case 5: Player_LengthUpdate(player_1, player1, 5, tile_manager);
			//		break;
			//	case 7: Player_LengthUpdate(player_1, player1up, 7, tile_manager);
			//		break;
			//	}
			//}

			/* Test HP increase power up */
			//if (input == 't')
			//{
			//	plusHP(player_1);
			//}

			/* Test HP decrease power down */
			//if (input == 'r')
			//{
			//	Player_Clear_Health(player_1);
			//	minusHP(player_1);
			//}

			/* quit game */
			if (input == 'q')
			{
				/* Free Players */
				Player_Free(&player_1);
				Player_Free(&player_2);
				for (i = 0; i < testStage->total_and_hydra; ++i)
				{
					/* Free enemies */
					Enemy_Free(&enemyContainer_P1[i]);
					Enemy_Free(&enemyContainer_P2[i]);
				}

				for (i = 0; i < 10; ++i)
				{
					/* Free powerups */
					Powerup_Free(&powerup[i]);
				}

				for (i = 0; i < 50; ++i)
				{
					/* Free enemy bullets */
					EnemyBullet_Free(&enemyBullet[i]);
				}
				/* Free Enemy AIs */
				Enemy_AI_Free(&enemy_AI_P1);
				Enemy_AI_Free(&enemy_AI_P2);

				/* Free Balls */
				Ball_Free(&ball_1);
				Ball_Free(&ball_2);

				/* Free Tile Map Manager */
				TileMapManager_Free(&tile_manager);

				/* Free Stage*/
				Stage_Free(&testStage);

				/* Free Sound*/
				Sound_Free(&sound);

				/* close game */
				exit(0);
			}

			/* Test enemy movements */
			if (input == 'm')
			{
				enemy_AI_P1->timer += 10000;
				enemy_AI_P2->timer += 10000;
			}
			/* Test pause */
			if (input == 'p')
			{
				time_t pauseclock;
				optionmenu_Print(testStage, 0, 15);

				pauseclock = time(NULL);


				select = _getch();
				while (select)
				{
					if (select == '1' || select == '2' || select == '3')
						break;

					printf("");
					select = _getch();
				}
				switch (select)
				{
				case '1': testStage->start_time += time(NULL) - pauseclock; optionmenu_Clear();
					break;
				case '2': testStage_Free(testStage, player_1, player_2, enemyContainer_P1, enemyContainer_P2, enemyBullet, enemy_AI_P1, enemy_AI_P2,
					powerup, ball_1, ball_2, tile_manager);
					SetConsoleWindowSize(80, 25); 
					//stopMusic(sound->Battle);
					//loadMusic_Menu(sound, "..\\CollisionDetectionTest\\Musics\\MainMenuMusic.ogg");
					//playMusic_Menu(sound);
					MainMenuStageCreate(sound);
					break;
				case '3': testStage_Free(testStage, player_1, player_2, enemyContainer_P1, enemyContainer_P2, enemyBullet, enemy_AI_P1, enemy_AI_P2,
					powerup, ball_1, ball_2, tile_manager);
					Sound_Free(&sound);
					exit(0);
					break;
				}

			}

			/* Spawn powerups from first enemy */
			if (input == 'u')
			{
				int k;

				for (k = 0; k < 10; k++)
				{
					if (powerup[k] == NULL)
					{
						powerup[k] = Powerup_Create(enemyContainer_P2[0], ball_2);
						break;
					}
				}

			}
			/* Test reset ball to player 1 */
			if (input == '3')
			{

				TileMapManager_Ball(ball_1, tile_manager, 0);
				ball_1->ball_fired_flag = 0;
				ball_1->balldirection = -1;
				Ball_MoveBallWithPlayer(ball_1, tile_manager);
				TileMapManager_Ball(ball_1, tile_manager, 4);


			}
			/* Test reset ball to player 2 */
			if (input == '4')
			{
				TileMapManager_Ball(ball_2, tile_manager, 0);
				ball_2->ball_fired_flag = 0;
				ball_2->balldirection = -1;

				Ball_MoveBallWithPlayer(ball_2, tile_manager);
				TileMapManager_Ball(ball_2, tile_manager, 5);

			}

			/* Test enemy bullet */
			if (input == 'l')
			{
				for (i = 0; i < 50; ++i)
				{
					if (enemyBullet[i] == NULL)
					{
						enemyBullet[i] = EnemyBullet_Create(sound, enemyContainer_P1[rand() % 9], 0);
						break;
					}
				}
			}
		}

		/* Win Condition */
		/*if (player_1->health == 0 || player_2->health == 0)
		break;*/

	}
	/*if (player_1->health == 0)
	{
	WorldData_GoToXY(28, 36);
	printf("PLAYER 2 WINS!");
	}
	if (player_2->health == 0)
	{
	WorldData_GoToXY(28, 36);
	printf("PLAYER 1 WINS!");
	}*/

	Sleep(1000);
	select = _getch();
	SetConsoleWindowSize(80, 25);
	testStage_Free(testStage, player_1, player_2, enemyContainer_P1, enemyContainer_P2, enemyBullet, enemy_AI_P1, enemy_AI_P2,
		powerup, ball_1, ball_2, tile_manager);
	//stopMusic(sound->Battle);
	//loadMusic_Menu(sound, "..\\CollisionDetectionTest\\Musics\\MainMenuMusic.ogg");
	//playMusic_Menu(sound);
	MainMenuStageCreate(sound);
}