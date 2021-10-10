/******************************************************************************
All content © 2017 Digipen Institute of Technology Singapore, all rights reserved

filename			Stage.c

authors				Ong Tze Ang(30%)
					Lois Ngm(30%)
					Wong ZhiHao(10%)
					Audry May dela Paz Enriquez(30%)
DP emails			ong.tzeang
					lois.ngm
					wong.zhihao
					a.enriquez

Brief Description:
Functions to draw and clear stage data

******************************************************************************/

/* Load Libraries */
#include "stdafx.h"
#include <windows.h>			/* COORD, HANDLE etc */
#include <stdio.h>				/* Printf */
#include <time.h>				/* Delta Time and current time */
#include "Player.h"				/* Player object and data */
#include "Enemy.h"				/* Enemy object and data */
#include "Stage.h"				/* Stage object and data */
#include "EnemyAI.h"			/* Enemy AI object and data */
#include "FMODInit.h"			/* Sound object and data */
#include "Ball.h"				/* Ball object and data */
#include "TileMapManager.h"		/* Tile manager object and data */
#include "Powerups.h"			/* Powerup object and data */
#include "EnemyTypes.h"			/* Enemy Types objects and data */
#include "EnemyBullets.h"		/* Enemy Bullets objects and data */
#include "GameStateMainMenu.h"	/* Main Menu functions */
#include "WorldData.h"			/*WorldData_GoToXY*/


/*Function that prints the pause menu*/
void optionmenu_Print(Stage *stage, int CoOp_stage_num, int PvP_stage_num)
{
	int x, y;	/*iteration counters*/

				/*Characters used to draw the menu borders*/
	const char side_borders = 186;
	const char player_borders = 205;
	const char topleft_corner = 201;
	const char topright_corner = 187;
	const char bottomleft_corner = 200;
	const char bottomright_corner = 188;


	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	/* Save current attributes */
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;

	/*When choosing to playing coop, set the text colors depending on the stage types*/
	if (stage->play_type == CoOp)
	{
		/*When chosen stage type is normal, print the option as White color */
		if (stage->stage_type == NORMAL)
		{
			SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
			WorldData_GoToXY(81, 29);
			printf("NORMAL  STAGE %d", CoOp_stage_num);
		}
		/*When chosen stage type is ice, print the option as cyan color */
		else if (stage->stage_type == ICE)
		{
			SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			WorldData_GoToXY(83, 29);
			printf("ICE STAGE %d", CoOp_stage_num);
		}
		/*When chosen stage type is fire, print the option as fire color */
		else if (stage->stage_type == FIRE)
		{
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
			WorldData_GoToXY(82, 29);
			printf("FIRE  STAGE %d", CoOp_stage_num);
		}

		/*Print the text Coop*/
		WorldData_GoToXY(86, 31);
		printf("Co-Op");
	}
	/*When choosing to playing PvP, set the text colors depending on the stage types*/
	else if (stage->play_type == PvP)
	{
		/*When choosen stage type is normal, set the color of the option as White color */
		if (stage->stage_type == NORMAL)
			SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
		/*When choosen stage type is ice, set the color of the option as cyan color */
		else if (stage->stage_type == ICE)
			SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		/*When choosen stage type is fire,set the color of the option as fire color */
		else if (stage->stage_type == FIRE)
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);

		/*Prints the different stage level options*/
		WorldData_GoToXY(85, 29);
		printf("STAGE %d", PvP_stage_num);
		/*Print the text PvP*/
		WorldData_GoToXY(87, 31);
		printf("PvP");
	}

	/*Sets the color of the pause borders as white*/
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);

	/* Prints the pause menu left border */
	for (x = 79, y = 34; y < 43; y++)
	{
		WorldData_GoToXY(x, y);
		printf("%c", side_borders);
	}
	/*Prints the pause menu right border */
	for (x = 97, y = 34; y < 43; y++)
	{
		WorldData_GoToXY(x, y);
		printf("%c", side_borders);
	}
	/* Prints the pause menu top border */
	for (x = 80, y = 33; x < 97; x++)
	{
		WorldData_GoToXY(x, y);
		printf("%c", player_borders);
	}
	/* Prints the pause menu bottom border */
	for (x = 80, y = 43; x < 97; x++)
	{
		WorldData_GoToXY(x, y);
		printf("%c", player_borders);
	}

	/*Prints the corners of the pause menu borders*/
	WorldData_GoToXY(79, 33);
	printf("%c", topleft_corner);
	WorldData_GoToXY(79, 43);
	printf("%c", bottomleft_corner);
	WorldData_GoToXY(97, 33);
	printf("%c", topright_corner);
	WorldData_GoToXY(97, 43);
	printf("%c", bottomright_corner);

	/*Prints the text that shows the option of pause the menu borders*/
	WorldData_GoToXY(85, 35);
	printf("PAUSED");
	WorldData_GoToXY(82, 37);
	printf("1. Resume");
	WorldData_GoToXY(82, 39);
	printf("2. Main Menu");
	WorldData_GoToXY(82, 41);
	printf("3. Exit");
	SetConsoleTextAttribute(hConsole, saved_attributes);
}

/*Function that clear pause menu*/
void optionmenu_Clear()
{
	int x, y;/*iteration couners*/

			 /* Prints spaces over the pause menu Stage name */
	for (x = 81, y = 29; x < 97; ++x)
	{
		WorldData_GoToXY(x, y);
		printf(" ");
	}

	/* Prints spaces over the pause menu Stage name */
	for (x = 86, y = 31; x < 92; ++x)
	{
		WorldData_GoToXY(x, y);
		printf(" ");
	}

	/* Prints spaces over the left border of the pause menu*/
	for (x = 79, y = 34; y < 43; y++)
	{
		WorldData_GoToXY(x, y);
		printf(" ");
	}
	/*Prints spaces over the right border of the pause menu */
	for (x = 97, y = 34; y < 43; y++)
	{
		WorldData_GoToXY(x, y);
		printf(" ");
	}
	/* Prints spaces over the top border of the pause menu */
	for (x = 80, y = 33; x < 97; x++)
	{
		WorldData_GoToXY(x, y);
		printf(" ");
	}
	/*Prints spaces over the bottom border of the pause menu */
	for (x = 80, y = 43; x < 97; x++)
	{
		WorldData_GoToXY(x, y);
		printf(" ");
	}

	/*Prints over the text showing the options*/
	WorldData_GoToXY(79, 33);
	printf(" ");
	WorldData_GoToXY(79, 43);
	printf(" ");
	WorldData_GoToXY(97, 33);
	printf(" ");
	WorldData_GoToXY(97, 43);
	printf(" ");
	WorldData_GoToXY(85, 35);
	printf("      ");
	WorldData_GoToXY(82, 37);
	printf("         ");
	WorldData_GoToXY(82, 39);
	printf("            ");
	WorldData_GoToXY(82, 41);
	printf("       ");
}

/* Frees the stage data of this object */
void Stages_Free(Stage *stage, Player *player_1, Player *player_2, Enemy *enemyP1[], Enemy *enemyP2[], EnemyBullet *enemyBullet[],
	Enemy_AI *enemy_AI_P1, Enemy_AI *enemy_AI_P2, Powerup *powerup[], Ball *ball_1, Ball *ball_2, TileMapManager *tileManager)
{
	int i;/*iteration counter*/

		  /*Frees the player objects and data*/
	Player_Free(&player_1);
	Player_Free(&player_2);

	/*Frees all the enemy objects and data*/
	for (i = 0; i < stage->total_and_hydra; ++i)
	{
		Enemy_Free(&enemyP1[i]);
		Enemy_Free(&enemyP2[i]);
	}

	/*Frees all the powerup objects and data*/
	for (i = 0; i < 10; ++i)
	{
		Powerup_Free(&powerup[i]);
	}

	/*Frees all the Enemy Bullet objects and data*/
	for (i = 0; i < 50; ++i)
	{
		EnemyBullet_Free(&enemyBullet[i]);
	}

	/*Frees the Enemy AI objects and data*/
	Enemy_AI_Free(&enemy_AI_P1);
	Enemy_AI_Free(&enemy_AI_P2);

	/*Frees the Ball objects and data*/
	Ball_Free(&ball_1);
	Ball_Free(&ball_2);

	/*Frees the tileManager data and objects*/
	TileMapManager_Free(&tileManager);

	/*Frees the stafe data and objects*/
	Stage_Free(&stage);
}