/****************************************************************************** /
/*!
\file			GameStateMgr.cpp
\project name	Outback
\author(s)	 	Primary:	Binte Mohd Rizal, Nurul Dania

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "main.h"

// ---------------------------------------------------------------------------
// globals

// variables to keep track the current, previous and next game state
unsigned int	gGameStateInit;
unsigned int	gGameStateCurr;
unsigned int	gGameStatePrev;
unsigned int	gGameStateNext;

// pointer to functions for game state life cycles functions
void (*GameStateLoad)()		= 0;
void (*GameStateInit)()		= 0;
void (*GameStateUpdate)()	= 0;
void (*GameStateDraw)()		= 0;
void (*GameStateFree)()		= 0;
void (*GameStateUnload)()	= 0;

/******************************************************************************/
/*!

*/
/******************************************************************************/
void GameStateMgrInit(unsigned int gameStateInit)
{
	// set the initial game state
	gGameStateInit = gameStateInit;

	// reset the current, previoud and next game
	gGameStateCurr = 
	gGameStatePrev = 
	gGameStateNext = gGameStateInit;

	// call the update to set the function pointers
	GameStateMgrUpdate();

}

/******************************************************************************/
/*!

*/
/******************************************************************************/
void GameStateMgrUpdate()
{
	AudioEngineUpdate();

	if ((gGameStateCurr == GS_RESTART) || (gGameStateCurr == GS_QUIT))
		return;

	switch (gGameStateCurr)
	{
	case GS_LOGO:
		GameStateLoad = logoLoad;
		GameStateInit = logoInit;
		GameStateUpdate = logoUpdate;
		GameStateDraw = logoDraw;
		GameStateFree = logoFree;
		GameStateUnload = logoUnload;
		break;
	case GS_MAINMENU:
		GameStateLoad = GameStateMainMenuLoad;
		GameStateInit = GameStateMainMenuInit;
		GameStateUpdate = GameStateMainMenuUpdate;
		GameStateDraw = GameStateMainMenuDraw;
		GameStateFree = GameStateMainMenuFree;
		GameStateUnload = GameStateMainMenuUnload;
		break;
	case GS_TUTORIAL:
		GameStateLoad = GameStatePlatformLoad;
		GameStateInit = GameStatePlatformInit;
		GameStateUpdate = GameStatePlatformUpdate;
		GameStateDraw = GameStatePlatformDraw;
		GameStateFree = GameStatePlatformFree;
		GameStateUnload = GameStatePlatformUnload;
		break;
	case GS_LEVEL1:
		GameStateLoad	= GameStatePlatformLoad;
		GameStateInit	= GameStatePlatformInit;
		GameStateUpdate	= GameStatePlatformUpdate;
		GameStateDraw	= GameStatePlatformDraw;
		GameStateFree	= GameStatePlatformFree;
		GameStateUnload	= GameStatePlatformUnload;
		break;
	case GS_LEVEL2:
		GameStateLoad = GameStatePlatformLoad;
		GameStateInit = GameStatePlatformInit;
		GameStateUpdate = GameStatePlatformUpdate;
		GameStateDraw = GameStatePlatformDraw;
		GameStateFree = GameStatePlatformFree;
		GameStateUnload = GameStatePlatformUnload;
		break;
	case GS_LEVEL3:
		GameStateLoad = GameStatePlatformLoad;
		GameStateInit = GameStatePlatformInit;
		GameStateUpdate = GameStatePlatformUpdate;
		GameStateDraw = GameStatePlatformDraw;
		GameStateFree = GameStatePlatformFree;
		GameStateUnload = GameStatePlatformUnload;
		break;
	case GS_WIN:
		GameStateLoad = GameStateWinLoad;
		GameStateInit = GameStateWinInit;
		GameStateUpdate = GameStateWinUpdate;
		GameStateDraw = GameStateWinDraw;
		GameStateFree = GameStateWinFree;
		GameStateUnload = GameStateWinUnload;
		break;
	case GS_GAMEOVER:
		GameStateLoad = OBGameOverLoad;
		GameStateInit = OBGameOverInit;
		GameStateUpdate = OBGameOverUpdate;
		GameStateDraw = OBGameOverDraw;
		GameStateFree = OBGameOverFree;
		GameStateUnload = OBGameOverUnload;
		break;
	case GS_CREDITS:
		GameStateLoad = CreditsLoad;
		GameStateInit = CreditsInit;
		GameStateUpdate = CreditsUpdate;
		GameStateDraw = CreditsDraw;
		GameStateFree = CreditsFree;
		GameStateUnload = CreditsUnload;
		break;
	case GS_LVLSELECT:
		GameStateLoad = LvlSelectLoad;
		GameStateInit = LvlSelectInit;
		GameStateUpdate = LvlSelectUpdate;
		GameStateDraw = LvlSelectDraw;
		GameStateFree = LvlSelectFree;
		GameStateUnload = LvlSelectUnload;
		break;
	case GS_OPTIONS:
		GameStateLoad = OptionsLoad;
		GameStateInit = OptionsInit;
		GameStateUpdate = OptionsUpdate;
		GameStateDraw = OptionsDraw;
		GameStateFree = OptionsFree;
		GameStateUnload = OptionsUnload;
		break;
	case GS_INSTRUCTIONS:
		GameStateLoad = InstructLoad;
		GameStateInit = InstructInit;
		GameStateUpdate = InstructUpdate;
		GameStateDraw = InstructDraw;
		GameStateFree = InstructFree;
		GameStateUnload = InstructUnload;
		break;
	case GS_INSTRUCTIONS2:
		GameStateLoad = InstructLoad;
		GameStateInit = InstructInit;
		GameStateUpdate = InstructUpdate;
		GameStateDraw = InstructDraw;
		GameStateFree = InstructFree;
		GameStateUnload = InstructUnload;
		break;
	default:
		AE_FATAL_ERROR("invalid state!!");
	}



}
