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
extern int level;
f32 currGG = -0.39f;
int selection = 0;
char gameOverMsg[100], brestart[100], mainMenu[100], quit[100], barrow[100], bcomment[100];


void OBGameOverLoad()
{
	AEGfxSetBackgroundColor(0.10f, 0.00f, 0.08f);
}

void OBGameOverInit()
{
	memset(gameOverMsg, 0, 100 * sizeof(char));
	sprintf_s(gameOverMsg, "Congratulations!!! You are dead, already");

	memset(brestart, 0, 100 * sizeof(char));
	sprintf_s(brestart, "Restart");

	memset(mainMenu, 0, 100 * sizeof(char));
	sprintf_s(mainMenu, "Main Menu");

	memset(quit, 0, 100 * sizeof(char));
	sprintf_s(quit, "Quit");

	memset(barrow, 0, 100 * sizeof(char));
	sprintf_s(barrow, "^^^");

	memset(bcomment, 0, 100 * sizeof(char));
	sprintf_s(bcomment, "Left/Right to control, SPACE to choose");
}

void OBGameOverUpdate()
{
	if (AEInputCheckTriggered(AEVK_RIGHT) || AEInputCheckTriggered(AEVK_D))
	{
		currGG += 0.30f;
		selection += 1;
	}

	if (AEInputCheckTriggered(AEVK_LEFT) || AEInputCheckTriggered(AEVK_A))
	{
		currGG -= 0.30f;
		selection -= 1;
	}

	if (AEInputCheckTriggered(AEVK_SPACE) && selection == 2)
	{
		gGameStateNext = GS_LEVEL1;
		level = 0;
	}

	//if (AEInputCheckTriggered(AEVK_RETURN) && currPause == 0.15f)
		//gGameStateNext = OB_INSTRUCTIONS;

	//if (AEInputCheckTriggered(AEVK_RETURN) && currGG == 0.00f)
		//gGameStateNext = OB_SETTINGS;

	//if (AEInputCheckTriggered(AEVK_RETURN) && currGG == -0.15f)
		//gGameStateNext = OB_CREDITS;

	if (AEInputCheckTriggered(AEVK_SPACE) && selection == 1)
		gGameStateNext = GS_MAINMENU;

	if (AEInputCheckTriggered(AEVK_SPACE) && selection == 0)
		gGameStateNext = GS_QUIT;

	if (AEInputCheckTriggered(AEVK_ESCAPE))
		gGameStateNext = GS_MAINMENU;

	if (selection > 2)
	{
		currGG = -0.39f;
		selection = 0;
	}
	if (selection < 0)
	{
		currGG = 0.21f;
		selection = 2;
	}

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
	AEGfxPrint(fontID, gameOverMsg, -0.5, 0.50f, 1.1f, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, brestart, 0.2, 0.15f, 1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, mainMenu, -0.16f, 0.15f, 1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, quit, -0.4f, 0.15f, 1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, barrow, currGG, 0.07f, 1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, bcomment, -0.45f, -0.20f, 1.0f, 1.0f, 1.0f, 1.0f);
	
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

