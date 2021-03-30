/****************************************************************************** /
/*!
\file		MainMenu.cpp
\author 	Sim Xuan Yee
\par    	email: xuanyee.s@digipen.edu
\date		14th March 2021
\brief		This file contains the definitions of the functions needed to run
			the pasuescreen of the game
			//to be deleted
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "PauseScreen.h"
#include "Main.h"

extern s8 fontID;
f32 currPause = 0.30f;
char cont[100], restart[100], setting[100], sound[100], help[100], aexit[100], arrow[100], comment[100], mm[100];



void OBPauseScreenLoad()
{
	AEGfxSetBackgroundColor(0.83f, 0.54f, 0.00f);
}

void OBPauseScreenInit()
{
	memset(cont, 0, 100 * sizeof(char));
	sprintf_s(cont, "Continue");

	memset(restart, 0, 100 * sizeof(char));
	sprintf_s(restart, "Restart");

	memset(setting, 0, 100 * sizeof(char));
	sprintf_s(setting, "Setting");

	memset(sound, 0, 100 * sizeof(char));
	sprintf_s(sound, "Sound : On");

	memset(help, 0, 100 * sizeof(char));		//might change to sfx
	sprintf_s(help, "Help");

	memset(aexit, 0, 100 * sizeof(char));
	sprintf_s(aexit, "Exit");

	memset(mm, 0, 100 * sizeof(char));
	sprintf_s(mm, "Return to MainMenu");

	memset(arrow, 0, 100 * sizeof(char));
	sprintf_s(arrow, "--->");

	memset(comment, 0, 100 * sizeof(char));
	sprintf_s(comment, "You are PAUSED (Playn't)");
}

void OBPauseScreenUpdate()
{
	if (AEInputCheckTriggered(AEVK_UP))
		currPause += 0.15f;

	if (AEInputCheckTriggered(AEVK_DOWN))
		currPause -= 0.15f;

	if (AEInputCheckTriggered(AEVK_RETURN) && currPause == 0.45f)
		gGameStateNext = GS_LEVEL1;

	//if (AEInputCheckTriggered(AEVK_RETURN) && currPause == 0.15f)
		//gGameStateNext = OB_INSTRUCTIONS;

	//if (AEInputCheckTriggered(AEVK_RETURN) && currPause == 0.00f)
		//gGameStateNext = OB_SETTINGS;

	//if (AEInputCheckTriggered(AEVK_RETURN) && currPause == -0.15f)
		//gGameStateNext = OB_CREDITS;

	if (AEInputCheckTriggered(AEVK_RETURN) && currPause == -0.30f)
		gGameStateNext = GS_MAINMENU;

	if (AEInputCheckTriggered(AEVK_RETURN) && currPause == -0.45f
		|| AEInputCheckTriggered(AEVK_ESCAPE))
		gGameStateNext = GS_QUIT;

	if (currPause > 0.50f)
		currPause = -0.45f;

	if (currPause < -0.50f)
		currPause = 0.45f;
}

void OBPauseScreenDraw()
{
	//char strBuffer[100];
	//memset(strBuffer, 0, 100 * sizeof(char));
	//sprintf_s(strBuffer, "OUTBACK");
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxTextureSet(NULL, 0, 0);
	AEGfxSetTransparency(1.0f);
	AEGfxPrint(fontID, cont, -0.1f, 0.45f, 1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, restart, -0.1f, 0.30f, 1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, setting, -0.1f, 0.15f, 1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, sound, -0.10f, 0.00f, 1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, help, -0.10f, -0.15f, 1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, mm, -0.10f, -0.30f, 1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, aexit, -0.10f, -0.45f, 1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, arrow, -0.3f, currPause, 1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, comment, -0.40f, -0.60f, 1.0f, 1.0f, 1.0f, 1.0f);
	
}

void OBPauseScreenFree()
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

void OBPauseScreenUnload()
{

}

