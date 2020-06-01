#pragma once
#ifndef __SOUND_H__
#define __SOUND_H__

#include "irrKlang/irrKlang.h"	// irrKlang for sound
#pragma comment(lib, "irrKlang.lib")

// constant
static const char* colision_sound_path = "../bin/sound/colision.wav";
static const char* colision_with_floor_sound_path = "../bin/sound/grass.wav";
static const char* colision_with_wall_sound_path = "../bin/sound/bricks.wav";
static const char* colision_with_plate_1_sound_path = "../bin/sound/iron_plate.wav";	// 철판
static const char* colision_with_plate_2_sound_path = "../bin/sound/jump_1.wav";		
static const char* colision_with_plate_3_sound_path = "../bin/sound/jump_2.wav";		// 스티키
static const char* colision_with_plate_4_sound_path = "../bin/sound/jump_3_ice.wav";		// 얼음
static const char* intro_sound_path = "../bin/sound/intro.wav";
static const char* outro_sound_path = "../bin/sound/outro.wav";
static const char* playing_sound_path = "../bin/sound/playing.wav";
static const char* storm_sound_path = "../bin/sound/storm.wav";
static const char* bomb_sound_path = "../bin/sound/bomb.wav";

//*************************************
// irrKlang objects
irrklang::ISoundEngine* engine = nullptr;
irrklang::ISoundSource* sound_src = nullptr;
irrklang::ISoundSource* sound_floor_src = nullptr;
irrklang::ISoundSource* sound_wall_src = nullptr;
irrklang::ISoundSource* sound_plate_1_src = nullptr;
irrklang::ISoundSource* sound_plate_2_src = nullptr;
irrklang::ISoundSource* sound_plate_3_src = nullptr;
irrklang::ISoundSource* sound_plate_4_src = nullptr;
irrklang::ISoundSource* sound_intro_src = nullptr;
irrklang::ISoundSource* sound_outro_src = nullptr;
irrklang::ISoundSource* sound_playing_src = nullptr;
irrklang::ISoundSource* sound_storm_src = nullptr;
irrklang::ISoundSource* sound_bomb_src = nullptr;


bool	sound_init() {
	// create engine
	engine = irrklang::createIrrKlangDevice();
	if (!engine) {
		return false;
	}

	//add sound source from the sound file
	// sound_src =		  engine->addSoundSourceFromFile(colision_sound_path);
	sound_floor_src = engine->addSoundSourceFromFile(colision_with_floor_sound_path);
	sound_wall_src =  engine->addSoundSourceFromFile(colision_with_wall_sound_path);
	sound_plate_1_src = engine->addSoundSourceFromFile(colision_with_plate_1_sound_path);
	sound_plate_2_src = engine->addSoundSourceFromFile(colision_with_plate_2_sound_path);
	sound_plate_3_src = engine->addSoundSourceFromFile(colision_with_plate_3_sound_path);
	sound_plate_4_src = engine->addSoundSourceFromFile(colision_with_plate_4_sound_path);

	sound_intro_src = engine->addSoundSourceFromFile(intro_sound_path);
	sound_outro_src = engine->addSoundSourceFromFile(outro_sound_path);
	sound_playing_src = engine->addSoundSourceFromFile(playing_sound_path);
	sound_storm_src = engine->addSoundSourceFromFile(storm_sound_path);
	sound_bomb_src = engine->addSoundSourceFromFile(bomb_sound_path);

	return true;
}

#endif // !__SOUND_H__
