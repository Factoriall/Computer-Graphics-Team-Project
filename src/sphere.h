#pragma once
#ifndef __SPHERE_H__
#define __SPHERE_H__
#include "object.h"

// constant
static const char* sphere_image_path = "../bin/images/earth.png";
static const float basic_power = 12.3f;
// opengl variables
GLuint	SphereTexture = 0;			// ���� �ؽ��� ����

// global variables
float	power = basic_power;		// ���� ���� ����
float	max_charge_time = 1.0f;		// �ִ� ���� �ð�
float	min_charge_time = 0.4f;		// �ּ� ���� �ð�
float	charge_range = max_charge_time - min_charge_time;

struct jp_t {//jump ������ ǥ��
	bool jumpping_now = false;		// true - ������ư�� ������ ��
	float startTime = 0.0f;			// ������ư ���� �ð�
	float endTime = 0.0f;			// ������ư ���� �ð�
	void jump_action(sphere_t& sp);	// ����ȿ�� ���� �Լ�
	float get_gauge(float t);		// callback ���������� return
} jp; // flags of keys for smooth changes

// implement fuctions
void sphere_t::update(float del_t) {
	angle += angle_speed * del_t;
	// angle_speed -= angle_speed * 0.1f * del_t; ���� ��

	model_matrix = mat4::translate(center) *
		mat4::rotate(vec3(0, 0, 1), angle) *
		mat4::scale(radius);
}
void jp_t::jump_action(sphere_t& sp) {
	//jump_once = true;
	float gauge = min((jp.endTime - jp.startTime)/charge_range + min_charge_time, max_charge_time) * power;
	float angle = pointer.angle + PI / 4.0f; // ���� ����

	sp.angle_speed = -cos(angle) * gauge;
	sp.x_speed += gauge * cos(angle);
	sp.y_speed += gauge * sin(angle);
	power = basic_power;					// ���� �Ŀ��� ����
	printf("Jump!  gauge: %f, v(x, y) = %.2f, %.2f\n", gauge, sp.x_speed, sp.y_speed);
};
float jp_t::get_gauge(float t) {
	return  jumpping_now ? min((t - startTime)/charge_range + min_charge_time, max_charge_time) : 0;
}

// render function
void render_sphere(GLuint program, sphere_t & sphere, float del_t) {
	glBindVertexArray(sphere_vertex_array);
	
	if (SphereTexture != 0) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, SphereTexture);
		glUniform1i(glGetUniformLocation(program, "TEX"), 0);
	}

	sphere.update(del_t);

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