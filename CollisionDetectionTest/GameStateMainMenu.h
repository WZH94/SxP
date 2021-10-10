/******************************************************************************
All content © 2017 Digipen Institute of Technology Singapore, all rights reserved

filename    GameStateMainMenu.h
author      Ong Tze Ang
DP email    tzeang.ong@digipen.edu
course      RTIS

Brief Description:
This file handles every GameState transitions and GameState designs.
******************************************************************************/
#pragma once
/* struct def */
typedef struct Sound Sound;
typedef struct Enemy Enemy;
typedef struct Player Player;
typedef struct Ball Ball;
typedef struct TileMapManager TileMapManager;
typedef struct Stage Stage;
typedef struct Highscore Highscore;
/* Game State Init*/
void GameStateInit();
/* Main Menu GameState */
void MainMenu(Stage *stage_menu, Sound *sound);
/* Pvp stage selection menu */
void PVPStageSelectionMenu(Stage* stage_menu, Ball* ball_menu, Sound* sound);

/* Pvp terrain selection menu */
void PVPTerrainSelectionMenu(Stage* stage_menu, Ball* ball_menu, Sound* sound, int stage);
/* CoOp Terrain selection menu */
void CoOpTerrainSelectionMenu(Stage* stage_menu, Ball* ball_menu, Sound* sound);
/* Terrain GameState */
void TerrainMenu(Stage *stage_menu, Sound *sound);
/* HowToPlay Gamestate */
void HowToPlayMenu(Stage *stage_menu, Sound *sound);
/* Powerup Gamestate */
void PowerupMenu(Stage *stage_menu, Sound *sound);
/* Gamemode Gamestate */
void GamemodeMenu(Stage *stage_menu, Sound *sound);
/* Credits Gamestate */
void CreditsMenu(Stage *stage_menu, Sound *sound);
/* Monsterpedia GameState */
void MonsterpediaMenu(Stage *stage_menu, Sound *sound);
/* Monsterpedia BB GameState */
void MonsterpediaBB(Stage *stage_menu, Sound *sound);
/* Monsterpedia Wall Gamestate */
void MonsterpediaWall(Stage *stage_menu, Sound *sound);
/* Monsterpedia Shooter GameState */
void MonsterpediaShooter(Stage *stage_menu, Sound *sound);
/* Monsterpedia BigShot Gamestate */
void MonsterpediaBigShot(Stage *stage_menu, Sound *sound);
/* Monsterpedia Pea GameState */
void MonsterpediaPea(Stage *stage_menu, Sound *sound);
/* Monsterpedia Arc GameState */
void MonsterpediaArc(Stage *stage_menu, Sound *sound);
/* Monsterpedia Torpedo GameState */
void MonsterpediaTorpedo(Stage *stage_menu, Sound *sound);
/* Monsterpedia Jebus GameState */
void MonsterpediaJebus(Stage *stage_menu, Sound *sound);
/* Monsterpedia Hydra GameState */
void MonsterpediaHydra(Stage *stage_menu, Sound *sound);
/* Free mainmenu objects */
void MainMenu_Free(Stage *stage, Ball *ball, TileMapManager *tileManager);
/* Buffer function to create stage in mainmenu */
void MainMenuStageCreate(Sound *sound);
/* Highscore stage selection GameState */
void Highscore_Menu(Stage *stage_menu, Sound *sound);
/* Highscore display GameState */
void Highscore_Display(Stage *stage_menu, Sound *sound, int id);