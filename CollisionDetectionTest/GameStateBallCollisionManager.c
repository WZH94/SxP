/* Load Libraries */
#include <stdio.h>			/*printf*/
#include <windows.h>		/*GetAsyncKeyState*/
#include "Ball.h"			/*Ball direction*/
#include "Ball_CollisionManager.h"	/*Ball Collision Manager header file*/
#include "TileMapManager.h" /*Tile objects and data*/
#include "WorldData.h"		/* WorldData_GotoXY */
#include "EnemyAI.h"
#include "Powerups.h"
#include "EnemyBullets.h"

/* Size of screen */
#define SIZE_Y 78 /*IF SIZE_Y CHANGES, CHANGE TO LARGEST SIZE_Y*/

/*if the ball hits the borders, checks the bounce ball accordingly*/
void Ball_BorderCollision(Sound *sound, Ball* ball, Stage* stage, TileMapManager *tileManager)
{
	int direction = ball->balldirection; /*temporary value of direction since default angle is 45*/
	int ballhitflag = 0;/*if ball hits (ballhitflag = 1), change the angle of the ball*/

#pragma region CORNER_COLLISION

						/*TOP RIGHT CORNER*/
	if (ball->y_ballpos == stage->player1_start_posY && ball->x_ballpos == stage->size_x - 2)
	{
		if (ball->ball_type == 2)
		{
			if (ball->balldirection == UP)
			{
				direction = DOWN;
			}
			else
			{
				direction = BOTTOMLEFT45;
				ballhitflag = 1;
			}
		}
		else
		{
			ball->ballisdeadflag = 1;
			if (ball->timetorevival == 0)
			{
				/*Clears player health and sets new score and health*/
				Player_Clear_Health(ball->friendplayer);
				ball->friendplayer->health -= 1;
				ball->friendplayer->score -= 1;
			}
		}
	}

	/*TOP LEFT CORNER*/
	else if (ball->y_ballpos == stage->player1_start_posY && ball->x_ballpos == 1)
	{
		if (ball->ball_type == 2)
		{
			if (ball->balldirection == UP)
			{
				direction = DOWN;
			}
			else
			{
				direction = BOTTOMRIGHT45;
				ballhitflag = 1;
			}
		}
		else {
			ball->ballisdeadflag = 1;
			if (ball->timetorevival == 0)
			{
				/*Clears player health and sets new score and health*/
				Player_Clear_Health(ball->friendplayer);
				ball->friendplayer->health -= 1;
				ball->friendplayer->score -= 1;
			}
		}
	}

	/*BOTTOM RIGHT CORNER*/
	else if (ball->y_ballpos == stage->player2_start_posY + 1 && ball->x_ballpos == stage->size_x - 2)
	{
		if (ball->ball_type == 1)
		{
			if (ball->balldirection == DOWN)
				ball->balldirection = UP;
			else
			{
				ball->balldirection = TOPLEFT45;
				ballhitflag = 1;
			}
		}
		else {
			ball->ballisdeadflag = 1;
			if (ball->timetorevival == 0)
			{
				/*Clears player health and sets new score and health*/
				Player_Clear_Health(ball->friendplayer);
				ball->friendplayer->health -= 1;
				ball->friendplayer->score -= 1;
			}
		}
	}

	/*BOTTOM LEFT CORNER*/
	else if (ball->y_ballpos == stage->player1_start_posY && ball->x_ballpos == 1)
	{
		if (ball->ball_type == 1)
		{
			if (ball->balldirection == DOWN)
				ball->balldirection = UP;
			else
			{
				direction = TOPRIGHT45;
				ballhitflag = 1;
			}
		}
		else {
			ball->ballisdeadflag = 1;
			if (ball->timetorevival == 0)
			{
				/*Clears player health and sets new score and health*/
				Player_Clear_Health(ball->friendplayer);
				ball->friendplayer->health -= 1;
				ball->friendplayer->score -= 1;
			}
		}
	}
#pragma endregion

#pragma region GENERAL_BORDER_COLLISION

	/*checks if ball hits the top border*/
	else if (ball->y_ballpos == stage->player1_start_posY)
	{
		if (ball->ball_type == 1)
			/*if ball has hit the top border, it has missed its ball*/
		{
			ball->ballisdeadflag = 1;
			if (ball->timetorevival == 0)
			{
				/*Clears player health and sets new score and health*/
				Player_Clear_Health(ball->friendplayer);
				ball->friendplayer->health -= 1;
				ball->friendplayer->score -= 1;
			}


		}

		/*sets ball two's direction*/
		else if (direction == TOPRIGHT45 || direction == TOPRIGHT60 || direction == TOPRIGHT20)
		{
			direction = BOTTOMRIGHT45;
			ballhitflag = 1;
		}
		else if (direction == TOPLEFT45 || direction == TOPLEFT60 || direction == TOPLEFT20)
		{
			direction = BOTTOMLEFT45;
			ballhitflag = 1;
		}
		else if (direction == UP)
		{
			direction = DOWN;
		}
	}

	/*checks if ball1 hits the bottom border*/
	else if (ball->y_ballpos == stage->player2_start_posY + 1)
	{
		if (ball->ball_type == 2)/*if ball has hit the top border, it has missed its ball*/
		{

			if (ball->timetorevival == 0)
			{
				/*Clears player health and sets new score and health*/
				Player_Clear_Health(ball->friendplayer);
				ball->friendplayer->health -= 1;
				ball->friendplayer->score -= 1;
			}

			ball->ballisdeadflag = 1;


		}

		/*sets ball one's direction*/
		else if (direction == BOTTOMRIGHT45 || direction == BOTTOMRIGHT60 || direction == BOTTOMRIGHT20)
		{
			direction = TOPRIGHT45;
			ballhitflag = 1;
		}
		else if (direction == BOTTOMLEFT45 || direction == BOTTOMLEFT60 || direction == BOTTOMLEFT20)
		{
			direction = TOPLEFT45;
			ballhitflag = 1;
		}
		else if (direction == DOWN)
		{
			direction = UP;
		}

	}

	/*right border*/
	else if (ball->x_ballpos >= stage->size_x - 2)
	{
		if (direction == BOTTOMRIGHT45 || direction == BOTTOMRIGHT60 || direction == BOTTOMRIGHT20)
		{
			direction = BOTTOMLEFT45;
			ballhitflag = 1;
		}
		else if (direction == TOPRIGHT45 || direction == TOPRIGHT60 || direction == TOPRIGHT20)
		{
			direction = TOPLEFT45;
			ballhitflag = 1;
		}
	}

	/*left border*/
	else if (ball->x_ballpos == 1)
	{
		if (direction == BOTTOMLEFT45 || direction == BOTTOMLEFT60 || direction == BOTTOMLEFT20)
		{
			direction = BOTTOMRIGHT45;
			ballhitflag = 1;
		}
		else if (direction == TOPLEFT45 || direction == TOPLEFT60 || direction == TOPLEFT20)
		{
			direction = TOPRIGHT45;
			ballhitflag = 1;
		}
	}
#pragma endregion

	/*readjusts angle of ball*/
	if (ballhitflag == 1)
	{
		loadSound_Ball(sound, "..\\SFX\\Bounce.wav");
		playSound_Ball(sound);
		/* if ball angle is 60 degrees */
		if (ball->balldirection >= 4 && ball->balldirection <= 7)
			direction += 4;
		/* if ball angle i 20 degrees */
		else if (ball->balldirection >= 8 && ball->balldirection <= 11)
			direction += 8;
	}

	ball->balldirection = direction;
}

/*checks if ball collides with anything using TileMap, returns 1 if no collision*/
int Ball_CollisionManager(Sound *sound, TileMapManager* tilemap, Ball* ball, int current_x_pos, int current_y_pos, int next_x_ballpos, int next_y_ballpos,
	Stage* stage, Enemy* enemy1, Enemy* enemy2, Enemy_AI* enemy_AI1, Enemy_AI* enemy_AI2, Powerup *powerup, EnemyBullet *enemyBullet)
{
	int result;

	result = TileMapManager_Collision_Ball(tilemap, next_x_ballpos, next_y_ballpos);

	/* No collision */
	if (result == 0)
	{
		return 1; /* No collision */
	}


	/* Ball collided with border */
	else if (result == 6)
	{
		Ball_BorderCollision(sound, ball, stage, tilemap);
	}

	else if (result == 9)
	{
		ball->ballchar == "";
	}
	else
	{
		ball->ballchar == "O";
	}

	/* Collision detected */
	return 0;
}

