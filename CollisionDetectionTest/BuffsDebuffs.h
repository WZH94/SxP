/******************************************************************************
All content © 2017 Digipen Institute of Technology Singapore, all rights reserved

filename    BuffsDebuffs.h
author      Wong Zhihao
DP email    zhihao.wong@digipen.edu
course      RTIS

Brief Description:
Declaration of powerup type functions.

******************************************************************************/

#pragma once

/* Struct forward declarations */
typedef struct Player Player;
typedef struct Ball Ball;
typedef struct Stage Stage;
typedef struct TileMapManager TileMapManager;
typedef struct Sound Sound;

/* Increases player HP by one when player gets the buff */
void Buff_HP(Player *player, Sound *sound);

/* Decreases player HP by one when player gets the debuff */
void Debuff_HP(Player *player, Sound *sound);

/* Updates the length of the player from buffs and debuffs */
void Player_LengthUpdate(Player* player, int new_length, TileMapManager *tileManager, Sound *sound);

/* Ensures the player length does not exceed borders if its length increases */
void Player_LengthBorderCheck(Stage *stage, Player *player);

/* Copies the correct player ASCII */
void Player_Copy(Player *player, const char *play[]);

/* Speeds up player's ball */
void Ball_Fast(Ball *ball, Player *player, Sound *sound);

/* Slows down player's ball */
void Ball_Slow(Ball *ball, Player *player, Sound *sound);

/* Swaps the player's left and right controls */
void PlayerControls_Swap(Player *player, Sound *sound);

/* Returns the player controls back to normal */
void PlayerControls_Revert(Player *player, Sound *sound);

/* Sets player speed back to normal */
void PlayerSpeed_Fast(Player *player, Sound *sound);

/* Slows player speed by half */
void PlayerSpeed_Slow(Player *player, Sound *sound);

/* Swaps the players' balls */
void Ball_Swap(Ball *ball, Sound *sound, TileMapManager *tileManager);