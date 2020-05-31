#pragma once
#ifndef __INTRO_H__
#define __INTRO_H__
#include "object.h"

// constant
static const char* logo_image_path = "../bin/images/logo.png";
static const char* intro_image_path[] = {
	"../bin/images/intro_capture_1.png",
	"../bin/images/intro_capture_2.png",
	"../bin/images/intro_capture_3.png"
};
static const float	max_intro_logo_proceed = 4.0f;
static const float	max_reading_time = 1.5f;

// opengl variables
GLuint	LogoTexture = 0;
GLuint	IntroTexture[] = { 0, 0, 0 };

float	reading_time = 0.0;				// 대기시간, 시간 저장용 변수, 사용 후 0으로 둘 것
float	running_time = 0.0f;			// 진행시간, 시잔 저장용 변수, 사용 후 0으로 둘 것
/*
	0 - 인트로 띄울 준비가 됬음 ( 시작하면 1로 진행)
	1 - 로고 뛰우는 중 -> 로고가 뜬 후 사용자 입력 기다리는 중 (마우스 버튼으로 다음 진행)
	2+ - 설명서 1~ (마우스 클릭으로 다음 진행)
	...
	-1 - 인트로 할일 없음 (모든 기능 정지)
*/
int		intro_state = -1;
float	intro_logo_proceed_time = 0;	// logo 시간 저장용 변수, 사용 후 0으로 둘 것
bool	intro_next_text_ready = false;	// "버튼을 누르시오" 텍스트 출력용 제어 변수
bool	intro_logo = false;				// Logo 렌더링 제어 변수
int		intro_descriptor_index = -1;	// -1 은 기능 취소

//	rect structure :  xyz-pos, xy-scale, xyz-axis, rotation_angle
rect_t	introLogo = { vec3(0, 0.3f, 0), vec2(5.0f, 1.3f), vec3(0), 0 };
rect_t	introDescriptor[] = {	// 순서대로 넘기는 설명 책
	{ vec3(0, -2.0f, 4.0f), vec2(6.0f, 4.0f), vec3(0), 0 },
	{ vec3(0, -2.0f, 4.0f), vec2(6.0f, 4.0f), vec3(0), 0 },
	{ vec3(0, -2.0f, 4.0f), vec2(6.0f, 4.0f), vec3(0), 0 }
};


void reset_intro_state() {
	intro_state = 0;
}

int running_intro(float del_t, bool next_click) {
	running_time += del_t;
	int next_game_mod = 0;
	float percent;

	switch (intro_state) {
	case -1:
		// not ready to start
		return next_game_mod;
	case 0:
		// ready to start
		intro_logo_proceed_time = 0;
		intro_next_text_ready = false;
		intro_descriptor_index = -1;	// -1 은 기능 취소
		intro_logo_proceed_time = 0;
		intro_logo = true;
		intro_state++;
		break;
	case 1:
		// 로고 등장 타임
		intro_logo_proceed_time += del_t;
		percent = sin(intro_logo_proceed_time * PI / (max_intro_logo_proceed * 2));
		if (intro_logo_proceed_time > max_intro_logo_proceed) {
			percent = 1.0f;
			intro_next_text_ready = true;
			// 버튼 누르면 다음 단계로 감
			if (next_click) {
				intro_logo_proceed_time++;
				introLogo.scale = vec2(0);
				intro_next_text_ready = false;
				intro_logo = false;
				intro_state++;
				break;
			}
		}
		introLogo.scale = vec2(5.0f, 1.3f) * percent * 1.8f;
		break;
	case 2:
		reading_time += del_t;
		if (reading_time > max_reading_time) {
			intro_next_text_ready = true;
			// 버튼 누르면 다음 단계로 감
			if (next_click) {
				reading_time = 0;
				intro_logo_proceed_time++;
				intro_next_text_ready = false;
				intro_descriptor_index = 1;
				intro_state++;
				break;
			}
		}
		intro_descriptor_index = 0;
		break;
	case 3:
		reading_time += del_t;
		if (reading_time > max_reading_time) {
			intro_next_text_ready = true;
			// 버튼 누르면 다음 단계로 감
			if (next_click) {
				reading_time = 0;
				intro_logo_proceed_time++;
				intro_next_text_ready = false;
				intro_descriptor_index = 2;
				intro_state++;
				break;
			}
		}
		intro_descriptor_index = 1;
		break;
	case 4:
		reading_time += del_t;
		if (reading_time > max_reading_time) {
			intro_next_text_ready = true;
			// 버튼 누르면 다음 단계로 감
			if (next_click) {
				reading_time = 0;
				intro_logo_proceed_time++;
				intro_next_text_ready = false;
				intro_descriptor_index = -1;
				intro_state++;
				break;
			}
		}
		intro_descriptor_index = 2;
		break;
	default:
		intro_state = -1;
		intro_descriptor_index = -1;
		next_game_mod = 1;
	}

	return next_game_mod;
}



// render function
void render_introLogo(GLuint program, rect_t& logo) {
	glBindVertexArray(rect_vertex_array);

	if (LogoTexture != 0) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, LogoTexture);
		glUniform1i(glGetUniformLocation(program, "TEX"), 0);
	}

	mat4 model_matrix = mat4::translate(logo.center) *
		mat4::rotate(logo.axle, logo.angle) *
		mat4::scale(logo.scale.x, logo.scale.y, 0);

	glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, model_matrix);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);
}
void render_introBoard(GLuint program) {
	glBindVertexArray(rect_vertex_array);

	if (IntroTexture[intro_descriptor_index] != 0) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, IntroTexture[intro_descriptor_index]);
		glUniform1i(glGetUniformLocation(program, "TEX"), 0);
	}

	mat4 model_matrix = mat4::translate(introDescriptor[intro_descriptor_index].center) *
		mat4::rotate(introDescriptor[intro_descriptor_index].axle, introDescriptor[intro_descriptor_index].angle) *
		mat4::scale(introDescriptor[intro_descriptor_index].scale.x, introDescriptor[intro_descriptor_index].scale.y, 0);

	glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, model_matrix);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);
}
#endif 