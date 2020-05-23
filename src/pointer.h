#pragma once
#ifndef __POINTER_H__
#define __POINTER_H__
#include "object.h"

// constants
static const char* pointer_image_path = "../bin/images/pointer.jpg";

// opengl variables
GLuint	PointerTexture = 0;

struct pointer_t {
	vec3	center = vec3(0);
	float	scale = 0;
	float	angle = 0;

	void	update(sphere_t& sp);
};

// render function
void render_pointer(GLuint program, pointer_t & pointer, float gauge) {
	glBindVertexArray(pointer_vertex_array);

	GLint uloc;

	gauge /= PI/4;
	vec4 point_color = vec4(sin(gauge), 0.1f, cos(gauge), 1.0f);
	uloc = glGetUniformLocation(program, "b_solid_color");	if (uloc > -1) glUniform1i(uloc, 1);
	uloc = glGetUniformLocation(program, "solid_color");		if (uloc > -1) glUniform4fv(uloc, 1, point_color);	// pointer version

	mat4 model_matrix = mat4::translate(pointer.center) *
		mat4::rotate(vec3(0, 0, 1), pointer.angle) *
		mat4::scale(pointer.scale);
	glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, model_matrix);
	glDrawElements(GL_TRIANGLES, 35*2*3, GL_UNSIGNED_INT, nullptr);

	uloc = glGetUniformLocation(program, "b_solid_color");	if (uloc > -1) glUniform1i(uloc, 0);
}

// implement
void pointer_t::update(sphere_t & sp) {
	center = sp.center;
}

// creating object functions
inline pointer_t create_pointer() //function to save the information about walls - 3 walls
{
	return { vec3(0, 0.0f, 1.0f), 0.4f, 0 };
}

pointer_t	pointer = create_pointer();

#endif 