/******************************************************************************
All content © 2017 Digipen Institute of Technology Singapore, all rights reserved

filename    GameStateMainMenu.c
author      Ong Tze Ang
DP email    tzeang.ong@digipen.edu
course      RTIS

Brief Description:
This file handles every GameState transitions and GameState designs.
******************************************************************************/

/* Load Libraries */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
/* Load Headers */
#include "Stage.h"
#include "Ball.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyTypes.h"
#include "EnemyBullets.h"
#include "GameInitialise.h"
#include "GameStateMainMenu.h"
#include "string.h"
#include "TileMapManager.h"
#include "FMODInit.h"
#include "fmod.h"
#include "WorldData.h"
#include "FireStage1.h"
#include "FireStage2.h"
#include "NormalStage1.h"
#include "NormalStage2.h"
#include "IceStage1.h"
#include "IceStage2.h"
#include "Player.h"
#include "Ball.h"
#include "Enemy.h"
#include "Highscore.h"
#include "Powerups.h"
#include "stdafx.h"

/* Definitions */
#define MAINMENU_BORDER_X 80
#define MAINMENU_BORDER_Y 25
#define TOPLEFT45 3
#define TOPRIGHT45 2
#define BOTTOMLEFT45 1
#define BOTTOMRIGHT45 0

/* ASCII ART */
static const char * DigiPenLogo[] = {
	"                                _______                  ________              ",
	"             /### /#######     |       \\                |        \\             ",
	"             /### /#########   |        \\  _          _ |   ___   |            ",
	"             /### /##########  |    |    ||_|        |_||         |            ",
	"             /### /########### |    |    | _   ____   _ |    ____/____   _____ ",
	"             /### /########### |    |    || | /    \\ | ||   |    /    \\ |     \\",
	"             /### /##########  |    |    || ||  __  || ||   |   |  --  ||     |",
	"             /### /#########   |        / | ||      || ||   |   |  ____||  |  |",
	"             /### /#######/    |_______/  |_| \\___  ||_||___|    \\____\\ |__|__|",
	"    /####### /### ///////                     /     |                          ",
	"  /######### /###                             \\____/                           ",
	" /########## /###                                                              ",
	"/########### /###                                                              ",
	"/########### /###      _____ ___   ___ __  _  __ ___ __ __       _     _  _    ",
	"//########## /### ||\\||_  | | | | | | |__ | ||__  | |__|  |_||\\|| ||  | || _|_|",
	" //######### /### || |__| | | | |_| | |__ |_||    | |__|__| || ||_||__|_||_| | ",
	"  ///####### /###                                                              ",
	"    ///////  ///                                                               ",
};
static const char *SxP[] = {
	"                                     ",
	"      _________      __________      ",
	"     /   _____/__  __\\______   \\     ",
	"     \\_____  \\\\  \\/  /|     ___/     ",
	"     /        \\>    < |    |         ",
	"    /_______  /__/\\_ \\|____|         ",
	"            \\/      \\/               "
};
static const char *NAO_What[] = {
	"     _______      _____   ________     __      __.__            __",
	"     \\      \\    /  _  \\  \\_____  \\   /  \\    /  \\  |__ _____ _/  |_",
	"     /   |   \\  /  /_\\  \\  /   |   \\  \\   \\/\\/   /  |  \\\\__  \\\\   __\\",
	"    /    |    \\/    |    \\/    |    \\  \\        /|   Y  \\/ __ \\|  |",
	"    \\____|__  /\\____|__  /\\_______  /   \\__/\\  / |___|  (____  /__|",
	"            \\/         \\/         \\/         \\/       \\/     \\/",

};
static const char *Monsterpedia[] = {
	" __  __  ___  _  _ ___ _____ ___ ___ ___ ___ ___ ___   _    ",
	"|  \\/  |/ _ \\| \\| / __|_   _| __| _ \\ _ \\ __|   \\_ _| /_\\   ",
	"| |\\/| | (_) | .` \\__ \\ | | | _||   /  _/ _|| |) | | / _ \\  ",
	"|_|  |_|\\___/|_|\\_|___/ |_| |___|_|_\\_| |___|___/___/_/ \\_\\ ",
};

/* Pause console */
void Wait(long waitTime)
{
	clock_t	wakeTime;
	wakeTime = waitTime + clock();
	while (wakeTime > clock());
}
/* Prints X indentation */
void PrintIndent(int column, const char *text)
{
	int i;
	for (i = 0; i < column; ++i)
		printf(" ");
	printf("%s\n", text);
}
/* Prints Player1 Controls at location*/
void PrintPlayer1Controls(int x, int y)
{
	WorldData_GoToXY(x, y);
	printf("Move: A D");
	WorldData_GoToXY(x, y + 1);
	printf("Fire: Spacebar");
	WorldData_GoToXY(x, y + 2);
	printf("Boost: W");
}
/* Prints Player2 Controls at location*/
void PrintPlayer2Controls(int x, int y)
{
	WorldData_GoToXY(x, y);
	printf("Move: <-- -->");
	WorldData_GoToXY(x, y + 1);
	printf("Fire: Enter");
	WorldData_GoToXY(x, y + 2);
	printf("Boost: ^");
}
/* Prints Main Menu */
void MainMenu_Print()
{
	/* Colour setup */
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;
	/* Paint yellow */
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	WorldData_GoToXY(10, 1);
	/* PrintSxPLogo */
	for (int i = 0; i < _countof(SxP); ++i)
		PrintIndent(23, SxP[i]);
	/* Prints selection options */
	printf("\n\n");
	PrintIndent(28, "1. PLAYER VERSUS PLAYER");
	printf("\n");
	PrintIndent(28, "2. CO-OP");
	printf("\n");
	PrintIndent(28, "3. HOW TO PLAY");
	printf("\n");
	PrintIndent(28, "4. MONSTERPEDIA");
	printf("\n");
	PrintIndent(28, "5. HIGHSCORE");
	printf("\n");
	PrintIndent(28, "6. CREDITS");
	printf("\n");
	PrintIndent(28, "7. QUIT");

	SetConsoleTextAttribute(hConsole, saved_attributes);
}
/* DigipenLogo animation */
void DigiPenLogo_Transition()
{
	int x, y;
	/* Colour setup */
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	/* Save current attributes */
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;
	/* Paint white */
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	/* Print DigipenLogo */
	for (x = 0, y = 0; y < _countof(DigiPenLogo); ++y)
	{
		char str1[100];
		WorldData_GoToXY(x, y);
		Wait(50);
		strcpy_s(str1, sizeof(str1), DigiPenLogo[y]);
		puts(str1);
		for (int i = 0; i < (signed)strlen(DigiPenLogo[y]); i++)
		{
			Wait(1);
			printf("%c", str1[i]);
		}
	}
	/* 2seconds */
	Wait(2000);
	/* Clear DigipenLogo */
	for (x = 0, y = 0; y < _countof(DigiPenLogo); ++y)
	{
		WorldData_GoToXY(x, y);
		Wait(50);
		for (int i = 0; i < (signed)strlen(DigiPenLogo[y]); i++)
		{
			Wait(1);
			printf(" ");
		}
	}
	SetConsoleTextAttribute(hConsole, saved_attributes);
}
/* TeamLogo animation */
void NAO_What_Transition()
{
	int x, y;
	/* Colour setup */
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	/* Save current attributes */
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;
	/* Paint Magenta */
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
	/* NAO_WhatLogo */
	for (x = 5, y = 0; y < _countof(NAO_What); ++y)
	{
		WorldData_GoToXY(x, y + 9);
		Wait(50);
		printf("%s\n", NAO_What[y]);
	}
	SetConsoleTextAttribute(hConsole, saved_attributes);
	/* 2seconds */
	Wait(2000);
}
/* Animates screen bottom to top */
void ScreenTransition(int x)
{
	int i;
	for (i = 0; i < x; printf("\n"), Wait(100), ++i);
}
/* SxPLogo animation */
void SxP_Transition()
{
	/* Colour setup */
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;
	/* Paint Cyan */
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	/* Print SxPLogo */
	for (int i = 0; i < _countof(SxP); ++i)
		PrintIndent(23, SxP[i]);
	SetConsoleTextAttribute(hConsole, saved_attributes);
	ScreenTransition(15);
}
/* GameState Init */
void GameStateInit()
{
	/* Sound&Stage setup */
	Sound *FMOD = NULL;
	Stage *stage_menu = NULL;
	FMOD = initialiseFMOD();
	/* Plays MainMenuMusic */
	loadMusic_Menu(FMOD, "..\\CollisionDetectionTest\\Musics\\MainMenuMusic.ogg");
	playMusic_Menu(FMOD);
	hidecursor();
	/* Console size/font/fontsize setup */
	SetConsoleWindowSize(80, 25);
	SetConsoleWindowPos();
	ConsoleFontControl();
	/* Transitions */
	DigiPenLogo_Transition();
	system("cls");
	NAO_What_Transition();
	system("cls");
	WorldData_GoToXY(10, 20);
	SxP_Transition();
	system("cls");
	MainMenu(stage_menu, FMOD);
}
/* Print selection options */
void PrintOptions(int x, int y)
{
	WorldData_GoToXY(x, y);
	printf("1. Next");
	WorldData_GoToXY(x, y + 2);
	printf("2. Back");
}
/* StageCreate buffer */
void MainMenuStageCreate(Sound *sound)
{
	Stage *stage_menu = NULL;

	MainMenu(stage_menu, sound);
}
/* MainMenu Control */
void MainMenu(Stage *stage_menu, Sound *sound)
{
	int select, i = 1;

	/* Object Declartion */
	time_t current_time = clock();
	TileMapManager *tile_manager = NULL;
	Ball *ball_menu = NULL;
	/* Colour setup */
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;
	hidecursor();
	system("cls");
	/* Restart randomisor */
	srand((unsigned)clock());
	/* Ball&Tilemanager creation */
	ball_menu = Ball_Create(stage_menu, 3, 'O', (rand() % 20) + 1, (rand() % 22) + 1, 1, 0, 0);
	tile_manager = TileMapManagerMainMenu_Create();
	if (stage_menu == NULL)
		stage_menu = Stage_Create(0, 0, 3, 0, 80, 25, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 22, 0);
	/* Print MainMenu */
	MainMenu_Print();
	Stage_MainMenu_Print(stage_menu);
	/* Ball setup */
	ball_menu->balldirection = rand() % 4;
	ball_menu->ball_fired_flag = 1;
	ball_menu->base_timer = -20;
	stage_menu->current_time = clock();
	/* MainMenu Ball updater */
	while (i++)
	{
		/* Delta Time implementation */
		stage_menu->delta_time = clock() - stage_menu->current_time;
		stage_menu->current_time = clock();
		/* Cyan and Yellow colour rotation */
		if (i % 2 == 0)
			SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
		else
			SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		/* Ball movement */
		Ball_Update(sound, ball_menu, tile_manager, stage_menu, NULL, NULL, NULL, NULL, NULL, NULL);
		/* Refreshes Tile map*/
		TileMapManagerMenu_Update(tile_manager);
		/* Sound  update */
		updateFMOD(sound);
		/* Exit ball updater when user input selection options */
		if (GetAsyncKeyState('1') || GetAsyncKeyState('2') || GetAsyncKeyState('3') || GetAsyncKeyState('4') || GetAsyncKeyState('5') || GetAsyncKeyState('6') || GetAsyncKeyState('7'))
		{
			while (GetAsyncKeyState('1'));
			while (GetAsyncKeyState('2'));
			while (GetAsyncKeyState('3'));
			while (GetAsyncKeyState('4'));
			while (GetAsyncKeyState('5'));
			while (GetAsyncKeyState('6'));
			while (GetAsyncKeyState('7'));
			/* .2s buffer time */
			if (clock() - current_time >= 200)
				break;
		}
	}
	SetConsoleTextAttribute(hConsole, saved_attributes);
	/* Reads in user input */
	select = _getch();
	while (select)
	{
		if (select == '1' || select == '2' || select == '3' || select == '4' || select == '5' || select == '6' || select == '7' || select == 't')
		{
			loadSound(sound, "..\\CollisionDetectionTest\\SFX\\MenuSelect.wav");
			playSound_Medium(sound);
			break;
		}
		/* Prevents unwanted characters */
		printf("");
		select = _getch();
	}
	if (select != '1' && select != '2')
		system("cls");
	/* Selection to GameState change */
	switch (select)
	{
		/* PVP Menu */
	case '1': PVPStageSelectionMenu(stage_menu, ball_menu, sound);
		break;
		/* Coop Menu */
	case '2': CoOpTerrainSelectionMenu(stage_menu, ball_menu, sound);
		break;
		/* HowToPlay GameState */
	case '3': TileMapManager_Free(&tile_manager);
		Ball_Free(&ball_menu);
		HowToPlayMenu(stage_menu, sound);
		break;
		/* Monsterpedia GameState */
	case '4': Ball_Free(&ball_menu); 
		MonsterpediaMenu(stage_menu, sound);
		break;
		/* Highscore GameState */
	case '5': TileMapManager_Free(&tile_manager);
		Ball_Free(&ball_menu);
		Highscore_Menu(stage_menu, sound);
		break;
		/* Credits GameState */
	case '6': Ball_Free(&ball_menu); 
		CreditsMenu(stage_menu, sound);
		break;
		/* Exit Game */
	case '7': MainMenu_Free(stage_menu, ball_menu, tile_manager);
		Sound_Free(&sound);
		exit(0);
		break;
		/* Debugging stage */
	case 't': Ball_Free(&ball_menu);
		SetConsoleWindowSize(210, 80);
		testStage(sound, 1);
		break;
	}

}

/* Prints the menu for selecting a stage in PvP Mode */
void PVPStageSelectionMenu(Stage* stage_menu, Ball* ball_menu, Sound* sound)
{
	int select, i = 1;
	/* Object Declaration */
	TileMapManager *tile_manager = NULL;
	time_t current_time = clock();
	/* Colour setup */
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;
	/* Tilemap Creation*/
	tile_manager = TileMapManagerPvPStage_Create();
	/* Paint Yellow */
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	/* Prints selection */
	WorldData_GoToXY(53, 8);
	printf("Stages");
	WorldData_GoToXY(53, 10);
	printf("1. Stage 1");
	WorldData_GoToXY(53, 12);
	printf("2. Stage 2");
	WorldData_GoToXY(53, 14);
	printf("3. Stage 3");
	WorldData_GoToXY(53, 16);
	printf("4. Stage 4");
	WorldData_GoToXY(53, 18);
	printf("5. Back");

	SetConsoleTextAttribute(hConsole, saved_attributes);
	/* MainMenu Ball updater */
	while (i++)
	{
		/* Delta time implementation */
		stage_menu->delta_time = clock() - stage_menu->current_time;
		stage_menu->current_time = clock();
		/* Cyan & Yellow colour rotation */
		if (i % 2 == 0)
			SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
		else
			SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		/* Ball movement */
		Ball_Update(sound, ball_menu, tile_manager, stage_menu, NULL, NULL, NULL, NULL, NULL, NULL);
		/* Tilemap refresh */
		TileMapManagerPvPStage_Update(tile_manager);
		/* Sound update */
		updateFMOD(sound);
		/* Exit ball updater when user input selection options */
		if (GetAsyncKeyState('1') || GetAsyncKeyState('2') || GetAsyncKeyState('3') || GetAsyncKeyState('4') || GetAsyncKeyState('5'))
		{
			while (GetAsyncKeyState('1'));
			while (GetAsyncKeyState('2'));
			while (GetAsyncKeyState('3'));
			while (GetAsyncKeyState('4'));
			while (GetAsyncKeyState('5'));
			/* .2s buffer time */
			if (clock() - current_time >= 200)
				break;
		}
	}
	SetConsoleTextAttribute(hConsole, saved_attributes);
	/* Reads user input */
	select = _getch();
	while (select)
	{
		if (select == '1' || select == '2' || select == '3' || select == '4' || select == '5')
		{
			loadSound(sound, "..\\CollisionDetectionTest\\SFX\\MenuSelect.wav");
			playSound_Medium(sound);
			break;
		}
		/* Prevents unwanted input */
		printf("");
		select = _getch();
	}
	/* Selection to GameState change */
	switch (select)
	{
		/* PVP Stages menu */
	case '1': PVPTerrainSelectionMenu(stage_menu, ball_menu, sound, 1);
		break;
	case '2': PVPTerrainSelectionMenu(stage_menu, ball_menu, sound, 2);
		break;
	case '3': PVPTerrainSelectionMenu(stage_menu, ball_menu, sound, 3);
		break;
	case '4': PVPTerrainSelectionMenu(stage_menu, ball_menu, sound, 4);
		break;
		/* Back */
	case '5': TileMapManager_Free(&tile_manager);
		MainMenu(stage_menu, sound);
		break;
	}
}

/* Prints the menu for selecting a terrain in PvP Mode */
void PVPTerrainSelectionMenu(Stage* stage_menu, Ball* ball_menu, Sound* sound, int stage)
{
	int select, i = 1;
	/* Object Declartion*/
	TileMapManager *tile_manager = NULL;
	time_t current_time = clock();
	/* Colour Setup */
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;
	/* Tile map Creation*/
	tile_manager = TileMapManagerPvPTerrain_Create();
	/* Yellow */
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	WorldData_GoToXY(66, 8);
	printf("Terrains");
	/* White */
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	WorldData_GoToXY(66, 10);
	printf("1. Normal");
	/* Cyan */
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	WorldData_GoToXY(66, 12);
	printf("2. Ice");
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
	/* Red */
	WorldData_GoToXY(66, 14);
	printf("3. Fire");
	/* Yellow */
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	WorldData_GoToXY(66, 16);
	printf("4. Back");
	SetConsoleTextAttribute(hConsole, saved_attributes);
	/* Ballupdater */
	while (i++)
	{
		/* Delta time implementaiton*/
		stage_menu->delta_time = clock() - stage_menu->current_time;
		stage_menu->current_time = clock();
		/* Cyan and yellow colour rotation */
		if (i % 2 == 0)
			SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
		else
			SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		/* Ball movement*/
		Ball_Update(sound, ball_menu, tile_manager, stage_menu, NULL, NULL, NULL, NULL, NULL, NULL);
		/* Refreshes tilemap*/
		TileMapManagerPvPTerrain_Update(tile_manager);
		/* sound udpate */
		updateFMOD(sound);
		/* Exit loop on user input */
		if (GetAsyncKeyState('1') || GetAsyncKeyState('2') || GetAsyncKeyState('3') || GetAsyncKeyState('4'))
		{
			while (GetAsyncKeyState('1'));
			while (GetAsyncKeyState('2'));
			while (GetAsyncKeyState('3'));
			while (GetAsyncKeyState('4'));
			/* .2s buffer time */
			if (clock() - current_time >= 200)
				break;
		}
	}
	SetConsoleTextAttribute(hConsole, saved_attributes);
	/* user input */
	select = _getch();
	while (select)
	{
		if (select == '1' || select == '2' || select == '3' || select == '4')
		{
			loadSound(sound, "..\\CollisionDetectionTest\\SFX\\MenuSelect.wav");
			playSound_Medium(sound);
			break;
		}
		/* prevent unwatned char */
		printf("");
		select = _getch();
	}
	system("cls");
	/* PVP GAME STATE CHANGE */
	switch (select)
	{
		/* Normal stage */
	case '1':
		switch (stage)
		{
			/* Normal stage 1 pregamestate change setup */
		case 1: MainMenu_Free(stage_menu, ball_menu, tile_manager);
			SetConsoleWindowSize(80, 70);
			stopMusic(sound->MainMenu);
			WorldData_GoToXY(33, 35);
			printf("Normal Stage 1");
			WorldData_GoToXY(38, 37);
			printf("PvP");
			Sleep(2000);
			system("cls");
			WorldData_GoToXY(37, 35);
			printf("Ready?");
			Sleep(1000);
			system("cls");
			SetConsoleWindowSize(107, 80);
			loadMusic_Battle(sound, "..\\CollisionDetectionTest\\Musics\\CombatMusic.ogg");
			playMusic_Battle(sound);
			iceStage_1(sound, PvP, NORMAL);
			break;
			/* Normal stage 2 pregamestate change setup */
		case 2: MainMenu_Free(stage_menu, ball_menu, tile_manager);
			SetConsoleWindowSize(80, 70);
			stopMusic(sound->MainMenu);
			WorldData_GoToXY(33, 35);
			printf("Normal Stage 2");
			WorldData_GoToXY(38, 37);
			printf("PvP");
			Sleep(2000);
			system("cls");
			WorldData_GoToXY(37, 35);
			printf("Ready?");
			Sleep(1000);
			system("cls");
			SetConsoleWindowSize(107, 80);
			loadMusic_Battle(sound, "..\\CollisionDetectionTest\\Musics\\CombatMusic.ogg");
			playMusic_Battle(sound);
			iceStage_2(sound, PvP, NORMAL);
			break;
			/* Normal stage 3 pregamestate change setup */
		case 3: MainMenu_Free(stage_menu, ball_menu, tile_manager);
			SetConsoleWindowSize(80, 70);
			stopMusic(sound->MainMenu);
			WorldData_GoToXY(33, 35);
			printf("Normal Stage 3");
			WorldData_GoToXY(38, 37);
			printf("PvP");
			Sleep(2000);
			system("cls");
			WorldData_GoToXY(37, 35);
			printf("Ready?");
			Sleep(1000);
			system("cls");
			SetConsoleWindowSize(107, 80);
			loadMusic_Battle(sound, "..\\CollisionDetectionTest\\Musics\\CombatMusic.ogg");
			playMusic_Battle(sound);
			fireStage_1(sound, PvP, NORMAL);
			break;
			/* Normal stage 4 pregamestate change setup */
		case 4: MainMenu_Free(stage_menu, ball_menu, tile_manager);
			SetConsoleWindowSize(80, 70);
			stopMusic(sound->MainMenu);
			WorldData_GoToXY(33, 35);
			printf("Normal Stage 4");
			WorldData_GoToXY(38, 37);
			printf("PvP");
			Sleep(2000);
			system("cls");
			WorldData_GoToXY(37, 35);
			printf("Ready?");
			Sleep(1000);
			system("cls");
			SetConsoleWindowSize(107, 80);
			loadMusic_Battle(sound, "..\\CollisionDetectionTest\\Musics\\CombatMusic.ogg");
			playMusic_Battle(sound);
			fireStage_2(sound, PvP, NORMAL);
			break;
		}
		break;
		/* Ice Stage */
	case '2':
		switch (stage)
		{
			/* Ice stage 1 pregamestate change setup */
		case 1: MainMenu_Free(stage_menu, ball_menu, tile_manager);
			SetConsoleWindowSize(80, 70);
			stopMusic(sound->MainMenu);
			WorldData_GoToXY(33, 35);
			SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			printf("Ice Stage 1");
			SetConsoleTextAttribute(hConsole, saved_attributes);
			WorldData_GoToXY(37, 37);
			printf("PvP");
			Sleep(2000);
			system("cls");
			WorldData_GoToXY(37, 35);
			printf("Ready?");
			Sleep(1000);
			system("cls");
			SetConsoleWindowSize(107, 80);
			loadMusic_Battle(sound, "..\\CollisionDetectionTest\\Musics\\CombatMusic.ogg");
			playMusic_Battle(sound);
			iceStage_1(sound, PvP, ICE);
			break;
			/* Ice stage 2 pregamestate change setup */
		case 2: MainMenu_Free(stage_menu, ball_menu, tile_manager);
			SetConsoleWindowSize(80, 70);
			stopMusic(sound->MainMenu);
			WorldData_GoToXY(33, 35);
			SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			printf("Ice Stage 2");
			SetConsoleTextAttribute(hConsole, saved_attributes);
			WorldData_GoToXY(37, 37);
			printf("PvP");
			Sleep(2000);
			system("cls");
			WorldData_GoToXY(37, 35);
			printf("Ready?");
			Sleep(1000);
			system("cls");
			SetConsoleWindowSize(107, 80);
			loadMusic_Battle(sound, "..\\CollisionDetectionTest\\Musics\\CombatMusic.ogg");
			playMusic_Battle(sound);
			iceStage_2(sound, PvP, ICE);
			break;
			/* Ice stage 3 pregamestate change setup */
		case 3: MainMenu_Free(stage_menu, ball_menu, tile_manager);
			SetConsoleWindowSize(80, 70);
			stopMusic(sound->MainMenu);
			WorldData_GoToXY(33, 35);
			SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			printf("Ice Stage 3");
			SetConsoleTextAttribute(hConsole, saved_attributes);
			WorldData_GoToXY(37, 37);
			printf("PvP");
			Sleep(2000);
			system("cls");
			WorldData_GoToXY(37, 35);
			printf("Ready?");
			Sleep(1000);
			system("cls");
			SetConsoleWindowSize(107, 80);
			loadMusic_Battle(sound, "..\\CollisionDetectionTest\\Musics\\CombatMusic.ogg");
			playMusic_Battle(sound);
			fireStage_1(sound, PvP, ICE);
			break;
			/* Ice stage 4 pregamestate change setup */
		case 4: MainMenu_Free(stage_menu, ball_menu, tile_manager);
			SetConsoleWindowSize(80, 70);
			stopMusic(sound->MainMenu);
			WorldData_GoToXY(33, 35);
			SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			printf("Ice Stage 4");
			SetConsoleTextAttribute(hConsole, saved_attributes);
			WorldData_GoToXY(37, 37);
			printf("PvP");
			Sleep(2000);
			system("cls");
			WorldData_GoToXY(37, 35);
			printf("Ready?");
			Sleep(1000);
			system("cls");
			SetConsoleWindowSize(107, 80);
			loadMusic_Battle(sound, "..\\CollisionDetectionTest\\Musics\\CombatMusic.ogg");
			playMusic_Battle(sound);
			fireStage_2(sound, PvP, ICE);
			break;
		}
		break;
		/* Fire Stage */
	case '3':
		switch (stage)
		{
			/* Fire stage 1 pregamestate change setup */
		case 1: MainMenu_Free(stage_menu, ball_menu, tile_manager);
			SetConsoleWindowSize(80, 70);
			stopMusic(sound->MainMenu);
			WorldData_GoToXY(33, 35);
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
			printf("Fire Stage 1");
			SetConsoleTextAttribute(hConsole, saved_attributes);
			WorldData_GoToXY(37, 37);
			printf("PvP");
			Sleep(2000);
			system("cls");
			WorldData_GoToXY(37, 35);
			printf("Ready?");
			Sleep(1000);
			system("cls");
			SetConsoleWindowSize(107, 80);
			loadMusic_Battle(sound, "..\\CollisionDetectionTest\\Musics\\CombatMusic.ogg");
			playMusic_Battle(sound);
			iceStage_1(sound, PvP, FIRE);
			break;
			/* Fire stage 2 pregamestate change setup */
		case 2: MainMenu_Free(stage_menu, ball_menu, tile_manager);
			SetConsoleWindowSize(80, 70);
			stopMusic(sound->MainMenu);
			WorldData_GoToXY(33, 35);
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
			printf("Fire Stage 2");
			SetConsoleTextAttribute(hConsole, saved_attributes);
			WorldData_GoToXY(37, 37);
			printf("PvP");
			Sleep(2000);
			system("cls");
			WorldData_GoToXY(37, 35);
			printf("Ready?");
			Sleep(1000);
			system("cls");
			SetConsoleWindowSize(107, 80);
			loadMusic_Battle(sound, "..\\CollisionDetectionTest\\Musics\\CombatMusic.ogg");
			playMusic_Battle(sound);
			iceStage_2(sound, PvP, FIRE);
			break;
			/* Fire stage 3 pregamestate change setup */
		case 3: MainMenu_Free(stage_menu, ball_menu, tile_manager);
			SetConsoleWindowSize(80, 70);
			stopMusic(sound->MainMenu);
			WorldData_GoToXY(33, 35);
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
			printf("Fire Stage 3");
			SetConsoleTextAttribute(hConsole, saved_attributes);
			WorldData_GoToXY(37, 37);
			printf("PvP");
			Sleep(2000);
			system("cls");
			WorldData_GoToXY(37, 35);
			printf("Ready?");
			Sleep(1000);
			system("cls");
			SetConsoleWindowSize(107, 80);
			loadMusic_Battle(sound, "..\\CollisionDetectionTest\\Musics\\CombatMusic.ogg");
			playMusic_Battle(sound);
			fireStage_1(sound, PvP, FIRE);
			break;
			/* Fire stage 4 pregamestate change setup */
		case 4: MainMenu_Free(stage_menu, ball_menu, tile_manager);
			SetConsoleWindowSize(80, 70);
			stopMusic(sound->MainMenu);
			WorldData_GoToXY(33, 35);
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
			printf("Fire Stage 4");
			SetConsoleTextAttribute(hConsole, saved_attributes);
			WorldData_GoToXY(37, 37);
			printf("PvP");
			Sleep(2000);
			system("cls");
			WorldData_GoToXY(37, 35);
			printf("Ready?");
			Sleep(1000);
			system("cls");
			SetConsoleWindowSize(107, 80);
			loadMusic_Battle(sound, "..\\CollisionDetectionTest\\Musics\\CombatMusic.ogg");
			playMusic_Battle(sound);
			fireStage_2(sound, PvP, FIRE);
			break;
		}
		break;
		/* Back */
	case '4': TileMapManager_Free(&tile_manager);
		MainMenu_Print();
		Stage_MainMenu_Print(stage_menu);
		PVPStageSelectionMenu(stage_menu, ball_menu, sound);
		break;
	}
}
/* Prints the menu for selecting a terrain in CoOp Mode */
void CoOpTerrainSelectionMenu(Stage* stage_menu, Ball* ball_menu, Sound* sound)
{
	int select, i = 1;
	/* Object declaration*/
	TileMapManager *tile_manager = NULL;
	time_t current_time = clock();
	/* Colour setup */
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;
	/* Tilemap Creation*/
	tile_manager = TileMapManagerCoOpMenu_Create();
	/* Yellow */
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	WorldData_GoToXY(53, 8);
	printf("Terrain");
	/* White */
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	WorldData_GoToXY(53, 10);
	printf("1. Normal 1");
	WorldData_GoToXY(53, 12);
	printf("2. Normal 2");
	WorldData_GoToXY(53, 14);
	/* Cyan */
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	printf("3. Ice 1");
	WorldData_GoToXY(53, 16);
	printf("4. Ice 2");
	WorldData_GoToXY(53, 18);
	/* Red */
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("5. Fire 1");
	WorldData_GoToXY(53, 20);
	printf("6. Fire 2");
	/* Yellow */
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	WorldData_GoToXY(53, 22);
	printf("7. Back");
	SetConsoleTextAttribute(hConsole, saved_attributes);
	/* Ball updater */
	while (i++)
	{
		/* Delta time implementation */
		stage_menu->delta_time = clock() - stage_menu->current_time;
		stage_menu->current_time = clock();
		/* Cyan & Yellow colour rotation */
		if (i % 2 == 0)
			SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
		else
			SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		/* Ball movement */
		Ball_Update(sound, ball_menu, tile_manager, stage_menu, NULL, NULL, NULL, NULL, NULL, NULL);
		/* Tilemap refresher*/
		TileMapManagerCoOpMenu_Update(tile_manager);
		/* sound update */
		updateFMOD(sound);
		/* Exit loop on user input */
		if (GetAsyncKeyState('1') || GetAsyncKeyState('2') || GetAsyncKeyState('3') || GetAsyncKeyState('4') || GetAsyncKeyState('5') || GetAsyncKeyState('6') || GetAsyncKeyState('7'))
		{
			while (GetAsyncKeyState('1'));
			while (GetAsyncKeyState('2'));
			while (GetAsyncKeyState('3'));
			while (GetAsyncKeyState('4'));
			while (GetAsyncKeyState('5'));
			while (GetAsyncKeyState('6'));
			while (GetAsyncKeyState('7'));
			/* .2s buffer */
			if (clock() - current_time >= 200)
				break;
		}
	}
	SetConsoleTextAttribute(hConsole, saved_attributes);
	/* User input */
	select = _getch();
	while (select)
	{
		if (select == '1' || select == '2' || select == '3' || select == '4' || select == '5' || select == '6' || select == '7')
		{
			loadSound(sound, "..\\CollisionDetectionTest\\SFX\\MenuSelect.wav");
			playSound_Medium(sound);
			break;
		}
		/* Prevent unwanted char */
		printf("");
		select = _getch();
	}
	system("cls");
	/* CoOp GameState change*/
	switch (select)
	{
		/* Normal stage 1 */
	case '1': MainMenu_Free(stage_menu, ball_menu, tile_manager);
		SetConsoleWindowSize(80, 70);
		stopMusic(sound->MainMenu);
		WorldData_GoToXY(33, 35);
		printf("Normal Stage 1");
		WorldData_GoToXY(37, 37);
		printf("Co-Op");
		Sleep(2000);
		system("cls");
		WorldData_GoToXY(37, 35);
		printf("Ready?");
		Sleep(1000);
		system("cls");
		SetConsoleWindowSize(107, 80);
		loadMusic_Battle(sound, "..\\CollisionDetectionTest\\Musics\\CombatMusic.ogg");
		playMusic_Battle(sound);
		normalStage_1(sound, CoOp, NORMAL);
		break;
		/* Normal stage 2 */
	case '2': MainMenu_Free(stage_menu, ball_menu, tile_manager);
		stopMusic(sound->MainMenu);
		SetConsoleWindowSize(80, 70);
		WorldData_GoToXY(33, 35);
		printf("Normal Stage 2");
		WorldData_GoToXY(37, 37);
		printf("Co-Op");
		Sleep(2000);
		system("cls");
		WorldData_GoToXY(37, 35);
		printf("Ready?");
		Sleep(1000);
		system("cls");
		SetConsoleWindowSize(107, 80);
		loadMusic_Battle(sound, "..\\CollisionDetectionTest\\Musics\\CombatMusic.ogg");
		playMusic_Battle(sound);
		normalStage_2(sound, CoOp, NORMAL);
		break;
		/* Ice stage 1 */
	case '3': MainMenu_Free(stage_menu, ball_menu, tile_manager);
		stopMusic(sound->MainMenu);
		SetConsoleWindowSize(80, 70);
		WorldData_GoToXY(33, 35);
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		printf("Ice Stage 1");
		SetConsoleTextAttribute(hConsole, saved_attributes);
		WorldData_GoToXY(36, 37);
		printf("Co-Op");
		Sleep(2000);
		system("cls");
		WorldData_GoToXY(37, 35);
		printf("Ready?");
		Sleep(1000);
		system("cls");
		SetConsoleWindowSize(107, 80);
		loadMusic_Battle(sound, "..\\CollisionDetectionTest\\Musics\\CombatMusic.ogg");
		playMusic_Battle(sound);
		iceStage_1(sound, CoOp, ICE);
		break;
		/* Ice stage 2 */
	case '4': MainMenu_Free(stage_menu, ball_menu, tile_manager);
		stopMusic(sound->MainMenu);
		SetConsoleWindowSize(80, 70);
		WorldData_GoToXY(33, 35);
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		printf("Ice Stage 2");
		SetConsoleTextAttribute(hConsole, saved_attributes);
		WorldData_GoToXY(36, 37);
		printf("Co-Op");
		Sleep(2000);
		system("cls");
		WorldData_GoToXY(37, 35);
		printf("Ready?");
		Sleep(1000);
		system("cls");
		SetConsoleWindowSize(107, 80);
		loadMusic_Battle(sound, "..\\CollisionDetectionTest\\Musics\\CombatMusic.ogg");
		playMusic_Battle(sound);
		iceStage_2(sound, CoOp, ICE);
		break;
		/* Fire stage 1 */
	case '5': MainMenu_Free(stage_menu, ball_menu, tile_manager);
		stopMusic(sound->MainMenu);
		SetConsoleWindowSize(80, 70);
		WorldData_GoToXY(33, 35);
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
		printf("Fire Stage 1");
		SetConsoleTextAttribute(hConsole, saved_attributes);
		WorldData_GoToXY(36, 37);
		printf("Co-Op");
		Sleep(2000);
		system("cls");
		WorldData_GoToXY(37, 35);
		printf("Ready?");
		Sleep(1000);
		system("cls");
		SetConsoleWindowSize(107, 80);
		loadMusic_Battle(sound, "..\\CollisionDetectionTest\\Musics\\CombatMusic.ogg");
		playMusic_Battle(sound);
		fireStage_1(sound, CoOp, FIRE);
		break;
		/* Fire stage 2 */
	case '6': MainMenu_Free(stage_menu, ball_menu, tile_manager);
		stopMusic(sound->MainMenu);
		SetConsoleWindowSize(80, 70);
		WorldData_GoToXY(33, 35);
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
		printf("Fire Stage 2");
		SetConsoleTextAttribute(hConsole, saved_attributes);
		WorldData_GoToXY(36, 37);
		printf("Co-Op");
		Sleep(2000);
		system("cls");
		WorldData_GoToXY(37, 35);
		printf("Ready?");
		Sleep(1000);
		system("cls");
		SetConsoleWindowSize(107, 80);
		loadMusic_Battle(sound, "..\\CollisionDetectionTest\\Musics\\CombatMusic.ogg");
		playMusic_Battle(sound);
		fireStage_2(sound, CoOp, FIRE);
		break;
		/* Back */
	case '7': TileMapManager_Free(&tile_manager);
		MainMenu(stage_menu, sound);
		break;
	}
}
/* HowToPlay GameState */
void HowToPlayMenu(Stage *stage_menu, Sound *sound)
{
	/* variable declation */
	int select;
	int i, x, y;
	const char normalBullet = '|';
	const char leftBullet = '\\';
	const char rightBullet = '/';
	const char bigBullet_P1 = 239;
	const char bigBullet_P2 = 'U';
	const char peaBullet = 248;
	const char torpedoBullet = 254;
	/* Colour setup */
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	/* Print Player1 */
	for (i = 0, x = 26, y = 4; i < PLAYER_HEIGHT; ++i, ++y)
	{
		WorldData_GoToXY(x, y);
		printf("%s", player1[i]);
	}
	PrintPlayer1Controls(23, 7);
	/* Print Player2 */
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	for (i = 0, x = 49, y = 4; i < PLAYER_HEIGHT; ++i, ++y)
	{
		WorldData_GoToXY(x, y);
		printf("%s", player2[i]);
	}
	PrintPlayer2Controls(46, 7);	
	/* White */
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	WorldData_GoToXY(5, 11);
	printf("EACH PLAYER HAS A BALL, USE YOUR PEDDLE TO HIT AND BOUNCE THE BALL BACK!");
	WorldData_GoToXY(19, 13);
	printf("DO NOT GET HIT BY ENEMY FIRE!    %c %c %c %c %c %c %c", normalBullet, leftBullet, rightBullet, bigBullet_P1, bigBullet_P2, peaBullet, torpedoBullet);
	WorldData_GoToXY(20, 15);	
	printf("COLLECT POWERUPS, BUT BE CAREFUL OF DEBUFFS!");
	WorldData_GoToXY(30, 17);
	printf("GOOD LUCK HAVE FUN!");
	SetConsoleTextAttribute(hConsole, saved_attributes);
	/* Prints gamestate */
	Stage_MainMenu_Print(stage_menu);
	PrintOptions(36, 20);
	/* User input */
	select = _getch();
	while (select)
	{
		if (select == '1' || select == '2')
			break;
		/* Prevents unwanted char */
		printf("");
		select = _getch();
	}
	system("cls");
	/* Selection options select */
	switch (select)
	{
	case '1': 
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\MenuSelect.wav");
		playSound_Medium(sound);
		GamemodeMenu(stage_menu, sound);
		break;
	case '2': 
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\MenuSelect.wav");
		playSound_Medium(sound);
		MainMenu(stage_menu, sound);
		break;
	}
}
/* Game Mode GameState */
void GamemodeMenu(Stage *stage_menu, Sound *sound)
{
	int select;
	/* Colour setup */
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;
	/* Print Gamemode Text */
	WorldData_GoToXY(35, 3);
	printf("Game Mode");
	/* White */
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	WorldData_GoToXY(5, 6);
	printf("CO-OP: DO NOT MISS ANY BALLS! IT DAMAGES BOTH PLAYERS.");
	WorldData_GoToXY(5, 8);
	printf("       WORK TOGETHER AND CLEAR THE MONSTERS TO WIN!");
	WorldData_GoToXY(5, 10);
	printf("       A POWERUP DROPS THAT KILLS ENEMIES AFTER YOU'VE WHITTLED THEM DOWN!");
	WorldData_GoToXY(5, 12);
	printf("  TIP: SURVIVE TILL POWERUP DROPS");
	WorldData_GoToXY(5, 14);
	printf("  PVP: AVOID OPPONENTS BALL WHILE AIMING TO HIT YOUR OPPONENT!");
	WorldData_GoToXY(5, 16);
	printf("       COLLECT POWERUPS AND GAIN AN ADVANTAGE!");
	SetConsoleTextAttribute(hConsole, saved_attributes);
	/* Prints game state */
	Stage_MainMenu_Print(stage_menu);
	PrintOptions(36, 20);
	/* User input */
	select = _getch();
	while (select)
	{
		if (select == '1' || select == '2')
			break;
		/* prevents unwanted char */
		printf("");
		select = _getch();
	}
	system("cls");
	/* change gamestate */
	switch (select)
	{
	case '1':
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\MenuSelect.wav");
		playSound_Medium(sound);
		PowerupMenu(stage_menu, sound);
		break;
	case '2':
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\MenuSelect.wav");
		playSound_Medium(sound);
		HowToPlayMenu(stage_menu, sound);
		break;
	}
}
/* Powerup GameState */
void PowerupMenu(Stage *stage_menu, Sound *sound)
{
	/* Declartion */
	int select;
	const char RANDOM = '?';
	const char HP_BUFF = '+';
	const char HP_DEBUFF = 196;
	const char LENGTH_BUFF = 'L';
	const char LENGTH_DEBUFF = 'S';
	const char BALL_SPEED_BUFF = 232;
	const char BALL_SPEED_DEBUFF = 233;
	const char CONTROLS_DEBUFF = 145;
	const char PLAYER_SPEED_BUFF = 247;
	const char PLAYER_SPEED_DEBUFF = 174;
	const char BALL_SWAP = 228;
	const char KILL_ENEMY = 235;
	/* Colour setup */
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;

	WorldData_GoToXY(35, 1);
	printf("Powerup");
	/* Green & Red & Magenta*/
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	WorldData_GoToXY(5, 3);
	printf("%c : +1 to Health", HP_BUFF);
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
	WorldData_GoToXY(40, 3);
	printf("%c : -1 to Health", HP_DEBUFF);
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	WorldData_GoToXY(5, 5);
	printf("%c : Increases Paddle length", LENGTH_BUFF);
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
	WorldData_GoToXY(40, 5);
	printf("%c : Decreases Paddle length", LENGTH_DEBUFF);
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	WorldData_GoToXY(5, 7);
	printf("%c : Decreases Ball speed", BALL_SPEED_BUFF);
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
	WorldData_GoToXY(40, 7);
	printf("%c : Increases Ball speed", BALL_SPEED_DEBUFF);
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	WorldData_GoToXY(5, 9);
	printf("%c : Increases Player move speed", PLAYER_SPEED_BUFF);
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
	WorldData_GoToXY(40, 9);
	printf("%c : Decreases Player move speed", PLAYER_SPEED_DEBUFF);
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
	WorldData_GoToXY(40, 11);
	printf("%c : Temporarily swaps Player movements", CONTROLS_DEBUFF);
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	WorldData_GoToXY(25, 13);
	printf("%c : Swaps both Players' Balls positions", BALL_SWAP);
	WorldData_GoToXY(25, 15);
	printf("%c : Random powerup", RANDOM);
	WorldData_GoToXY(25, 17);
	printf("%c : Co-Op Mode only, kills a random enemy", KILL_ENEMY);

	SetConsoleTextAttribute(hConsole, saved_attributes);
	/* Print stage */
	Stage_MainMenu_Print(stage_menu);
	WorldData_GoToXY(36, 20);
	printf("1. Next");
	WorldData_GoToXY(36, 22);
	printf("2. Back");
	/* GameState change */
	select = _getch();
	while (select)
	{
		if (select == '1' || select == '2')
			break;

		printf("");
		select = _getch();
	}
	system("cls");
	switch (select)
	{
		/* Terrain gamestate */
	case '1': 
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\MenuSelect.wav");
		playSound_Medium(sound);
		TerrainMenu(stage_menu, sound);
		break;
		/* HowToPlay gamestate */
	case '2':
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\MenuSelect.wav");
		playSound_Medium(sound);
		HowToPlayMenu(stage_menu, sound);
		break;
	}
}
/* Terrain GameState */
void TerrainMenu(Stage *stage_menu, Sound *sound)
{
	int select;
	/* Colour setup */
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;

	WorldData_GoToXY(37, 3);
	printf("Terrain");
	WorldData_GoToXY(20, 7);
	/* Cyan and Red */
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	printf("Ice: Freezes both players for a duration");
	SetConsoleTextAttribute(hConsole, saved_attributes);
	WorldData_GoToXY(20, 9);
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("Fire: Speeds up enemies and enemy fire");
	SetConsoleTextAttribute(hConsole, saved_attributes);
	/* Print stage */
	Stage_MainMenu_Print(stage_menu);
	WorldData_GoToXY(36, 20);
	printf("1. Main Menu");
	WorldData_GoToXY(36, 22);
	printf("2. Back");
	/* User selection */
	select = _getch();
	while (select)
	{
		if (select == '1' || select == '2')
			break;

		printf("");
		select = _getch();
	}
	system("cls");
	switch (select)
	{
		/* MainMenu */
	case '1':
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\MenuSelect.wav");
		playSound_Medium(sound);
		MainMenu(stage_menu, sound);
		break;
		/* PowerupMenu */
	case '2':
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\MenuSelect.wav");
		playSound_Medium(sound);
		PowerupMenu(stage_menu, sound);
		break;
	}
}
/* Monsterpedia GameStates */
void MonsterpediaMenu(Stage *stage_menu, Sound *sound)
{
	int select;
	int x, y;
	/* COlour setup */
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;
	/* Yellow */
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	for (x = 10, y = 0; y < _countof(Monsterpedia); ++y)
	{
		WorldData_GoToXY(x, y + 9);
		Wait(50);
		printf("%s\n", Monsterpedia[y]);
	}
	SetConsoleTextAttribute(hConsole, saved_attributes);
	/* Stage print */
	Stage_MainMenu_Print(stage_menu);
	WorldData_GoToXY(36, 19);
	printf("1. Next page");
	WorldData_GoToXY(36, 21);
	printf("2. Exit Monsterpedia");
	/* User selection */
	select = _getch();
	while (select)
	{
		if (select == '1' || select == '2')
			break;

		printf("");
		select = _getch();
	}
	system("cls");
	switch (select)
	{
		/* BB Menu */
	case '1': 
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\MenuSelect.wav");
		playSound_Medium(sound);
		MonsterpediaBB(stage_menu, sound);
		break;
		/* Back */
	case '2': 
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\MenuSelect.wav");
		playSound_Medium(sound);
		MainMenu(stage_menu, sound);
		break;
	}
}
void MonsterpediaBB(Stage *stage_menu, Sound *sound)
{
	int select, i, x, y;
	/* Colour setup */
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;

	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;
	SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
	/* Print enemyBB */
	for (i = 0, x = 38, y = 5; i < ENEMY_HEIGHT; ++i, ++y)
	{
		WorldData_GoToXY(x, y);
		printf("%s", enemyBB[i]);
	}
	SetConsoleTextAttribute(hConsole, saved_attributes);

	WorldData_GoToXY(20, 11);
	printf("Name: BB");
	WorldData_GoToXY(20, 12);
	printf("Type: Non-shooters");
	WorldData_GoToXY(20, 13);
	printf("Health: 1");
	WorldData_GoToXY(20, 14);
	printf("Threat: None");
	WorldData_GoToXY(20, 15);
	printf("Description: Casual Wall made from paper.");
	WorldData_GoToXY(20, 16);
	printf("             Does nothing.");
	Stage_MainMenu_Print(stage_menu);
	WorldData_GoToXY(36, 19);
	printf("1. Next page");
	WorldData_GoToXY(36, 21);
	printf("2. Previous page");
	/* User selection */
	select = _getch();
	while (select)
	{
		if (select == '1' || select == '2')
			break;

		printf("");
		select = _getch();
	}
	system("cls");
	switch (select)
	{
		/* Wall menu */
	case '1': 
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\MenuSelect.wav");
		playSound_Medium(sound);
		MonsterpediaWall(stage_menu, sound);
		break;
		/* back */
	case '2': 
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\MenuSelect.wav");
		playSound_Medium(sound);
		MonsterpediaMenu(stage_menu, sound);
		break;
	}
}
void MonsterpediaWall(Stage *stage_menu, Sound *sound)
{
	int select, i, x, y;

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;
	SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);

	/* Print enemyWall_4 */
	for (i = 0, x = 28, y = 5; i < ENEMY_HEIGHT; ++i, ++y)
	{
		WorldData_GoToXY(x, y);
		printf("%s", enemyWall_4[i]);
	}
	/* Print enemyWall_3 */
	for (i = 0, x = 34, y = 5; i < ENEMY_HEIGHT; ++i, ++y)
	{
		WorldData_GoToXY(x, y);
		printf("%s", enemyWall_3[i]);
	}
	/* Print enemyWall_2 */
	for (i = 0, x = 40, y = 5; i < ENEMY_HEIGHT; ++i, ++y)
	{
		WorldData_GoToXY(x, y);
		printf("%s", enemyWall_2[i]);
	}
	/* Print enemyWall_1 */
	for (i = 0, x = 46, y = 5; i < ENEMY_HEIGHT; ++i, ++y)
	{
		WorldData_GoToXY(x, y);
		printf("%s", enemyWall_1[i]);
	}
	SetConsoleTextAttribute(hConsole, saved_attributes);

	WorldData_GoToXY(20, 11);
	printf("Name: Wall");
	WorldData_GoToXY(20, 12);
	printf("Type: Non-shooters");
	WorldData_GoToXY(20, 13);
	printf("Health: 4");
	WorldData_GoToXY(20, 14);
	printf("Threat: None unless you're a scrub");
	WorldData_GoToXY(20, 15);
	printf("Description: HARDCORE Wall made from bricks.");
	WorldData_GoToXY(20, 16);
	printf("             Does nothing.");

	Stage_MainMenu_Print(stage_menu);
	WorldData_GoToXY(36, 19);
	printf("1. Next page");
	WorldData_GoToXY(36, 21);
	printf("2. Previous page");
	select = _getch();
	while (select)
	{
		if (select == '1' || select == '2')
			break;

		printf("");
		select = _getch();
	}
	system("cls");
	switch (select)
	{
		/* Shooter menu*/
	case '1': 
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\MenuSelect.wav");
		playSound_Medium(sound);
		MonsterpediaShooter(stage_menu, sound);
		break;
		/* BB menu */
	case '2': 
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\MenuSelect.wav");
		playSound_Medium(sound);
		MonsterpediaBB(stage_menu, sound);
		break;
	}
}
void MonsterpediaShooter(Stage *stage_menu, Sound *sound)
{
	int select, i, x, y;

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);

	/* Print enemyShooter */
	for (i = 0, x = 38, y = 5; i < ENEMY_HEIGHT; ++i, ++y)
	{
		WorldData_GoToXY(x, y);
		printf("%s", enemyShooter_P1[i]);
	}
	SetConsoleTextAttribute(hConsole, saved_attributes);

	WorldData_GoToXY(20, 11);
	printf("Name: Shooter");
	WorldData_GoToXY(20, 12);
	printf("Type: Shooters");
	WorldData_GoToXY(20, 13);
	printf("Health: 1");
	WorldData_GoToXY(20, 14);
	printf("Damage: 1");
	WorldData_GoToXY(20, 15);
	printf("Threat: Minimal");
	WorldData_GoToXY(20, 16);
	printf("Description: Casual pew.");
	WorldData_GoToXY(20, 17);
	printf("             Shoots regular shots.");
	Stage_MainMenu_Print(stage_menu);
	WorldData_GoToXY(36, 19);
	printf("1. Next page");
	WorldData_GoToXY(36, 21);
	printf("2. Previous page");
	/* User selection */
	select = _getch();
	while (select)
	{
		if (select == '1' || select == '2')
			break;

		printf("");
		select = _getch();
	}
	system("cls");
	switch (select)
	{
		/* BigShot menu*/
	case '1': 
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\MenuSelect.wav");
		playSound_Medium(sound);
		MonsterpediaBigShot(stage_menu, sound);
		break;
		/* Wall menu */
	case '2': 
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\MenuSelect.wav");
		playSound_Medium(sound);
		MonsterpediaWall(stage_menu, sound);
		break;
	}
}
void MonsterpediaBigShot(Stage *stage_menu, Sound *sound)
{
	int select, i, x, y;

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;

	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);

	/* Print enemyBigShot_2 */
	for (i = 0, x = 34, y = 5; i < ENEMY_HEIGHT; ++i, ++y)
	{
		WorldData_GoToXY(x, y);
		printf("%s", enemyBigShot_P1_2[i]);
	}
	/* Print enemyBigShot_1 */
	for (i = 0, x = 40, y = 5; i < ENEMY_HEIGHT; ++i, ++y)
	{
		WorldData_GoToXY(x, y);
		printf("%s", enemyBigShot_P1_1[i]);
	}
	SetConsoleTextAttribute(hConsole, saved_attributes);

	WorldData_GoToXY(20, 11);
	printf("Name: BigShaq");
	WorldData_GoToXY(20, 12);
	printf("Type: Shooters");
	WorldData_GoToXY(20, 13);
	printf("Health: 2+2=4-1.....lol no its just 2");
	WorldData_GoToXY(20, 14);
	printf("Damage: 2");
	WorldData_GoToXY(20, 15);
	printf("Threat: Not hot");
	WorldData_GoToXY(20, 16);
	printf("Description: BOOM you don' know.");
	WorldData_GoToXY(20, 17);
	printf("             Shoots strong shots.");
	Stage_MainMenu_Print(stage_menu);
	WorldData_GoToXY(36, 19);
	printf("1. Next page");
	WorldData_GoToXY(36, 21);
	printf("2. Previous page");
	/* User selection */
	select = _getch();
	while (select)
	{
		if (select == '1' || select == '2')
			break;

		printf("");
		select = _getch();
	}
	system("cls");
	switch (select)
	{
		/* Pea menu */
	case '1': 
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\MenuSelect.wav");
		playSound_Medium(sound);
		MonsterpediaPea(stage_menu, sound);
		break;
		/* Shooter menu */
	case '2': 
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\MenuSelect.wav");
		playSound_Medium(sound);
		MonsterpediaShooter(stage_menu, sound);
		break;
	}
}
void MonsterpediaPea(Stage *stage_menu, Sound *sound)
{
	int select, i, x, y;

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;

	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);

	/* Print enemyPea */
	for (i = 0, x = 38, y = 5; i < ENEMY_HEIGHT; ++i, ++y)
	{
		WorldData_GoToXY(x, y);
		printf("%s", enemyPea_P1[i]);
	}
	SetConsoleTextAttribute(hConsole, saved_attributes);

	WorldData_GoToXY(20, 11);
	printf("Name: Pea");
	WorldData_GoToXY(20, 12);
	printf("Type: Shooters");
	WorldData_GoToXY(20, 13);
	printf("Health: 1");
	WorldData_GoToXY(20, 14);
	printf("Damage: 1");
	WorldData_GoToXY(20, 15);
	printf("Threat: Minimal");
	WorldData_GoToXY(20, 16);
	printf("Description: Pew pew pew pew pew pew pew pew pew.");
	WorldData_GoToXY(20, 17);
	printf("             Shoots regular shots faster.");
	Stage_MainMenu_Print(stage_menu);
	WorldData_GoToXY(36, 19);
	printf("1. Next page");
	WorldData_GoToXY(36, 21);
	printf("2. Previous page");
	/* User Selection */
	select = _getch();
	while (select)
	{
		if (select == '1' || select == '2')
			break;

		printf("");
		select = _getch();
	}
	system("cls");
	switch (select)
	{
		/* Arc menu */
	case '1': 
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\MenuSelect.wav");
		playSound_Medium(sound);
		MonsterpediaArc(stage_menu, sound);
		break;
		/* BigShot menu */
	case '2': 
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\MenuSelect.wav");
		playSound_Medium(sound);
		MonsterpediaBigShot(stage_menu, sound);
		break;
	}
}
void MonsterpediaArc(Stage *stage_menu, Sound *sound)
{
	int select, i, x, y;

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED);

	/* Print enemyArc */
	for (i = 0, x = 32, y = 5; i < ENEMY_HEIGHT; ++i, ++y)
	{
		WorldData_GoToXY(x, y);
		printf("%s", enemyArc_P1_3[i]);
	}
	for (i = 0, x = 38, y = 5; i < ENEMY_HEIGHT; ++i, ++y)
	{
		WorldData_GoToXY(x, y);
		printf("%s", enemyArc_P1_2[i]);
	}
	for (i = 0, x = 44, y = 5; i < ENEMY_HEIGHT; ++i, ++y)
	{
		WorldData_GoToXY(x, y);
		printf("%s", enemyArc_P1_1[i]);
	}
	SetConsoleTextAttribute(hConsole, saved_attributes);

	WorldData_GoToXY(20, 11);
	printf("Name: Arc");
	WorldData_GoToXY(20, 12);
	printf("Type: Shooters");
	WorldData_GoToXY(20, 13);
	printf("Health: 3");
	WorldData_GoToXY(20, 14);
	printf("Damage: 1");
	WorldData_GoToXY(20, 15);
	printf("Threat: God tier");
	WorldData_GoToXY(20, 16);
	printf("Description: 3 angle shot.");
	WorldData_GoToXY(20, 17);
	printf("             Shoots regular shots at 3 different angles.");
	Stage_MainMenu_Print(stage_menu);
	WorldData_GoToXY(36, 19);
	printf("1. Next page");
	WorldData_GoToXY(36, 21);
	printf("2. Previous page");
	/* User selection */
	select = _getch();
	while (select)
	{
		if (select == '1' || select == '2')
			break;

		printf("");
		select = _getch();
	}
	system("cls");
	switch (select)
	{
		/* torpedo menu */
	case '1': 
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\MenuSelect.wav");
		playSound_Medium(sound);
		MonsterpediaTorpedo(stage_menu, sound);
		break;
		/* Pea menu */
	case '2': 
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\MenuSelect.wav");
		playSound_Medium(sound);
		MonsterpediaPea(stage_menu, sound);
		break;
	}
}
void MonsterpediaTorpedo(Stage *stage_menu, Sound *sound)
{
	int select, i, x, y;

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;

	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);

	/* Print enemyTorpedo */
	for (i = 0, x = 32, y = 5; i < ENEMY_HEIGHT; ++i, ++y)
	{
		WorldData_GoToXY(x, y);
		printf("%s", enemyTorpedo_3[i]);
	}
	for (i = 0, x = 38, y = 5; i < ENEMY_HEIGHT; ++i, ++y)
	{
		WorldData_GoToXY(x, y);
		printf("%s", enemyTorpedo_2[i]);
	}
	for (i = 0, x = 44, y = 5; i < ENEMY_HEIGHT; ++i, ++y)
	{
		WorldData_GoToXY(x, y);
		printf("%s", enemyTorpedo_1[i]);
	}
	SetConsoleTextAttribute(hConsole, saved_attributes);

	WorldData_GoToXY(20, 11);
	printf("Name: Torpedo");
	WorldData_GoToXY(20, 12);
	printf("Type: Shooters");
	WorldData_GoToXY(20, 13);
	printf("Health: 3");
	WorldData_GoToXY(20, 14);
	printf("Damage: 1");
	WorldData_GoToXY(20, 15);
	printf("Threat: Average");
	WorldData_GoToXY(20, 16);
	printf("Description: homed shots.");
	WorldData_GoToXY(20, 17);
	printf("             Shoots regular shots that");
	WorldData_GoToXY(20, 18);
	printf("             homes onto the player.");
	Stage_MainMenu_Print(stage_menu);
	WorldData_GoToXY(36, 19);
	printf("1. Next page");
	WorldData_GoToXY(36, 21);
	printf("2. Previous page");
	/* User selection */
	select = _getch();
	while (select)
	{
		if (select == '1' || select == '2')
			break;

		printf("");
		select = _getch();
	}
	system("cls");
	switch (select)
	{
		/* Jebus menu */
	case '1': 
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\MenuSelect.wav");
		playSound_Medium(sound);
		MonsterpediaJebus(stage_menu, sound);
		break;
		/* Arc menu */
	case '2': 
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\MenuSelect.wav");
		playSound_Medium(sound);
		MonsterpediaArc(stage_menu, sound);
		break;
	}
}
void MonsterpediaJebus(Stage *stage_menu, Sound *sound)
{

	int select, i, x, y;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;

	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

	/* Print enemyJebus */
	for (i = 0, x = 34, y = 5; i < ENEMY_HEIGHT; ++i, ++y)
	{
		WorldData_GoToXY(x, y);
		printf("%s", enemyJebus_2[i]);
	}
	for (i = 0, x = 40, y = 5; i < ENEMY_HEIGHT; ++i, ++y)
	{
		WorldData_GoToXY(x, y);
		printf("%s", enemyJebus_1[i]);
	}
	SetConsoleTextAttribute(hConsole, saved_attributes);

	WorldData_GoToXY(20, 11);
	printf("Name: Jebus");
	WorldData_GoToXY(20, 12);
	printf("Type: Unique");
	WorldData_GoToXY(20, 13);
	printf("Health: 2");
	WorldData_GoToXY(20, 14);
	printf("Threat: Strong");
	WorldData_GoToXY(20, 15);
	printf("Description: Revives and heals adjacent.");
	WorldData_GoToXY(20, 16);
	printf("             Does not shoot.");
	Stage_MainMenu_Print(stage_menu);
	WorldData_GoToXY(36, 19);
	printf("1. Next page");
	WorldData_GoToXY(36, 21);
	printf("2. Previous page");
	/* User selection */
	select = _getch();
	while (select)
	{
		if (select == '1' || select == '2')
			break;

		printf("");
		select = _getch();
	}
	system("cls");
	switch (select)
	{
		/* Hydra menu */
	case '1': 
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\MenuSelect.wav");
		playSound_Medium(sound);
		MonsterpediaHydra(stage_menu, sound);
		break;
		/* torpedo menu */
	case '2': 
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\MenuSelect.wav");
		playSound_Medium(sound);
		MonsterpediaTorpedo(stage_menu, sound);
		break;
	}
}
void MonsterpediaHydra(Stage *stage_menu, Sound *sound)
{
	int select, i, x, y;

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;

	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	for (i = 0, x = 26, y = 5; i < ENEMY_HEIGHT; ++i, ++y)
	{
		WorldData_GoToXY(x, y);
		printf("%s", enemyShooter_P1[i]);
	}
	for (i = 0, x = 32, y = 5; i < ENEMY_HEIGHT; ++i, ++y)
	{
		WorldData_GoToXY(x, y);
		printf("%s", enemyBigShot_P1_2[i]);
	}
	for (i = 0, x = 38, y = 5; i < ENEMY_HEIGHT; ++i, ++y)
	{
		WorldData_GoToXY(x, y);
		printf("%s", enemyPea_P1[i]);
	}
	for (i = 0, x = 44, y = 5; i < ENEMY_HEIGHT; ++i, ++y)
	{
		WorldData_GoToXY(x, y);
		printf("%s", enemyArc_P1_3[i]);
	}
	for (i = 0, x = 50, y = 5; i < ENEMY_HEIGHT; ++i, ++y)
	{
		WorldData_GoToXY(x, y);
		printf("%s", enemyTorpedo_3[i]);
	}
	SetConsoleTextAttribute(hConsole, saved_attributes);

	WorldData_GoToXY(20, 11);
	printf("Name: Hydra");
	WorldData_GoToXY(20, 12);
	printf("Type: Unique");
	WorldData_GoToXY(20, 13);
	printf("Health: ?");
	WorldData_GoToXY(20, 14);
	printf("Threat: God Tier");
	WorldData_GoToXY(20, 15);
	printf("Description: Takes the form of any shooter with their");
	WorldData_GoToXY(20, 16);
	printf("             stats and behaviour.");
	WorldData_GoToXY(20, 17);
	printf("             Splits into two when killed.");
	Stage_MainMenu_Print(stage_menu);
	WorldData_GoToXY(36, 19);
	printf("1. Exit Monsterpedia");
	WorldData_GoToXY(36, 21);
	printf("2. Previous page");
	/* User selection */
	select = _getch();
	while (select)
	{
		if (select == '1' || select == '2')
			break;

		printf("");
		select = _getch();
	}
	system("cls");
	switch (select)
	{
		/* Main menu */
	case '1': 
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\MenuSelect.wav");
		playSound_Medium(sound);
		MainMenu(stage_menu, sound);
		break;
		/* Jesbus menu */
	case '2': 
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\MenuSelect.wav");
		playSound_Medium(sound);
		MonsterpediaJebus(stage_menu, sound);
		break;
	}
}
/* Credits GameState */
void CreditsMenu(Stage *stage_menu, Sound *sound)
{
	int select;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	/* Save current attributes */
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;
	Stage_MainMenu_Print(stage_menu);
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	WorldData_GoToXY(36, 3);
	printf("Credits");
	WorldData_GoToXY(10, 5);
	printf("All content © 2017 Digipen Institute of Technology Singapore"); 
	WorldData_GoToXY(32, 6);
	printf("all rights reserved");
	WorldData_GoToXY(24, 9);
	printf("DigiPen President..Claude Comair");
	WorldData_GoToXY(24, 11);
	printf("Instructors........Cristian Fagundo");
	WorldData_GoToXY(24, 12);
	printf("                   Andy Logam Tan");
	WorldData_GoToXY(24, 13);
	printf("                   Cheng Ding Xiang");
	WorldData_GoToXY(24, 15);
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("Producer...........Audry May");
	WorldData_GoToXY(24, 16);
	printf("Lead Programmer....Lois Ngm");
	WorldData_GoToXY(24, 17);
	printf("Lead Design/Sound..Wong ZhiHao");
	WorldData_GoToXY(24, 18);
	printf("Product Manager....Ong Tze Ang");
	WorldData_GoToXY(37, 21);
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("1. Back");

	SetConsoleTextAttribute(hConsole, saved_attributes);
	/* User in put */
	select = _getch();
	while (select)
	{
		if (select == '1')
			break;

		printf("");
		select = _getch();
	}
	system("cls");
	switch (select)
	{
		/* Back */
	case '1': 
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\MenuSelect.wav");
		playSound_Medium(sound);
		MainMenu(stage_menu, sound);
		break;
	}
}
/* Free objects */
void MainMenu_Free(Stage *stage, Ball *ball, TileMapManager *tileManager)
{
	Stage_Free(&stage);
	Ball_Free(&ball);
	TileMapManager_Free(&tileManager);
}

/* HighScore GameState */
void Highscore_Menu(Stage *stage_menu, Sound *sound)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	int select;
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;

	Stage_MainMenu_Print(stage_menu);

	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	WorldData_GoToXY(35, 3);
	printf("Highscore");
	WorldData_GoToXY(31, 7);
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	printf("1. Normal Stage 1");
	WorldData_GoToXY(31, 9);
	printf("2. Normal Stage 2");
	WorldData_GoToXY(31, 11);
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	printf("3. Ice Stage 1");
	WorldData_GoToXY(31, 13);
	printf("4. Ice Stage 2");
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
	WorldData_GoToXY(31, 15);
	printf("5. Fire Stage 1");
	WorldData_GoToXY(31, 17);
	printf("6. Fire Stage 2");
	WorldData_GoToXY(36, 22);
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("7. Back");
	SetConsoleTextAttribute(hConsole, saved_attributes);
	/* User selection */
	select = _getch();
	while (select)
	{
		if (select == '1'|| select == '2' || select == '3' || select == '4' || select == '5' || select == '6' || select == '7')
			break;

		printf("");
		select = _getch();
	}
	system("cls");
	/* Show high score for stages */
	switch (select)
	{
	case '1':
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\MenuSelect.wav");
		playSound_Low(sound);
		Highscore_Display(stage_menu, sound, 1);
		break;
	case '2':
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\MenuSelect.wav");
		playSound_Low(sound);
		Highscore_Display(stage_menu, sound, 2);
		break;
	case '3':
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\MenuSelect.wav");
		playSound_Low(sound);
		Highscore_Display(stage_menu, sound, 3);
		break;
	case '4':
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\MenuSelect.wav");
		playSound_Low(sound);
		Highscore_Display(stage_menu, sound, 4);
		break;
	case '5':
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\MenuSelect.wav");
		playSound_Low(sound);
		Highscore_Display(stage_menu, sound, 5);
		break;
	case '6':
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\MenuSelect.wav");
		playSound_Low(sound);
		Highscore_Display(stage_menu, sound, 6);
		break;
		/* Back */
	case '7':
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\MenuSelect.wav");
		playSound_Low(sound);
		MainMenu(stage_menu, sound);
		break;
	default:
		break;
	}
}
/* HighScore Display GameState */
void Highscore_Display(Stage *stage_menu, Sound *sound, int id)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	Highscore *highscore = NULL;
	int select, i = 0;
	highscore = Highscore_Create(id);
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;
	Stage_MainMenu_Print(stage_menu);
	/* Print Highscore gamestate */
	switch (id)
	{
	case 1:
		WorldData_GoToXY(33, 3);
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		printf("Normal Stage 1");
		break;
	case 2:
		WorldData_GoToXY(33, 3);
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		printf("Normal Stage 2");
		break;
	case 3:
		WorldData_GoToXY(34, 3);
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		printf("Ice Stage 1");
		break;
	case 4:
		WorldData_GoToXY(34, 3);
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		printf("Ice Stage 2");
		break;
	case 5:
		WorldData_GoToXY(34, 3);
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
		printf("Fire Stage 1");
		break;
	case 6:
		WorldData_GoToXY(34, 3);
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
		printf("Fire Stage 2");
		break;
	default:
		break;

	}
	/*  Print respective highscores */
	WorldData_GoToXY(35, 4);
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("Highscore");
	for (i = 0; i < TOP10/2; ++i)
	{
	WorldData_GoToXY(20, 7+i*2);
	printf("%.2d%4s%10.4d", i+1, highscore->name[i], highscore->score[i]);
	}
	for (; i < TOP10; ++i)
	{

	WorldData_GoToXY(43, 7 + (i-5) * 2);
	printf("%.2d%4s%10.4d", i + 1, highscore->name[i], highscore->score[i]);
	}
	WorldData_GoToXY(35, 18);
	printf("1. Back");
	SetConsoleTextAttribute(hConsole, saved_attributes);
	select = _getch();
	while (select)
	{
		if (select == '1')
			break;

		printf("");
		select = _getch();
	}
	system("cls");
	switch (select)
	{
		/* Back */
	case '1':
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\MenuSelect.wav");
		playSound_Medium(sound);
		Highscore_Free(&highscore);
		Highscore_Menu(stage_menu, sound);
		break;
	}
}