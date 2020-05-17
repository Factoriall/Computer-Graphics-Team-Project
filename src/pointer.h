#pragma once
#ifndef __POINTER_H__
#define __POINTER_H__
#include "object.h"

// constants
static const char* pointer_image_path = "../bin/images/pointer.jpg";

// opengl variables
GLuint	PointerTexture = 0;

// render function
void render_pointer(GLuint program, pointer_t & pointer) {
	glBindVertexArray(pointer_vertex_array);

	if (PointerTexture != 0) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, PointerTexture);
		glUniform1i(glGetUniformLocation(program, "TEX"), 0);
	}

	mat4 model_matrix = mat4::translate(pointer.center) *
		mat4::rotate(vec3(0, 0, 1), pointer.angle) *
		mat4::scale(pointer.scale);
	glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, model_matrix);
	glDrawElements(GL_TRIANGLES, 35*2*3, GL_UNSIGNED_INT, nullptr);
}

// implement
void pointer_t::update(float t, sphere_t & sp) {
	center = sp.center;
}

// creating object functions
inline pointer_t create_pointer() //function to save the information about walls - 3 walls
{
	return { vec3(0, 0.3f, 1.0f), 0.4f, 0 };
}

#endif 