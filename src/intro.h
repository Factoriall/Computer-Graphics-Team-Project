#pragma once
#ifndef __INTRO_H__
#define __INTRO_H__
#include "object.h"

// constant
static const char* intro_image_path = "../bin/images/intro_capture.png";

// opengl variables
GLuint	IntroTexture = 0;

// render function
void render_introBoard(GLuint program, rect_t& introBoard) {
	glBindVertexArray(rect_vertex_array);


	if (IntroTexture != 0) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, IntroTexture);
		glUniform1i(glGetUniformLocation(program, "TEX"), 0);
	}

	mat4 model_matrix = mat4::translate(introBoard.center) *
		mat4::rotate(introBoard.axle, introBoard.angle) *
		mat4::scale(introBoard.scale.x, introBoard.scale.y, 0);

	glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, model_matrix);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);
	
}

// creating object functions
inline rect_t create_introBoard() //function to save the information about walls - 3 walls
{
	return { vec3(30.0f, 0, 0), vec2(4.81f, 7.32f), vec3(0, 0, 0), 0 };
}

rect_t		introBoard = create_introBoard();

#endif 