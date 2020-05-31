#pragma once
#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__
#include "plate.h"

// constant
static const char* space_image_path = "../bin/images/space.jpg";
float space_animation_speed = 0.005f;

// opengl variables
GLuint	SpaceTexture = 0;


// render function
void render_space(GLuint program, plate_t& plates) {
	for (rect_t r : plates.rect) {
		render_rect(program, r, SpaceTexture);
	}
}

void update_space(plate_t& space, float del_t) {
	space.time += del_t;
	float s = sin(space.time), c = cos(space.time);
	space.center += vec3(space_animation_speed * s, space_animation_speed * c, space_animation_speed * s);
	space.update_plate();
}

// creating object functions
inline plate_t create_space() //function to save the information about walls - 3 walls
{
	plate_t p;
	p.make_plate(vec3(0.0f, -40.0f, 40.0f) + away_from_map, vec3(200.0f), 0);

	return p;
}

auto		space = std::move(create_space());

#endif 