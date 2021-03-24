/******************************************************************************/
/*!
\file		MainMenu.cpp
\author 	Javin Ong J-min
\par    	email: ong.j@digipen.edu
\date   	15/03/2021
\brief      This file contains the definition for the main menu function.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "main.h"

extern s8 fontID;
f32 curMainMenu = 0.15f;

/******************************************************************************/
/*!
	"Load" Main menu
*/
/******************************************************************************/
void GameStateMainMenuLoad()
{
	AEGfxSetBackgroundColor(0.96f, 0.64f, 0.12f);
}

/******************************************************************************/
/*!
	"Initialize" Main menu
*/
/******************************************************************************/
void GameStateMainMenuInit()
{

}

/******************************************************************************/
/*!
	"Update" Main menu
*/
/******************************************************************************/
void GameStateMainMenuUpdate()
{
	/*
	1. check if the cursor > 0.15
		set the cursor -0.15
	2. check if the cursor < -0.15
		set the cursor 0.15
	3. if up key
		cursor += 0.15f
	4. if down key
		cursor -= 0.15f
	*/

	if (curMainMenu > 0.15f)
		curMainMenu = -0.3f;

	if (curMainMenu < -0.3f)
		curMainMenu = 0.15f;

	if (AEInputCheckTriggered(AEVK_UP) || AEInputCheckTriggered(AEVK_W))
		curMainMenu += 0.15f;

	if (AEInputCheckTriggered(AEVK_DOWN) || AEInputCheckTriggered(AEVK_S))
		curMainMenu -= 0.15f;

	if (AEInputCheckTriggered(AEVK_RETURN) && curMainMenu == 0.15f)
	{
		gGameStateNext = GS_LEVEL1;
	}

	if (AEInputCheckTriggered(AEVK_RETURN) && curMainMenu == 0.0f)
	{
		gGameStateNext = GS_LEVEL2;
	}

	if (AEInputCheckTriggered(AEVK_RETURN) && curMainMenu == -0.15f)
	{
		gGameStateNext = GS_LEVEL3;
	}

	if (AEInputCheckTriggered(AEVK_RETURN) && curMainMenu == -0.3f
		|| AEInputCheckTriggered(AEVK_ESCAPE))
	{
		gGameStateNext = GS_QUIT;
	}
}

/******************************************************************************/
/*!
	"Draw" Main menu
*/
/******************************************************************************/
void GameStateMainMenuDraw()
{
	char strBuffer[100];
	memset(strBuffer, 0, 100 * sizeof(char));
	sprintf_s(strBuffer, "PLATFORMER GAME");

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxTextureSet(NULL, 0, 0);
	AEGfxSetTransparency(1.0f);
	AEGfxPrint(fontID, strBuffer, -0.35f, 0.40f, 1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, "Please press 'ENTER' to confirm selection", -0.65f, 0.30f, 1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, "Level 1", -0.15f, 0.15f, 1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, "Level 2", -0.15f, 0.00f, 1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, "Level 3", -0.15f, -0.15f, 1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, "EXIT", -0.15f, -0.3f, 1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, "--->", -0.45f, curMainMenu, 1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, "Press 'BACKSPACE' to return to Main Menu", -0.65f, -0.45f, 1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, "Collect all the coins to win!", -0.45f, -0.75f, 1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, "WASD and arrow keys compatable!", -0.55f, -0.90f, 1.0f, 1.0f, 1.0f, 1.0f);
}

/******************************************************************************/
/*!
	"Free" Main menu
*/
/******************************************************************************/
void GameStateMainMenuFree()
{

}

/******************************************************************************/
/*!
	"Unload" Main menu
*/
/******************************************************************************/
void GameStateMainMenuUnload()
{

}