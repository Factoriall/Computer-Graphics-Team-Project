#pragma once
#ifndef __STORM_H__
#define __STORM_H__
#include "object.h"

// constant
static const char* storm_image_path = "../bin/images/storm.jpg";
static const float angle_speed = 0.02f;

// opengl variables
GLuint	StormTexture = 0;

// render function
void render_storm(GLuint program, circle_t & storm) {
	glBindVertexArray(circle_vertex_array);

	if (StormTexture != 0) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, StormTexture);
		glUniform1i(glGetUniformLocation(program, "TEX"), 0);
	}

	storm.angle += angle_speed;

	mat4 model_matrix = mat4::translate(storm.center) *
		mat4::rotate(vec3(0, 0, 1.0f), storm.angle) *
		mat4::scale(storm.scale, storm.scale, 0);

	glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, model_matrix);
	glDrawElements(GL_TRIANGLES, 2*3*(N+1), GL_UNSIGNED_INT, nullptr);
}

// creating object functions
inline circle_t create_storm() //function to save the information about walls - 3 walls
{
	return { vec3(0.0f, 29.0f, 1.0f), 1.0f, 0.0f};
}

circle_t		storm = create_storm();

#endif 