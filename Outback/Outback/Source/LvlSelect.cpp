/****************************************************************************** /
/*!
\file			LvlSelect.cpp
\project name	Outback
\author(s)	 	Primary:	Lim Sim Chee, Shannon
				Secondary:	Binte Mohd Rizal, Nurul Dania

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "main.h"

extern s8 fontID;
extern s8 fontTitle;
f32 currLevel = 0.30f * 360;
int lvlSelect = 0;
extern bool endless;
extern bool newGame;
extern int level;
extern bool fullscreen;

AEGfxVertexList	* lvlMesh = 0 , * hlMesh = 0;
AEGfxTexture	* lvlTex = 0;

void LvlSelectLoad()
{

	AEGfxSetCamPosition(0, 0);
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	// Create level selection header
	AEGfxMeshStart();
	AEGfxTriAdd(
		-90.0f, -90.0f, 0xFFFFFFFF, 0.0f, 1.0f,
		450.0f, -90.0f, 0xFFFFFFFF, 1.0f, 1.0f,
		-90.0f, 90.0f, 0xFFFFFFFF, 0.0f, 0.0f
	);

	AEGfxTriAdd(
		450.0f, -90.0f, 0xFFFFFFFF, 1.0f, 1.0f,
		450.0f, 90.0f, 0xFFFFFFFF, 1.0f, 0.0f,
		-90.0f, 90.0f, 0xFFFFFFFF, 0.0f, 0.0f
	);

	lvlMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(lvlMesh, "Failed to create level mesh!");
	lvlTex = AEGfxTextureLoad("..\\Resources\\Logo\\LvlSelect0.png");
	AE_ASSERT_MESG(lvlTex, "Failed to create level texture!");

	//Highlight
	AEGfxMeshStart();
	AEGfxTriAdd(
		-50.0f, -10.5f, 0xFFFF0000, 0.0f, 1.0f,
		50.0f, -10.5f, 0xFFFF0000, 1.0f, 1.0f,
		-50.0f, 10.5f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxTriAdd(
		50.0, -10.5f, 0xFFFF0000, 1.0f, 1.0f,
		50.0f, 10.5f, 0xFFFF0000, 1.0f, 0.0f,
		-50.0f, 10.5f, 0xFFFF0000, 0.0f, 0.0f);

	hlMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(hlMesh, "Failed to create Enemy Mesh!");

	MainMenuBGMLoad();
}

void LvlSelectInit()
{
	AudioEngineInit();
	endless = false;
	newGame = true;
	level = 0;
	fullscreen = true;
}

void LvlSelectUpdate()
{

	if (AEInputCheckTriggered(AEVK_UP) || AEInputCheckTriggered(AEVK_W))
	{
		currLevel += 0.30f * 300;
		lvlSelect--;
	}

	if (AEInputCheckTriggered(AEVK_DOWN) || AEInputCheckTriggered(AEVK_S))
	{
		currLevel -= 0.30f * 300;
		lvlSelect++;
	}

	if (AEInputCheckTriggered(AEVK_RETURN) && lvlSelect == 0)
	{
		gGameStateNext = GS_TUTORIAL;
	}

	if (AEInputCheckTriggered(AEVK_RETURN) && lvlSelect == 1)
	{
		gGameStateNext = GS_LEVEL1;
	}

	if (AEInputCheckTriggered(AEVK_RETURN) && lvlSelect == 2)
	{
		gGameStateNext = GS_LEVEL1;
		endless = true;
	}

	if (AEInputCheckTriggered(AEVK_BACK))
	{
		gGameStateNext = GS_MAINMENU;
	}

	if (lvlSelect > 2)
		lvlSelect = 0;

	if (lvlSelect < 0)
		lvlSelect = 2;

	if (currLevel > 0.30f * 360)
		currLevel = -0.45f * 270;

	if (currLevel < -0.30f * 360)
		currLevel = 0.45 * 360;
}

void LvlSelectDraw()
{
	char strBuffer[100];
	memset(strBuffer, 0, 100 * sizeof(char));
	sprintf_s(strBuffer, "-->");

	// Drawing Level header
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetPosition(-175.0f, 235.0f);
	AEGfxTextureSet(lvlTex, 0.0f, 0.0f);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetTransparency(1.0f);
	AEGfxMeshDraw(lvlMesh, AE_GFX_MDM_TRIANGLES);

	switch (lvlSelect)
	{
	case 0:
		AEGfxPrint(fontID, strBuffer, -0.45f, 0.30f, 1.0f, 1.0f, 1.0f, 1.0f);
		break;
	case 1:
		AEGfxPrint(fontID, strBuffer, -0.45f, 0.00f, 1.0f, 1.0f, 1.0f, 1.0f);
		break;
	case 2:
		AEGfxPrint(fontID, strBuffer, -0.45f, -0.30f, 1.0f, 1.0f, 1.0f, 1.0f);
		break;
	default:
		AE_FATAL_ERROR("Cannot select level!!");
	}


	AEGfxPrint(fontID, "TUTORIAL", -0.15f, 0.30f, 1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, "NORMAL MODE", -0.15f, 0.00f, 1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, "ENDLESS MODE", -0.15f, -0.30f, 1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, "Press BACKSPACE", -0.9f, -0.75f, 1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, "for Main Menu", -0.9f, -0.95f, 1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, "Press ENTER to", 0.3f, -0.75f, 1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, "select level", 0.3f, -0.95f, 1.0f, 1.0f, 1.0f, 1.0f);
}


void LvlSelectFree()
{
	AEGfxMeshFree(lvlMesh);
	AEGfxMeshFree(hlMesh);

	AEGfxTextureUnload(lvlTex);
}

void LvlSelectUnload()
{

	MainMenuBGMUnload();
}