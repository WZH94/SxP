/******************************************************************************
All content © 2017 Digipen Institute of Technology Singapore, all rights reserved

filename			BuffsDebuffs.c
author				Wong Zhihao
DP email			zhihao.wong@digipen.edu
course				RTIS

Brief Description:
Contains the functions of every powerup in the game.

******************************************************************************/

#include <string.h>			/* strncpy */
#include <stdio.h>			/* printf */
#include <stdlib.h>			/* rand */
#include "BuffsDebuffs.h"	/* Function declarations */
#include "Ball.h"			/* Ball object and functions */
#include "Player.h"			/* Player object and functions */
#include "FMODInit.h"		/* Sound object and functions */
#include "Stage.h"			/* Stage object and data */
#include "WorldData.h"		/* Misc Functions */
#include "TileMapManager.h" /* Tile manager object and functions */

/***********************
Buff and Debuffs to HP
***********************/

/* Increases player HP by one when player gets the buff */
void Buff_HP(Player *player, Sound *sound)
{
	/* Increase player health by one */
	++player->health;
	/* Increase player score */
	player->score += 50;

	loadSound(sound, "..\\CollisionDetectionTest\\SFX\\PlayerBuffCollect.wav");
	playSound_Medium(sound);

	loadSound(sound, "..\\CollisionDetectionTest\\SFX\\HPBuff.wav");
	playSound_High(sound);
}

/* Decreases player HP by one when player gets the debuff */
void Debuff_HP(Player *player, Sound *sound)
{
	/* Clear health before adjusting */
	Player_Clear_Health(player);
	/* Decrease player health by one */
	--player->health;
	/* Increase player score */
	player->score += 200;

	loadSound(sound, "..\\CollisionDetectionTest\\SFX\\PlayerDebuffCollect.wav");
	playSound_Medium(sound);

	loadSound(sound, "..\\CollisionDetectionTest\\SFX\\HPDebuff.wav");
	playSound_High(sound);
}

/**********************************
Buff and Debuffs to Player Length
**********************************/
#define PLAYER_HEIGHT 2

/* Updates the length of the player from buffs and debuffs */
void Player_LengthUpdate(Player *player, int new_length, TileMapManager *tileManager, Sound *sound)
{
	/* Check if player length is not at the max when increasing */
	if (new_length > 0)
	{
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\PlayerBuffCollect.wav");
		playSound_Medium(sound);

		if (player->x_l <= 9)
		{
			/* Clear player before adjusting */
			Player_Clear_Select(player, tileManager);
			/* Increase player length */
			player->x_l += new_length;
			/* Increase player score */
			player->score += 50;

			loadSound(sound, "..\\CollisionDetectionTest\\SFX\\LengthBuff.wav");
			playSound_High(sound);
		}
	}
	/* Check if player length is not at the min when decreasing */
	else if (new_length < 0)
	{
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\PlayerDebuffCollect.wav");
		playSound_Medium(sound);

		if (player->x_l >= 5)
		{
			/* Clear player before adjusting */
			Player_Clear_Select(player, tileManager);
			/* Increase player length */
			player->x_l += new_length;
			/* Increase player score */
			player->score += 100;

			loadSound(sound, "..\\CollisionDetectionTest\\SFX\\LengthDebuff.wav");
			playSound_High(sound);
		}
	}
	/* Powerup only increases score */
	else player->score += 200;;

	/* Sets player art to the correct art based on its length */
	if (player->id == 1) {
		switch (player->x_l)
		{
		case 3: Player_Copy(player, player1downdown);
			break;
		case 5: Player_Copy(player, player1down);
			break;
		case 7: Player_Copy(player, player1);
			break;
		case 9: Player_Copy(player, player1up);
			break;
		case 11: Player_Copy(player, player1upup);
			break;
		}
	}
	/* For player 2 */
	else if (player->id == 2)
	{
		switch (player->x_l)
		{
		case 3: Player_Copy(player, player2downdown);
			break;
		case 5: Player_Copy(player, player2down);
			break;
		case 7: Player_Copy(player, player2);
			break;
		case 9: Player_Copy(player, player2up);
			break;
		case 11: Player_Copy(player, player2upup);
			break;
		}
	}

	/* Print player */
	Player_Print(player, tileManager);
}

/* Ensures the player length does not exceed borders if its length increases */
void Player_LengthBorderCheck(Stage *stage, Player *player)
{
	/* Moves the player so its new length does not go out of game area */
	if (player->x_p == (stage->size_x - player->x_l - 1))
		player->x_p -= 2;
	if (player->x_p == (stage->size_x - player->x_l - 2))
		player->x_p -= 1;
}

/* Copies the correct player ASCII */
void Player_Copy(Player *player, const char *play[])
{
	int i;	/* Loop counter */

	/* Updates player art */
	for (i = 0; i < PLAYER_HEIGHT; ++i)
		strncpy_s(player->playerStyle[i], player->x_l + 1, play[i], player->x_l + 1);
}

/********************************
Buff and Debuffs to Ball Speeds
*********************************/

/* Speeds up player's ball */
void Ball_Fast(Ball *ball, Player *player, Sound *sound)
{
	/* Checks if it not at the max */
	if (ball->base_timer <= 85)
	{
		ball->base_timer += 15;
	}

	/* Increases player's score */
	player->score += 200;

	loadSound(sound, "..\\CollisionDetectionTest\\SFX\\PlayerDebuffCollect.wav");
	playSound_Medium(sound);

	loadSound(sound, "..\\CollisionDetectionTest\\SFX\\BallDebuff.wav");
	playSound_High(sound);
}

/* Slows down player's ball */
void Ball_Slow(Ball *ball, Player *player, Sound *sound)
{
	/* Makes sure ball doesn't move too slowly */
	if (ball->base_timer >= -60)
	{
		ball->base_timer -= 20;
	}

	/* Increases player's score */
	player->score += 100;

	loadSound(sound, "..\\CollisionDetectionTest\\SFX\\PlayerBuffCollect.wav");
	playSound_Medium(sound);

	loadSound(sound, "..\\CollisionDetectionTest\\SFX\\BallBuff.wav");
	playSound_High(sound);
}

/****************************
Swap player controls debuff
*****************************/

/* Swaps the player's left and right controls */
void PlayerControls_Swap(Player *player, Sound *sound)
{
	/* Checks if player's controls are not already swapped */
	if (player->swap_flag == 0)
	{
		int tmp;	/* Store value for swapping */

		/* Prints on the game sidebar that player controls are swapped */
		Stage_Control_Swap_Print(1);

		/* Flags the player's controls as swapped */
		player->swap_flag = 1;

		/* Swap the controls */
		tmp = player->left;
		player->left = player->right;
		player->right = tmp;

		/* Increase player score */
		player->score += 250;
		/* Sets the time where the player's controls are swapped */
		player->swap_time = 15000;
	}

	/* Player's controls already swapped, reset the timing */
	else
		player->swap_time = 15000;

	loadSound(sound, "..\\CollisionDetectionTest\\SFX\\PlayerDebuffCollect.wav");
	playSound_Medium(sound);

	loadSound(sound, "..\\CollisionDetectionTest\\SFX\\ControlsSwap.wav");
	playSound_High(sound);
}

/* Returns the player controls back to normal */
void PlayerControls_Revert(Player *player, Sound *sound)
{
	int tmp;	/* Store value for swapping */

	/* Prints on the game sidebar that player controls are normal */
	Stage_Control_Swap_Print(0);

	/* Swaps back player controls */
	tmp = player->left;
	player->left = player->right;
	player->right = tmp;

	/* Player's controls no longer swapped */
	player->swap_flag = 0;

	loadSound(sound, "..\\CollisionDetectionTest\\SFX\\ControlsRevert.wav");
	playSound_High(sound);
}

/**********************************
Buff and Debuffs to Player Speed
**********************************/

/* Speeds player speed */
void PlayerSpeed_Fast(Player *player, Sound *sound)
{
	/* Checks if player's speed is already at the max */
	if (player->base_timer <= 38)
		player->base_timer += 7;

	/* Increases player score */
	player->score += 100;

	loadSound(sound, "..\\CollisionDetectionTest\\SFX\\PlayerBuffCollect.wav");
	playSound_Medium(sound);

	loadSound(sound, "..\\CollisionDetectionTest\\SFX\\PlayerSpeedBuff.wav");
	playSound_High(sound);
}

/* Slows player speed */
void PlayerSpeed_Slow(Player *player, Sound *sound)
{
	/* Drops the player's speed */
	player->base_timer -= 7;

	/* Increases player's score */
	player->score += 200;

	loadSound(sound, "..\\CollisionDetectionTest\\SFX\\PlayerDebuffCollect.wav");
	playSound_Medium(sound);

	loadSound(sound, "..\\CollisionDetectionTest\\SFX\\PlayerSpeedDebuff.wav");
	playSound_High(sound);
}

/******************
 Ball Swap Powerup
*******************/

/* Swaps the players' balls */
void Ball_Swap(Ball *ball, Sound *sound, TileMapManager *tileManager)
{
	int x, y;	/* Store values for swapping */

	/* Check if either ball is not currently dead */
	if (ball->ballisdeadflag == 0 && ball->enemyball->ballisdeadflag == 0)
	{
		/* Clears the ball position in game area */
		TileMapManager_Ball(ball, tileManager, 0);
		WorldData_GoToXY(ball->x_ballpos, ball->y_ballpos);
		printf(" ");

		/* Clears the other ball position in game area */
		TileMapManager_Ball(ball->enemyball, tileManager, 0);
		WorldData_GoToXY(ball->enemyball->x_ballpos, ball->enemyball->y_ballpos);
		printf(" ");

		/* Swaps the ball's positions  */
		x = ball->x_ballpos;
		y = ball->y_ballpos;

		ball->x_ballpos = ball->enemyball->x_ballpos;
		ball->y_ballpos = ball->enemyball->y_ballpos;

		ball->enemyball->x_ballpos = x;
		ball->enemyball->y_ballpos = y;

		/* This causes a bug when I print the balls in their new position so it's being commented */
		//if (ball->ball_type == 1)
		//{
		//	TileMapManager_Ball(ball, tileManager, 3);
		//	TileMapManager_Ball(ball->enemyball, tileManager, 4);
		//}

		//else if (ball->ball_type == 2)
		//{
		//	TileMapManager_Ball(ball, tileManager, 4);
		//	TileMapManager_Ball(ball->enemyball, tileManager, 3);
		//}
		//WorldData_GoToXY(ball->x_ballpos, ball->y_ballpos);
		//printf("%c", ball->ballchar);

		//WorldData_GoToXY(ball->enemyball->x_ballpos, ball->enemyball->y_ballpos);
		//printf("%c", ball->enemyball->ballchar);

		/* If ball is still on player's peddle, fire it */
		if (ball->ball_fired_flag == 0)
		{
			ball->ball_fired_flag = 1;

			ball->balldirection = (rand() % 2);/*between bottom left and bottom right*/
			if (ball->ball_type == 1) ball->balldirection += 2;/*Ensures ball2 moves only top left and top right*/

			loadSound(sound, "..\\CollisionDetectionTest\\SFX\\PlayerFire.wav");
			playSound_High(sound);
		}

		if (ball->enemyball->ball_fired_flag == 0)
		{
			ball->enemyball->ball_fired_flag = 1;

			ball->enemyball->balldirection = (rand() % 2);/*between bottom left and bottom right*/
			if (ball->enemyball->ball_type == 1) ball->enemyball->balldirection += 2;/*Ensures ball2 moves only top left and top right*/

			loadSound(sound, "..\\CollisionDetectionTest\\SFX\\PlayerFire.wav");
			playSound_High(sound);
		}

		ball->friendplayer->score += 400;

		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\BallSwap.wav");
		playSound_High(sound);
	}
	loadSound(sound, "..\\CollisionDetectionTest\\SFX\\PlayerDebuffCollect.wav");
	playSound_Medium(sound);
}