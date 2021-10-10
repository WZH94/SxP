/******************************************************************************
All content © 2017 Digipen Institute of Technology Singapore, all rights reserved

filename			WorldData.h
author				Ong Tze Ang(95%)
Lois Ngm(5%)
DP email			ong.tzeang
lois.ngm


Brief Description:
This file declares the WorldData interface, which is used to manage the entire
set of data in the game world.

******************************************************************************/

#pragma once

/*Move cursor to the input coordinates*/
void WorldData_GoToXY(int eex, int eey);

/* Hide cursor */
void hidecursor();

/* Show cursor */
void showcursor();

/* Sets the console window size */
void SetConsoleWindowSize(short x, short y);

/*Opens the console window in a specific position*/
void SetConsoleWindowPos();

/*Sets the font size of the text for the entire game*/
void ConsoleFontControl();