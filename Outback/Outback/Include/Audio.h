/****************************************************************************** /
/*!
\file		Audio.h
\author 	Lim Sim Chee, Shannon
\par    	email: lim.s@digipen.edu
\date		15th March 2021
\brief		This file contains the declarations of the functions to play the
			music and the sound effects of the game.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#ifndef MUSIC_H_
#define MUSIC_H_

extern bool			setMute;
extern bool			setUnmute;
extern bool			mute;
extern bool			boomerangCollision;
extern bool			playerJump;
extern bool			changeLevel;
extern bool			gameOver;
extern bool			carEngineRev;

void AudioEngineLoad();
void AudioEngineInit();
void AudioEngineUpdate();
void AudioEngineDraw();
void AudioEngineFree();
void AudioEngineUnload();

void MainMenuBGMLoad();
void Level1BGMLoad();
void Level2BGMLoad();
void Level3BGMLoad();
void GameOverBGMLoad();

void MainMenuBGMUnload();
void Level1BGMUnload();
void Level2BGMUnload();
void Level3BGMUnload();
void GameOverBGMUnload();


#endif // !MUSIC_H_

