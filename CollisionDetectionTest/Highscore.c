/******************************************************************************
All content © 2017 Digipen Institute of Technology Singapore, all rights reserved

filename			Highscore.c
author				Ngm Hui Min, Lois
DP email			lois.ngm@digipen.edu
course				RTIS

Brief Description:
Contains the algorithmn for highscore. Displays and updates the highscore of
Coop stages, normal stage 1, normal stage 2, ice stage 1, ice stage 2, fire
stage 1 and fire stage 2.
******************************************************************************/
#include "stdafx.h"
#include "Stage.h" /* Stage object */
#include <stdio.h> /* for fopen_s(), fprintf, gets() and EOF*/
#include <stdlib.h> /* for malloc, free */
#include <string.h> /* strncpy */
#include "Highscore.h" /* fucntion declarations */
#include "Player.h" /* Player object */
#include "WorldData.h" /* WorldData_GotoXY(), showcursor(), hidecursor() */
#include <windows.h> /* HANDLE, CONSOLE_SCREEN_BUFFER_INFO, WORD, GetConsoleScreenBufferInfo(), SetConsoleTextAttribute() */
#include <conio.h>	/*_getch()*/

/* Create a new Highscore object for stages, using the provided parameters */
Highscore *Highscore_Create(int id)
{
	char (*pstr)[4]; /* name */
	int *pScore = 0; /* score */
	int c; /* for checking number of inputs */
	int current_line_number; /* looping variable */
	int ch; /* check character if null or end of file */

	FILE *file;
	errno_t tmp;
	Highscore* highscore; /* The new Highscore object for stages */
	highscore = (Highscore*)malloc(sizeof(Highscore)); /* allocate the memory for the new Highscore object */
	if (highscore == NULL)
	{
		return NULL; /* malloc can fail! */
	}
	pScore = highscore->score; /* points to the first score */
 	pstr = highscore->name; /* points to the second score */
	/* depending on stage, read the respective files */
	switch (id)
	{
	case 1: tmp = fopen_s(&file, "highscoreNormal1.txt", "r+"); /* Normal Stage 1 */
		break;
	case 2: tmp = fopen_s(&file, "highscoreNormal2.txt", "r+"); /* Normal Stage 2 */
		break;
	case 3: tmp = fopen_s(&file, "highscoreIce1.txt", "r+"); /* Ice Stage 1 */
		break;
	case 4: tmp = fopen_s(&file, "highscoreIce2.txt", "r+"); /* Ice Stage 2 */
		break;
	case 5: tmp = fopen_s(&file, "highscoreFire1.txt", "r+"); /* Fire Stage 1 */
		break;
	case 6: tmp = fopen_s(&file, "highscoreFire2.txt", "r+"); /* Fire Stage 2 */
		break;
	default: tmp = fopen_s(&file, "highscoreNormal1.txt", "r+"); /* default */
		break;
	}
	/* loop through to get data from file */
	for (current_line_number = 0; current_line_number < TOP10; ++current_line_number)
	{
		/* scan the file line */
		c = fscanf_s(file, "%d %s", pScore, *pstr, 4);
		/* if there is not 2 inputs for one score and one name */
		if (c != 2)
			printf("error");
		
		ch = fgetc(file);
		if (ch == EOF) /* if end of file, break out of loop */
			break;
		/* point to next element or array of char */
		pScore++;
		pstr++;
	}
	/* When completed, close file */
	fclose(file);
	return highscore; /* return highscore object */
}

/* Free the memory associated with a given Highscore object */
void Highscore_Free(Highscore **highscore)
{

	/* safety check on the parameters */
	if (highscore == NULL)
		return;  /* take no action if the parameters are invalid */

	/* free the given item */
	free(*highscore);

	/* clear the provided pointer */
	*highscore = NULL;
}

int Highscore_Check(Player *player_1, Player* player_2, int id)
{
	/* colour */
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	int avgScore; /* average score between player 1 and 2 */
	int i = 0, j = 0; /* looping variable */
	int position = -1; /* intialize the position of the highscore to -1 */
	char pName[300] = { "COM" }; /* name to get input */
	FILE *file;
	errno_t tmp;
	Highscore* highscore; /* The new Highscore object for stages */
	/* saved attributes for the original colour */
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;
	/* calculate the average score between the 2 players */
	avgScore = (player_1->score + player_2->score) / 2;
	/* open the highscore text file according to the stage */
	highscore = Highscore_Create(id);
	/* clear the screen */
	system("cls");
	/* overwrite the file depending on the stage */
	switch (id)
	{
		/* Normal Stage 1 */
	case 1: 
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); /* WHITE */
		tmp = fopen_s(&file, "highscoreNormal1.txt", "w");
		break;
		/* Normal Stage 2 */
	case 2:
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); /* WHITE */
		tmp = fopen_s(&file, "highscoreNormal2.txt", "w");
		break;
		/* Ice Stage 1 */
	case 3:
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); /* CYAN */
		tmp = fopen_s(&file, "highscoreIce1.txt", "w");
		break;
		/* Ice Stage 2 */
	case 4:
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); /* CYAN */
		tmp = fopen_s(&file, "highscoreIce2.txt", "w");
		break;
		/* Fire Stage 1 */
	case 5:
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY); /* RED */
		tmp = fopen_s(&file, "highscoreFire1.txt", "w");
		break;
		/* Fire Stage 2 */
	case 6:
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY); /* RED */
		tmp = fopen_s(&file, "highscoreFire2.txt", "w");
		break;
		/* default */
	default:
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); /* WHITE */
		tmp = fopen_s(&file, "highscoreNormal1.txt", "w");
		break;
	}
	/* Set the window size for to display */
	SetConsoleWindowSize(80, 25);

	/* Loops through the entire file */
	for (i = 0; i < TOP10; ++i)
	{
		/* if the player is in the highscore, get the player's name */
		if (avgScore > highscore->score[i])
		{
			switch (id)
			{
			case 1:
				WorldData_GoToXY(32, 8);
				printf("Normal Stage 1");
				break;
			case 2:
				WorldData_GoToXY(32, 8);
				printf("Normal Stage 2");
				break;
			case 3:
				WorldData_GoToXY(34, 8);
				printf("Ice Stage 1");
				break;
			case 4:
				WorldData_GoToXY(34, 8);
				printf("Ice Stage 2");
				break;
			case 5:
				WorldData_GoToXY(33, 8);
				printf("Fire Stage 1");
				break;
			case 6:
				WorldData_GoToXY(33, 8);
				printf("Fire Stage 2");
				break;
			default: 
				break;
			}
			/* Congratulatory message */
			WorldData_GoToXY(20, 10);
			printf("Congratulations! You made it to the highscore!");
			WorldData_GoToXY(30, 11);
			/* display the average score */
			printf("Average Score: %d", avgScore);
			SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); /* WHITE */
			/* prompt for the player's name */
			WorldData_GoToXY(20, 13);
			printf("Please enter your name (up to 3 characters):");
			showcursor();
			WorldData_GoToXY(35, 14);
			scanf_s("%s", pName, 300);
			/* if user did not enter a name, set a default name */
			if (pName[0] == '\0')
				strcpy(pName, "COM");
			hidecursor();
			/* start shift the scores down if necessary */
			for (j = TOP10-1; j > i; --j)
			{
				highscore->score[j] = highscore->score[j-1];
				strcpy(highscore->name[j], highscore->name[j-1]);
			}
			/* replace the highscore */
			highscore->score[i] = avgScore;
			/* the player's position in the highscore */
			position = i;
			/* Copy the first 3 characters of the string the player input (in case it was longer than 3 characters) */
			strncpy(highscore->name[i], pName, 3);
			/* make all letters uppercase */
			for (j = 0; j < 4; j++)
			{
				if (highscore->name[i][j] >= 'a' && highscore->name[i][j] <= 122)
				{
					if (highscore->name[i][j] == '\0')
						break;
					highscore->name[i][j] -= 32;
				}
			}
			break;
		}
	}
	/* overwrite the file */
	for (i = 0; i < TOP10; ++i)
	{
		fprintf(file, "%d %s\n", highscore->score[i], highscore->name[i]);
	}
	/* When completed, close file */
	fclose(file);
	/* return the position of which the player is in the highscore */
	return position;
}

void Highscore_Display_Stage(Stage *stage_menu, int id, int position)
{
	/* colour */
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	/* create highscore object */
	Highscore *highscore = NULL;
	/* press any key to enter */
	int select;
	int i = 0; /* looping variable */
	highscore = Highscore_Create(id); /* Creates a new Highscore object according to the stage */
	/* save attributes of the original colour */
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;
	/* Print the borders of the console for highscore page */
	Stage_MainMenu_Print(stage_menu);
	/* Prints according to the stage */
	switch (id)
	{
		/* Normal Stage 1 */
	case 1:
		WorldData_GoToXY(33, 3);
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY); /* WHITE */
		printf("Normal Stage 1");
		break;
		/* Normal Stage 2 */
	case 2:
		WorldData_GoToXY(33, 3);
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY); /* WHITE */
		printf("Normal Stage 2");
		break;
		/* Ice Stage 1 */
	case 3:
		WorldData_GoToXY(34, 3);
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); /* CYAN */
		printf("Ice Stage 1");
		break;
		/* Ice Stage 2 */
	case 4:
		WorldData_GoToXY(34, 3);
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); /* CYAN */
		printf("Ice Stage 2");
		break;
		/* Fire Stage 1 */
	case 5:
		WorldData_GoToXY(34, 3);
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY); /* RED */
		printf("Fire Stage 1");
		break;
		/* Fire Stage 2 */
	case 6:
		WorldData_GoToXY(34, 3);
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY); /* RED */
		printf("Fire Stage 2");
		break;
		/* default */
	default:
		break;

	}
	WorldData_GoToXY(35, 4);
	/* Set the colour back to yellow for highscore printing */
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY); /* YELLOW */
	printf("Highscore"); /* Header */
	for (i = 0; i < TOP10 / 2; ++i)
	{
		WorldData_GoToXY(20, 8 + i * 2);
		/* if the player is at this particular position, print it in a different colour */
		if (position == i)
		{
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY); /* MAGENTA */
			printf("%.2d%4s%10.4d", i + 1, highscore->name[i], highscore->score[i]);
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); /* WHITE */
		}
		else
			printf("%.2d%4s%10.4d", i + 1, highscore->name[i], highscore->score[i]);
	}
	for (; i < TOP10; ++i)
	{

		WorldData_GoToXY(43, 8 + (i - 5) * 2);
		if (position == i)
		{
			/* if the player is at this particular position, print it in a different colour */
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY); /* MAGENTA */
			printf("%.2d%4s%10.4d", i + 1, highscore->name[i], highscore->score[i]);
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); /* WHITE */
		}
		else
			printf("%.2d%4s%10.4d", i + 1, highscore->name[i], highscore->score[i]);
	}
	WorldData_GoToXY(25, 20);
	printf("Press any key to continue");
	/* change back to original colour */
	SetConsoleTextAttribute(hConsole, saved_attributes);
	/* Wait for user input */
	Sleep(2000);
	select = _getch();
	/* clear the screen */
	system("cls");
}