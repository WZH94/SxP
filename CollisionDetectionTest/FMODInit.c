/******************************************************************************
All content © 2017 Digipen Institute of Technology Singapore, all rights reserved

filename			FMODInit.c
author				Wong Zhihao
DP email			zhihao.wong@digipen.edu
course				RTIS

Brief Description:
Contents of sound object of the game, and handles all audio related functions.

******************************************************************************/

#include "stdafx.h"
#include "FMODInit.h"	  /* Function declarations */
#include "fmod_errors.h"  /* FMOD Error codes */
#include <stdio.h>		  /* printf */
#include <stdlib.h>		  /* exit, malloc */

/* Error checking function for FMOD */
void FmodErrorCheck(FMOD_RESULT result)
{
	/* If FMOD has an error, FMOD is 0 */
	if (result != FMOD_OK)
	{
		/* Prints the FMOD error if there is any and shuts down the program */
		printf("FMOD error! (%d) (%s)", result, FMOD_ErrorString(result));
		exit(-1);
	}
}

/* Creates the FMOD object */
Sound *initialiseFMOD()
{
	/* the new object to be returned */
	Sound *sound = NULL;

	/* allocate the memory for a new Sound object */
	sound = (Sound*)malloc(sizeof(Sound));

	if (sound == NULL)
	{
		return NULL; /* malloc can fail */
	}

	/* Create the FMOD interface object */
	FmodErrorCheck(FMOD_System_Create(&sound->Fmodsystem));

	/* Initialise FMOD with the interface object, 128 channels */
	FmodErrorCheck(FMOD_System_Init(sound->Fmodsystem, 128, FMOD_INIT_NORMAL, 0));

	/* Returns the object created */
	return sound;
}

/* Frees the Sound object */
void Sound_Free(Sound **soundPtr)
{
	releaseFMOD(*soundPtr);

	/* safety check on the parameters */
	if ((soundPtr == NULL) || (*soundPtr == NULL))
	{
		return;  /* take no action if the parameters are invalid */
	}

	/* free the given item */
	free(*soundPtr);

	/* clear the provided pointer */
	*soundPtr = NULL;
}

/* Updates FMOD every frame */
void updateFMOD(Sound *sound)
{
	FMOD_System_Update(sound->Fmodsystem);
}

/* Adjusts pan level for sounds originating from player 1 */
void SoundChannel_Player1_Pan(Sound *sound, int x)
{
	float pan = ((float)x - 35) / 35;
	FmodErrorCheck(FMOD_Channel_SetPan(sound->Player1, pan));
}

/* Adjusts pan level for sounds originating from player 2 */
void SoundChannel_Player2_Pan(Sound *sound, int x)
{
	float pan = ((float)x - 35) / 35;
	FmodErrorCheck(FMOD_Channel_SetPan(sound->Player2, pan));
}

/* Loads sounds into memory */
void loadSound(Sound *sound, const char * filename)
{
	/* Path and filename of audio file to be loaded */
	const char * soundname = filename;
	FmodErrorCheck(FMOD_System_CreateSound(sound->Fmodsystem, soundname, FMOD_DEFAULT, 0, &sound->Sound));
}

/* Loads the music into memory through stream */
void loadMusic_Menu(Sound *sound, const char * filename)
{
	/* Path and filename of audio file to be loaded */
	const char * soundname = filename;
	FmodErrorCheck(FMOD_System_CreateStream(sound->Fmodsystem, soundname, FMOD_LOOP_NORMAL, 0, &sound->Music));
}

/* Loads the music into memory through stream */
void loadMusic_Battle(Sound *sound, const char * filename)
{
	/* Path and filename of audio file to be loaded */
	const char * soundname = filename;
	FmodErrorCheck(FMOD_System_CreateStream(sound->Fmodsystem, soundname, FMOD_LOOP_NORMAL, 0, &sound->Music));
}

/* Plays the Main Menu music through the Music CHANNEL */
void playMusic_Menu(Sound *sound)
{
	FmodErrorCheck(FMOD_System_PlaySound(sound->Fmodsystem, sound->Music, NULL, 0, &sound->MainMenu));
}

/* Plays the Battle music through the Battle CHANNEL */
void playMusic_Battle(Sound *sound)
{
	FmodErrorCheck(FMOD_System_PlaySound(sound->Fmodsystem, sound->Music, NULL, 0, &sound->Battle));
}

/* Stops the music in the inputted CHANNEL */
void stopMusic(FMOD_CHANNEL *channel)
{
	FmodErrorCheck(FMOD_Channel_Stop(channel));
}

/* Plays the sound of player 1 sounds */
void playSound_Player1(Sound *sound, int x)
{
	/* Starts the sound paused */
	FmodErrorCheck(FMOD_System_PlaySound(sound->Fmodsystem, sound->Sound, NULL, 1, &sound->Player1));
	/* Sets the volume to 2 out of 1 (amplified) cause my sounds are soft */
	FmodErrorCheck(FMOD_Channel_SetVolume(sound->Player1, 2.0f));
	/* Pans the sound based on the x location of player 1 */
	SoundChannel_Player1_Pan(sound, x);
	/* Unpauses the sound */
	FmodErrorCheck(FMOD_Channel_SetPaused(sound->Player1, 0));
}

/* Plays the sound of player 1 sounds */
void playSound_Player2(Sound *sound, int x)
{
	/* Starts the sound paused */
	FmodErrorCheck(FMOD_System_PlaySound(sound->Fmodsystem, sound->Sound, NULL, 1, &sound->Player2));
	/* Sets the volume to 2 out of 1 (amplified) cause my sounds are soft */
	FmodErrorCheck(FMOD_Channel_SetVolume(sound->Player2, 2.0f));
	/* Pans the sound based on the x location of player 2 */
	SoundChannel_Player2_Pan(sound, x);
	/* Unpauses the sound */
	FmodErrorCheck(FMOD_Channel_SetPaused(sound->Player2, 0));
}

/* Plays low priority sounds */
void playSound_Low(Sound *sound)
{
	/* Starts the sound paused */
	FmodErrorCheck(FMOD_System_PlaySound(sound->Fmodsystem, sound->Sound, NULL, 1, &sound->Low));
	/* Sets the volume to 1 out of 1 (max unamplifed volume) for low priority sounds in the game the player does not really need to hear */
	FmodErrorCheck(FMOD_Channel_SetVolume(sound->Low, 1.0f));
	/* Unpauses the sound */
	FmodErrorCheck(FMOD_Channel_SetPaused(sound->Low, 0));
}

/* Plays medium priority sounds */
void playSound_Medium(Sound *sound)
{
	/* Starts the sound paused */
	FmodErrorCheck(FMOD_System_PlaySound(sound->Fmodsystem, sound->Sound, NULL, 1, &sound->Medium));
	/* Sets the volume to 1.7 out of 1 (amplifed volume) for medium priority sounds in the game the player has to be aware of */
	FmodErrorCheck(FMOD_Channel_SetVolume(sound->Medium, 1.7f));
	/* Unpauses the sound */
	FmodErrorCheck(FMOD_Channel_SetPaused(sound->Medium, 0));
}

/* Plays high priority sounds */
void playSound_High(Sound *sound)
{
	/* Starts the sound paused */
	FmodErrorCheck(FMOD_System_PlaySound(sound->Fmodsystem, sound->Sound, NULL, 1, &sound->High));
	/* Sets the volume to 3 out of 1 (amplifed volume) for high priority sounds in the game the player MUST be aware of */
	FmodErrorCheck(FMOD_Channel_SetVolume(sound->High, 3.0f));
	/* Unpauses the sound */
	FmodErrorCheck(FMOD_Channel_SetPaused(sound->High, 0));
}


/* Free the FMOD Objects */
void releaseFMOD(Sound *sound)
{
	/* Releases the Sound */
	FmodErrorCheck(FMOD_Sound_Release(sound->Sound));
	/* Releases the system */
	FmodErrorCheck(FMOD_System_Release(sound->Fmodsystem));
}