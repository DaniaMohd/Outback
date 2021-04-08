/******************************************************************************/
/*!
\file		Credits.cpp
\author 	Lim Sim Chee, Shannon
\par    	email: lim.s@digipen.edu
\date   	3rd April 2021
\brief      This file contains the declearations of the functions needed for
			the credits screen

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "main.h"

extern s8 fontID;

static AEGfxTexture		* credsTex, * headTex, * tmTex, * profTex, * endTex, * presTex, * execTex, * backTex;
static AEGfxVertexList	* credsMesh, * headMesh, * tmMesh, * profMesh, * endMesh, * presMesh, * execMesh, * backMesh;

char strBuffer[100];

void CreditsLoad()
{
	// Create credits header
	AEGfxSetCamPosition(0, 0);
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	AEGfxMeshStart();
	AEGfxTriAdd(
		-50.0f, -50.0f, 0xFFFFFFFF, 0.0f, 1.0f,
		450.0f, -50.0f, 0xFFFFFFFF, 1.0f, 1.0f,
		-50.0f, 50.0f, 0xFFFFFFFF, 0.0f, 0.0f
	);

	AEGfxTriAdd(
		450.0f, -50.0f, 0xFFFFFFFF, 1.0f, 1.0f,
		450.0f, 50.0f, 0xFFFFFFFF, 1.0f, 0.0f,
		-50.0f, 50.0f, 0xFFFFFFFF, 0.0f, 0.0f
	);

	credsMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(credsMesh, "Failed to create Credits mesh!");
	credsTex = AEGfxTextureLoad("..\\Resources\\Logo\\Credits0.png");
	AE_ASSERT_MESG(credsTex, "Failed to create Credits texture!");

	// Create start of the credits
	AEGfxMeshStart();
	AEGfxTriAdd(
		-95.0f, -95.0f, 0xFFFFFFFF, 0.0f, 1.0f,
		450.0f, -95.0f, 0xFFFFFFFF, 1.0f, 1.0f,
		-95.0f, 95.0f, 0xFFFFFFFF, 0.0f, 0.0f
	);

	AEGfxTriAdd(
		450.0f, -95.0f, 0xFFFFFFFF, 1.0f, 1.0f,
		450.0f, 95.0f, 0xFFFFFFFF, 1.0f, 0.0f,
		-95.0f, 95.0f, 0xFFFFFFFF, 0.0f, 0.0f
	);

	headMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(headMesh, "Failed to create start mesh!");
	headTex = AEGfxTextureLoad("..\\Resources\\Logo\\start0.png");
	AE_ASSERT_MESG(headTex, "Failed to create start texture!");

	// Create name of the team members
	AEGfxMeshStart();
	AEGfxTriAdd(
		-80.0f, -80.0f, 0xFFFFFFFF, 0.0f, 1.0f,
		250.0f, -80.0f, 0xFFFFFFFF, 1.0f, 1.0f,
		-80.0f, 80.0f, 0xFFFFFFFF, 0.0f, 0.0f
	);

	AEGfxTriAdd(
		250.0f, -80.0f, 0xFFFFFFFF, 1.0f, 1.0f,
		250.0f, 80.0f, 0xFFFFFFFF, 1.0f, 0.0f,
		-80.0f, 80.0f, 0xFFFFFFFF, 0.0f, 0.0f
	);

	tmMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(tmMesh, "Failed to create team name mesh!");
	tmTex = AEGfxTextureLoad("..\\Resources\\Logo\\tm0.png");
	AE_ASSERT_MESG(tmTex, "Failed to create team name texture!");

	// Create professors names
	AEGfxMeshStart();
	AEGfxTriAdd(
		-100.0f, -100.0f, 0xFFFFFFFF, 0.0f, 1.0f,
		250.0f, -100.0f, 0xFFFFFFFF, 1.0f, 1.0f,
		-100.0f, 100.0f, 0xFFFFFFFF, 0.0f, 0.0f
	);

	AEGfxTriAdd(
		250.0f, -100.0f, 0xFFFFFFFF, 1.0f, 1.0f,
		250.0f, 100.0f, 0xFFFFFFFF, 1.0f, 0.0f,
		-100.0f, 100.0f, 0xFFFFFFFF, 0.0f, 0.0f
	);

	profMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(profMesh, "Failed to create prof name mesh!");
	profTex = AEGfxTextureLoad("..\\Resources\\Logo\\prof0.png");
	AE_ASSERT_MESG(profTex, "Failed to create prof name texture!");

	// Create President name
	AEGfxMeshStart();
	AEGfxTriAdd(
		-80.0f, -80.0f, 0xFFFFFFFF, 0.0f, 1.0f,
		250.0f, -80.0f, 0xFFFFFFFF, 1.0f, 1.0f,
		-80.0f, 80.0f, 0xFFFFFFFF, 0.0f, 0.0f
	);

	AEGfxTriAdd(
		250.0f, -80.0f, 0xFFFFFFFF, 1.0f, 1.0f,
		250.0f, 80.0f, 0xFFFFFFFF, 1.0f, 0.0f,
		-80.0f, 80.0f, 0xFFFFFFFF, 0.0f, 0.0f
	);

	presMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(presMesh, "Failed to create president name mesh!");
	presTex = AEGfxTextureLoad("..\\Resources\\Logo\\pres0.png");
	AE_ASSERT_MESG(presTex, "Failed to create president name texture!");

	// Create executive names
	AEGfxMeshStart();
	AEGfxTriAdd(
		-110.0f, -110.0f, 0xFFFFFFFF, 0.0f, 1.0f,
		250.0f, -110.0f, 0xFFFFFFFF, 1.0f, 1.0f,
		-110.0f, 110.0f, 0xFFFFFFFF, 0.0f, 0.0f
	);

	AEGfxTriAdd(
		250.0f, -110.0f, 0xFFFFFFFF, 1.0f, 1.0f,
		250.0f, 110.0f, 0xFFFFFFFF, 1.0f, 0.0f,
		-110.0f, 110.0f, 0xFFFFFFFF, 0.0f, 0.0f
	);

	execMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(execMesh, "Failed to create excutive name mesh!");
	execTex = AEGfxTextureLoad("..\\Resources\\Logo\\exec0.png");
	AE_ASSERT_MESG(execTex, "Failed to create executive name texture!");

	// Create end of credits
	AEGfxMeshStart();
	AEGfxTriAdd(
		-95.0f, -95.0f, 0xFFFFFFFF, 0.0f, 1.0f,
		450.0f, -95.0f, 0xFFFFFFFF, 1.0f, 1.0f,
		-95.0f, 95.0f, 0xFFFFFFFF, 0.0f, 0.0f
	);

	AEGfxTriAdd(
		450.0f, -95.0f, 0xFFFFFFFF, 1.0f, 1.0f,
		450.0f, 95.0f, 0xFFFFFFFF, 1.0f, 0.0f,
		-95.0f, 95.0f, 0xFFFFFFFF, 0.0f, 0.0f
	);

	endMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(endMesh, "Failed to create end mesh!");
	endTex = AEGfxTextureLoad("..\\Resources\\Logo\\end0.png");
	AE_ASSERT_MESG(endTex, "Failed to create end texture!");

	// Creating return to main menu
	AEGfxMeshStart();
	AEGfxTriAdd(
		-95.0f, -95.0f, 0xFFFFFFFF, 0.0f, 1.0f,
		450.0f, -95.0f, 0xFFFFFFFF, 1.0f, 1.0f,
		-95.0f, 95.0f, 0xFFFFFFFF, 0.0f, 0.0f
	);

	AEGfxTriAdd(
		450.0f, -95.0f, 0xFFFFFFFF, 1.0f, 1.0f,
		450.0f, 95.0f, 0xFFFFFFFF, 1.0f, 0.0f,
		-95.0f, 95.0f, 0xFFFFFFFF, 0.0f, 0.0f
	);

	backMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(backMesh, "Failed to create return mesh!");
	backTex = AEGfxTextureLoad("..\\Resources\\Logo\\back0.png");
	AE_ASSERT_MESG(backTex, "Failed to create return texture!");
}

void CreditsInit()
{

}

void CreditsUpdate()
{
	if (AEInputCheckTriggered(AEVK_BACK))
	{
		gGameStateNext = GS_MAINMENU;
	}
}

void CreditsDraw()
{
	// Drawing credits header
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetPosition(-185.0f, 255.0f);
	AEGfxTextureSet(credsTex, 0.0f, 0.0f);
	AEGfxSetTintColor(1.f, 1.0f, 1.0f, 1.0f);
	AEGfxSetTransparency(1.0f);
	AEGfxMeshDraw(credsMesh, AE_GFX_MDM_TRIANGLES);

	// Drawing start of credits
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetPosition(-185.0f, 135.0f);
	AEGfxTextureSet(headTex, 0.0f, 0.0f);
	AEGfxSetTintColor(1.f, 1.0f, 1.0f, 1.0f);
	AEGfxSetTransparency(1.0f);
	AEGfxMeshDraw(headMesh, AE_GFX_MDM_TRIANGLES);

	// Drawing team members
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetPosition(-285.0f, 35.0f);
	AEGfxTextureSet(tmTex, 0.0f, 0.0f);
	AEGfxSetTintColor(1.f, 1.0f, 1.0f, 1.0f);
	AEGfxSetTransparency(1.0f);
	AEGfxMeshDraw(tmMesh, AE_GFX_MDM_TRIANGLES);

	// Drawing Professors 
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetPosition(-285.0f, -125.0f);
	AEGfxTextureSet(profTex, 0.0f, 0.0f);
	AEGfxSetTintColor(1.f, 1.0f, 1.0f, 1.0f);
	AEGfxSetTransparency(1.0f);
	AEGfxMeshDraw(profMesh, AE_GFX_MDM_TRIANGLES);

	// Drawing President name
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetPosition(105.0f, 35.0f);
	AEGfxTextureSet(presTex, 0.0f, 0.0f);
	AEGfxSetTintColor(1.f, 1.0f, 1.0f, 1.0f);
	AEGfxSetTransparency(1.0f);
	AEGfxMeshDraw(presMesh, AE_GFX_MDM_TRIANGLES);

	// Drawing Executive names
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetPosition(105.0f, -60.0f);
	AEGfxTextureSet(execTex, 0.0f, 0.0f);
	AEGfxSetTintColor(1.f, 1.0f, 1.0f, 1.0f);
	AEGfxSetTransparency(1.0f);
	AEGfxMeshDraw(execMesh, AE_GFX_MDM_TRIANGLES);

	// Drawing the end of credits
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetPosition(-185.0f, -230.0f);
	AEGfxTextureSet(endTex, 0.0f, 0.0f);
	AEGfxSetTintColor(1.f, 1.0f, 1.0f, 1.0f);
	AEGfxSetTransparency(1.0f);
	AEGfxMeshDraw(endMesh, AE_GFX_MDM_TRIANGLES);

	// Lead players back to main menu
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetPosition(-185.0f, -310.0f);
	AEGfxTextureSet(backTex, 0.0f, 0.0f);
	AEGfxSetTintColor(1.f, 1.0f, 1.0f, 1.0f);
	AEGfxSetTransparency(1.0f);
	AEGfxMeshDraw(backMesh, AE_GFX_MDM_TRIANGLES);

}

void CreditsFree()
{

}

void CreditsUnload()
{
	AEGfxMeshFree(credsMesh);
	AEGfxMeshFree(headMesh);
	AEGfxMeshFree(tmMesh);
	AEGfxMeshFree(profMesh);
	AEGfxMeshFree(presMesh);
	AEGfxMeshFree(execMesh);
	AEGfxMeshFree(endMesh);
	AEGfxMeshFree(backMesh);

	AEGfxTextureUnload(credsTex);
	AEGfxTextureUnload(headTex);
	AEGfxTextureUnload(tmTex);
	AEGfxTextureUnload(profTex);
	AEGfxTextureUnload(presTex);
	AEGfxTextureUnload(execTex);
	AEGfxTextureUnload(endTex);
	AEGfxTextureUnload(backTex);
}