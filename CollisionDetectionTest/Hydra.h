/******************************************************************************
All content © 2017 Digipen Institute of Technology Singapore, all rights reserved

filename    Hydra.h
author      Wong Zhihao
DP email    zhihao.wong@digipen.edu
course      RTIS

Brief Description:
Hydra function declarations
******************************************************************************/
#pragma once

/* Struct forward declarations */
typedef struct Enemy Enemy;
typedef struct Stage Stage;
typedef struct Enemy_AI Enemy_AI;
typedef struct TileMapManager TileMapManager;
typedef struct Sound Sound;

/* Initialises the hydra unit to take on one of the shooter's form */
void Hydra_Init(Enemy *enemy);

/* Causes the hydra to split when it dies */
void Hydra_Die(Stage *stage, Enemy *enemyContainer[], Enemy_AI *enemy_AI, TileMapManager *tileManager, int hydra_location, Sound *sound);

/* Checks if the dead hydra is at the edges of the map */
int Hydra_Position(Stage *stage, Enemy *enemy);