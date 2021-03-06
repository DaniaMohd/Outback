/****************************************************************************** /
/*!
\file			Options.cpp
\project name	Outback
\author(s)	 	Primary:	Lim Sim Chee, Shannon

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "main.h"

extern s8 fontID;

extern bool setMute;
extern bool setUnmute;
extern bool endless;
extern bool newGame;
extern bool fullscreen;
bool settingsBool;
bool downVol;
bool upVol;
unsigned int chooseSettings;
extern float volume;

AEGfxTexture* setTex = 0;
AEGfxVertexList* setMesh = 0;

void OptionsLoad()
{
	AEGfxSetCamPosition(0, 0);
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	// Create options header
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

	setMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(setMesh, "Failed to create settings mesh!");
	setTex = AEGfxTextureLoad("..\\Resources\\Logo\\Settings0.png");
	AE_ASSERT_MESG(setTex, "Failed to create settings texture!");



	MainMenuBGMLoad();
}

void OptionsInit()
{
	AudioEngineInit();
	chooseSettings = 0;
	endless = false;
	newGame = true;
	fullscreen = true;
	downVol = false;
	upVol = false;
	settingsBool = false;
	volume = 0.5f;

}

void OptionsUpdate()
{
	switch (chooseSettings)
	{
		case 0:
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
			break;
		case 1:
			downVol = true;
			break;
		case 2:
			upVol = true;
			break;
		case 3:
			if (setMute)
			{
				setMute = false;
				setUnmute = true;

				MainMenuBGMUnload();
				MainMenuBGMLoad();
			}
			else
			{
				setMute = true;
				setUnmute = false;
			}
			break;
		default:
			AE_FATAL_ERROR("Cannot choose options!!");
	}


	

	if (AEInputCheckTriggered(AEVK_M))
	{
		ToggleAudioMute();
	}

	if (AEInputCheckTriggered(AEVK_ESCAPE))
	{
		ToggleAudioPause();
	}

	if (downVol)
	{
		if (AEInputCheckTriggered(AEVK_9))
		{
			DecreaseVol();
		}
	}
	
	if (upVol)
	{
		if (AEInputCheckTriggered(AEVK_0))
		{
			IncreaseVol();
		}
	}

	if (AEInputCheckTriggered(AEVK_BACK))
	{
		gGameStateNext = GS_MAINMENU;
	}
}

void OptionsDraw()
{
	char strBuffer[100];

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetPosition(-175.0f, 235.0f);
	AEGfxTextureSet(setTex, 0.0f, 0.0f);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxMeshDraw(setMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);

	sprintf_s(strBuffer, "To decrease volume: 9");
	AEGfxPrint(fontID, strBuffer, -0.35f, 0.15f, 1.0f, 1.0f, 1.0f, 1.0f);

	sprintf_s(strBuffer, "To increase volume: 0");
	AEGfxPrint(fontID, strBuffer, -0.35f, 0.00f, 1.0f, 1.0f, 1.0f, 1.0f);

	sprintf_s(strBuffer, "To mute / unmute: M");
	AEGfxPrint(fontID, strBuffer, -0.35f, -0.15f, 1.0f, 1.0f, 1.0f, 1.0f);

	sprintf_s(strBuffer, "To toggle fullscreen: N");
	AEGfxPrint(fontID, strBuffer, -0.35f, -0.30f, 1.0f, 1.0f, 1.0f, 1.0f);

	AEGfxPrint(fontID, "Press BACKSPACE", -0.9f, -0.75f, 1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(fontID, "for Main Menu", -0.9f, -0.95f, 1.0f, 1.0f, 1.0f, 1.0f);
}

void OptionsFree()
{
	AEGfxMeshFree(setMesh);

	AEGfxTextureUnload(setTex);
}

void OptionsUnload()
{
	MainMenuBGMUnload();
}