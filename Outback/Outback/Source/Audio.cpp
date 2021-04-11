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
FMOD::Sound		* mainMenuBGM, * Level1BGM, * Level2BGM, * Level3BGM,
				* GameOverBGM, * Jump, * highScore, * beeSFX, * emuSFX,
				* spiderSFX, * lootSFX, * boomerangSFX, * keySelect,
				* playerSFX;
FMOD::Channel	* channel1/*, * channel2, * channel3*/;
FMOD_RESULT		result;
unsigned int	version;
void			* extradriverdata;

float			volume = 0.5f;
float			timer = 0.0f;

bool			setMute = false;
bool			setUnmute = false;
bool			mute = false;

extern bool downVol;
extern bool upVol;
extern bool gameIsPaused;
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

	if (downVol)
	{
		if (AEInputCheckTriggered(AEVK_9))
		{
			DecreaseVol();
		}
	}

	if (upVol)
	{
		if (AEInputCheckTriggered(AEVK_0))
		{
			IncreaseVol();
		}
	}

	result = Syst->update();
	if (result != FMOD_RESULT::FMOD_OK) return;
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

void Level2BGMLoad()
{
	result = Syst->createSound("..\\Resources\\Music\\Outback_Desert.wav", FMOD_LOOP_NORMAL, 0, &Level2BGM);
	if (result != FMOD_RESULT::FMOD_OK) return;

	if (!setMute)
	{
		channel1->setVolume(volume);
		result = Syst->playSound(Level2BGM, 0, false, &channel1);
		if (result != FMOD_RESULT::FMOD_OK) return;
	}
}

void Level3BGMLoad()
{
	result = Syst->createSound("..\\Resources\\Music\\Outback_Wetlands.wav", FMOD_LOOP_NORMAL, 0, &Level3BGM);
	if (result != FMOD_RESULT::FMOD_OK) return;

	if (!setMute)
	{
		channel1->setVolume(volume);
		result = Syst->playSound(Level3BGM, 0, false, &channel1);
		if (result != FMOD_RESULT::FMOD_OK) return;
	}

}


void GameOverBGMLoad()
{
	result = Syst->createSound("..\\Resources\\Music\\Outback_GameOver.wav", FMOD_LOOP_NORMAL, 0, &GameOverBGM);
	if (result != FMOD_RESULT::FMOD_OK) return;

	if (!setMute)
	{
		channel1->setVolume(volume);
		result = Syst->playSound(GameOverBGM, 0, false, &channel1);
		if (result != FMOD_RESULT::FMOD_OK) return;
	}
}

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

void Level2BGMUnload()
{
	result = Level2BGM->release();
	if (result != FMOD_RESULT::FMOD_OK) return;
}

void Level3BGMUnload()
{
	result = Level3BGM->release();
	if (result != FMOD_RESULT::FMOD_OK) return;
}

void GameOverBGMUnload()
{
	result = GameOverBGM->release();
	if (result != FMOD_RESULT::FMOD_OK) return;
}

void ToggleAudioMute()
{
	if (AEInputCheckTriggered(AEVK_M))
	{
		if (mute == false) {
			channel1->setMute(false);
			mute = true;
		}
		else if (mute == true) {
			channel1->setMute(true);
			mute = false;
		}
	}
}

void ToggleAudioPause()
{
	if (gameIsPaused == false) {
		channel1->setPaused(false);
		mute = true;
	}
	else if (gameIsPaused == true) {
		channel1->setPaused(true);
		mute = false;
	}
}

void DecreaseVol()
{
	//Press 9 to decrease background music volume
	if (downVol)												
	{
		channel1->getVolume(&volume);

		volume -= 0.01f;

		if (volume < 0)
		{
			volume = 0;
		}

		channel1->setVolume(volume);
	}
}

void IncreaseVol()
{
	//Press 0 to increase background music volume
	if (upVol)
	{
		channel1->getVolume(&volume);
		volume += 0.01f;

		if (volume > 1)
		{
			volume = 1;
		}

		channel1->setVolume(volume);
	}
}