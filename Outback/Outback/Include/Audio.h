/****************************************************************************** /
/*!
\file			Audio.h
\project name	Outback
\author(s)	 	Primary:	Lim Sim Chee, Shannon

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#ifndef AUDIO_H_
#define AUDIO_H_

extern bool			setMute;
extern bool			setUnmute;
extern bool			mute;
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

void ToggleAudioMute();
void ToggleAudioPause();

void DecreaseVol();
void IncreaseVol();

void carEngineLoad();
void carEngineUnload();

#endif // !AUDIO_H_

