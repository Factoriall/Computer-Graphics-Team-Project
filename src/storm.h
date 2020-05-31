#pragma once
#ifndef __STORM_H__
#define __STORM_H__
#include "object.h"

// constant
static const char* storm_image_path = "../bin/images/storm.jpg";
static const float basic_angle_speed = 0.6f;		// 시공의 폭풍의 기본 회전 속도
static const float del_speed = 12.0f;				// 시공의 폭풍의 회전 속도 증가량
static const float max_proceed = 3.5f;				// 시공의 폭풍 진입 경과 시간

// opengl variables
GLuint	StormTexture = 0;

// variable
vec3 sphere_pos;		// 구를 폭풍쪽으로 이동
vec3 del_sphere_pos;
vec3 cam_at;			// 카메라를 폭풍쪽으로 이동
vec3 del_cam_at;
vec3 cam_eye;			// 카메라를 폭풍쪽으로 이동
vec3 del_cam_eye;

// render function
void render_storm(GLuint program, circle_t& storm, float del_t) {
	glBindVertexArray(circle_vertex_array);

	if (StormTexture != 0) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, StormTexture);
		glUniform1i(glGetUniformLocation(program, "TEX"), 0);
	}

	storm.angle += storm.angle_speed * del_t;

	mat4 model_matrix = mat4::translate(storm.center) *
		mat4::rotate(vec3(0, 0, 1.0f), storm.angle) *
		mat4::scale(storm.scale, storm.scale, 0);

	glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, model_matrix);
	glDrawElements(GL_TRIANGLES, 2 * 3 * (N + 1), GL_UNSIGNED_INT, nullptr);
}
bool circle_t::in_storm(vec3 sp_center, float sp_r){
	// 공의 위치와 크기를 받아 시공의 폭풍과 인접해 있는지 판단하여 return
	return (center - sp_center).length() < scale + sp_r ? 1 : 0;
}
int circle_t::go_to_storm(sphere_t & sp, camera & cam, float del_t) {
	proceed += del_t;
	float percent = sin(proceed * PI / (max_proceed * 2));
	if (proceed > max_proceed) return 1;

	angle_speed = basic_angle_speed + del_speed * percent;
	sp.center = sphere_pos + del_sphere_pos * percent;
	cam.at = cam_at + del_cam_at * percent;
	cam.eye = cam_eye + del_cam_eye * percent;

	return 0;
}

// creating object functions
inline circle_t create_storm() //function to save the information about walls - 3 walls
{
	return { vec3(0.0f, 64.0f, 1.0f), 1.0f, 0.0f};	// 맵 위에 생성
	//return { vec3(2.0f, 2.0f, 0.3f), 1.0f, 0.0f };		// 바로 옆에 생성
}

circle_t		storm = create_storm();

void reset_storm() {
	sphere_pos = 0;		// 구를 폭풍쪽으로 이동
	del_sphere_pos = 0;
	cam_at = 0;			// 카메라를 폭풍쪽으로 이동
	del_cam_at = 0;
	cam_eye = 0;			// 카메라를 폭풍쪽으로 이동
	del_cam_eye = 0;
}
#endif 