#pragma once
#ifndef __CAMERA_H__
#define __CAMERA_H__

// camera structure
struct camera
{
	vec3	eye = vec3(0, 4.0f, 10.0f);//camera position
	vec3	at = vec3(0, 0.0f, 0.0f);//target position
	vec3	up = vec3(0, 1, 0);//up
	mat4	view_matrix = mat4::look_at(eye, at, up);

	float	fovy = PI / 4.0f; // must be in radian
	float	aspect_ratio;
	float	dNear = 1.0f;
	float	dFar = 1000.0f;
	mat4	projection_matrix;
	void	update(vec3 sphere_center);
	void	update();
};

// implement fuction
void camera::update(vec3 sphere_center) {
	eye.y = sphere_center.y + 2.0f;
	eye.z = 6.0f;
	at = sphere_center;
	view_matrix = mat4::look_at(eye, at, up);
}
void camera::update() {
	view_matrix = mat4::look_at(eye, at, up);
}

camera	cam_for_dev;
camera	cam_for_play;
camera	cam_intro;

#endif

