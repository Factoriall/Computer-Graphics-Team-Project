#pragma once
#ifndef __SPHERE_H__
#define __SPHERE_H__
#include "object.h"

// constant
static const char* sphere_image_path = "../bin/images/earth.png";

// opengl variables
GLuint	SphereTexture = 0;

// global variables
bool	stop_simulation = false;
float	paused_time = 0.0f;

// implement fuctions
void sphere_t::update(float t) {
	model_matrix = mat4::translate(center) *
		mat4::rotate(vec3(1, 0, 0), angle.y) *
		mat4::rotate(vec3(0, 1, 0), angle.x) *
		mat4::scale(radius);
}

// render function
void render_sphere(GLuint program, sphere_t & sphere, float t) {
	glBindVertexArray(sphere_vertex_array);
	
	if (SphereTexture != 0) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, SphereTexture);
		glUniform1i(glGetUniformLocation(program, "TEX"), 0);
	}

	sphere.update(t);

	glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, sphere.model_matrix);
	if (b_index_buffer)	glDrawElements(GL_TRIANGLES, M * (N * 3 + 3) * 2, GL_UNSIGNED_INT, nullptr);
	else				glDrawArrays(GL_TRIANGLES, 0, M * (N * 3 + 3) * 2);
}

// creating object functions
inline sphere_t create_sphere() {
	return { vec3(0.0f, 1.0f, 1.0f), 0.3f, vec2(0, 0) };
}

#endif 