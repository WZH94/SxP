/******************************************************************************
All content © 2017 Digipen Institute of Technology Singapore, all rights reserved

filename    FMODInit.h
author      Wong Zhihao
DP email    zhihao.wong@digipen.edu
course      RTIS

Brief Description:
This file declares the Sound object and all the function declarations.
******************************************************************************/
#pragma once
#include "fmod.h"		  /* FMOD functions */

/* Contents of Sound object */
typedef struct Sound {
	FMOD_RESULT result;			/* For error checking */
	FMOD_SYSTEM *Fmodsystem;	/* The FMOD System to pass around the entire project */
	FMOD_SOUND *Sound;			/* The Sound */
	FMOD_SOUND *Music;			/* Music */
	FMOD_CHANNEL *Player1;		/* Channel for player 1 panning */
	FMOD_CHANNEL *Player2;		/* Channel for player 2 panning */
	FMOD_CHANNEL *Low;			/* Low priority sounds, lower volume */
	FMOD_CHANNEL *Medium;		/* Medium priority sounds, medium volume */
	FMOD_CHANNEL *High;			/* High priority sounds, high volume */
	FMOD_CHANNEL *MainMenu;
	FMOD_CHANNEL *Battle;
} Sound;

/* Error checking function for FMOD */
void FmodErrorCheck(FMOD_RESULT result);

/* Creates the FMOD object */
Sound *initialiseFMOD();

/* Frees the Sound object */
void Sound_Free(Sound **soundPtr);

/* Updates FMOD every frame */
void updateFMOD(Sound *sound);

/* Loads the music into memory through stream */
void loadMusic_Menu(Sound *sound, const char * filename);

/* Loads the music into memory through stream */
void loadMusic_Battle(Sound *sound, const char * filename);

/* Plays the Main Menu music through the Music CHANNEL */
void playMusic_Menu(Sound *sound);

/* Plays the Battle music through the Battle CHANNEL */
void playMusic_Battle(Sound *sound);

/* Stops the music in the inputted CHANNEL */
void stopMusic(FMOD_CHANNEL *channel);

/* Loads sounds into memory */
void loadSound(Sound *sound, const char * filename);

/* Plays the sound of player 1 sounds */
void playSound_Player1(Sound *sound, int x);

/* Plays the sound of player 2 sounds */
void playSound_Player2(Sound *sound, int x);

/* Plays low priority sounds */
void playSound_Low(Sound *sound);

/* Plays medium priority sounds */
void playSound_Medium(Sound *sound);

/* Plays high priority sounds */
void playSound_High(Sound *sound);

/* Free the FMOD Objects */
void releaseFMOD(Sound *sound);