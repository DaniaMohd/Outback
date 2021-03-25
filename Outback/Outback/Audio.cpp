/****************************************************************************** /
/*!
\file		Audio.cpp
\author 	Lim Sim Chee, Shannon
\par    	email: lim.s@digipen.edu
\date		15th March 2021
\brief		This file contains the definitions of the functions to play the
			music and the sound effects of the game.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "main.h"

FMOD::System	* Syst;
FMOD::Sound		* mainMenuBGM, * Level1BGM/*, * Level2BGM, * Level3BGM,
				* GameOverBGM, * Jump, * highScore, * beeSFX, * emuSFX,
				* spiderSFX, * lootSFX, * boomerangSFX, * keySelect,
				* playerSFX*/;
FMOD::Channel	* channel1/*, * channel2, * channel3*/;
FMOD_RESULT		result;
unsigned int	version;
void			* extradriverdata;

float			volume = 0.5f;
float			timer = 0.0f;

bool			setMute = false;
bool			setUnmute = false;
bool			mute = false;
//bool			boomerangCollision = false;
//bool			playerJump = false;
//bool			changeLevel = false;
//bool			gameOver = false;
//bool			carEngineRev = false;

void AudioEngineLoad()
{
	result = FMOD::System_Create(&Syst);
	if (result != FMOD_RESULT::FMOD_OK) return;

	result = Syst->init(32, FMOD_INIT_NORMAL, extradriverdata);
	if (result != FMOD_RESULT::FMOD_OK) return;
}

void AudioEngineInit()
{
	mute = false;

	setUnmute = false;
}

void AudioEngineUpdate()
{
	if (setMute)
	{
		channel1->setMute(true);
	}

	if (setUnmute)
	{
		channel1->setMute(false);
	}
}

void AudioEngineDraw()
{

}

void AudioEngineFree()
{

}

void AudioEngineUnload()
{
	result = Syst->close();
	if (result != FMOD_RESULT::FMOD_OK) return;
}

void MainMenuBGMLoad()
{
	result = Syst->createSound("..\\Resources\\Music\\Outback_MainMenu.wav", FMOD_LOOP_NORMAL, 0, &mainMenuBGM);
	if (result != FMOD_RESULT::FMOD_OK) return;

	if (!setMute)
	{
		channel1->setVolume(volume);
		result = Syst->playSound(mainMenuBGM, 0, false, &channel1);
		if (result != FMOD_RESULT::FMOD_OK) return;
	}
}

void Level1BGMLoad()
{
	result = Syst->createSound("..\\Resources\\Music\\Outback_Grassland.wav", FMOD_LOOP_NORMAL, 0, &Level1BGM);
	if (result != FMOD_RESULT::FMOD_OK) return;

	if (!setMute)
	{
		channel1->setVolume(volume);
		result = Syst->playSound(Level1BGM, 0, false, &channel1);
		if (result != FMOD_RESULT::FMOD_OK) return;
	}
}

//void Level2BGMLoad()
//{
//
//}
//
//void Level3BGMLoad()
//{
//
//}
//
//void GameOverBGMLoad()
//{
//
//}

void MainMenuBGMUnload()
{
	result = mainMenuBGM->release();
	if (result != FMOD_RESULT::FMOD_OK) return;
}

void Level1BGMUnload()
{
	result = Level1BGM->release();
	if (result != FMOD_RESULT::FMOD_OK) return;
}

//void Level2BGMUnload()
//{
//
//}
//
//void Level3BGMUnload()
//{
//
//}
//
//void GameOverBGMUnload()
//{
//
//}

