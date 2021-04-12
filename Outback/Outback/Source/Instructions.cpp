/****************************************************************************** /
/*!
\file			Instructions.cpp
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

AEGfxTexture* instructTex, * powerUpTex;
AEGfxVertexList* instructMesh, * powerUpMesh;

void InstructLoad()
{
	AEGfxSetCamPosition(0, 0);
	AEGfxSetBackgroundColor(0.75f, 0.75f, 0.75f);

	// Create credits header
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

	instructMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(instructMesh, "Failed to create instructions mesh!");
	instructTex = AEGfxTextureLoad("..\\Resources\\Logo\\Instuctions0.png");
	AE_ASSERT_MESG(instructTex, "Failed to create instructions texture!");


	// Create credits header
	AEGfxMeshStart();
	AEGfxTriAdd(
		-300.0f, -300.0f, 0xFFFFFFFF, 0.0f, 1.0f,
		450.0f, -300.0f, 0xFFFFFFFF, 1.0f, 1.0f,
		-300.0f, 300.0f, 0xFFFFFFFF, 0.0f, 0.0f
	);

	AEGfxTriAdd(
		450.0f, -300.0f, 0xFFFFFFFF, 1.0f, 1.0f,
		450.0f, 300.0f, 0xFFFFFFFF, 1.0f, 0.0f,
		-300.0f, 300.0f, 0xFFFFFFFF, 0.0f, 0.0f
	);

	powerUpMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(powerUpMesh, "Failed to create power up mesh!");
	powerUpTex = AEGfxTextureLoad("..\\Resources\\Logo\\PowerUps0.png");
	AE_ASSERT_MESG(powerUpTex, "Failed to create power up texture!");
}

void InstructInit()
{

}

void InstructUpdate()
{
	if (AEInputCheckTriggered(AEVK_BACK))
	{
		gGameStateNext = GS_MAINMENU;
	}

	if (AEInputCheckReleased(AEVK_RIGHT) || AEInputCheckReleased(AEVK_LEFT))
	{
		if (gGameStateCurr == GS_INSTRUCTIONS)
		{
			gGameStateNext = GS_INSTRUCTIONS2;
		}
		if (gGameStateCurr == GS_INSTRUCTIONS2)
		{
			gGameStateNext = GS_INSTRUCTIONS;
		}
	}

	if (AEInputCheckReleased(AEVK_RETURN))
	{
		gGameStateNext = GS_TUTORIAL;
	}
}

void InstructDraw()
{
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetPosition(-195.0f, 255.0f);
	AEGfxTextureSet(instructTex, 0.0f, 0.0f);
	AEGfxSetTintColor(1.f, 1.0f, 1.0f, 1.0f);
	AEGfxSetTransparency(1.0f);
	AEGfxMeshDraw(instructMesh, AE_GFX_MDM_TRIANGLES);

	if (gGameStateCurr == GS_INSTRUCTIONS)
	{
		AEGfxPrint(fontID, "A / < : to move left", -0.35f, 0.45f, 1.0f, 0.0f,0.0f, 0.0f);
		AEGfxPrint(fontID, "D / > : to move right", -0.35f, 0.30f, 1.0f, 0.0f, 0.0f, 0.0f);
		AEGfxPrint(fontID, "Spacebar / ^ : to jump", -0.35f, 0.15f, 1.0f, 0.0f, 0.0f, 0.0f);
		AEGfxPrint(fontID, "W : to climb the ladders", -0.35f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
		AEGfxPrint(fontID, "J : to shoot the boomerang", -0.35f, -0.15f, 1.0f, 0.0f, 0.0f, 0.0f);

		AEGfxPrint(fontID, "Press < / > for", 0.55f, -0.75f, 1.0f, 0.0f, 0.0f, 0.0f);
		AEGfxPrint(fontID, "next page", 0.55f, -0.95f, 1.0f, 0.0f, 0.0f, 0.0f);
	}

	else if (gGameStateCurr == GS_INSTRUCTIONS2)
	{
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetPosition(-115.0f, 0.0f);
		AEGfxTextureSet(powerUpTex, 0.0f, 0.0f);
		AEGfxSetTintColor(1.f, 1.0f, 1.0f, 1.0f);
		AEGfxSetTransparency(1.0f);
		AEGfxMeshDraw(powerUpMesh, AE_GFX_MDM_TRIANGLES);

		AEGfxPrint(fontID, "Press < / > for", 0.55f, -0.75f, 1.0f, 0.0f, 0.0f, 0.0f);
		AEGfxPrint(fontID, "previous page", 0.55f, -0.95f, 1.0f, 0.0f, 0.0f, 0.0f);
	}

	AEGfxPrint(fontID, "Press ENTER to start tutorial", -0.35f, -0.45f, 1.0f, 0.0f, 0.0f, 0.0f);

	AEGfxPrint(fontID, "Press BACKSPACE", -0.9f, -0.75f, 1.0f, 0.0f, 0.0f, 0.0f);
	AEGfxPrint(fontID, "for Main Menu", -0.9f, -0.95f, 1.0f, 0.0f, 0.0f, 0.0f);
}

void InstructFree()
{
	AEGfxMeshFree(instructMesh);

	AEGfxTextureUnload(instructTex);
}

void InstructUnload()
{

}