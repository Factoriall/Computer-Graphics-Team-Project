#pragma once
#ifndef __PLATE_H__
#define __PLATE_H__
#include "object.h"

// constant
static const char* plate_image_path = "../bin/images/plate.jpg";

// opengl variables
GLuint	PlateTexture = 0;


// global variables
vec3	away_from_map = vec3(-0.5f, 0, -1.0f);	// adjust plate to map

// implement fuctions
void plate_t::make_plate(vec3 _center, vec3 _scale) {
	center = _center;
	scale = _scale;

	rect[0] = { _center - vec3(0, 0, _scale.z / 2.0f), vec2(_scale.x,_scale.y), vec3(0), 0 };
	rect[1] = { _center - vec3(_scale.x / 2.0f, 0, 0), vec2(_scale.z, _scale.y), vec3(0, 1, 0), PI / 2 };
	rect[2] = { _center - vec3(0, 0, _scale.z / 2.0f), vec2(_scale.x, _scale.z), vec3(1, 0, 0), PI / 2 };
	rect[3] = { _center + vec3(_scale.x / 2.0f, 0, 0), vec2(_scale.z, _scale.y), vec3(0, 1, 0),  PI / 2 };
	rect[4] = { _center - vec3(0  , -_scale.y, _scale.z / 2.0f), vec2(_scale.x, _scale.z), vec3(1, 0, 0), PI / 2 };
	rect[5] = { _center + vec3(0, 0, _scale.z / 2.0f), vec2(_scale.x,_scale.y), vec3(0), 0 };
}

// render function
void render_plate(GLuint program, std::vector<plate_t>& plates) {
	for (auto& c : plates) {
		for (rect_t r : c.rect) {
			render_rect(program, r, PlateTexture);
		}
	}
}

// creating object functions
inline std::vector<plate_t> create_plates() {
	std::vector<plate_t> cube_vector;
	plate_t cube;

	// 뒷 벽을 기준으로 x 가 좌우, y가 높이(y_scale = 0.1f 고정), z가 앞뒤(z=2.0f, z_scale=2.0f) 고정하면 대충 맵에 맞음
	cube.make_plate(vec3(0.0f, 3, 2) + away_from_map, vec3(3.0f, 0.5f, 2.0f));
	cube_vector.push_back(cube);
	cube.make_plate(vec3(-2.0f, 6, 2) + away_from_map, vec3(3.0f, 0.5f, 2.0f));
	cube_vector.push_back(cube);
	cube.make_plate(vec3(2.0f, 9, 2) + away_from_map, vec3(3.0f, 0.5f, 2.0f));
	cube_vector.push_back(cube);
	cube.make_plate(vec3(1.0f, 12, 2) + away_from_map, vec3(3.0f, 0.5f, 2.0f));
	cube_vector.push_back(cube);
	cube.make_plate(vec3(-3.0f, 15, 2) + away_from_map, vec3(3.0f, 0.5f, 2.0f));
	cube_vector.push_back(cube);
	cube.make_plate(vec3(1.0f, 18, 2) + away_from_map, vec3(3.0f, 0.5f, 2.0f));
	cube_vector.push_back(cube);
	cube.make_plate(vec3(4.0f, 21, 2) + away_from_map, vec3(3.0f, 0.5f, 2.0f));
	cube_vector.push_back(cube);
	cube.make_plate(vec3(-3.0f, 24, 2) + away_from_map, vec3(3.0f, 0.5f, 2.0f));
	cube_vector.push_back(cube);
	cube.make_plate(vec3(1.0f, 24, 2) + away_from_map, vec3(3.0f, 0.5f, 2.0f));
	cube_vector.push_back(cube);
	cube.make_plate(vec3(0.5f, 27, 2) + away_from_map, vec3(3.0f, 0.5f, 2.0f));
	cube_vector.push_back(cube);

	return cube_vector;
}

auto		plates = std::move(create_plates());

#endif 