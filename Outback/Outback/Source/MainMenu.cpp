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
extern s8 fontTitle;
f32 curMainMenu = 0.15f * 360;
int curr = 0;
bool endless;
extern bool newGame;
extern int level;
bool fullscreen;

AEGfxVertexList *vertex = 0, *vertexBG = 0;
AEGfxTexture	*texture = 0;

char MainQuit[100], MainConfirm[100], exitComment[100];
AEGfxVertexList* MainExitMesh;
AEGfxTexture* MainExitTex;
AEGfxVertexList* selectionMainMesh;
int selectMainX;
int b = 0;
extern					s8 fontID;
int						exitDia;



/******************************************************************************/
/*!
	"Load" Main menu
*/
/******************************************************************************/
void GameStateMainMenuLoad()
{
	AEGfxSetBackgroundColor(0.96f, 0.64f, 0.12f);

	float camX = 0;
	float camY = 0;
	AEGfxSetCamPosition(camX, camY);
	
	//BG Image Mesh
	AEGfxMeshStart();
	AEGfxTriAdd(
				-400.5f, -400.5f, 0xFFFF0000, 0.0f, 1.0f,
				 400.5f, -400.5f, 0xFFFF0000, 1.0f, 1.0f,
				-400.5f,  400.5f, 0xFFFF0000, 0.0f, 0.0f);

	AEGfxTriAdd( 400.5,  -400.5f, 0xFFFF0000, 1.0f, 1.0f,
				 400.5f,  400.5f, 0xFFFF0000, 1.0f, 0.0f,
				-400.5f,  400.5f, 0xFFFF0000, 0.0f, 0.0f);

	vertexBG = AEGfxMeshEnd();
	AE_ASSERT_MESG(vertexBG, "Failed to create BG Mesh!");
	texture = AEGfxTextureLoad("..\\Resources\\Textures\\Placeholder.png");
	AE_ASSERT_MESG(texture, "Failed to create BG Texture!");
	

	//Highlight
	AEGfxMeshStart();
	AEGfxTriAdd(
		-50.0f, -10.5f, 0xFFFF0000, 0.0f, 1.0f,
		 50.0f, -10.5f, 0xFFFF0000, 1.0f, 1.0f,
		-50.0f,  10.5f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxTriAdd(
		 50.0, -10.5f, 0xFFFF0000, 1.0f, 1.0f,
		 50.0f, 10.5f, 0xFFFF0000, 1.0f, 0.0f,
		-50.0f, 10.5f, 0xFFFF0000, 0.0f, 0.0f);

	vertex = AEGfxMeshEnd();
	AE_ASSERT_MESG(vertex, "Failed to create Enemy Mesh!");


	AEGfxMeshStart();
	AEGfxTriAdd(
		-100.0f * 2, -100.0f, 0x00FF00FF, 0.0f, 0.0f,
		100.0f * 2, -100.0f, 0x00FFFF00, 0.0f, 0.0f,
		-100.0f * 2, 100.0f, 0x0000FFFF, 0.0f, 0.0f);
	AEGfxTriAdd(
		100.0f * 2, -100.0f, 0x00FFFFFF, 0.0f, 0.0f,
		-100.0f * 2, 100.0f, 0x00FFFFFF, 0.0f, 0.0f,
		100.0f * 2, 100.0f, 0x00FFFFFF, 0.0f, 0.0f);

	MainExitMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(MainExitMesh, "Failed to create range Mesh!");
	MainExitTex = AEGfxTextureLoad("..\\Resources\\Textures\\pausepop.png");
	AE_ASSERT_MESG(MainExitTex, "Failed to create pause text!!");
	memset(MainQuit, 0, 100 * sizeof(char));
	sprintf_s(MainQuit, "Please confirm to quit the program");
	memset(MainConfirm, 0, 100 * sizeof(char));
	sprintf_s(MainConfirm, "YES / NO");
	memset(exitComment, 0, 100 * sizeof(char));
	sprintf_s(exitComment, "Left/Right to control, SPACE to choose");

	//selection mesh
	AEGfxMeshStart();
	AEGfxTriAdd(
		-25.0f, -10.5f, 0xFFFF0000, 0.0f, 1.0f,
		25.0f, -10.5f, 0xFFFF0000, 1.0f, 1.0f,
		-25.0f, 10.5f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxTriAdd(
		25.0, -10.5f, 0xFFFF0000, 1.0f, 1.0f,
		25.0f, 10.5f, 0xFFFF0000, 1.0f, 0.0f,
		-25.0f, 10.5f, 0xFFFF0000, 0.0f, 0.0f);
	selectionMainMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(selectionMainMesh, "Failed to create Enemy Mesh!");
	selectMainX = -17;

	MainMenuBGMLoad();
}

/******************************************************************************/
/*!
	"Initialize" Main menu
*/
/******************************************************************************/
void GameStateMainMenuInit()
{
	AudioEngineInit();
	endless = false;
	newGame = true;
	level = 0;
	fullscreen = true;
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
	if (exitDia == 0)
	{
		if (curr > 3)
			curr = 0;

		if (curr < 0)
			curr = 3;

		if (AEInputCheckTriggered(AEVK_UP) || AEInputCheckTriggered(AEVK_W))
		{
			curMainMenu += 0.15f * 300;
			curr--;
		}

		if (AEInputCheckTriggered(AEVK_DOWN) || AEInputCheckTriggered(AEVK_S))
		{
			curMainMenu -= 0.15f * 300;
			curr++;
		}

		if (AEInputCheckReleased(AEVK_RETURN) && curr == 0)
		{
			gGameStateNext = GS_LVLSELECT;
		}

		if (AEInputCheckReleased(AEVK_RETURN) && curr == 1)
		{
			gGameStateNext = GS_OPTIONS;
		}

		if (AEInputCheckReleased(AEVK_RETURN) && curr == 2)
		{
			gGameStateNext = GS_CREDITS;
		}

		if (AEInputCheckReleased(AEVK_RETURN) && curr == 3)
		{
			exitDia = 1;
		}

		if (AEInputCheckTriggered(AEVK_N))
		{
			if (fullscreen == true)
			{
				fullscreen = false;
			}
			else
			{
				fullscreen = true;
			}

			AEToogleFullScreen(fullscreen);
			if (endless == true)
			{
				endless = false;
			}
			else if (endless == false)
			{
				endless = true;
			}
		}
		if (curMainMenu > 0.15f * 360)
			curMainMenu = -0.3f * 270;

		if (curMainMenu < -0.3f * 360)
			curMainMenu = 0.15f * 360;
	}
	if (exitDia == 1)
	{
		if (AEInputCheckTriggered(AEVK_A) || AEInputCheckTriggered(AEVK_LEFT))
		{
			if (b != 1)
				b = 1;
			selectMainX -= 62;
		}
		if (AEInputCheckTriggered(AEVK_D) || AEInputCheckTriggered(AEVK_RIGHT))
		{
			if (b != 1)
				b = 1;
			selectMainX += 62;
		}
		if (selectMainX < -17)
			selectMainX = 45;
		if (selectMainX > 45)
			selectMainX = -17;
		if (AEInputCheckTriggered(AEVK_SPACE) && b == 1)
		{
			if (selectMainX == -17)
			{
				gGameStateNext = GS_QUIT;
			}
			else
			{
				exitDia = 0;
			}
		}
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
	sprintf_s(strBuffer, "OUTBACK");

	//For BG image
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetPosition(0.5f, 0.5f);
	AEGfxTextureSet(texture, 0, 0);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxMeshDraw(vertexBG, AE_GFX_MDM_TRIANGLES);

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);

	if (exitDia == 0)
	{
		//For the highlight
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxMeshDraw(vertex, AE_GFX_MDM_TRIANGLES);
		// Set position for the highlight
		AEGfxSetPosition(-230, curMainMenu);
	}
	//The title's position
	AEGfxPrint(fontTitle, strBuffer, -0.8f, 0.60f, 1.0f, 1.0f, 1.0f, 1.0f);
	//AEGfxPrint(fontID, "Please press 'ENTER' to confirm selection", -0.65f, 0.30f, 1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, "LEVEL SELECTION", -0.7f, 0.15f, 1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, "SETTINGS", -0.7f, 0.00f, 1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, "CREDITS", -0.7f, -0.15f, 1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, "EXIT",	  -0.7f, -0.3f, 1.0f, 1.0f, 1.0f, 1.0f);

	//The arrow to select
	//AEGfxPrint(fontID, "<--", -0.45f, curMainMenu, 1.0f, 1.0f, 1.0f, 1.0f);
	//AEGfxPrint(fontID, "Press 'BACKSPACE' to return to Main Menu", -0.65f, -0.45f, 1.0f, 1.0f, 1.0f, 1.0f);
	//AEGfxPrint(fontID, "Collect all the coins to win!", -0.45f, -0.75f, 1.0f, 1.0f, 1.0f, 1.0f);
	//AEGfxPrint(fontID, "WASD and arrow keys compatable!", -0.55f, -0.90f, 1.0f, 1.0f, 1.0f, 1.0f);
	if (exitDia == 1)
	{
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		// Set position for object 2
		AEGfxSetPosition(0.0f, 0.0f);	//rtriangle
		// No tint
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		// Set texture
		AEGfxTextureSet(MainExitTex, 1, 1);
		AEGfxMeshDraw(MainExitMesh, AE_GFX_MDM_TRIANGLES);

		if (b == 1)
		{
			AEGfxSetRenderMode(AE_GFX_RM_COLOR);
			AEGfxMeshDraw(selectionMainMesh, AE_GFX_MDM_TRIANGLES);
			// Set position for the highlight
			AEGfxSetPosition(selectMainX, -52);
		}
		AEGfxPrint(fontID, MainQuit, -0.41f, 0.20f, 1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxPrint(fontID, exitComment, -0.45f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxPrint(fontID, MainConfirm, -0.1f, -0.2f, 1.0f, 1.0f, 1.0f, 1.0f);
	}


}

/******************************************************************************/
/*!
	"Free" Main menu
*/
/******************************************************************************/
void GameStateMainMenuFree()
{
	// Freeing the objects and textures
	AEGfxMeshFree(vertex);
	AEGfxMeshFree(vertexBG);

	AEGfxTextureUnload(texture);

}

/******************************************************************************/
/*!
	"Unload" Main menu
*/
/******************************************************************************/
void GameStateMainMenuUnload()
{
	MainMenuBGMUnload();
}