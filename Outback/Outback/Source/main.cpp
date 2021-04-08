/******************************************************************************/
/*!
\file		main.cpp
\author 	Javin Ong J-min
\par    	email: ong.j@digipen.edu
\date   	15/03/2021
\brief      This file contains the definition for the main function.

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