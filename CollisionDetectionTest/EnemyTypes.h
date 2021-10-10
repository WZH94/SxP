/******************************************************************************
All content © 2017 Digipen Institute of Technology Singapore, all rights reserved

filename    EnemyType.h
author      Wong Zhihao
DP email    zhihao.wong@digipen.edu
course      RTIS

Brief Description:
This file declares the functions for Enemy Types

******************************************************************************/

#pragma once

/**********************************************************************************************************
Summary of enemy types:
TYPE 0 - 1 are non shooters
TYPE 2 - 5 are shooters
TYPE 7 is supporter
TYPE 8 is hydra

TYPE 0 - BB, Weak wall											 <- Evolves to The Wall
TYPE 1 - The Wall, Strong wall									 <- Mid game BB
TYPE 2 - Shooter, Ordinary shooter								 <- Evolves to Pea and BigShot
TYPE 3 - BigShot, Stronger shooter, 2 damage					 <- Late Game Shooter
TYPE 4 - Pea, Shoots at a faster rate							 <- Mid Game Shooter
TYPE 5 - Arc, Shoots 3 bullets in an arc						 <- MIGHT NOT USE, TOO SIMILAR TO BIGSHOT
TYPE 6 - Torpedo, Shoots homing bullets							 <- Late Game enemy
TYPE 7 - Jebus, Revives and heals enemies at random around it	 <- Late Game enemy
TYPE 8 - Hydra, Splits into enemy types 2 - 6 when it dies, depending on which type it spawned as
***********************************************************************************************************/

/***********************************************
BB Enemy TYPE 0, Just a wall that moves, HP: 1
************************************************/

/* BB enemy */
static const char *enemyBB[3] = {
	"----",
	"-''-",
	"----"
};

/*************************************************************************
The Wall Enemy TYPE 1, Does not shoot, Does not need no education, HP: 4
**************************************************************************/

/* Wall enemy, 4 HP*/
static const char *enemyWall_4[3] = {
	"====",
	"=oo=",
	"===="
};

/* Wall enemy, 3 HP*/
static const char *enemyWall_3[3] = {
	"====",
	"=.o=",
	"===="
};

/* Wall enemy, 2 HP*/
static const char *enemyWall_2[3] = {
	"====",
	"=..=",
	"===="
};

/* Wall enemy, 1 HP*/
static const char *enemyWall_1[3] = {
	"====",
	"=><=",
	"===="
};

/**************************************************
Shooter Enemy TYPE 2, Shoots at the player, HP: 1
***************************************************/

/* Shooter enemy that faces Player 1 */
static const char *enemyShooter_P1[3] = {
	"/~~\\",
	"|..|",
	"\\--/"
};

/* Shooter enemy that faces Player 2 */
static const char *enemyShooter_P2[3] = {
	"\\~~/",
	"|..|" ,
	"/--\\"
};

/****************************************************
BigShot Enemy TYPE 3, Stronger shooter, HP: 2
*****************************************************/

/* BigShot enemy that faces Player 1, 2 HP */
static const char *enemyBigShot_P1_2[3] = {
	"/~~\\",
	"<@@>" ,
	"\\==/"
};

/* BigShot enemy that faces Player 1, 1 HP */
static const char *enemyBigShot_P1_1[3] = {
	"/~~\\",
	"<oo>" ,
	"\\==/"
};

/* BigShot enemy that faces Player 2, 2 HP */
static const char *enemyBigShot_P2_2[3] = {
	"\\~~/",
	"<@@>" ,
	"/==\\"
};

/* BigShot enemy that faces Player 2, 1 HP */
static const char *enemyBigShot_P2_1[3] = {
	"\\~~/",
	"<oo>" ,
	"/==\\"
};

/****************************************************
Pea Enemy TYPE 4, Fast shooter, HP: 1
*****************************************************/

/* Pea enemy that faces Player 1 */
static const char *enemyPea_P1[3] = {
	"'~~'",
	"|''|" ,
	"'--'"
};

/* BigShot enemy that faces Player 2, 1 HP */
static const char *enemyPea_P2[3] = {
	"'~~'",
	"|''|" ,
	"'--'"
};

/************************************************************************
Arc Enemy TYPE 5, Fires 3 bullets in an arc, HP: 3
*************************************************************************/

/* Arc enemy that faces Player 1, 3 HP */
static const char *enemyArc_P1_3[3] = {
	"M^^M",
	"{@@}" ,
	"W==W"
};

/* Arc enemy that faces Player 1, 2 HP */
static const char *enemyArc_P1_2[3] = {
	"m^^m",
	"{@@}" ,
	"w==w"
};

/* Arc enemy that faces Player 1, 1 HP */
static const char *enemyArc_P1_1[3] = {
	"n^^n",
	"{oo}" ,
	"w--w"
};

/* Arc enemy that faces Player 2, 3 HP */
static const char *enemyArc_P2_3[3] = {
	"W^^W",
	"{@@}" ,
	"M==M"
};

/* Arc enemy that faces Player 2, 2 HP */
static const char *enemyArc_P2_2[3] = {
	"w^^w",
	"{@@}" ,
	"m==m"
};

/* Arc enemy that faces Player 2, 1 HP */
static const char *enemyArc_P2_1[3] = {
	"w^^w",
	"{oo}" ,
	"n--n"
};

/**************************************************
Torpedo Enemy TYPE 6, Fires homing bullets, HP: 3
***************************************************/

/* Torpedo enemy, 3 HP*/
static const char *enemyTorpedo_3[3] = {
	"[MM]",
	"[--]",
	"[==]"
};

/* Torpedo enemy, 2 HP*/
static const char *enemyTorpedo_2[3] = {
	"[MM]",
	"[--]",
	"[--]"
};

/* Torpedo enemy, 1 HP*/
static const char *enemyTorpedo_1[3] = {
	"[MM]",
	"[!!]",
	"[--]"
};

/*************************************************************
Jebus Enemy TYPE 7, Heals or revives enemies around it, HP: 2
**************************************************************/

/* Jebus enemy, 2 HP*/
static const char *enemyJebus_2[3] = {
	"[TT]",
	"[TT]",
	"[==]"
};

/* Jebus enemy, 1 HP*/
static const char *enemyJebus_1[3] = {
	"[TT]",
	"[tt]",
	"[--]"
};


/*********************************************************************
Hydra Enemy TYPE 10, Spawns 2 enemies of its type when it dies, HP: 1
**********************************************************************/

/* Struct forward declarations */
typedef struct Enemy Enemy;
typedef struct Stage Stage;

/* Randomises the type of enemy to assign it to */
int EnemyType_Assigner(Enemy *enemy, Stage *stage);

/* Chooses which data to send for initialisation based on its type */
void EnemyType_Switcher(Enemy *enemy);

/* Initialises the unit based on its type and given data */
void EnemyType_Init(Enemy *enemy, const char *enemyChar[]);

/* Changes art of enemy based on HP when it gets hit */
void EnemyTypeHP_ArtSwitch(Enemy *enemy);