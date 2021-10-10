/******************************************************************************
All content © 2017 Digipen Institute of Technology Singapore, all rights reserved

filename			Ball.c
author				Enriquez, Audry May dela Paz
DP email			a.enriquez@digipen.edu
course				BSGD

Brief Description:
Contents of ball object of the game.

******************************************************************************/

/* Load Libraries */
#include "stdafx.h"
#include <stdio.h>							/*printf*/
#include <windows.h>						/*GetAsyncKeyState*/
#include <time.h>							/*srand, rand, time*/
#include "Ball.h"							/*Ball header file*/
#include "TileMapManager.h"					/*Tile objects and data*/
#include "WorldData.h"						/* WorldData_GotoXY */
#include "EnemyAI.h"                        /*Enemybehaviour*/
#include "Powerups.h"						/*Powerups*/
#include "EnemyBullets.h"                   /*Enemy bullets*/
#include "Ball_CollisionManager.h"			/*CollisionManager*/
#include "Player.h"							/* Player objects and data */
#include "Stage.h"							/* Stage objects and data */
#include "FMODInit.h"						/* Sound objects and data */


/*time to revive the ball*/
#define timetorevive 1200

/* Create a new Item object with the provided data */
Ball* Ball_Create(Stage *stage, int name, char ballcharacter, int x_start_pos, int y_start_pos, char fire, Player *friendplayer, Player *enemyplayer)
{
	Ball *ball = NULL; /* the new item to be returned */

					   /* allocate the memory for a new object */
	ball = (Ball*)malloc(sizeof(Ball));

	if (ball == NULL)
	{
		return NULL; /* malloc can fail */
	}

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	/* Save current attributes */
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;
	/*Prints ball at the middle of the bat*/

	/*Initialise the ball objects*/
	ball->ball_type = name;					/*type of the ball*/
	ball->ballchar = ballcharacter;			/*Char of the ball*/
	ball->firebutton = fire;				/*The button players have to press*/
	ball->x_ballpos = x_start_pos;			/*the current position of x */
	ball->y_ballpos = y_start_pos;			/*the current position of y */
	ball->start_y_ballpos = y_start_pos;	/*the starting position of y */
	ball->ball_fired_flag = 0;				/*Initialise the ball to not fired*/
	ball->balldirection = STOP;				/*Initialise the ball's direction to STOP*/
	ball->friendplayer = friendplayer;		/*Initialise the player the ball belongs to*/
	ball->enemyplayer = enemyplayer;		/*Initialise the other player in the game.*/
	ball->frames = 112;						/* Updates every 120 frames, WILL NOT CHANGE */

	/* Adjusts the speed, sets it to be faster in PvP mode */
	if (stage != NULL && stage->play_type == PvP)
		ball->base_timer = 20;
	else
		ball->base_timer = 0;

	ball->timer = ball->base_timer;			/* Timer always starts from base_timer's value */
	ball->ballhitballflag = 0;				/*Initialise the flag to not have been hit by another ball*/
	ball->ballisdeadflag = 0;				/*Intiialise the ball to not dead*/
	ball->timetorevival = 0;				/*Initialise the ball's time to revival to zero*/

	if (stage != NULL && stage->play_type == PvP)
	{
		ball->shoot_time = time(NULL) + 7;		/* Initialises to 0, player can hold on to ball for any length of time at the start of play */
		ball->auto_shoot_control = 1;			/* Ball will not autoshoot at the start of play */
	}

	else if (stage != NULL && stage->play_type == CoOp)
	{
		ball->shoot_time = 0;					/* Initialises to 0, player can hold on to ball for any length of time at the start of play */
		ball->auto_shoot_control = 0;			/* Ball will not autoshoot at the start of play */
	}
	
	/*Sets colors of the ball depending on type*/
	if (ball->ball_type == 1)
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	else if (ball->ball_type == 2)
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	WorldData_GoToXY(x_start_pos, y_start_pos);
	printf("%c", ballcharacter);
	SetConsoleTextAttribute(hConsole, saved_attributes);


	/* return the new object */
	return ball;
}

/*Updates ball's other ball - allows for the ball to bounce each other*/
void Ball_UpdateEnemyBall(Ball* ball1, Ball* ball2)
{
	ball1->enemyball = ball2;
	ball2->enemyball = ball1;
}


/*If ball has not been shot, move the ball with the player*/
void Ball_MoveBallWithPlayer(Ball* ball, TileMapManager *tileManager)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	/* Save current attributes */
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;

	/*clears the ball at old position*/
	TileMapManager_Ball(ball, tileManager, 0);
	WorldData_GoToXY(ball->x_ballpos, ball->y_ballpos);
	printf(" ");

	/*Sets new coordinates of the ball*/
	ball->x_ballpos = ball->friendplayer->x_p + ball->friendplayer->x_l / 2;
	ball->y_ballpos = ball->start_y_ballpos;

	/*Draws ball at new coordinates*/
	WorldData_GoToXY(ball->x_ballpos, ball->y_ballpos);
	if (ball->ball_type == 1)
	{
		TileMapManager_Ball(ball, tileManager, 4);
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	}
	else if (ball->ball_type == 2)
	{
		TileMapManager_Ball(ball, tileManager, 5);
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	}
	printf("%c", ball->ballchar);
	SetConsoleTextAttribute(hConsole, saved_attributes);

}

/*Moves ball accordingly, and checks for collision everytime before it moves in a certain direction*/
void Ball_MovementManager(Sound *sound, Ball* ball, TileMapManager* tilemap, Stage* stage, Enemy* enemy1[], Enemy* enemy2[],
	Enemy_AI* enemy_AI1, Enemy_AI* enemy_AI2, Powerup *powerup[], EnemyBullet *enemyBullet[])
{
	int ballx = ball->x_ballpos;/*temporary value storing the x-coord of ball*/
	int bally = ball->y_ballpos;/*temporary value storing the y-coord of ball*/
	int i;/*Loop counter*/

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	/* Save current attributes */
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;

	/*Clears ball at old position*/
	TileMapManager_Ball(ball, tilemap, 0);
	WorldData_GoToXY(ballx, bally);
	if (ball->ballchar != 0)
		printf(" ");

	/*Switch case checking what direction to move the ball in*/
	switch (ball->balldirection)
	{
	/*45 degrees moves ball one space in each direction. Before moving it checks if it collides with anything*/
#pragma region degrees45_movement_cases

	
	case(BOTTOMRIGHT45):
		++ballx;
		++bally;
		if (Ball_CollisionManager(sound, tilemap, ball, ball->x_ballpos, ball->y_ballpos, ballx, bally, stage, enemy1, enemy2, enemy_AI1, enemy_AI2, powerup, enemyBullet))
		{
			ball->x_ballpos = ballx;
			ball->y_ballpos = bally;
		}
		break;
	case(BOTTOMLEFT45):
		--ballx;
		++bally;
		if (Ball_CollisionManager(sound, tilemap, ball, ball->x_ballpos, ball->y_ballpos, ballx, bally, stage, enemy1, enemy2, enemy_AI1, enemy_AI2, powerup, enemyBullet))
		{
			ball->x_ballpos = ballx;
			ball->y_ballpos = bally;
		}

		break;
	case(TOPRIGHT45):
		++ballx;
		--bally;
		if (Ball_CollisionManager(sound, tilemap, ball, ball->x_ballpos, ball->y_ballpos, ballx, bally, stage, enemy1, enemy2, enemy_AI1, enemy_AI2, powerup, enemyBullet))
		{
			ball->x_ballpos = ballx;
			ball->y_ballpos = bally;
		}
		break;
	case(TOPLEFT45):
		--ballx;
		--bally;
		if (Ball_CollisionManager(sound, tilemap, ball, ball->x_ballpos, ball->y_ballpos, ballx, bally, stage, enemy1, enemy2, enemy_AI1, enemy_AI2, powerup, enemyBullet))
		{
			ball->x_ballpos = ballx;
			ball->y_ballpos = bally;
		}
		break;
#pragma endregion


	/*60 degrees moves ball one space in in x direction, two in y. Before moving it checks if it collides with anything*/
#pragma region degrees60_movement_cases

		
	case(BOTTOMRIGHT60):
		++ballx;
		for (i = 0; i < 2; ++i)
		{
			++bally;
			if (Ball_CollisionManager(sound, tilemap, ball, ball->x_ballpos, ball->y_ballpos, ballx, bally, stage, enemy1, enemy2, enemy_AI1, enemy_AI2, powerup, enemyBullet))
			{
				ball->y_ballpos = bally;
				ball->x_ballpos = ballx;
			}

			else break;
		}
		break;
	case(BOTTOMLEFT60):
		--ballx;
		for (i = 0; i < 2; ++i)
		{
			++bally;
			if (Ball_CollisionManager(sound, tilemap, ball, ball->x_ballpos, ball->y_ballpos, ballx, bally, stage, enemy1, enemy2, enemy_AI1, enemy_AI2, powerup, enemyBullet))
			{
				ball->y_ballpos = bally;
				ball->x_ballpos = ballx;
			}

			else break;
		}
		break;
	case(TOPRIGHT60):
		++ballx;
		for (i = 0; i < 2; ++i)
		{
			--bally;
			if (Ball_CollisionManager(sound, tilemap, ball, ball->x_ballpos, ball->y_ballpos, ballx, bally, stage, enemy1, enemy2, enemy_AI1, enemy_AI2, powerup, enemyBullet))
			{
				ball->y_ballpos = bally;
				ball->x_ballpos = ballx;
			}

			else break;
		}
		break;
	case(TOPLEFT60):
		--ballx;
		for (i = 0; i < 2; ++i)
		{
			--bally;
			if (Ball_CollisionManager(sound, tilemap, ball, ball->x_ballpos, ball->y_ballpos, ballx, bally, stage, enemy1, enemy2, enemy_AI1, enemy_AI2, powerup, enemyBullet))
			{
				ball->x_ballpos = ballx;
				ball->y_ballpos = bally;
			}

			else break;
		}
		break;
#pragma endregion


	/*20 degrees moves ball one space in in y direction, two in x.  Before moving it checks if it collides with anything*/
#pragma region degrees20_movement_cases
	
	
	case(BOTTOMRIGHT20):

		++bally;
		for (i = 0; i < 2; ++i)
		{
			++ballx;
			if (Ball_CollisionManager(sound, tilemap, ball, ball->x_ballpos, ball->y_ballpos, ballx, bally, stage, enemy1, enemy2, enemy_AI1, enemy_AI2, powerup, enemyBullet))
			{
				ball->x_ballpos = ballx;
				ball->y_ballpos = bally;
			}

			else break;
		}
		break;
	case(BOTTOMLEFT20):

		++bally;
		for (i = 0; i < 2; ++i)
		{
			--ballx;
			if (Ball_CollisionManager(sound, tilemap, ball, ball->x_ballpos, ball->y_ballpos, ballx, bally, stage, enemy1, enemy2, enemy_AI1, enemy_AI2, powerup, enemyBullet))
			{
				ball->x_ballpos = ballx;
				ball->y_ballpos = bally;
			}

			else break;
		}
		break;
	case(TOPRIGHT20):

		--bally;
		for (i = 0; i < 2; ++i)
		{
			++ballx;
			if (Ball_CollisionManager(sound, tilemap, ball, ball->x_ballpos, ball->y_ballpos, ballx, bally, stage, enemy1, enemy2, enemy_AI1, enemy_AI2, powerup, enemyBullet))
			{
				ball->x_ballpos = ballx;
				ball->y_ballpos = bally;
			}

			else break;
		}
		break;
	case(TOPLEFT20):
		--bally;
		for (i = 0; i < 2; ++i)
		{
			--ballx;
			if (Ball_CollisionManager(sound, tilemap, ball, ball->x_ballpos, ball->y_ballpos, ballx, bally, stage, enemy1, enemy2, enemy_AI1, enemy_AI2, powerup, enemyBullet))
			{
				ball->x_ballpos = ballx;
				ball->y_ballpos = bally;
			}

			else break;
		}
		break;
#pragma endregion

		/*Moves the ball 2 spaces only in the y direction.   Before moving it checks if it collides with anything*/
#pragma region Straight_movement_cases

	case (DOWN):
		for (i = 0; i < 2; ++i)
		{
			++bally;
			if (Ball_CollisionManager(sound, tilemap, ball, ball->x_ballpos, ball->y_ballpos, ballx, bally, stage, enemy1, enemy2, enemy_AI1, enemy_AI2, powerup, enemyBullet))
			{
				ball->y_ballpos = bally;
				ball->x_ballpos = ballx;
			}

			else break;
		}
		break;
	case (UP):
		for (i = 0; i < 2; ++i)
		{
			--bally;
			if (Ball_CollisionManager(sound, tilemap, ball, ball->x_ballpos, ball->y_ballpos, ballx, bally, stage, enemy1, enemy2, enemy_AI1, enemy_AI2, powerup, enemyBullet))
			{
				ball->x_ballpos = ballx;
				ball->y_ballpos = bally;
			}

			else break;
		}


		break;
#pragma endregion
	}

	/*Checks if ball is out of bounds before drawing and updating the ball's position*/
	if (Ball_isOutofBounds(ball, stage, sound, tilemap))
	{
		/*Draws ball at new position as well as update ball's position in the tilemap manager*/
		if (ball->ball_type == 1)
		{
			TileMapManager_Ball(ball, tilemap, 4);
			SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		}
		else if (ball->ball_type == 2)
		{
			TileMapManager_Ball(ball, tilemap, 5);

			SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
		}
		WorldData_GoToXY(ball->x_ballpos, ball->y_ballpos);
		if (ball->ballchar != 0)
		{
			printf("%c", ball->ballchar);
			SetConsoleTextAttribute(hConsole, saved_attributes);
		}
	}
}

/*Updates the ball*/
void Ball_Update(Sound *sound, Ball* ball, TileMapManager *tileManager, Stage* stage, Enemy* enemy1[], Enemy* enemy2[], Enemy_AI* enemy_AI1, Enemy_AI* enemy_AI2,
	Powerup* powerup[], EnemyBullet *enemyBullet[])
{
	int check = 1;/*checks if the ball has just been fired. 0 if the ball has just been fired, 1 if not fired at all or it has been a while since firing*/

	/* Handles auto shooting of the ball if it has not been fired by shoot_time */
#pragma region Shooting_the_ball

	if (ball->ball_fired_flag == 0 && ball->shoot_time == time(NULL) && ball->auto_shoot_control == 1)
	{
		int direction;/*Temporary direction of the ball*/

		/*Sets the ball to auto shoot if the ball holds on to it for very long*/
		ball->auto_shoot_control = 0;
		/*updates the ball fired flag to just fired*/
		ball->ball_fired_flag = 1;

		/*play the sound of the ball being fired*/
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\PlayerFire.wav");
		if (ball->ball_type == 1)
			playSound_Player1(sound, ball->friendplayer->x_p);
		else if (ball->ball_type == 2)
			playSound_Player2(sound, ball->friendplayer->x_p);

		/*sets the direction of the ball*/
		direction = (rand() % 2);/*randomly chooses between bottom left and bottom right at 45 degree angle*/
		if (ball->ball_type == 2) direction += 2;/*Ensures ball2 moves only top left and top right at 45 degree angle*/

		ball->balldirection = direction;
		
		/*ball has just been fired*/
		check = 0;
	}

#pragma endregion

	/*checks if the ball has just been missed by the player and hence cannot pick it up*/
#pragma region Moving_the_ball
	if (ball->ballisdeadflag == 0)
	{
		/*Checks if player has fired the ball*/
		if (GetAsyncKeyState(ball->firebutton) && ball->ball_fired_flag == 0)
		{
			/*Temporary direction of the ball*/
			int direction;
			/*Sets the ball to auto shoot*/
			ball->auto_shoot_control = 0;
			/*updates the ball fired flag to has been shot*/
			ball->ball_fired_flag = 1;

			/*Plays the sound of the ball being fired*/
			loadSound(sound, "..\\CollisionDetectionTest\\SFX\\PlayerFire.wav");
			if (ball->ball_type == 1)
				playSound_Player1(sound, ball->friendplayer->x_p);
			else if (ball->ball_type == 2)
				playSound_Player2(sound, ball->friendplayer->x_p);

			direction = (rand() % 2);/*between bottom left and bottom right*/
			if (ball->ball_type == 2) direction += 2;/*Ensures ball2 moves only top left and top right*/

			ball->balldirection = direction;/*sets the balls direcction*/
			check = 0;/*ball has just been fired*/

		}
		/*checks if ball is not fired and thus moves with friendly player*/
		if (ball->ball_fired_flag == 0)
			Ball_MoveBallWithPlayer(ball, tileManager);

		/*ball has been fired, checks for collision detection and move accordingly*/
		else if (ball->ball_fired_flag == 1)
		{
			if ((ball->timer += 1 * Delta_Time(stage)) >= ball->frames)
			{
				/*Adjust speed of ball depending on the angles*/
				if (ball->balldirection >= BOTTOMRIGHT60 && ball->balldirection <= TOPLEFT60)
					ball->timer = ball->base_timer - 30;
				else if (ball->balldirection >= BOTTOMRIGHT20 && ball->balldirection <= TOPLEFT20)
					ball->timer = ball->base_timer - 15;
				else
					ball->timer = ball->base_timer;/*timer for 45 angles*/

				/*if ball has just been fired, do not do collision check*/
				if (check) Ball_MovementManager(sound, ball, tileManager, stage, enemy1, enemy2, enemy_AI1, enemy_AI2, powerup, enemyBullet);
			}
		}
	}

#pragma endregion

	/*if the ball has just been missed by the player, countdown till the next time player can use it*/
#pragma region Reviving_the_ball

	else if (ball->ballisdeadflag == 1)
	{
		/*updates the countup to the ball's revival time*/
		ball->timetorevival += 1 * Delta_Time(stage);
		/*increases the friendplayer's invulnerable time*/
		ball->friendplayer->invulnerable_time += 1 * Delta_Time(stage);

		/*if the ball has exceed the time and is ready to be revived*/
		if (ball->timetorevival >= timetorevive)
		{
			/*Sets the ball to not dead*/
			ball->ballisdeadflag = 0;
			
			/*prints ball back to player, readies it for firing, set direction to stop*/
			ball->ball_fired_flag = 0;
			Ball_MoveBallWithPlayer(ball, tileManager);
			ball->balldirection = STOP;

			/*rests the time to revival*/
			ball->timetorevival = 0;

			/* After player has missed, player can only hold on to the ball for a max of 4 seconds before it automatically fires again */
			ball->shoot_time = time(NULL) + 4;
			ball->auto_shoot_control = 1;

			/*Play the sound of the ball respawning*/
			loadSound(sound, "..\\CollisionDetectionTest\\SFX\\BallRespawn.wav");
			playSound_Low(sound);
		}
	}
#pragma endregion
}

/* Free the memory associated with a given Ball object */
void Ball_Free(Ball** Ballptr)
{
	/* safety check on the parameters */
	if ((Ballptr == NULL) || (*Ballptr == NULL))
	{
		return;  /* take no action if the parameters are invalid */
	}

	/* free the given item */
	free(*Ballptr);

	/* clear the provided pointer */
	*Ballptr = NULL;
}

/*Checks the direction of the ball and if it is, returns the direction of the ball, in correct angle if the checkdirection is correct*/
int Ball_DirectionChanger(Ball* ball, int CheckDirection, int DirectionChange, int ChangeAngleLater, Sound* sound)
{
	int temp_balldirection = STOP;
	int Checked = 0;
	int balldirection = ball->balldirection;

	/*Checks for singular directions in a particular angle*/
	if (CheckDirection >= 0)
	{
		if (balldirection == CheckDirection)
		{
			temp_balldirection = DirectionChange;
			Checked = 1;
		}
	}

	else
	{
		/*Checks for more than one direction*/
		switch (CheckDirection)
		{
		/*checks for topright directions in all angles*/
		case(General_TOPRIGHT):
			if (balldirection == TOPRIGHT20 || balldirection == TOPRIGHT45 || balldirection == TOPRIGHT60)
			{
				temp_balldirection = DirectionChange;
				Checked = 1;
			}
			break;
		/*checks for topleft directions in all angles*/
		case(General_TOPLEFT):
			if (balldirection == TOPLEFT20 || balldirection == TOPLEFT45 || balldirection == TOPLEFT60)
			{
				temp_balldirection = DirectionChange;
				Checked = 1;
			}
			break;
		/*checks for bottomright directions in all angles*/
		case(General_BOTTOMRIGHT):
			if (balldirection == BOTTOMRIGHT20 || balldirection == BOTTOMRIGHT45 || balldirection == BOTTOMRIGHT60)
			{
				temp_balldirection = DirectionChange;
				Checked = 1;
			}
			break;
		/*checks for bottomleft directions in all angles*/
		case(General_BOTTOMLEFT):
			if (balldirection == BOTTOMLEFT20 || balldirection == BOTTOMLEFT45 || balldirection == BOTTOMLEFT60)
			{
				temp_balldirection = DirectionChange;
				Checked = 1;
			}
			break;

		/*Checks for all topleft directions in all angles as well as up*/
		case(General_UPWARDLEFT):

			if (balldirection == TOPLEFT20 || balldirection == TOPLEFT45 || balldirection == TOPLEFT60
				|| balldirection == UP)
			{
				temp_balldirection = DirectionChange;
				Checked = 1;
			}
			break;
		/*Checks for all bottomleft directions in all angles as well as down*/
		case(General_DOWNWARDLEFT):
			if (balldirection == BOTTOMLEFT20 || balldirection == BOTTOMLEFT45 || balldirection == BOTTOMLEFT60
				|| balldirection == DOWN)
			{
				temp_balldirection = DirectionChange;
				Checked = 1;
			}
			break;
		
		/*Checks for all topright directions in all angles as well as up*/
		case(General_UPWARDRIGHT):

			if (balldirection == TOPRIGHT20 || balldirection == TOPRIGHT45 || balldirection == TOPRIGHT60
				|| balldirection == UP)
			{
				temp_balldirection = DirectionChange;
				Checked = 1;
			}
			break;
		
		/*Checks for all bottomright directions in all angles as well as down*/
		case(General_DOWNWARDRIGHT):
			if (balldirection == BOTTOMRIGHT20 || balldirection == BOTTOMRIGHT45 || balldirection == BOTTOMRIGHT60
				|| balldirection == DOWN)
			{
				temp_balldirection = DirectionChange;
				Checked = 1;
			}
			break;

		/*Checks for all upwards direction in all angles*/
		case(General_UPWARDS):

			if (balldirection == TOPRIGHT20 || balldirection == TOPRIGHT45 || balldirection == TOPRIGHT60
				|| balldirection == TOPLEFT20 || balldirection == TOPLEFT45 || balldirection == TOPLEFT60
				|| balldirection == UP)
			{
				temp_balldirection = DirectionChange;
				Checked = 1;
			}
			break;
			
		/*Checks for all downwards direction in all directions*/
		case(General_DOWNWARDS):
			if (balldirection == BOTTOMRIGHT20 || balldirection == BOTTOMRIGHT45 || balldirection == BOTTOMRIGHT60
				|| balldirection == BOTTOMLEFT20 || balldirection == BOTTOMLEFT45 || balldirection == BOTTOMLEFT60
				|| balldirection == DOWN)
			{
				temp_balldirection = DirectionChange;
				Checked = 1;
			}
			break;

		/*Checks for all directions travelling in the 45 degrees*/
		case(General_Angle45):
			if (balldirection >= BOTTOMRIGHT45 && balldirection <= TOPLEFT45)
			{
				temp_balldirection = DirectionChange;
				Checked = 1;
			}
			break;

		/*Checks for all directions travelling in the 60 degrees*/
		case(General_Angle60):
			if (balldirection >= BOTTOMRIGHT60 && balldirection <= TOPLEFT60)
			{
				temp_balldirection = DirectionChange;
				Checked = 1;
			}
			break;

		/*Checks for all directions travelling in the 20 degrees*/
		case(General_Angle20):
			if (balldirection >= BOTTOMRIGHT20 && balldirection <= TOPLEFT20)
			{
				temp_balldirection = DirectionChange;
				Checked = 1;
			}
			break;

		/*Checks for all directions*/
		case(General_AllDirections):
			if (balldirection >= BOTTOMRIGHT45 && balldirection <= TOPLEFT20)
			{
				temp_balldirection = DirectionChange;
				Checked = 1;
			}
			break;
		}
	}

	/*readjusts angle of ball*/
	if (ChangeAngleLater && Checked)
	{
		/*play the sound of the ball bouncing*/
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\Bounce.wav");
		playSound_Low(sound);

		/* adjust ball angle of the tempdirection if the ball angle is 60 degrees */
		if (ball->balldirection >= 4 && ball->balldirection <= 7)
			temp_balldirection += 4;
		/* adjust ball angle of the tempdirection  if the if ball angle is 20 degrees */
		else if (ball->balldirection >= 8 && ball->balldirection <= 11)
			temp_balldirection += 8;
	}

	/*return the temp_balldirection*/
	return temp_balldirection;
}

/*Functions changes the player's health and score accordingly*/
void Ball_AffectPlayerHealthandScore(Player* healthplayer1, int health1, Player* healthplayer2, int health2, Player* scoreplayer1, int score1, Player* scoreplayer2, int score2)
{
	/*Checks if the pointer is not a NULL pointer*/
	if (healthplayer1 != NULL)
	{
		/*Clears the health for redrawing*/
		Player_Clear_Health(healthplayer1);
		/*adjust the players health*/
		healthplayer1->health += health1;
	}
	/*Checks if the pointer is not a NULL pointer*/
	if (healthplayer2 != NULL)
	{
		/*Clears the health for redrawing*/
		Player_Clear_Health(healthplayer2);
		/*adjust the players health*/
		healthplayer2->health += health2;
	}

	/*Checks if the pointer is not a NULL pointer*/
	if (scoreplayer1 != NULL)
	{
		/*Change the player's score*/
		scoreplayer1->score += score1;
	}
	/*Checks if the pointer is not a NULL pointer*/
	if (scoreplayer2 != NULL)
	{
		/*Change the player's score*/
		scoreplayer2->score += score2;
	}
}
