/************************************************************
All content © 2017 Digipen Institute of Technology Singapore, all rights reserved

filename    Hydra.h
author      Wong Zhihao
DP email    zhihao.wong@digipen.edu
course      RTIS

Brief Description :
Jebus function declarations
*************************************************************/

#pragma once

/* Struct forward declarations */
typedef struct Stage Stage;
typedef struct Enemy Enemy;
typedef struct Enemy_AI Enemy_AI;
typedef struct TileMapManager TileMapManager;
typedef struct Sound Sound;

/* Handles the AI of Jebus */
void Jebus_AI(Stage *stage, Enemy *enemy[], Enemy_AI *enemy_AI, TileMapManager *tileManager, Sound *sound);

/* Function to heal enemies around Jebus */
void Jebus_Heal(Enemy *enemy, Enemy_AI *enemy_AI, TileMapManager *tileManager, Sound *sound);

/* Function to revive enemies around Jebus */
int Jebus_Revive(Enemy *enemy, Enemy_AI *enemy_AI, TileMapManager *tileManager, Sound *sound);