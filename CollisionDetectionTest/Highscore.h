/******************************************************************************
All content © 2017 Digipen Institute of Technology Singapore, all rights reserved

filename			Highscore.h
author				Ngm Hui Min, Lois
DP email			lois.ngm@digipen.edu
course				RTIS

Brief Description:
Header file for Highscore.c which contains the contents of the highscore object 
******************************************************************************/
/* Show only top 10 of highscore */
#define TOP10 10

/* Contents of a Highscore */
typedef struct Highscore
{
	char name[TOP10][4];
	int score[TOP10];
}Highscore;

/* necessary structs */
typedef struct Player Player;
typedef struct Stage Stage;

/* Create the highscore object according to the stage (MAIN MENU) */
Highscore *Highscore_Create(int id);

/* Free the highscore object */
void Highscore_Free(Highscore **highscore);

/* Checks if the player is in the highscore and returns the position of which the player is in */
int Highscore_Check(Player *player1, Player *player2, int id);

/* Displays the highscore to the player after a stage if they have won */
void Highscore_Display_Stage(Stage *stage_menu, int id, int position);