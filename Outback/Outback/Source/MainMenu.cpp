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
f32 curMainMenu = 0.15f;

AEGfxVertexList *vertex = 0;
AEGfxTexture	*texture = 0;

/******************************************************************************/
/*!
	"Load" Main menu
*/
/******************************************************************************/
void GameStateMainMenuLoad()
{
	AEGfxSetBackgroundColor(0.96f, 0.64f, 0.12f);

	/*
	GameObj* pObj;
	pObj = sGameObjList + sGameObjNum++;
	pObj->type = TYPE_OBJECT_EMPTY;

	//BG Image Mesh
	AEGfxMeshStart();
	AEGfxTriAdd(
		-10.5f, -10.5f, 0xFFFF0000, 0.0f, 1.0f,
		10.5f, -10.5f, 0xFFFF0000, 1.0f, 1.0f,
		-10.5f, 10.5f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxTriAdd(
		10.5, -10.5f, 0xFFFF0000, 1.0f, 1.0f,
		10.5f, 10.5f, 0xFFFF0000, 1.0f, 0.0f,
		-10.5f, 10.5f, 0xFFFF0000, 0.0f, 0.0f);

	pObj->pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(vertex, "Failed to create BG Mesh!");
	pObj->pTex = AEGfxTextureLoad("..\\Resources\\Textures\\Placeholder.png");
	AE_ASSERT_MESG(pObj->pTex, "Failed to create Block Texture!");
	*/

	//Highlight
	AEGfxMeshStart();
	AEGfxTriAdd(
		-10.5f, -10.5f, 0xFFFF0000, 0.0f, 1.0f,
		10.5f, -10.5f, 0xFFFF0000, 1.0f, 1.0f,
		-10.5f, 10.5f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxTriAdd(
		10.5, -10.5f, 0xFFFF0000, 1.0f, 1.0f,
		10.5f, 10.5f, 0xFFFF0000, 1.0f, 0.0f,
		-10.5f, 10.5f, 0xFFFF0000, 0.0f, 0.0f);

	vertex = AEGfxMeshEnd();
	AE_ASSERT_MESG(vertex, "Failed to create Enemy Mesh!");
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
	//Make rectangle mesh
	//render behind
	

	char strBuffer[100];
	memset(strBuffer, 0, 100 * sizeof(char));
	sprintf_s(strBuffer, "OUTBACK");

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxTextureSet(NULL, 0, 0);
	AEGfxSetTransparency(1.0f);

	//For the highlight
	// Set position for the highlight
	AEGfxMeshDraw(vertex, AE_GFX_MDM_TRIANGLES);
	AEGfxSetPosition(0.0f, 30.0f);

	//The title's position
	AEGfxPrint(fontTitle, strBuffer, -0.8f, 0.60f, 1.0f, 1.0f, 1.0f, 1.0f);
	//AEGfxPrint(fontID, "Please press 'ENTER' to confirm selection", -0.65f, 0.30f, 1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, "LEVEL 1", -0.7f, 0.15f, 1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, "LEVEL 2", -0.7f, 0.00f, 1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, "LEVEL 3", -0.7f, -0.15f, 1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, "EXIT",	  -0.7f, -0.3f, 1.0f, 1.0f, 1.0f, 1.0f);

	//The arrow to select
	AEGfxPrint(fontID, "<--", -0.45f, curMainMenu, 1.0f, 1.0f, 1.0f, 1.0f);
	//AEGfxPrint(fontID, "Press 'BACKSPACE' to return to Main Menu", -0.65f, -0.45f, 1.0f, 1.0f, 1.0f, 1.0f);
	//AEGfxPrint(fontID, "Collect all the coins to win!", -0.45f, -0.75f, 1.0f, 1.0f, 1.0f, 1.0f);
	//AEGfxPrint(fontID, "WASD and arrow keys compatable!", -0.55f, -0.90f, 1.0f, 1.0f, 1.0f, 1.0f);
	


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

}

/******************************************************************************/
/*!
	"Unload" Main menu
*/
/******************************************************************************/
void GameStateMainMenuUnload()
{

}