#pragma once
#ifndef __CAMERA_H__
#define __CAMERA_H__

float more_view_angle = 0.0f;
vec3  cam_away = vec3(0, 5.0f, 15.0f);
// camera structure
struct camera
{
	vec3	eye = vec3(0, 2.0f, 14.0f);//camera position
	vec3	at = vec3(0, 0.0f, 0.0f);//target position
	vec3	up = vec3(0, 1, 0);//up
	

	float	fovy = PI / 4.0f; // must be in radian
	float	dNear = 1.0f;
	float	dFar = 1000.0f;
	float	aspect_ratio;
	mat4	view_matrix = mat4::look_at(eye, at, up);
	mat4	projection_matrix;
	void	update(vec3 sphere_center);
	void	update();
};

// implement fuction
void camera::update(vec3 sphere_center) {
	eye.y = sphere_center.y + cam_away.y;
	eye.x = sphere_center.x;	// 눈이 공 따라가게
	eye.z = cam_away.z + more_view_angle;
	at = sphere_center;
	view_matrix = mat4::look_at(eye, at, up);
}
void camera::update() {
	view_matrix = mat4::look_at(eye, at, up);
}

camera	cam_for_dev;
camera	cam_for_play;
camera	cam_intro;
camera* cam_now = &cam_intro;

void reset_camera() {
	cam_for_play = { vec3(0, 2.0f, 14.0f), vec3(0, 0.0f, 0.0f), vec3(0, 1, 0), PI/4.0f,  1.0f, 1000.0f};
	cam_for_play.update();
	cam_for_dev = { vec3(0, 2.0f, 14.0f), vec3(0, 0.0f, 0.0f), vec3(0, 1, 0), PI / 4.0f,  1.0f, 1000.0f };
	cam_for_dev.update();
	cam_now = &cam_intro;
}
#endif

