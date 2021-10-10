/******************************************************************************
All content © 2017 Digipen Institute of Technology Singapore, all rights reserved

filename			WorldData.c
author				Ong Tze Ang(95%)
					Lois Ngm(5%)
DP email			ong.tzeang
					lois.ngm


Brief Description:
This file defines the WorldData interface, which is used to manage the entire
set of data in the game world.

******************************************************************************/

#include "stdafx.h"			/* NULL, malloc/free, printf, strcpy_s */
#include <windows.h>		/*SetConsoleCursorPosition*/
#include "WorldData.h"		/* Function declarations */


/*Move cursor to the input coordinates*/
void WorldData_GoToXY(int eex, int eey)
{
	COORD coord;				/*Coordinate system*/
	coord.X = (short)eex;		/*X coordinates*/
	coord.Y = (short)eey;		/*Y coordinates*/
								/*Move cursor to the X and Y*/
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

/* Hide the cursor */
void hidecursor()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	/*The percentage of the character cell that is filled by the cursor*/
	info.dwSize = 100;
	/*Set cursor to be visible*/
	info.bVisible = FALSE;
	/*Sets the size and visibility of the cursor for the specified console screen buffer*/
	SetConsoleCursorInfo(consoleHandle, &info);
}

/* Shows the cursor*/
void showcursor()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	/*The percentage of the character cell that is filled by the cursor*/
	info.dwSize = 100;
	/*Set cursor to be visible*/
	info.bVisible = TRUE;
	/*Sets the size and visibility of the cursor for the specified console screen buffer*/
	SetConsoleCursorInfo(consoleHandle, &info);
}

/* Sets the console window size according to input*/
void SetConsoleWindowSize(short x, short y)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	/*Sets the title of the window to the game name*/
	SetConsoleTitle(TEXT("SxP"));

	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
	/*Retrieves information about the specified console screen buffer*/
	if (!GetConsoleScreenBufferInfo(h, &bufferInfo));;
	SMALL_RECT winInfo = bufferInfo.srWindow;
	COORD windowSize = { winInfo.Right - winInfo.Left + 1, winInfo.Bottom - winInfo.Top + 1 };

	if (windowSize.X > x || windowSize.Y > y)
	{
		/* window size needs to be adjusted before the buffer size can be reduced.*/
		SMALL_RECT info =
		{
			0,
			0,
			x < windowSize.X ? x - 1 : windowSize.X - 1,
			y < windowSize.Y ? y - 1 : windowSize.Y - 1
		};

		if (!SetConsoleWindowInfo(h, TRUE, &info));

	}

	COORD size = { x, y };
	/*Change the internal buffer size*/
	if (!SetConsoleScreenBufferSize(h, size));


	SMALL_RECT info = { 0, 0, x - 1, y - 1 };
	/*Change the console window size*/
	if (!SetConsoleWindowInfo(h, TRUE, &info));
}

/*Opens the console window in a specific position*/
void SetConsoleWindowPos()
{
	/*Get the window console handle(isn't the right code but works for these sample*/
	HWND ConsoleWindow;
	ConsoleWindow = GetForegroundWindow();

	/*Getting the desktop handle and rectangl*/
	HWND   hwndScreen;
	RECT   rectScreen;
	hwndScreen = GetDesktopWindow();
	GetWindowRect(hwndScreen, &rectScreen);

	/* Get the current width and height of the console*/
	RECT rConsole;
	GetWindowRect(ConsoleWindow, &rConsole);
	int Width = rConsole.left = rConsole.right;
	int Height = rConsole.bottom - rConsole.top;

	/*calculate the window console to center of the screen	*/
	SetWindowPos(ConsoleWindow, NULL, 0, 0, Width, Height, SWP_SHOWWINDOW || SWP_NOSIZE);
}

/*Sets the font size of the text for the entire game*/
void ConsoleFontControl()
{
	CONSOLE_FONT_INFOEX info = { 0 };
	info.cbSize = sizeof(info);
	/*Sets the height of each character as 10 in the font, leave width as zero and font weight as normal*/
	info.dwFontSize.Y = 10;
	info.FontWeight = FW_NORMAL;
	/*Sets the typeface to consolas*/
	wcscpy(info.FaceName, L"Consolas");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), 0, &info);
}
