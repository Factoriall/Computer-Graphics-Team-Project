#pragma once
#ifndef __PLATE_H__
#define __PLATE_H__
#include "object.h"

// constant
static const char* plate1_image_path = "../bin/images/plate.jpg";
static const char* plate2_image_path = "../bin/images/plate2.jpg";
static const char* plate3_image_path = "../bin/images/plate3.jpg";
static const char* plate4_image_path = "../bin/images/plate4.jpg";

// opengl variables
GLuint	Plate1Texture = 0;
GLuint	Plate2Texture = 0;
GLuint	Plate3Texture = 0;
GLuint	Plate4Texture = 0;

// global variables
vec3	away_from_map = vec3(-0.5f, 0, -1.0f);	// adjust plate to map

// implement fuctions
void plate_t::make_plate(vec3 _center, vec3 _scale, int _type) {
	center = _center;
	scale = _scale;
	type = _type;

	rect[0] = { _center - vec3(0, 0, _scale.z / 2.0f), vec2(_scale.x,_scale.y), vec3(0), 0 };
	rect[1] = { _center - vec3(_scale.x / 2.0f, 0, 0), vec2(_scale.z, _scale.y), vec3(0, 1, 0), PI / 2 };
	rect[2] = { _center - vec3(0, 0, _scale.z / 2.0f), vec2(_scale.x, _scale.z), vec3(1, 0, 0), PI / 2 };
	rect[3] = { _center + vec3(_scale.x / 2.0f, 0, 0), vec2(_scale.z, _scale.y), vec3(0, 1, 0),  PI / 2 };
	rect[4] = { _center - vec3(0  , -_scale.y, _scale.z / 2.0f), vec2(_scale.x, _scale.z), vec3(1, 0, 0), PI / 2 };
	rect[5] = { _center + vec3(0, 0, _scale.z / 2.0f), vec2(_scale.x,_scale.y), vec3(0), 0 };
}

void plate_t::update_plate() {
	rect[0] = { center - vec3(0, 0, scale.z / 2.0f), vec2(scale.x,scale.y), vec3(0), 0 };
	rect[1] = { center - vec3(scale.x / 2.0f, 0, 0), vec2(scale.z, scale.y), vec3(0, 1, 0), PI / 2 };
	rect[2] = { center - vec3(0, 0, scale.z / 2.0f), vec2(scale.x, scale.z), vec3(1, 0, 0), PI / 2 };
	rect[3] = { center + vec3(scale.x / 2.0f, 0, 0), vec2(scale.z, scale.y), vec3(0, 1, 0),  PI / 2 };
	rect[4] = { center - vec3(0  , -scale.y, scale.z / 2.0f), vec2(scale.x, scale.z), vec3(1, 0, 0), PI / 2 };
	rect[5] = { center + vec3(0, 0, scale.z / 2.0f), vec2(scale.x,scale.y), vec3(0), 0 };
}


// render function
void render_plate(GLuint program, std::vector<plate_t>& plates) {
	for (auto& c : plates) {
		for (rect_t r : c.rect) {
			switch (c.type)
			{
			case 1:
				render_rect(program, r, Plate1Texture);
				break;
			case 2:
				render_rect(program, r, Plate2Texture);
				break;
			case 3:
				render_rect(program, r, Plate3Texture);
				break;
			case 4:
				render_rect(program, r, Plate4Texture);
				break;
			}
		}
	}
}

// creating object functions
inline std::vector<plate_t> create_plates() {
	std::vector<plate_t> cube_vector;
	plate_t cube;

	// 뒷 벽을 기준으로 x 가 좌우, y가 높이(y_scale = 0.1f 고정), z가 앞뒤(z=2.0f, z_scale=2.0f) 고정하면 대충 맵에 맞음
	// (x,y,z (고정) ) -> 위치 , (x' , y' , z' (고정) -> 크기 )  x'/2 + x 가 15.5, -14.5이상되면 안됨
	cube.make_plate(vec3(-3.0f, 2, 2) + away_from_map, vec3(3.0f, 0.6f, 4.0f), 3);
	cube_vector.push_back(cube);
	cube.make_plate(vec3(-6.0f, 4, 2) + away_from_map, vec3(3.0f, 0.6f, 4.0f), 3);
	cube_vector.push_back(cube);
	cube.make_plate(vec3(-9.0f, 6, 2) + away_from_map, vec3(3.0f, 0.6f, 4.0f), 3);
	cube_vector.push_back(cube);
	cube.make_plate(vec3(-12.0f, 8, 2) + away_from_map, vec3(3.0f, 0.6f, 4.0f), 3);
	cube_vector.push_back(cube);
	cube.make_plate(vec3(-9.0f, 11, 2) + away_from_map, vec3(3.0f, 0.6f, 4.0f), 3);
	cube_vector.push_back(cube);
	cube.make_plate(vec3(-5.0f, 14, 2) + away_from_map, vec3(3.0f, 0.6f, 4.0f), 3);
	cube_vector.push_back(cube);
	cube.make_plate(vec3(-1.0f, 11, 2) + away_from_map, vec3(3.0f, 0.6f, 4.0f), 3);
	cube_vector.push_back(cube);
	cube.make_plate(vec3(7.0f, 11, 2) + away_from_map, vec3(6.0f, 0.6f, 4.0f), 3);
	cube_vector.push_back(cube);
	cube.make_plate(vec3(11.0f, 14, 2) + away_from_map, vec3(3.0f, 0.6f, 4.0f), 3);
	cube_vector.push_back(cube);
	cube.make_plate(vec3(14.0f, 17, 2) + away_from_map, vec3(3.0f, 0.6f, 4.0f), 3);
	cube_vector.push_back(cube);
	cube.make_plate(vec3(11.0f, 20, 2) + away_from_map, vec3(3.0f, 0.6f, 4.0f), 3);
	cube_vector.push_back(cube);

	cube.make_plate(vec3(6.0f, 23, 2) + away_from_map, vec3(5.0f, 0.6f, 4.0f), 1);
	cube_vector.push_back(cube);
	cube.make_plate(vec3(1.0f, 26, 2) + away_from_map, vec3(5.0f, 0.6f, 4.0f), 1);
	cube_vector.push_back(cube);
	cube.make_plate(vec3(-4.0f, 23, 2) + away_from_map, vec3(5.0f, 0.6f, 4.0f), 1);
	cube_vector.push_back(cube);
	cube.make_plate(vec3(-9.0f, 26, 2) + away_from_map, vec3(5.0f, 0.6f, 4.0f), 1);
	cube_vector.push_back(cube);
	cube.make_plate(vec3(-13.0f, 29, 2) + away_from_map, vec3(3.0f, 0.6f, 4.0f), 3);
	cube_vector.push_back(cube);
	cube.make_plate(vec3(-9.0f, 32, 2) + away_from_map, vec3(2.0f, 0.6f, 4.0f), 3);
	cube_vector.push_back(cube);
	cube.make_plate(vec3(-5.0f, 34, 2) + away_from_map, vec3(2.0f, 0.6f, 4.0f), 3);
	cube_vector.push_back(cube);
	cube.make_plate(vec3(-1.0f, 33, 2) + away_from_map, vec3(2.0f, 0.6f, 4.0f), 3);
	cube_vector.push_back(cube);
	cube.make_plate(vec3(4.0f, 38, 2) + away_from_map, vec3(2.0f, 0.6f, 4.0f), 3);
	cube_vector.push_back(cube);

	cube.make_plate(vec3(10.0f, 36, 2) + away_from_map, vec3(3.0f, 0.6f, 4.0f), 3);
	cube_vector.push_back(cube);
	cube.make_plate(vec3(13.0f, 34, 3) + away_from_map, vec3(5.0f, 0.6f, 4.0f), 4);
	cube_vector.push_back(cube);
	cube.make_plate(vec3(10.0f, 47, 3) + away_from_map, vec3(3.0f, 0.6f, 4.0f), 3);
	cube_vector.push_back(cube);
	cube.make_plate(vec3(6.0f, 44, 3) + away_from_map, vec3(3.0f, 0.6f, 4.0f), 3);
	cube_vector.push_back(cube);
	cube.make_plate(vec3(1.0f, 45, 3) + away_from_map, vec3(4.0f, 0.6f, 4.0f), 1);
	cube_vector.push_back(cube);
	cube.make_plate(vec3(-6.0f, 41, 3) + away_from_map, vec3(5.0f, 0.6f, 4.0f), 1);
	cube_vector.push_back(cube);
	cube.make_plate(vec3(-13.0f, 44, 3) + away_from_map, vec3(2.0f, 0.6f, 4.0f), 3);
	cube_vector.push_back(cube);
	cube.make_plate(vec3(-14.0f, 49, 3) + away_from_map, vec3(1.0f, 0.6f, 4.0f), 1);
	cube_vector.push_back(cube);

	cube.make_plate(vec3(-9.0f, 52, 3) + away_from_map, vec3(7.0f, 0.6f, 4.0f), 2);
	cube_vector.push_back(cube);
	cube.make_plate(vec3(-3.0f, 53, 3) + away_from_map, vec3(4.5f, 0.6f, 4.0f), 2);
	cube_vector.push_back(cube);
	cube.make_plate(vec3(4.0f, 54, 3) + away_from_map, vec3(5.0f, 0.6f, 4.0f), 2);
	cube_vector.push_back(cube);
	cube.make_plate(vec3(11.0f, 57, 3) + away_from_map, vec3(5.0f, 0.6f, 4.0f), 1);
	cube_vector.push_back(cube);
	cube.make_plate(vec3(10.0f, 61, 3) + away_from_map, vec3(7.0f, 0.6f, 4.0f), 2);
	cube_vector.push_back(cube);




	return cube_vector;
}

auto		plates = std::move(create_plates());

#endif 