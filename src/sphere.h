#pragma once
#ifndef __SPHERE_H__
#define __SPHERE_H__
#include "object.h"

// constant
static const char* sphere_image_path = "../bin/images/earth.png";

// opengl variables
GLuint	SphereTexture = 0;

// global variables
float	power = 0.2f;
bool	stop_simulation = false;
float	paused_time = 0.0f;
float	gauge = 0.0f;
float	angle = 0.0f;

struct jp_t {//jump 게이지 표현
	bool jump = false;
	float startTime = 0.0f;
	float endTime = 0.0f;
	operator bool() const { return jump; }
	void jump_action(sphere_t& sp);
	float get_gauge(float t);
} jp; // flags of keys for smooth changes

// implement fuctions
void sphere_t::update(float t) {
	angle += angle_speed;
	angle_speed *= 0.997f;
	
	model_matrix = mat4::translate(center) *
		mat4::rotate(vec3(0, 0, 1), angle) *
		mat4::scale(radius);
}
void jp_t::jump_action(sphere_t& sp) {
	gauge = min(endTime - startTime, 2.0f) * power;
	angle = pointer.angle + PI / 4.0f;//각도 조정

	sp.x_speed = gauge * cos(angle);
	sp.y_speed = gauge * sin(angle);
};
float jp_t::get_gauge(float t) {
	if (jump) {
		return gauge = min(t - startTime, 2.0f);
	}
	else {
		return 0;
	}
	
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
	return { vec3(0.0f, 1.0f, 1.0f), 0.3f, 0};
}

sphere_t	sphere = create_sphere();

#endif 