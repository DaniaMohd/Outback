/******************************************************************************/
/*!
\file		Logo.cpp
\author 	Lim Sim Chee, Shannon 
\par    	email: lim.s@digipen.edu
\date   	5th April 2021
\brief      This file contains the definitions for the splash screen,
			showing the Digipen logo, FMOD logo and the group logo

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "main.h"

extern s8					Font;

static float				logoTimer = 0.0f;

static AEGfxTexture			* dpTex, * grpTex, * fmodTex;
static AEGfxVertexList		* dpMesh, * grpMesh, * fmodMesh;

void logoLoad()
{
	AEGfxSetCamPosition(0, 0);
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	// Creating mesh for Digipen logo

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
	dpTex = AEGfxTextureLoad("..\\Resources\\Logo\\DigiPen_Singapore_WEB_RED.png");
	AE_ASSERT_MESG(dpTex, "Failed to crdeate Digipen Logo texture!");

	// Creating mesh for FMOD logo

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

	fmodMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(fmodMesh, "Failed to create FMOD Logo mesh!");
	fmodTex = AEGfxTextureLoad("..\\Resources\\Logo\\FMOD Logo White - Black Background.png");
	AE_ASSERT_MESG(fmodTex, "Failed to crdeate FMOD Logo texture!");

	//Creating mesh for group logo

	AEGfxMeshStart();
	AEGfxTriAdd(
		-150.0f, -150.0f, 0xFFFFFFFF, 0.0f, 1.0f,
		150.0f, -150.0f, 0xFFFFFFFF, 1.0f, 1.0f,
		-150.0f, 150.0f, 0xFFFFFFFF, 0.0f, 0.0f
	);

	AEGfxTriAdd(
		150.0f, -150.0f, 0xFFFFFFFF, 1.0f, 1.0f,
		150.0f, 150.0f, 0xFFFFFFFF, 1.0f, 0.0f,
		-150.0f, 150.0f, 0xFFFFFFFF, 0.0f, 0.0f
	);

	grpMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(grpMesh, "Failed to create group Logo mesh!");
	grpTex = AEGfxTextureLoad("..\\Resources\\Logo\\BrainDeadPigeonLogo.jpg");
	AE_ASSERT_MESG(grpTex, "Failed to crdeate group Logo texture!");
}

void logoInit()
{
	logoTimer = 0.0f;
}

void logoUpdate()
{
	logoTimer += g_dt;

	if ((AEInputCheckReleased(AEVK_RETURN) 
		|| AEInputCheckCurr(AEVK_SPACE) 
		|| AEInputCheckCurr(AEVK_LBUTTON) 
		|| AEInputCheckCurr(AEVK_RBUTTON) 
		|| AEInputCheckCurr(AEVK_ESCAPE)) 
		&& logoTimer > 3.0f)
	{
		gGameStateNext = GS_MAINMENU;
	}

	if (logoTimer > 7.0f)
	{
		gGameStateNext = GS_MAINMENU;
	}
}

void logoDraw()
{
	// Duration to draw Digipen logo

	if (logoTimer <= 3.0f)
	{
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetPosition(-200.0f, 0.0f);
		AEGfxTextureSet(dpTex, 0.0f, 0.0f);
		AEGfxSetTintColor(1.f, 1.0f, 1.0f, 1.0f);
		AEGfxSetTransparency(1.0f);

		AEGfxMeshDraw(dpMesh, AE_GFX_MDM_TRIANGLES);
	}

	// Duration to draw FMOD logo

	if (logoTimer > 3.0f && logoTimer <= 5.0f)
	{
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetPosition(-185.0f, 0.0f);
		AEGfxTextureSet(fmodTex, 0.0f, 0.0f);
		AEGfxSetTintColor(1.f, 1.0f, 1.0f, 0.5f);
		AEGfxSetTransparency(1.0f);

		AEGfxMeshDraw(fmodMesh, AE_GFX_MDM_TRIANGLES);
	}

	//Duration to draw group logo

	if (logoTimer > 5.0f && logoTimer <= 7.0f)
	{
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetPosition(0.0f, 0.0f);
		AEGfxTextureSet(grpTex, 0.0f, 0.0f);
		AEGfxSetTintColor(1.f, 1.0f, 1.0f, 1.0f);
		AEGfxSetTransparency(1.0f);

		AEGfxMeshDraw(grpMesh, AE_GFX_MDM_TRIANGLES);
	}
}

void logoFree()
{

}

void logoUnload()
{
	// free the mesh 
	AEGfxMeshFree(dpMesh);
	AEGfxMeshFree(fmodMesh);
	AEGfxMeshFree(grpMesh);

	// unload the texture
	AEGfxTextureUnload(dpTex);
	AEGfxTextureUnload(fmodTex);
	AEGfxTextureUnload(grpTex);
}