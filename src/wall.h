#pragma once
#ifndef __WALL_H__
#define __WALL_H__
#include "object.h"

// constants
static const char* brick_image_path = "../bin/images/lava.png";

// opengl variables
GLuint	WallTexture = 0;


// render function
void render_wall(GLuint program, std::vector<rect_t>& walls) {
	glBindVertexArray(rect_vertex_array);

	if (WallTexture != 0) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, WallTexture);
		glUniform1i(glGetUniformLocation(program, "TEX"), 0);
	}

	for (auto& w : walls) {//move the walls using the information inside the walls struct
		mat4 model_matrix = mat4::translate(w.center) *
			mat4::rotate(w.axle, w.angle) *
			mat4::scale(w.scale.x, w.scale.y, 0);

		glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, model_matrix);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);
	}
}

// creating object functions
inline std::vector<rect_t> create_walls() //function to save the information about walls - 3 walls
{
	std::vector<rect_t> walls;
	rect_t w;
	
	w = { vec3(0), vec2(30.0f, 100.0f), vec3(0, 1.0f, 0), 0 };
	walls.emplace_back(w);
	w = { vec3(-15.0f, 0, 1.0f), vec2(10.0f, 100.0f), vec3(0, 1.0f, 0), PI / 2.0f };
	walls.emplace_back(w);
	w = { vec3(15.0f, 0, 1.0f), vec2(10.0f, 100.0f), vec3(0, 1.0f, 0), (PI * 3.0f) / 2.0f };
	walls.emplace_back(w);
	

	return walls;
}

auto		walls = std::move(create_walls());

#endif 