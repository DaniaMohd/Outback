/******************************************************************************/
/*!
\file		Logo.cpp
\author 	Lim Sim Chee, Shannon
\par    	email: lim.s@digipen.edu
\date   	5th April 2021
\brief      This file contains the declearations of the functions needed for
			the credits screen

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "main.h"

extern s8					Font;

static float				logoTimer = 0.0f;
//static float				animateTimer = 0.0f;

static AEGfxTexture			* dpTex, * grpTex;
static AEGfxVertexList		* dpMesh, * grpMesh;

void logoLoad()
{
	AEGfxSetCamPosition(0, 0);
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

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

	dpMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(dpMesh, "Failed to create Digipen Logo mesh!");
	dpTex = AEGfxTextureLoad("..\\Resources\\Logo\\DigiPen_RED.png");
	AE_ASSERT_MESG(dpTex, "Failed to crdeate Digipen Logo texture!");
}

void logoInit()
{
	logoTimer = 0.0f;
	//animateTimer = 0.0f;
}

void logoUpdate()
{
	logoTimer += g_dt;

	if ((AEInputCheckCurr(AEVK_RETURN)) || (AEInputCheckCurr(AEVK_SPACE)) && logoTimer > 3.0f)
	{
		gGameStateNext = GS_MAINMENU;
	}

	if (logoTimer > 4.0f)
	{
		gGameStateNext = GS_MAINMENU;
	}
}

void logoDraw()
{
	if (logoTimer <= 3.0f)
	{
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetPosition(-200.0f, 0.0f);
		AEGfxTextureSet(dpTex, 0.0f, 0.0f);
		AEGfxSetTintColor(1.f, 1.0f, 1.0f, 0.5f);
		AEGfxSetTransparency(1.0f);

		AEGfxMeshDraw(dpMesh, AE_GFX_MDM_TRIANGLES);
	}
}

void logoFree()
{

}

void logoUnload()
{
	AEGfxMeshFree(dpMesh);
	AEGfxTextureUnload(dpTex);
}