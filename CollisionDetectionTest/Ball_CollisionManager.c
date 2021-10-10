/******************************************************************************
All content © 2017 Digipen Institute of Technology Singapore, all rights reserved

filename			Ball_CollisionManager.c
author				Enriquez, Audry May dela Paz
DP email			a.enriquez@digipen.edu
course				BSGD

Brief Description:
Fuctions for the collision detection for ball
******************************************************************************/

/* Load Libraries */
#include <stdio.h>					/*printf*/
#include <windows.h>				/*GetAsyncKeyState*/
#include "Ball.h"					/*Ball direction*/
#include "Ball_CollisionManager.h"	/*Ball Collision Manager header file*/
#include "TileMapManager.h"			/*Tile objects and data*/
#include "WorldData.h"				/* WorldData_GotoXY */
#include "EnemyAI.h"				/*EnemyAi objects and data*/
#include "Powerups.h"				/*Powerups objects and data*/
#include "EnemyBullets.h"			/*Enemy bullet objects and data*/
#include "EnemyTypes.h"				/*Enemy types objects and data*/
#include "Hydra.h"					/*Hydra objects and data*/
#include "Player.h"					/* Player objects and data */
#include "Stage.h"					/* Stage objects and data */
#include "FMODInit.h"				/* Sound objects and data */
#include "Enemy.h"					/* Enemy objects and data */
#include "stdafx.h"



/*Scores in the game the ball can affect*/
#define scoreBallhitsenemyplayer            200
#define scorePlayerMissedBall               -50
#define scoreBallhitsenemy                   15

/* Size of screen */
#define bat_length 20 

/*If ball has hit the player, function does the appropriate actions*/
void Ball_PlayerCollision(Sound *sound, Ball* ball, Player* player, Stage* stage, TileMapManager *tileManager)
{
	int bat_positions[bat_length] = {0};	/*array holding the position of the ball, its size corresponds to the largest size of y*/
	int iteration = 0;					/*loop counter*/
	int position = 0;					/*position of player being considered*/
	int new_direction = STOP;		/*if ball has not hit bat, it will stop*/

	/*initialise bat_position array with respective position of ball*/
	for (position = player->x_p - 1, iteration = 0; iteration < (player->x_l + 2); ++iteration, ++position)
	{
		bat_positions[iteration] = position;
	}

#pragma region CENTER_COLLISION
	
	/*checks if ball has hit center, sets UP/DOWN*/
	if (ball->x_ballpos == bat_positions[player->x_l / 2 + 1])
	{
		/*play the sound of the collision*/
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\PH90.wav");
		if (ball->ball_type == 1)
			playSound_Player1(sound, player->x_p);
		else if (ball->ball_type == 2)
			playSound_Player2(sound, player->x_p);

		/*checks the direction of the ball, setting it to either up or down*/
		if ((new_direction = Ball_DirectionChanger(ball, General_UPWARDS, DOWN, DoNotChangeAngle, sound)) != STOP);
		else ((new_direction = Ball_DirectionChanger(ball, General_DOWNWARDS, UP, DoNotChangeAngle, sound)) != STOP);

	}
#pragma endregion

#pragma region CORNERS_COLLISION
	/*checks if ball has hit the left outermost corners, set 20, reflect directions*/
	else if (ball->x_ballpos == bat_positions[0])
	{
		/*play the sound for the collision*/
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\PH20.wav");
		if (ball->ball_type == 1)
			playSound_Player1(sound, player->x_p);
		else if (ball->ball_type == 2)
			playSound_Player2(sound, player->x_p);

		/*Checks if the ball hits the top corner of the player*/
		if (ball->y_ballpos == stage->ball1_start_posY || ball->y_ballpos == stage->ball2_start_posY)
		{
			/*Change the direction of the ball to an angle of 20 if the ball is travelling towards the left of the player*/
			if ((new_direction = Ball_DirectionChanger(ball, General_UPWARDLEFT, BOTTOMLEFT20, DoNotChangeAngle, sound)) != STOP);
			else if ((new_direction = Ball_DirectionChanger(ball, General_DOWNWARDLEFT, TOPLEFT20, DoNotChangeAngle, sound)) != STOP);
		}
		/*Checks if the ball hits the side of the player and st the angle of 20 if the ball is travelling towards the right of the player*/
		if ((new_direction = Ball_DirectionChanger(ball, General_UPWARDRIGHT, BOTTOMLEFT20, DoNotChangeAngle, sound)) != STOP);
		else if ((new_direction = Ball_DirectionChanger(ball, General_DOWNWARDRIGHT, TOPLEFT20, DoNotChangeAngle, sound)) != STOP);

	}
	/*checks if ball has hit the right outermost corners, set 20, reflect directions*/
	else if (ball->x_ballpos == bat_positions[player->x_l + 1])
	{
		/*Play the sound for the collision*/
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\PH20.wav");
		if (ball->ball_type == 1)
			playSound_Player1(sound, player->x_p);
		else if (ball->ball_type == 2)
			playSound_Player2(sound, player->x_p);

		/*Checks if it hits the top of the player at a rightwards direction and set the angle to 20*/
		if (ball->y_ballpos == stage->ball1_start_posY || ball->y_ballpos == stage->ball2_start_posY)
		{
			if ((new_direction = Ball_DirectionChanger(ball, General_UPWARDRIGHT, BOTTOMRIGHT20, DoNotChangeAngle, sound)) != STOP);
			else if ((new_direction = Ball_DirectionChanger(ball, General_DOWNWARDRIGHT, TOPRIGHT20, DoNotChangeAngle, sound)) != STOP);
		}
		
		/*Checks if hits the side of the player at a leftwards direction and set the angle to 20*/
		if ((new_direction = Ball_DirectionChanger(ball, General_UPWARDLEFT, BOTTOMRIGHT20, DoNotChangeAngle, sound)) != STOP);

		else if ((new_direction = Ball_DirectionChanger(ball, General_DOWNWARDLEFT, TOPRIGHT20, DoNotChangeAngle, sound)) != STOP);

	}
#pragma endregion

#pragma region OFF_CENTER_COLLISION

	/*checks if ball has hit bat on left side of the center, set 60 degrees leftward */
	else if (player->x_l > 3 && ball->x_ballpos == bat_positions[(player->x_l / 2)])
	{
		/*Play the sound*/
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\PH60.wav");
		if (ball->ball_type == 1)
			playSound_Player1(sound, player->x_p);
		else if (ball->ball_type == 2)
			playSound_Player2(sound, player->x_p);

		/*Checks the direction of the ball and sends it back leftwards at a 60 degrees angle*/
		if ((new_direction = Ball_DirectionChanger(ball, General_DOWNWARDS, TOPLEFT60, DoNotChangeAngle, sound)) != STOP);
		else if ((new_direction = Ball_DirectionChanger(ball, General_UPWARDS, BOTTOMLEFT60, DoNotChangeAngle, sound)) != STOP);
	}
	/*checks if ball has hit bat on right side of the center, set 60 degrees rightward */
	else if (player->x_l > 3 && ball->x_ballpos == bat_positions[(player->x_l / 2 + 2)])
	{
		/*Play the sound*/
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\PH60.wav");
		if (ball->ball_type == 1)
			playSound_Player1(sound, player->x_p);
		else if (ball->ball_type == 2)
			playSound_Player2(sound, player->x_p);

		/*Checks the direction of the ball and sends it back rightwards at a 60 degrees angle*/
		if ((new_direction = Ball_DirectionChanger(ball, General_DOWNWARDS, TOPRIGHT60, DoNotChangeAngle, sound)) != STOP);
		else if ((new_direction = Ball_DirectionChanger(ball, General_UPWARDS, BOTTOMRIGHT60, DoNotChangeAngle, sound)) != STOP);

	}

#pragma endregion

#pragma region everywhere_else_collision

	/*checks if ball has hit bat on left side , set 45 degrees leftward */
	else if (ball->x_ballpos >= player->x_p  && ball->x_ballpos < player->x_p + player->x_l / 2)
	{
		/*Play the sound*/
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\PH45.wav");
		if (ball->ball_type == 1)
			playSound_Player1(sound, player->x_p);
		else if (ball->ball_type == 2)
			playSound_Player2(sound, player->x_p);

		/*Checks the direction of the ball and send it back leftwardsat a 45 degrees angle*/
		if ((new_direction = Ball_DirectionChanger(ball, General_DOWNWARDS, TOPLEFT45, DoNotChangeAngle, sound)) != STOP);
		else if ((new_direction = Ball_DirectionChanger(ball, General_UPWARDS, BOTTOMLEFT45, DoNotChangeAngle, sound)) != STOP);
	}

	/*checks if ball has hit bat on right side, set 45 degrees rightward */
	else
	{
		/*Play the sound*/
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\PH45.wav");
		if (ball->ball_type == 1)
			playSound_Player1(sound, player->x_p);
		else if (ball->ball_type == 2)
			playSound_Player2(sound, player->x_p);

		/*Checks the direction of the ball and send it back rightwards at a 45 degree angle*/
		if ((new_direction = Ball_DirectionChanger(ball, General_DOWNWARDS, TOPRIGHT45, DoNotChangeAngle, sound)) != STOP);
		else if ((new_direction = Ball_DirectionChanger(ball, General_UPWARDS, BOTTOMRIGHT45, DoNotChangeAngle, sound)) != STOP);

	}

#pragma endregion

	/*If the ball collides with the player or not or not, check what to do with it*/

	/*Checks if there is no collision between the player and the ball*/
	if (new_direction == STOP)
	{
		/*If the ball that was missed is the players' or it was the other player but during CoOp mode, the player(s) will lose score*/
		if (player == ball->friendplayer || player == ball->enemyplayer && stage->play_type == CoOp)
		{
			/*Updates the ball's position in the tile manager*/
			TileMapManager_Ball(ball, tileManager, 0);
			{
				/*If PvP, affect the friend score and decrease his health*/
				if (stage->play_type == PvP)
					Ball_AffectPlayerHealthandScore(ball->friendplayer, -1, NULL, 0, ball->friendplayer, scorePlayerMissedBall, NULL, 0);
				
				/*If CoOp, affect both player healths but do nothing to score*/
				else if (stage->play_type == CoOp)
					Ball_AffectPlayerHealthandScore(ball->friendplayer, -1, ball->enemyplayer, -1, NULL, 0, NULL, 0);

			}
		}
		/* else if ball has missed enemyplayer, do nothing with the direction of the ball*/
	}
	/*if ball has hit the enemy player*/
	else if (player == ball->enemyplayer)
	{
		/*Time the player is not affected by the ball - prevents player from taking extra damage in case ball glitches*/
		if (player->invulnerable_time <= 0)
		{
			/*If PvP*/
			if (stage->play_type == PvP)
			{
				/*Affect the enemy players health, decrease his score and increase the friendplayers score by the same amount*/
				Ball_AffectPlayerHealthandScore(ball->enemyplayer, -1, NULL, 0, ball->friendplayer, scoreBallhitsenemyplayer, ball->enemyplayer, -scoreBallhitsenemyplayer);
				
				/*Play the sound of the player getting hit*/
				loadSound(sound, "..\\CollisionDetectionTest\\SFX\\PlayerGetHit.wav");
				if (ball->ball_type == 1)
					playSound_Player2(sound, player->x_p);
				else if (ball->ball_type == 2)
					playSound_Player1(sound, player->x_p);
			}
			/*else if (stage->play_type == CoOp) do nothing*/

			/*Changes ball direction*/
			ball->balldirection = new_direction;

			/*Set the invulnerable time to 50*/
			player->invulnerable_time = 50;
		}
	}
	/*if ball hits the friend player, set new direction*/
	else ball->balldirection = new_direction;
}

/*if the ball hits the borders, checks the bounce ball accordingly*/
void Ball_BorderCollision(Sound *sound, Ball* ball, Stage* stage)
{
	int direction = STOP; /*temporary value of direction*/
	
	/*Checks if the ball hits the topright corner of the screen directly*/
	if (ball->y_ballpos == stage->player1_start_posY && ball->x_ballpos == stage->size_x - 2)
	{
		/*If the ball belongs to player 2 and the game is in PvP mode or it is the main menu ball, send the ball back*/
		if (ball->ball_type == 2 && stage->play_type == PvP || ball->ball_type == 3)
		{
			/*If the ball's direction is UP, send it back DOWN*/
			if ((direction = Ball_DirectionChanger(ball, UP, DOWN, DoNotChangeAngle, sound)) != STOP);
			/*else set the ball's direction to bottom left and send it at the same angle in which it came from*/
			else if ((direction = Ball_DirectionChanger(ball, General_AllDirections, BOTTOMLEFT45, ChangeAngle, sound)) != STOP);
		}
		else
		{
			/*The player has missed the ball, do necessary functions*/
			Ball_isdeadPlayerMissed(ball, stage, sound);
		}
	}

	/*Checks if the ball hits the topleft corner of the screen directly*/
	else if (ball->y_ballpos == stage->player1_start_posY && ball->x_ballpos == 1)
	{
		/*If the ball belongs to player 2 and the game is in PvP mode or it is the main menu ball, send the ball back*/
		if (ball->ball_type == 2 && stage->play_type == PvP || ball->ball_type == 3)
		{
			/*If the ball's direction is UP, send it back DOWN*/
			if ((direction = Ball_DirectionChanger(ball, UP, DOWN, DoNotChangeAngle, sound)) != STOP);
			/*else set the ball's direction to bottomright and send it at the same angle in which it came from*/
			else if ((direction = Ball_DirectionChanger(ball, General_AllDirections, BOTTOMRIGHT45, ChangeAngle, sound)) != STOP);

		}
		else
		{
			/*The player has missed the ball, do necessary functions*/
			Ball_isdeadPlayerMissed(ball, stage, sound);
		}
	}

	/*Checks if the ball is hit at the BOTTOM RIGHT CORNER*/
	else if (ball->y_ballpos == stage->player2_start_posY + 1 && ball->x_ballpos == stage->size_x - 2)
	{
		/*If the ball belongs to player 1 and the game is in PvP mode or it is the main menu ball, send the ball back*/
		if (ball->ball_type == 1 && stage->play_type == PvP || ball->ball_type == 3)
		{
			/*If the ball's direction is DOWN, send it back UP*/
			if ((direction = Ball_DirectionChanger(ball, DOWN, UP, DoNotChangeAngle, sound)) != STOP);
			/*else set the ball's direction to topleft and send it at the same angle in which it came from*/
			else if ((direction = Ball_DirectionChanger(ball, General_AllDirections, TOPLEFT45, ChangeAngle, sound)) != STOP);

		}
		else 
		{
			/*The player has missed the ball, do necessary functions*/
			Ball_isdeadPlayerMissed(ball, stage, sound);
		}
	}

	/*Checks if the ball hits the bottomleft corner of the screen directly*/
	else if (ball->y_ballpos == stage->player2_start_posY + 1 && ball->x_ballpos == 1)
	{
		/*If the ball belongs to player 1 and the game is in PvP mode or it is the main menu ball, send the ball back*/
		if (ball->ball_type == 1 && stage->play_type == PvP || ball->ball_type == 3)
		{
			/*If the ball's direction is DOWN, send it back UP*/
			if ((direction = Ball_DirectionChanger(ball, DOWN, UP, DoNotChangeAngle, sound)) != STOP);
			/*else set the ball's direction to topright and send it at the same angle in which it came from*/
			else if ((direction = Ball_DirectionChanger(ball, General_AllDirections, TOPRIGHT45, ChangeAngle, sound)) != STOP);
		}
		else
		{
			/*The player has missed the ball, do necessary functions*/
			Ball_isdeadPlayerMissed(ball, stage, sound);
		}
	}

	/*checks if ball hits the top border*/
	else if (ball->y_ballpos == stage->player1_start_posY)
	{
		/*if ball has hit the top border, player 1 has missed its ball*/
		if (ball->ball_type == 1 || ball->ball_type == 2 && stage->play_type == CoOp)
			Ball_isdeadPlayerMissed(ball, stage, sound);
		
		/*else change ball 2 directions in PvP, in the same angle they came from */
		else if ((direction = Ball_DirectionChanger(ball, UP, DOWN, DoNotChangeAngle, sound)) != STOP);
		else if ((direction = Ball_DirectionChanger(ball, General_TOPRIGHT, BOTTOMRIGHT45, ChangeAngle, sound)) != STOP);
		else if ((direction = Ball_DirectionChanger(ball, General_TOPLEFT, BOTTOMLEFT45, ChangeAngle, sound)) != STOP);

	}

	/*checks if ball1 hits the bottom border*/
	else if (ball->y_ballpos == stage->player2_start_posY + 1)
	{
		/*if ball has hit the bottom border, it has missed its ball*/
		if (ball->ball_type == 2 || ball->ball_type == 1 && stage->play_type == CoOp)
			Ball_isdeadPlayerMissed(ball, stage, sound);

		/*sets ball one's direction in PvP, in the same angle they came from */
		else if ((direction = Ball_DirectionChanger(ball, DOWN, UP, DoNotChangeAngle, sound)) != STOP);
		else if ((direction = Ball_DirectionChanger(ball, General_BOTTOMRIGHT, TOPRIGHT45, ChangeAngle, sound)) != STOP);
		else if ((direction = Ball_DirectionChanger(ball, General_BOTTOMLEFT, TOPLEFT45, ChangeAngle, sound)) != STOP);

	}

	/*checks if ball hits the right border*/
	else if (ball->x_ballpos >= stage->size_x - 2)
	{
		/*sets the direction of the ball, sending it back in the same angle it came from*/
		if ((direction = Ball_DirectionChanger(ball, General_BOTTOMRIGHT, BOTTOMLEFT45, ChangeAngle, sound)) != STOP);
		else if ((direction = Ball_DirectionChanger(ball, General_TOPRIGHT, TOPLEFT45, ChangeAngle, sound)) != STOP);

	}

	/*checks if ball hits the left border*/
	else if (ball->x_ballpos == 1)
	{
		/*sets the direction of the ball, sending it back in the same angle it came from*/
		if ((direction = Ball_DirectionChanger(ball, General_BOTTOMLEFT, BOTTOMRIGHT45, ChangeAngle, sound)) != STOP);
		else if ((direction = Ball_DirectionChanger(ball, General_TOPLEFT, TOPRIGHT45, ChangeAngle, sound)) != STOP);

	}

	/*sets the ball direction to the resulting direction*/
	ball->balldirection = direction;
}

/*checks if ball collides with anything using TileMap, returns 1 if no collision*/
int Ball_CollisionManager(Sound *sound, TileMapManager* tilemap, Ball* ball, int current_x_pos, int current_y_pos, int next_x_ballpos, int next_y_ballpos,
	Stage* stage, Enemy* enemy1[], Enemy* enemy2[], Enemy_AI* enemy_AI1, Enemy_AI* enemy_AI2, Powerup *powerup[], EnemyBullet *enemyBullet[])
{
	/*Gives back the number, according to the tileManager, which the next position of the ball will go to*/
	int result = TileMapManager_Collision_Ball(tilemap, next_x_ballpos, next_y_ballpos);

	/*Checks if ball has previously been ball hit by another ball*/
	if (ball->ballhitballflag == 1)
	{
		/*Reflects the direction of the ball back at 20 degrees*/
		Ball_BallCollision(ball, sound);

		/*Sets the ball hit ball flag back to unhit*/
		ball->ballhitballflag = 0;
		return 0;
	}

	/* No collision */
	else if (result == 0)
	{
		/*If the ball is the mainmenu ball, sets the ball char*/	
		if (ball->ball_type == 3)
			ball->ballchar = 'O';
		return 1; /* No collision */
	}

	/* Ball collided with powerup */
	else if (result == 7)
	{
		/*function found in powerups.c*/
		Ball_Powerup_CollisionDetection(next_x_ballpos, next_y_ballpos, stage, tilemap, sound, ball, powerup, enemy1, enemy2, enemy_AI1, enemy_AI2);

		return 1; /* No collision */
	}

	/* Ball collided with enemy bullet */
	else if (result == 8)
	{
		/*function found in EnemyBullet.c*/
		Ball_Bullet_Collision(next_x_ballpos, next_y_ballpos, tilemap, sound, enemyBullet);

		return 1; /* No collision */
	}

	/* Ball collided with text */
	else if (result == 9)
	{
		/*Do not let the ball print*/
		ball->ballchar = 0;
		return 1;
	}

	else if (result > 0 && result < 7)
	{
		/* Ball collided with Player 1 */
		if (result == 1)
		{
			/* If ball belongs to player 1, do collision test with friendplayer. 
			else, checks if the ball belongs to the second player, do collision test with enemyplayer*/
			if (ball->ball_type == 1) Ball_PlayerCollision(sound, ball, ball->friendplayer, stage, tilemap);
			else if (ball->ball_type == 2) Ball_PlayerCollision(sound, ball, ball->enemyplayer, stage, tilemap);
		}

		/* Ball collided with Player 2 */
		else if (result == 2)
		{
			/* If ball belongs to player 1, do collision test with enemyplayer.
			else, checks if the ball belongs to the second player, do collision test with friendplayer*/
			if (ball->ball_type == 1)Ball_PlayerCollision(sound, ball, ball->enemyplayer, stage, tilemap);
			else if (ball->ball_type == 2) Ball_PlayerCollision(sound, ball, ball->friendplayer, stage, tilemap);
		}

		/* Ball collided with enemy */
		else if (result == 3)
		{
			/*Do collision test with enemy - check for both enemies*/
			Ball_Enemy_CollisionDetection(current_x_pos, current_y_pos, next_x_ballpos, next_y_ballpos, stage, enemy1, enemy_AI1, tilemap, sound, ball, powerup);
			Ball_Enemy_CollisionDetection(current_x_pos, current_y_pos, next_x_ballpos, next_y_ballpos, stage, enemy2, enemy_AI2, tilemap, sound, ball, powerup);
		}

		/* Ball collided with another ball*/
		else if (result == 4 || result == 5)
		{
			/*sets the other ball's ballhitballflag to hit*/
			ball->enemyball->ballhitballflag = 1;
			/*reflects the ball's direction at 20 degrees*/
			Ball_BallCollision(ball, sound);
		}

		/* Ball collided with border */
		else if (result == 6)
		{
			/*Checks which border the ball hits and act accordinly*/
			Ball_BorderCollision(sound, ball, stage);
		}

		/* Collision detected */
		return 0;
	}
	/*If ball does not find anything known, redraws ball to player -- ball is out of bounds for some unknown reason*/
	else
	{
		/*sets the ball's direction to stop, not fired but not dead*/
		ball->balldirection = STOP;
		ball->ball_fired_flag = 0;
		ball->ballisdeadflag = 0;
		/*redraws ball to the player*/
		Ball_MoveBallWithPlayer(ball, tilemap);

		/*play the sound of the ball respawning*/
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\BallRespawn.wav");
		playSound_Low(sound);

	}
	/* Collision detected */
	return 0;
}


/* Handles the functions that happen when the ball hits an enemy */
void Ball_Enemy_CollisionDetection(int current_x_pos, int current_y_pos, int x_pos, int y_pos, Stage* stage, Enemy* enemy[], Enemy_AI* enemy_AI, TileMapManager *tileManager,
	Sound *sound, Ball *ball, Powerup *powerup[])
{
	int j;
	for (j = 0; j < stage->total_and_hydra; ++j)
	{
		if (enemy[j] != NULL)
		{
			/* Check if the ball hit the enemy */
			if ((enemy[j]->id == 1 && x_pos >= enemy[j]->x_Pos && x_pos < enemy[j]->x_Pos + ENEMY_WIDTH
				&& y_pos >= enemy[j]->y_Pos && y_pos < enemy[j]->y_Pos + ENEMY_HEIGHT) ||
				(enemy[j]->id == 2 && x_pos >= enemy[j]->x_Pos && x_pos < enemy[j]->x_Pos + ENEMY_WIDTH
					&& y_pos <= enemy[j]->y_Pos && y_pos > enemy[j]->y_Pos - ENEMY_HEIGHT))
			{
				int enemy_x_pos = enemy[j]->x_Pos;
				int enemy_y_pos = enemy[j]->y_Pos;

				/* Change score */
				ball->friendplayer->score += scoreBallhitsenemy;

				--enemy[j]->HP;		   /* Drops enemy HP */
				enemy[j]->full_HP = 0; /* Enemy no longer at full HP */

				/* If HP not depleted, change its art */
				if (enemy[j]->HP > 0)
				{
					Enemy_IndvClear(enemy[j], enemy_AI, tileManager);
					EnemyTypeHP_ArtSwitch(enemy[j]);
					Enemy_IndvPrint(enemy[j], enemy_AI, tileManager);
				}

				/* HP depleted, make enemy dead and clear it from the field permanantely */
				else if (enemy[j]->HP == 0)
				{
					enemy[j]->alive = 0;

					Powerup_Spawner(powerup, enemy[j], ball);
					Enemy_IndvClear(enemy[j], enemy_AI, tileManager);
					if (enemy[j]->id == 1)
						TileMapManager_Enemy(enemy[j], 1, tileManager, 0);
					else if (enemy[j]->id == 2)
						TileMapManager_Enemy(enemy[j], -1, tileManager, 0);

					if (enemy[j]->is_hydra == 1)
						Hydra_Die(stage, &enemy[0], enemy_AI, tileManager, j, sound);
					/* Enemy died, play sound */
					loadSound(sound, "..\\CollisionDetectionTest\\SFX\\EnemyDie.wav");
					playSound_Low(sound);
				}

				//WorldData_GoToXY(ball->x_ballpos, ball->y_ballpos);
				//printf("%c", ball->ballchar);

				/* Changes the direction of the ball's bounce depending on which part of the enemy it hit,
				ball hit top and bottom of enemy */
				if (((y_pos == enemy_y_pos || y_pos == (enemy_y_pos + ((ENEMY_HEIGHT - 1) * enemy_AI->p_mult))) &&
					(x_pos >= enemy_x_pos && x_pos <= enemy_x_pos + ENEMY_WIDTH - 1)) &&
					(current_x_pos >= enemy_x_pos && current_x_pos <= enemy_x_pos + ENEMY_WIDTH - 1)
					)
				{
					int direction = STOP;

					if ((direction = Ball_DirectionChanger(ball, General_TOPRIGHT, BOTTOMRIGHT45, ChangeAngle, sound)) != STOP);
					else if ((direction = Ball_DirectionChanger(ball, General_TOPLEFT, BOTTOMLEFT45, ChangeAngle, sound)) != STOP);
					else if ((direction = Ball_DirectionChanger(ball, General_BOTTOMRIGHT, TOPRIGHT45, ChangeAngle, sound)) != STOP);
					else if ((direction = Ball_DirectionChanger(ball, General_BOTTOMLEFT, TOPLEFT45, ChangeAngle, sound)) != STOP);

					else if (ball->balldirection == DOWN)
					{
						if (x_pos == enemy_x_pos)               	direction = TOPLEFT20;
						else if (x_pos == enemy_x_pos + 1)          direction = TOPLEFT45;
						else if (x_pos == enemy_x_pos + 2)          direction = TOPRIGHT45;
						else if (x_pos == enemy_x_pos + 3)          direction = TOPRIGHT20;

					}
					else if (ball->balldirection == UP)
					{
						if (x_pos == enemy_x_pos)                   direction = BOTTOMLEFT20;
						else if (x_pos == enemy_x_pos + 1)          direction = BOTTOMLEFT45;
						else if (x_pos == enemy_x_pos + 2)          direction = BOTTOMRIGHT45;
						else if (x_pos == enemy_x_pos + 3)          direction = BOTTOMRIGHT20;
					}

					ball->balldirection = direction;
				}

				/* Ball hit the enemy on the sides */
				else if (
					(enemy[j]->id == 1 && y_pos >= enemy_y_pos && y_pos <= (enemy_y_pos + ENEMY_HEIGHT - 1) && current_y_pos >= enemy_y_pos && current_y_pos <=
					(enemy_y_pos + ENEMY_HEIGHT - 1)) /*checks for enemies with id 1*/

					|| (enemy[j]->id == 2 && y_pos <= enemy_y_pos && y_pos >= (enemy_y_pos - ENEMY_HEIGHT - 1) &&
						current_y_pos <= enemy_y_pos && current_y_pos >= (enemy_y_pos - ENEMY_HEIGHT - 1)) &&
						(current_x_pos < enemy_x_pos || current_x_pos > enemy_x_pos + ENEMY_WIDTH - 1 &&
					(x_pos == enemy_x_pos || x_pos == enemy_x_pos + ENEMY_WIDTH - 1))/*checks for enemies with id 2*/
					)
				{
					int direction = STOP;

					if ((direction = Ball_DirectionChanger(ball, General_TOPRIGHT, TOPLEFT45, ChangeAngle, sound)) != STOP);
					else if ((direction = Ball_DirectionChanger(ball, General_TOPLEFT, TOPRIGHT45, ChangeAngle, sound)) != STOP);
					else if ((direction = Ball_DirectionChanger(ball, General_BOTTOMRIGHT, BOTTOMLEFT45, ChangeAngle, sound)) != STOP);
					else if ((direction = Ball_DirectionChanger(ball, General_BOTTOMLEFT, BOTTOMRIGHT45, ChangeAngle, sound)) != STOP);


					ball->balldirection = direction;
				}

				/* Ball hit enemy on left edges */
				else if (((y_pos == enemy_y_pos || y_pos == (enemy_y_pos + ((ENEMY_HEIGHT - 1) * enemy_AI->p_mult))) &&
					x_pos == enemy_x_pos) && current_x_pos < enemy_x_pos)
				{
					int direction = STOP;

					if ((direction = Ball_DirectionChanger(ball, General_TOPRIGHT, TOPLEFT45, ChangeAngle, sound)) != STOP);
					else if ((direction = Ball_DirectionChanger(ball, General_TOPLEFT, TOPRIGHT45, ChangeAngle, sound)) != STOP);
					else if ((direction = Ball_DirectionChanger(ball, General_BOTTOMRIGHT, BOTTOMLEFT45, ChangeAngle, sound)) != STOP);
					else if ((direction = Ball_DirectionChanger(ball, General_BOTTOMLEFT, BOTTOMRIGHT45, ChangeAngle, sound)) != STOP);

					ball->balldirection = direction;
				}

				/* Ball hit enemy on right edges */
				else if (((y_pos == enemy_y_pos || y_pos == (enemy_y_pos + ((ENEMY_HEIGHT - 1) * enemy_AI->p_mult))) &&
					x_pos == enemy_x_pos + ENEMY_WIDTH - 1) && current_x_pos > enemy_x_pos + ENEMY_WIDTH - 1)
				{
					int direction = STOP;

					if ((direction = Ball_DirectionChanger(ball, General_TOPRIGHT, TOPLEFT45, ChangeAngle, sound)) != STOP);
					else if ((direction = Ball_DirectionChanger(ball, General_TOPLEFT, TOPRIGHT45, ChangeAngle, sound)) != STOP);
					else if ((direction = Ball_DirectionChanger(ball, General_BOTTOMRIGHT, BOTTOMLEFT45, ChangeAngle, sound)) != STOP);
					else if ((direction = Ball_DirectionChanger(ball, General_BOTTOMLEFT, BOTTOMRIGHT45, ChangeAngle, sound)) != STOP);

					ball->balldirection = direction;
				}
				WorldData_GoToXY(ball->x_ballpos, ball->y_ballpos);
				printf(" ");

				/* Enemy got hit, play sound! */
				/* Checks which direction it changed to and plays the correct sound */
				if (ball->balldirection >= 0 && ball->balldirection <= 3)
				{
					/* Ball bounces off at 45 degrees */
					loadSound(sound, "..\\CollisionDetectionTest\\SFX\\EnemyGetHit45.wav");
					playSound_Low(sound);
				}

				else if (ball->balldirection >= 4 && ball->balldirection <= 7)
				{
					/* Ball bounces off at 60 degrees */
					loadSound(sound, "..\\CollisionDetectionTest\\SFX\\EnemyGetHit60.wav");
					playSound_Low(sound);
				}

				else if (ball->balldirection >= 8 && ball->balldirection <= 11)
				{
					/* Ball bounces off at 20 degrees */
					loadSound(sound, "..\\CollisionDetectionTest\\SFX\\EnemyGetHit20.wav");
					playSound_Low(sound);
				}

				else if (ball->balldirection >= 12 && ball->balldirection <= 13)
				{
					/* Ball bounces off at 90 degrees */
					loadSound(sound, "..\\CollisionDetectionTest\\SFX\\EnemyGetHit90.wav");
					playSound_Low(sound);
				}
				break;
			}
		}
	}

}

/*if ball hits another ball,  function does the apporiate actions*/
void Ball_BallCollision(Ball *ball, Sound *sound)
{
	/*play the sound of the ball hitting */
	loadSound(sound, "..\\CollisionDetectionTest\\SFX\\PH20.wav");
	playSound_Medium(sound);

	int direction = STOP;
	
	/*checks the direction of the ball and reflects it back at a 20 degrees angle*/
	if ((direction = Ball_DirectionChanger(ball, General_TOPRIGHT, BOTTOMLEFT20, DoNotChangeAngle, sound)) != STOP);
	else if ((direction = Ball_DirectionChanger(ball, General_TOPLEFT, BOTTOMRIGHT20, DoNotChangeAngle, sound)) != STOP);
	else if ((direction = Ball_DirectionChanger(ball, General_BOTTOMRIGHT, TOPLEFT20, DoNotChangeAngle, sound)) != STOP);
	else if ((direction = Ball_DirectionChanger(ball, General_BOTTOMLEFT, TOPRIGHT20, DoNotChangeAngle, sound)) != STOP);
	else if ((direction = Ball_DirectionChanger(ball, UP, DOWN, DoNotChangeAngle, sound)) != STOP);
	else if ((direction = Ball_DirectionChanger(ball, DOWN, UP, DoNotChangeAngle, sound)) != STOP);

	ball->balldirection = direction;
}

 /*If player hit the ball during the player's update */
void Player_BallCollision(Player *player, int change_x, Stage *stage, Ball *ball, TileMapManager *tileManager, Sound *sound)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	/* Save current attributes */
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;

	TileMapManager_Ball(ball, tileManager, 0);
	WorldData_GoToXY(ball->x_ballpos, ball->y_ballpos);
	printf(" ");

	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

	if (ball->x_ballpos == 1 || ball->x_ballpos == stage->size_x - 2);
	else
		ball->x_ballpos += change_x;

	loadSound(sound, "..\\CollisionDetectionTest\\SFX\\PH20.wav");

	if (player->id == 1)
	{
		++ball->y_ballpos;
		playSound_Player1(sound, player->x_p);
		if (change_x > 0)
			ball->balldirection = BOTTOMRIGHT20;

		else if (change_x < 0)
			ball->balldirection = BOTTOMLEFT20;
	}

	else if (player->id == 2)
	{
		--ball->y_ballpos;
		playSound_Player2(sound, player->x_p);
		if (change_x > 0)
			ball->balldirection = TOPRIGHT20;

		else if (change_x < 0)
			ball->balldirection = TOPLEFT20;
	}

	WorldData_GoToXY(ball->x_ballpos, ball->y_ballpos);
	printf("%c", ball->ballchar);
	SetConsoleTextAttribute(hConsole, saved_attributes);
}

void Ball_isdeadPlayerMissed(Ball* ball, Stage* stage, Sound* sound)
{
	/*sets the ball to dead, therefore ball cannot be picked up the player*/
	ball->ballisdeadflag = 1;
	
	/*if the ball has just been missed by player, do the following*/
	if (ball->timetorevival == 0)
	{
		
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\PlayerMiss.wav");
		/*If the stage is coop*/
		if (stage->play_type == CoOp)
		{
			/*play the sound*/
			loadSound(sound, "..\\CollisionDetectionTest\\SFX\\PlayerMiss.wav");
			playSound_High(sound);
			
			/*Affect both players health but not their score*/
			Ball_AffectPlayerHealthandScore(ball->friendplayer, -1, ball->enemyplayer, -1, NULL, 0, NULL, 0);
		}
		/*If the stage is Pvp*/
		else if (stage->play_type == PvP)
		{
			/*Affect only friend player's health and score*/
			Ball_AffectPlayerHealthandScore(ball->friendplayer, -1, NULL, 0, ball->friendplayer, scorePlayerMissedBall, NULL, 0);
			
			/*Play the sound of the player missing the ball*/
			loadSound(sound, "..\\CollisionDetectionTest\\SFX\\PlayerMiss.wav");
			if (ball->ball_type == 1)
				playSound_Player1(sound, ball->friendplayer->x_p);
			else if (ball->ball_type == 2)
				playSound_Player2(sound, ball->friendplayer->x_p);
		}
	}
}

/*If ball goes out of bounds, redraws ball back to player -- in case ball glitches*/
int Ball_isOutofBounds(Ball *ball, Stage *stage, Sound* sound, TileMapManager *tilemanager)
{
	/*Checks if the ball is outside the borders*/
	if (   ball->x_ballpos > stage->size_x - 1
		|| ball->x_ballpos < 0
		|| ball->y_ballpos < stage->player1_start_posY - 1
		|| ball->y_ballpos > stage->player2_start_posY + 2)
	{
		/*sets the ball direction to stop, not fired but still alive*/
		ball->balldirection = STOP;
		ball->ball_fired_flag = 0;
		ball->ballisdeadflag = 0;

		/*do nothing to player's score and health*/

		/*Redraw and update the ball to player*/
		Ball_MoveBallWithPlayer(ball, tilemanager);

		/*play sound*/
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\BallRespawn.wav");
		playSound_Low(sound);
	
			return 0;/*ball is out of bounds*/
		}

	else return 1; /*ball is not out of bounds*/
}