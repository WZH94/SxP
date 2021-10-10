/* Load Libraries */
#include <stdio.h>			/*printf*/
#include <windows.h>		/*GetAsyncKeyState*/
#include <time.h>			/*srand, Sleep*/
#include "TileMapManager.h" /*Tile objects and data*/
#include "WorldData.h"		/* WorldData_GotoXY */
#include "GameStateBallCollisionManager.h"			/*CollisionManager*/
#include "GameStateBall.h"

/* Create a new Item object with the provided data */
BallMainMenu* BallMainMenu_Create(int name, char ballcharacter, int x_start_pos, int y_start_pos, char fire)
{
	BallMainMenu *ball = NULL; /* the new item to be returned */

					   /* allocate the memory for a new object */
	ball = (BallMainMenu*)malloc(sizeof(BallMainMenu));

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
	ball->ball_type = name;
	ball->ballchar = ballcharacter;
	ball->firebutton = fire;
	ball->x_ballpos = x_start_pos;
	ball->y_ballpos = y_start_pos;
	ball->start_y_ballpos = y_start_pos;
	ball->ball_fired_flag = 0;
	ball->balldirection = STOP;
	ball->frames = 5;					  /* Updates every 5 frames, WILL NOT CHANGE */
	ball->base_timer = 2;				  /* Adjusts the speed, ex. if increases to 1, takes 1 less frame to update */
	ball->timer = ball->base_timer;		  /* Timer always starts from base_timer's value */
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


/*Moves ball accordingly, and checks for collision everytime before it moves in a certain direction*/
void BallMainMenu_MovementManager(Sound *sound, BallMainMenu* ball, TileMapManager* tilemap, Stage* stage, Enemy* enemy1[], Enemy* enemy2[], Enemy_AI* enemy_AI1, Enemy_AI* enemy_AI2,
	Powerup *powerup, EnemyBullet *enemyBullet)
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
	printf(" ");

	/*Switch case checking what direction to move the ball in*/
	switch (ball->balldirection)
	{
		/*45 degrees moves ball one space in each direction*/
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
		/*--ballx;
		if (ballcollisiondetection(sound, tilemap, ball, ball->x_ballpos, ball->y_ballpos, ballx, bally, stage, enemy1, enemy2, enemy_AI1, enemy_AI2, powerup, enemyBullet))
		ball->x_ballpos = ballx;
		++bally;
		if (ballcollisiondetection(sound, tilemap, ball, ball->x_ballpos, ball->y_ballpos, ballx, bally, stage, enemy1, enemy2, enemy_AI1, enemy_AI2, powerup, enemyBullet))
		ball->y_ballpos = bally;*/
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

		/*60 degrees moves ball one space in in x direction, two in y*/
	case(BOTTOMRIGHT60):
		++ballx;
		//if (ballcollisiondetection(sound, tilemap, ball, ball->x_ballpos, ball->y_ballpos, ballx, bally, stage, enemy1, enemy2, enemy_AI1, enemy_AI2, powerup, enemyBullet))
		//	ball->x_ballpos = ballx;
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
		//if (ballcollisiondetection(sound, tilemap, ball, ball->x_ballpos, ball->y_ballpos, ballx, bally, stage, enemy1, enemy2, enemy_AI1, enemy_AI2, powerup, enemyBullet))
		//	ball->x_ballpos = ballx;
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
		//if (ballcollisiondetection(sound, tilemap, ball, ball->x_ballpos, ball->y_ballpos, ballx, bally, stage, enemy1, enemy2, enemy_AI1, enemy_AI2, powerup, enemyBullet))
		//	ball->x_ballpos = ballx;
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
		--ballx;/*
				if (ballcollisiondetection(sound, tilemap, ball, ball->x_ballpos, ball->y_ballpos, ballx, bally, stage, enemy1, enemy2, enemy_AI1, enemy_AI2, powerup, enemyBullet))
				ball->x_ballpos = ballx;*/
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


		/*20 degrees moves ball one space in in y direction, two in x*/
	case(BOTTOMRIGHT20):

		++bally;
		//if (ballcollisiondetection(sound, tilemap, ball, ball->x_ballpos, ball->y_ballpos, ballx, bally, stage, enemy1, enemy2, enemy_AI1, enemy_AI2, powerup, enemyBullet))
		//	ball->y_ballpos = bally;
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
		//if (ballcollisiondetection(sound, tilemap, ball, ball->x_ballpos, ball->y_ballpos, ballx, bally, stage, enemy1, enemy2, enemy_AI1, enemy_AI2, powerup, enemyBullet))
		//	ball->y_ballpos = bally;

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
		//if (ballcollisiondetection(sound, tilemap, ball, ball->x_ballpos, ball->y_ballpos, ballx, bally, stage, enemy1, enemy2, enemy_AI1, enemy_AI2, powerup, enemyBullet))
		//	ball->y_ballpos = bally;
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
		//if (ballcollisiondetection(sound, tilemap, ball, ball->x_ballpos, ball->y_ballpos, ballx, bally, stage, enemy1, enemy2, enemy_AI1, enemy_AI2, powerup, enemyBullet))
		//	ball->y_ballpos = bally;
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

	}


	/*Draws ball at new position as well as update ball's position*/
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
	printf("%c", ball->ballchar);
	SetConsoleTextAttribute(hConsole, saved_attributes);
}

/*Updates the ball*/
void BallMainMenu_Update(Sound *sound, BallMainMenu* ball, TileMapManager *tileManager, Stage* stage, Enemy* enemy1[], Enemy* enemy2[], Enemy_AI* enemy_AI1, Enemy_AI* enemy_AI2,
	Powerup* powerup, EnemyBullet *enemyBullet)
{
	int check = 1;/*checks if the ball has just been fired*/

				  /*Checks if player has fired the ball*/
	if (GetAsyncKeyState(ball->firebutton) && ball->ball_fired_flag == 0)
	{
		int direction;

		ball->ball_fired_flag = 1;/*updates the ball fired flag*/

		loadSound_Ball(sound, "..\\SFX\\PlayerFire.wav");
		playSound_Ball(sound);

		/*Randomises the balls initial movement*/
		srand(time(NULL));
		direction = (rand() % 2);/*between bottom left and bottom right*/
		if (ball->ball_type == 2) direction += 2;/*Ensures ball2 moves only top left and top right*/
												 //if (ball->ball_type == 2) direction = UP;
												 //if (ball->ball_type == 1) direction = DOWN;

		ball->balldirection = direction;/*sets the balls direcction*/
		check = 0;/*ball has just been fired*/

	}
	if (ball->ballisdeadflag == 0)
	{
		/*checks if ball is not fired and moves with friendly player*/
		if (ball->ball_fired_flag == 0)
			Ball_MoveBallWithPlayer(ball, tileManager);

		/*ball has been fired, checks for collision detection and move accordingly*/
		else if (ball->ball_fired_flag == 1)
		{
			if (++ball->timer >= ball->frames)
			{
				if (ball->balldirection >= 4 && ball->balldirection <= 7)
					ball->timer = ball->base_timer - ball->base_timer;
				else
					ball->timer = ball->base_timer;
				if (check) Ball_MovementManager(sound, ball, tileManager, stage, enemy1, enemy2, enemy_AI1, enemy_AI2, powerup, enemyBullet);
			}
		}
	}
}

/* Free the memory associated with a given Enemy object */
void BallMainMenu_Free(BallMainMenu** Ballptr)
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