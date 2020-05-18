#pragma once
#ifndef __FLOOR_H__
#define __FLOOR_H__
#include "object.h"

// constant
static const char* floor_image_path = "../bin/images/floor.jpg";

// opengl variables
GLuint	FloorTexture = 0;


// render function
void render_floor(GLuint program, std::vector<rect_t>& floors) {
	glBindVertexArray(rect_vertex_array);
	

	if (FloorTexture != 0) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, FloorTexture);
		glUniform1i(glGetUniformLocation(program, "TEX"), 0);
	}

	for (auto& w : floors) {//move the walls using the information inside the walls struct
		mat4 model_matrix = mat4::translate(w.center) *
			mat4::rotate(w.axle, w.angle) *
			mat4::scale(w.scale.x, w.scale.y, 0);

		glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, model_matrix);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);
	}
}

// creating object functions
inline std::vector<rect_t> create_floors() //function to save the information about walls - 3 walls
{
	std::vector<rect_t> floors;
	rect_t f;

	f = { vec3(0, 0 , -5.0f), vec2(10.0f, 10.0f), vec3(1.0f, 0, 0), PI / 2.0f };
	floors.emplace_back(f);


	return floors;
}

auto		floors = std::move(create_floors());

#endif 