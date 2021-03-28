/****************************************************************************** /
/*!
\file		MainMenu.cpp
\author 	Sim Xuan Yee
\par    	email: xuanyee.s@digipen.edu
\date		14th March 2021
\brief		This file contains the definitions of the functions needed to run
			the pasuescreen of the game

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "Main.h"

extern s8 fontID;
f32 currGG = -0.39f;
char gameOverMsg[100], brestart[100], mainMenu[100], quit[100], barrow[100], bcomment[100];



void OBGameOverLoad()
{
	AEGfxSetBackgroundColor(0.10f, 0.00f, 0.08f);
}

void OBGameOverInit()
{
	memset(gameOverMsg, 0, 100 * sizeof(char));
	sprintf_s(gameOverMsg, "You are dead, already");

	memset(brestart, 0, 100 * sizeof(char));
	sprintf_s(brestart, "Restart");

	memset(mainMenu, 0, 100 * sizeof(char));
	sprintf_s(mainMenu, "Main Menu");

	memset(quit, 0, 100 * sizeof(char));
	sprintf_s(quit, "Quit");

	memset(barrow, 0, 100 * sizeof(char));
	sprintf_s(barrow, "^^^");

	memset(bcomment, 0, 100 * sizeof(char));
	sprintf_s(bcomment, "Left/Right to control, Enter to choose");
}

void OBGameOverUpdate()
{
	if (AEInputCheckTriggered(AEVK_RIGHT))
		currGG += 0.30f;

	if (AEInputCheckTriggered(AEVK_LEFT))
		currGG -= 0.30f;

	if (AEInputCheckTriggered(AEVK_RETURN) && currGG >= 0.21f)
		gGameStateNext = GS_RESTART;

	//if (AEInputCheckTriggered(AEVK_RETURN) && currPause == 0.15f)
		//gGameStateNext = OB_INSTRUCTIONS;

	//if (AEInputCheckTriggered(AEVK_RETURN) && currGG == 0.00f)
		//gGameStateNext = OB_SETTINGS;

	//if (AEInputCheckTriggered(AEVK_RETURN) && currGG == -0.15f)
		//gGameStateNext = OB_CREDITS;

	if (AEInputCheckTriggered(AEVK_RETURN) && (currGG > -0.09f && currGG < -0.21))
		gGameStateNext = GS_MAINMENU;

	if (AEInputCheckTriggered(AEVK_RETURN) && (currGG > -0.39f && currGG < -0.09)
		|| AEInputCheckTriggered(AEVK_ESCAPE))
		gGameStateNext = GS_QUIT;

	if (currGG > 0.22f)
		currGG = -0.39f;

	if (currGG < -0.40f)
		currGG = 0.21f;
}

void OBGameOverDraw()
{
	//char strBuffer[100];
	//memset(strBuffer, 0, 100 * sizeof(char));
	//sprintf_s(strBuffer, "OUTBACK");
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxTextureSet(NULL, 0, 0);
	AEGfxSetTransparency(1.0f);
	AEGfxPrint(fontID, gameOverMsg, -0.3, 0.50f, 1.1f, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, brestart, 0.2, 0.15f, 1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, mainMenu, -0.16f, 0.15f, 1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, quit, -0.4f, 0.15f, 1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, barrow, currGG, 0.07f, 1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, bcomment, -0.40f, -0.20f, 1.0f, 1.0f, 1.0f, 1.0f);
	
}

void OBGameOverFree()
{
	/*free(cont);
	free(restart);
	free(setting);
	free(sound);
	free(help);
	free(aexit);
	free(arrow);
	free(comment);*/
}

void OBGameOverUnload()
{

}

