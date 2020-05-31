#pragma once
#ifndef __SOUND_H__
#define __SOUND_H__

#include "irrKlang/irrKlang.h"	// irrKlang for sound
#pragma comment(lib, "irrKlang.lib")

// constant
static const char* colision_sound_path = "../bin/sound/colision.wav";
static const char* colision_with_floor_sound_path = "../bin/sound/grass.wav";
static const char* colision_with_wall_sound_path = "../bin/sound/bricks.wav";
static const char* colision_with_plate_sound_path = "../bin/sound/iron_plate.wav";

//*************************************
// irrKlang objects
irrklang::ISoundEngine* engine = nullptr;
irrklang::ISoundSource* sound_src = nullptr;
irrklang::ISoundSource* sound_floor_src = nullptr;
irrklang::ISoundSource* sound_wall_src = nullptr;
irrklang::ISoundSource* sound_plate_src = nullptr;

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
	sound_plate_src = engine->addSoundSourceFromFile(colision_with_plate_sound_path);

	return true;
}

#endif // !__SOUND_H__
