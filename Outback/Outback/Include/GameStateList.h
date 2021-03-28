/******************************************************************************/
/*!
\file		GameStateList.h
\author 	Javin Ong J-min
\par    	email: ong.j@digipen.edu
\date   	15/03/2021
\brief      This file contains the definition for the Game State List function.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#ifndef CS230_GAME_STATE_LIST_H_
#define CS230_GAME_STATE_LIST_H_

// ---------------------------------------------------------------------------
// game state list

enum
{
	// list of all game states 
	GS_LEVEL1 = 0, 
	GS_LEVEL2,
	GS_LEVEL3,
	GS_MAINMENU,
	GS_WIN,
	// special game state. Do not change
	GS_RESTART,
	GS_QUIT, 
	GS_NUM,
	GS_PAUSE,
	GS_GAMEOVER
};

// ---------------------------------------------------------------------------

#endif // CS1130_GAME_STATE_LIST_H_