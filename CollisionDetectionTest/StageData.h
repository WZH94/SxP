/******************************************************************************
All content © 2017 Digipen Institute of Technology Singapore, all rights reserved

filename			Stage.h

authors				Ong Tze Ang(30%)
					Lois Ngm(30%)
					Wong ZhiHao(10%)
					Audry May dela Paz Enriquez(30%)
DP emails			ong.tzeang
					lois.ngm
					wong.zhihao
					a.enriquez

Brief Description:
Header file for StageData and macros defining important information for stage_create

******************************************************************************/

/*Player general information*/
#define PLAYER_HEALTH 3							/*X coordinate of the player's health stats*/
#define PLAYER_BOOST 30							/*X coordinate of the player's boost stats*/
#define PLAYER_SCORE 52							/*X coordinate of the player's score stats*/

/* Player one information */
#define PLAYER1_LEFT 65							 /*Player 1 button to move left -- A button*/
#define PLAYER1_RIGHT 68						/*Player 1 button to move right -- D button*/
#define PLAYER1_ATTACK 32						/*Player 1 button to shoot ball -- spacebar*/
#define PLAYER1_BOOST 87						/*Player 1 button to boost -- W*/
#define PLAYER1_START_POSX 10					/*Player 1 x coordinate when game start*/
#define PLAYER1_START_POSY 4					/*Player 1 y coordinate when game start*/
#define PLAYER1_STATBAR 1						/*Player 1 coordinate of stat bar */
#define PLAYER1_FIREBUTTON_SPACEBAR 32			/*Player 1 button to shoot ball -- spacebar*/


/* Player two information  */
#define PLAYER2_LEFT VK_LEFT					/*Player 2 button to move left -- left arrow*/
#define PLAYER2_RIGHT VK_RIGHT					/*Player 2 button to move right -- right arrow*/
#define PLAYER2_ATTACK 13						/*Player 2 button to shoot -- enter button*/
#define PLAYER2_BOOST VK_UP						/*Player 2 butto to boost -- up arrow*/
#define PLAYER2_START_POSX 5					/*Player 2 x coordinate when game starts*/
#define PLAYER2_START_POSY 72					/*Player 2 y coordinate when game starts*/
#define PLAYER2_STATBAR 76						/*Player 2 coordinate of stat bar */
#define PLAYER2_FIREBUTTON_ENTER 13				/*Player 2 button to shoot -- enter button*/

/*Ball one information */
#define Ball1StartPositionX 12					/*Starting X Position of the ball of player 1*/
#define Ball1StartPositionY 6					/*Starting Y Position of the ball of player 1*/

/*Ball two information */
#define Ball2StartPositionX 7					/*Starting X Position of the ball of player 2*/
#define Ball2StartPositionY 71					/*Starting Y Position of the ball of player 2*/

/*Function that prints the pause menu*/
void optionmenu_Print(Stage *stage, int CoOp_stage_num, int PvP_stage_num);

/*Function that clear pause menu*/
void optionmenu_Clear();

/*Typedef definitions*/
typedef struct Stage Stage;
typedef struct Player Player;
typedef struct Enemy Enemy;
typedef struct EnemyBullet EnemyBullet;
typedef struct Enemy_AI Enemy_AI;
typedef struct Powerup Powerup;
typedef struct Ball Ball;
typedef struct TileMapManager TileMapManager;

/* Frees the data of this object */
void Stages_Free(Stage *stage, Player *player_1, Player *player_2, Enemy *enemyP1[], Enemy *enemyP2[], EnemyBullet *enemyBullet[],
	Enemy_AI *enemy_AI_P1, Enemy_AI *enemy_AI_P2, Powerup *powerup[], Ball *ball_1, Ball *ball_2, TileMapManager *tileManager);