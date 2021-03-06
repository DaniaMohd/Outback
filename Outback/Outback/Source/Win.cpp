/****************************************************************************** /
/*!
\file			Win.cpp
\project name	Outback
\author(s)	 	Primary:	Binte Mohd Rizal, Nurul Dania

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "main.h"

extern s8 fontID;

/******************************************************************************/
/*!
	"Load" Main menu
*/
/******************************************************************************/
void GameStateWinLoad()
{
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.5f);
}

/******************************************************************************/
/*!
	"Initialize" Main menu
*/
/******************************************************************************/
void GameStateWinInit()
{

}

/******************************************************************************/
/*!
	"Update" Main menu
*/
/******************************************************************************/
void GameStateWinUpdate()
{
	if (AEInputCheckTriggered(AEVK_BACK))
	{
		gGameStateNext = GS_MAINMENU;
	}
}

/******************************************************************************/
/*!
	"Draw" Main menu
*/
/******************************************************************************/
void GameStateWinDraw()
{
	char strBuffer[100];
	memset(strBuffer, 0, 100 * sizeof(char));
	sprintf_s(strBuffer, "CONGRATULATIONS!!! YOU WON!!!");

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxTextureSet(NULL, 0, 0);
	AEGfxSetTransparency(1.0f);
	AEGfxPrint(fontID, strBuffer, -0.55f, 0.40f, 1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, "Press 'BACKSPACE' to return to main menu", -0.65f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
}

/******************************************************************************/
/*!
	"Free" Main menu
*/
/******************************************************************************/
void GameStateWinFree()
{

}

/******************************************************************************/
/*!
	"Unload" Main menu
*/
/******************************************************************************/
void GameStateWinUnload()
{

}