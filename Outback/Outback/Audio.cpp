/****************************************************************************** /
/*!
\file		Music.cpp
\author 	Lim Sim Chee, Shannon
\par    	email: lim.s@digipen.edu
\date
\brief

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "Main.h"

FMOD::System	* Syst;
FMOD::Sound		* mainMenuBGM, * Level1BGM, * Level2BGM, * Level3BGM,
				* GameOverBGM, * Jump, * highScore, * beeSFX, * emuSFX,
				* spiderSFX, * lootSFX, * boomerangSFX, * keySelect,
				* playerSFX;
FMOD::Channel	* channel1, * channel2, * channel3;
FMOD_RESULT		result;
unsigned int	version;
void			* extradriverdata;

float			volume = 0.5f;
float			timer = 0.0f;

bool			mute = false;
bool			boomerangCollision = false;
bool			playerJump = false;
bool			changeLevel = false;
bool			gameOver = false;
bool			carEngineRev = false;


