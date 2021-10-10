/******************************************************************************
All content © 2017 Digipen Institute of Technology Singapore, all rights reserved

filename			Player.c
author				Ngm Hui Min, Lois
DP email			lois.ngm@digipen.edu
course				RTIS

Brief Description:
Contents of player object of the game.

******************************************************************************/
#include "stdafx.h" 
#include <string.h> /* strncpy */
#include <Windows.h> /* HANDLE, CONSOLE_SCREEN_BUFFER_INFO, WORD, SetConosleTextAttribute(), GetStdHandle(), GetConsoleScreenBufferInfo() */
#include "WorldData.h" /* WorldData_GoToXY */
#include "Player.h" /* Function declarations */
#include "Stage.h" /* Stage object */
#include "TileMapManager.h" /* TileMapManager object,  TileMapManager_Player(), TileMapManager_Collision_Player() */
#include "Ball.h" /* Ball Object */
#include "Ball_CollisionManager.h" /* Player_BallCollision */
#include "Powerups.h" /* Powerup object, Player_Powerup_Collision() */
#include "EnemyBullets.h" /* EnemyBullets object, Player_Bullet_Collision() */
#include "FMODInit.h" /* for sound */
#include "BuffsDebuffs.h" /* buffs and debuffs from powerup */
#include "Enemy.h" /* Enemy object */
#include "EnemyAI.h" /* EnemyAI object */

/* initial values of the player's score and health*/
#define INIT_SCORE 0
#define INIT_HEALTH 8

/* Ascii code for health bars */
#define HEALTH_BAR 221

/* array to clear the player for redrawing */
const char *playerClear3[PLAYER_HEIGHT] = {
	"   ",
	"   "
};

/* array to clear the player for redrawing */
const char *playerClear5[PLAYER_HEIGHT] = {
	"     ",
	"     "
};

/* array to clear the player for redrawing */
const char *playerClear7[PLAYER_HEIGHT] = {
	"       ",
	"       "
};

/* array to clear the player for redrawing */
const char *playerClear9[PLAYER_HEIGHT] = {
	"         ",
	"         "
};

/* array to clear the player for redrawing */
const char *playerClear11[PLAYER_HEIGHT] = {
	"           ",
	"           "
};

/* Create a new Player object with the provided data */
Player* Player_Create(const char *play[], int l, int r, int a, int b, int x_player, int y_player, int x_length,
	int y_statbar, int x_health, int x_boost, int x_score, int id, TileMapManager *tileManager)
{
	int i;
	Player *player = NULL; /* the new item to be returned */
	/* For colouring */
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	/* Save current attributes of colour */
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;

	/* allocate the memory for a new object */
	player = (Player*)malloc(sizeof(Player));
	if (player == NULL)
	{
		return NULL; /* malloc can fail */
	}

	/* copy the data from the parameters into the new object */
	for (i = 0; i < PLAYER_HEIGHT; ++i)
		strncpy_s(player->playerStyle[i], x_length + 1, play[i], x_length + 1);
	player->left = l;					 /* ASCII for player left control */
	player->right = r;					 /* ASCII for player right control */
	player->attack = a;					 /* ASCII for player fire control */
	player->boost = b;					 /* ASCII for player boost control */
	player->boost_amt = 200;			 /* Boost amount of player */
	player->score = INIT_SCORE;			 /* Score of player */
	player->health = INIT_HEALTH;		 /* Health of player */
	player->x_p = x_player;				 /* x coord of player */
	player->y_p = y_player;				 /* y coord of player */
	player->x_l = x_length;				 /* Length of player */
	player->y_sb = y_statbar;			 /* y coord of player statbar */
	player->x_h = x_health;				 /* x coord of player health stat */
	player->x_b = x_boost;				 /* x coord of player boost stat */
	player->x_s = x_score;				 /* x coord of player score stat */
	player->id = id;					 /* depends if it's player 1 or 2 */
	/* set colour depending on which player */
	if (player->id == 1)
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY); /* CYAN */
	else if (player->id == 2)
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY); /* YELLOW */
	Player_Print(player, tileManager);	 /* Player print function */
	Player_Print_Score(player);			 /* Player score print function */
	Player_Print_Health(player);		 /* Player health print function */
	SetConsoleTextAttribute(hConsole, saved_attributes); /* Reset the colour back to the original */
	player->frames = 45;				 /* Updates every 2 frames AT MOST, WILL NOT CHANGE */
	player->base_timer = 0;				 /* Adjusts the speed, ex. if increases to 1, takes 1 less frame to update */
	player->timer = player->base_timer;	 /* Timer always starts from base_timer's value */
	player->invulnerable_time = 0;		 /* when player gets hit, the invulnerable time */
	player->swap_time = 0;				 /* when the player gets the swap powerup, the length of time affected */
	player->swap_flag = 0;				 /* stays as 1 as long as the player's control is swapped, otherwise 0 */

	/* Prints the life, boost and highscore with colour */
	if (player->id == 1)
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY); /* CYAN */
	else if (player->id == 2)
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY); /* YELLOW */
	WorldData_GoToXY(player->x_h, player->y_sb);
	printf("Life: ");

	WorldData_GoToXY(player->x_b, player->y_sb);
	printf("Boost: ");

	WorldData_GoToXY(player->x_s, player->y_sb);
	printf("Highscore: ");
	SetConsoleTextAttribute(hConsole, saved_attributes);
	/* return the new object */
	return player;
}


/* Free the memory associated with a given Player object */
void Player_Free(Player** playerPtr)
{
	/* safety check on the parameters */
	if ((playerPtr == NULL) || (*playerPtr == NULL))
		return;  /* take no action if the parameters are invalid */

	/* free the given item */
	free(*playerPtr);

	/* clear the provided pointer */
	*playerPtr = NULL;
}

/* Prints the player in the game */
void Player_Print(Player* player, TileMapManager *tileManager)
{
	int i = 0; /* loop variable */

	/* update the tile map*/
	TileMapManager_Player(player, tileManager, player->id);
	/* print the player */
	WorldData_GoToXY(player->x_p, player->y_p);
	for (i = 0; i < PLAYER_HEIGHT; ++i)
	{
		printf("%s", player->playerStyle[i]);
		WorldData_GoToXY(player->x_p, player->y_p + 1);
	}
}

/* Clears the area where the player is previously at */
void Player_Clear(Player *player, const char *player_clear[], TileMapManager *tileManager)
{
	/* updates the tile map */
	TileMapManager_Player(player, tileManager, 0);
	/* clear the area of the player */
	WorldData_GoToXY(player->x_p, player->y_p);
	int i = 0;
	for (i = 0; i < PLAYER_HEIGHT; ++i)
	{
		printf("%s", player_clear[i]);
		WorldData_GoToXY(player->x_p, player->y_p + 1);
	}
}

/* Selects the size to clear based on the player length */
void Player_Clear_Select(Player *player, TileMapManager *tileManager)
{
	switch (player->x_l)
	{
	case 3: Player_Clear(player, playerClear3, tileManager);
		break;
	case 5: Player_Clear(player, playerClear5, tileManager);
		break;
	case 7: Player_Clear(player, playerClear7, tileManager);
		break;
	case 9: Player_Clear(player, playerClear9, tileManager);
		break;
	case 11: Player_Clear(player, playerClear11, tileManager);
		break;
	}
}

/* Prints the health of the player on their respective areas */
void Player_Print_Health(Player *player)
{
	int i = 0; /* loop variable */
	/* for colour */
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	/* Save rent attributes of the original colour */
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;
	
	/* prints the player's health */
	WorldData_GoToXY(player->x_h + 6, player->y_sb);
	if (player->health >= 5)
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY); /* GREEN */
	else if (player->health >= 3)
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); /* YELLOW */
	else
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY); /* RED */

	/* prints the health bars */
	for (i = 0; i < player->health; ++i) 
		printf("%c", HEALTH_BAR);
	/* set the colour back */
	SetConsoleTextAttribute(hConsole, saved_attributes);
}

/* Clears the health of player before reprinting */
void Player_Clear_Health(Player *player)
{
	int i = 0; /* looping variable */

	/* clears the player's health */
	WorldData_GoToXY(player->x_h + 6, player->y_sb);
	for (i = 0; i < player->health; ++i) 
		printf(" ");
}

/* Prints the player's boost amount on their respective areas */
void Player_Print_Boost(Player *player)
{
	WorldData_GoToXY(player->x_b + 7, player->y_sb);
	printf("%2.0f ", player->boost_amt);
}

/* Prints the player's score on their respective areas */
void Player_Print_Score(Player *player)
{
	WorldData_GoToXY(player->x_s + 11, player->y_sb);
	printf("%.4d ", player->score);
}
/* Recharges the player's boost */
void PlayerBoost_Recharge(Player *player)
{
	if (player->boost_amt < 200.0f)
		/* Controls the speed of recharge */
		player->boost_amt += 0.2f;
}

/* Action done when player is to be moved */
void Player_Movement(Stage *stage, Player *player, Ball *ball1, Ball *ball2, Powerup *powerup[], EnemyBullet *enemyBullet[],
					 TileMapManager *tileManager, Sound *sound, Enemy *enemy1[], Enemy *enemy2[], Enemy_AI *enemy_AI1, Enemy_AI *enemy_AI2)
{
	int check_collision = 0;
	int change_x = 0;

	/* If player moves left */
	if (GetAsyncKeyState(player->left))
	{
		if ((player->timer += 1 * Delta_Time(stage)) >= player->frames)
		{
			player->timer = player->base_timer;
			/* Check if player will go out of bounds of game */
			if (player->x_p > 1)
			{
				check_collision = 1;
				change_x = -1;
				Player_Clear_Select(player, tileManager);
				/* Update player position */
				player->x_p -= 1;
			}
		}

		/* Checks if player is boosting, and if player has sufficient boost charge */
		if (GetAsyncKeyState(player->boost) && player->boost_amt > 1 && player->x_p > 1)
		{
			/* Depletes player's boost charge */
			--player->boost_amt;

			if ((player->timer += 1 * Delta_Time(stage)) >= player->frames)
			{
				player->timer = player->base_timer;

				if (player->x_p > 1)
				{
					check_collision = 1;
					change_x = -1;
					/* Moves the player an additional time */
					Player_Clear_Select(player, tileManager);
					player->x_p -= 1;
				}
			}
		}
	}
	/* If player moves right */
	if (GetAsyncKeyState(player->right))
	{
		if ((player->timer += 1 * Delta_Time(stage)) >= player->frames)
		{
			player->timer = player->base_timer;
			/* Check if player will go out of bounds of game */
			if (player->x_p < (stage->size_x - player->x_l - 1))
			{
				check_collision = 1;
				change_x = 1;
				Player_Clear_Select(player, tileManager);
				/* Update player position */
				player->x_p += 1;
			}
		}

		/* Checkks if player is boosting, and if player has sufficient boost charge */
		if (GetAsyncKeyState(player->boost) && player->boost_amt > 1 && player->x_p < (stage->size_x - player->x_l - 1))
		{
			/* Depletes player's boost charge */
			--player->boost_amt;

			if ((player->timer += 1 * Delta_Time(stage)) >= player->frames)
			{
				player->timer = player->base_timer;

				/* Check if player will go out of bounds of game */
				if (player->x_p < (stage->size_x - player->x_l - 1))
				{
					check_collision = 1;
					change_x = 1;
					/* Moves the player an additional time */
					Player_Clear_Select(player, tileManager);
					player->x_p += 1;
				}
			}
		}
	}
	/* if players are trying to go out of bounds */
	if (check_collision == 1)
	{
		int result;
		/* left border */
		if (change_x < 0)
		{
			result = TileMapManager_Collision_Player(tileManager, player->x_p, player->y_p);
			if (result)
				Player_CollisionDetection(result, player, change_x, stage, ball1, ball2, powerup, enemyBullet, tileManager, sound, 
										  enemy1, enemy2, enemy_AI1, enemy_AI2);

			if (player->id == 1)
			{
				result = TileMapManager_Collision_Player(tileManager, player->x_p, player->y_p + 1);
				if (result)
					Player_CollisionDetection(result, player, change_x, stage, ball1, ball2, powerup, enemyBullet, tileManager, sound, 
											  enemy1, enemy2, enemy_AI1, enemy_AI2);
			}

			else if (player->id == 2)
			{
				result = TileMapManager_Collision_Player(tileManager, player->x_p, player->y_p - 1);
				if (result)
					Player_CollisionDetection(result, player, change_x, stage, ball1, ball2, powerup, enemyBullet, tileManager, sound, 
											  enemy1, enemy2, enemy_AI1, enemy_AI2);
			}
		}
		/* right border */
		else if (change_x > 0)
		{
			result = TileMapManager_Collision_Player(tileManager, player->x_p + player->x_l - 1, player->y_p);
			if (result)
				Player_CollisionDetection(result, player, change_x, stage, ball1, ball2, powerup, enemyBullet, tileManager, sound, 
										  enemy1, enemy2, enemy_AI1, enemy_AI2);

			if (player->id == 1)
			{
				result = TileMapManager_Collision_Player(tileManager, player->x_p + player->x_l - 1, player->y_p + 1);
				if (result)
					Player_CollisionDetection(result, player, change_x, stage, ball1, ball2, powerup, enemyBullet, tileManager, sound,
											  enemy1, enemy2, enemy_AI1, enemy_AI2);
			}

			else if (player->id == 2)
			{
				result = TileMapManager_Collision_Player(tileManager, player->x_p + player->x_l - 1, player->y_p - 1);
				if (result)
					Player_CollisionDetection(result, player, change_x, stage, ball1, ball2, powerup, enemyBullet, tileManager, sound,
											  enemy1, enemy2, enemy_AI1, enemy_AI2);
			}
		}
	}

	Player_Print(player, tileManager);
}

/* Detects if player hit anythingn */
void Player_CollisionDetection(int result, Player *player, int change_x, Stage *stage, Ball *ball1, Ball *ball2, Powerup *powerup[], EnemyBullet *enemyBullet[],
							   TileMapManager *tileManager, Sound *sound, Enemy *enemy1[], Enemy *enemy2[], Enemy_AI *enemy_AI1, Enemy_AI *enemy_AI2)
{
	/* Player hit ball 1 */
	if (result == 4)
		Player_BallCollision(player, change_x, stage, ball1, tileManager, sound);

	/* Player hit ball 1 */
	else if (result == 5)
		Player_BallCollision(player, change_x, stage, ball2, tileManager, sound);

	/* Player hit powerup */
	else if (result == 7)
		if (player->id == 1)
			Player_Powerup_Collision(powerup, stage, player, ball1, tileManager, sound, enemy1, enemy2, enemy_AI1, enemy_AI2);
		else if (player->id == 2)
			Player_Powerup_Collision(powerup, stage, player, ball2, tileManager, sound, enemy1, enemy2, enemy_AI1, enemy_AI2);

	/* Player hit bullet */
	else if (result == 8)
		Player_Bullet_Collision(player, enemyBullet, tileManager, sound);
}

/* Update player data e.d. movement, score and health */
void Player_Update(Stage *stage, Player *player, Ball *ball1, Ball *ball2, Powerup *powerup[], EnemyBullet *enemyBullet[],
	TileMapManager *tilemanager, Sound *sound, Enemy *enemy1[], Enemy *enemy2[], Enemy_AI *enemy_AI1, Enemy_AI *enemy_AI2)
{
	/* colour */
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	/* Save current attributes of original colour */
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes; 

	if (player->id == 1)
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY); /* CYAN */
	else if (player->id == 2)
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY); /* YELLOW */
	/* minus off invulnerable time as game runs */
	if (player->invulnerable_time >= 0)
		player->invulnerable_time -= 1 * Delta_Time(stage);
	/* minus off swap time as game runs */
	if (player->swap_time > 0)
		player->swap_time -= 1 * Delta_Time(stage);
	/* revert back the controls when swap time is up */
	if (player->swap_flag == 1 && player->swap_time <= 0)
		PlayerControls_Revert(player, sound);

	/* update player movements, score, health and boost */
	Player_Print_Score(player);
	Player_Print_Health(player);
	Player_Print_Boost(player);
	Player_Movement(stage, player, ball1, ball2, powerup, enemyBullet, tilemanager, sound, enemy1, enemy2, enemy_AI1, enemy_AI2);
	PlayerBoost_Recharge(player);
	/* change back to original colour */
	SetConsoleTextAttribute(hConsole, saved_attributes);
}

/* When player gets damaged */
void minusHP(Player *player, Sound *sound)
{
	/* if player is vulnerable */
	if (player->invulnerable_time <= 0)
	{
		Player_Clear_Health(player);
		--player->health;

		player->invulnerable_time = 2000; /* set invulnerable time */

		/* play sound when player is hit */
		loadSound(sound, "..\\CollisionDetectionTest\\SFX\\PlayerGetHit.wav");
		playSound_Player2(sound, player->x_p);
	}
}