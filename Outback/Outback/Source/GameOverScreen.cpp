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
int selection = 0;
char gameOverMsg[100], brestart[100], mainMenu[100], quit[100], barrow[100], bcomment[100];
char ggQuit[100], ggConfirm[100];
int selectedA;
AEGfxVertexList* ggExitMesh;
AEGfxTexture* ggExitTex;


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

	AEGfxMeshStart();
	AEGfxTriAdd(
		-100.0f * 2, -100.0f, 0x00FF00FF, 0.0f, 0.0f,
		100.0f * 2, -100.0f, 0x00FFFF00, 0.0f, 0.0f,
		-100.0f * 2, 100.0f, 0x0000FFFF, 0.0f, 0.0f);
	AEGfxTriAdd(
		100.0f * 2, -100.0f, 0x00FFFFFF, 0.0f, 0.0f,
		-100.0f * 2, 100.0f, 0x00FFFFFF, 0.0f, 0.0f,
		100.0f * 2, 100.0f, 0x00FFFFFF, 0.0f, 0.0f);

	ggExitMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(ggExitMesh, "Failed to create range Mesh!");
	ggExitTex = AEGfxTextureLoad("..\\Resources\\Textures\\pausepop.png");
	AE_ASSERT_MESG(ggExitTex, "Failed to create pause text!!");
	memset(ggQuit, 0, 100 * sizeof(char));
	sprintf_s(ggQuit, "Please confirm to quit the program");
	memset(ggConfirm, 0, 100 * sizeof(char));
	sprintf_s(ggConfirm, "Press Y to quit, Press N to go Menu");
	selectedA = 0;
}

void OBGameOverUpdate()
{
	if (selectedA == 0)
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
		}
		if (AEInputCheckTriggered(AEVK_SPACE) && selection == 1)
		{
			gGameStateNext = GS_MAINMENU;
		}

		if (AEInputCheckTriggered(AEVK_SPACE) && selection == 0)
		{
			selectedA = 1;
		}


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
	if (selectedA == 1)
	{
		if (AEInputCheckTriggered(AEVK_Y))
		{
			gGameStateNext = GS_QUIT;
		}
		if (AEInputCheckTriggered(AEVK_N))
		{
			selectedA = 0;
		}
	}
}

void OBGameOverDraw()
{
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
	if (selectedA == 1)
	{
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		// Set position for object 2
		AEGfxSetPosition(0.0f, 0.0f);	//rtriangle
		// No tint
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		// Set texture
		AEGfxTextureSet(ggExitTex, 1, 1);
		AEGfxMeshDraw(ggExitMesh, AE_GFX_MDM_TRIANGLES);
		AEGfxPrint(fontID, ggQuit, -0.41f, 0.1f, 1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxPrint(fontID, ggConfirm, -0.43f, -0.1f, 1.0f, 1.0f, 1.0f, 1.0f);
	}
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

