/****************************************************************************** /
/*!
\file			main.cpp
\project name	Outback
\author(s)	 	Primary:	Binte Mohd Rizal, Nurul Dania

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "main.h"


// ---------------------------------------------------------------------------
// Globals
float	 g_dt;
double	 g_appTime;

s8 fontID = 0;
s8 fontTitle = 0;
/******************************************************************************/
/*!
	Starting point of the application
*/
/******************************************************************************/
int WINAPI WinMain(HINSTANCE instanceH, HINSTANCE prevInstanceH, LPSTR command_line, int show)
{
	UNREFERENCED_PARAMETER(prevInstanceH);
	UNREFERENCED_PARAMETER(command_line);
	// Initialize the system
	AESysInit (instanceH, show, 800, 600, 1, 60, false, NULL);
	AESysSetWindowTitle("Outback");
	fontTitle = AEGfxCreateFont("../Resources/Fonts/Arial Italic.ttf", 28 * 3);
	fontID = AEGfxCreateFont("../Resources/Fonts/Arial Italic.ttf", 22);
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.5f);

	AEToogleFullScreen(true);

	GameStateMgrInit(GS_LOGO);
	AudioEngineLoad();

	while(gGameStateCurr != GS_QUIT)
	{
		// reset the system modules
		AESysReset();

		// If not restarting, load the gamestate
		if(gGameStateCurr != GS_RESTART)
		{
			GameStateMgrUpdate();
			GameStateLoad();
		}
		else
			gGameStateNext = gGameStateCurr = gGameStatePrev;

		// Initialize the gamestate
		GameStateInit();

		while(gGameStateCurr == gGameStateNext)
		{
			AESysFrameStart();

			AEInputUpdate();

			GameStateUpdate();

			AudioEngineUpdate();
			
			GameStateDraw();
			
			AESysFrameEnd();

			// check if forcing the application to quit
			if ((AESysDoesWindowExist() == false))
				gGameStateNext = GS_QUIT;

			g_dt = (f32)AEFrameRateControllerGetFrameTime();
			g_appTime += g_dt;
		}
		
		GameStateFree();

		if(gGameStateNext != GS_RESTART)
			GameStateUnload();

		gGameStatePrev = gGameStateCurr;
		gGameStateCurr = gGameStateNext;
	}
	AudioEngineUnload();
	AEGfxDestroyFont(fontID);
	AEGfxDestroyFont(fontTitle);
	// free the system
	AESysExit();
}