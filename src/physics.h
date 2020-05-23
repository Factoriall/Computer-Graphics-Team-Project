#pragma once

#ifndef __PHYSICS_H__
#define __PHYSICS_H__
#include "object.h"

const float gravity = 0.98f;						// gravity
const float e_x = 0.87f;							// elasticity_x  x 방향 마찰계수
const float e_y = 0.87f;							// elasticity_y  y 방향 마찰계수
float		last_t = 0.0f;						// 시간경과 측정을 위한 저장용 변수
float		game_speed = 100.0f;				// 애니메이션의 속도

// collide fuction
bool	floor_collide(float sphere_center_y, float floor_y, float radius)	//바닥과 충돌 감지
{
	if (floor_y + radius > sphere_center_y)
	{
		return 1;
	}
	return 0;
}
bool	wall_collide(float sphere_center_x, float wall_x, float radius)	// 뒷벽안함 ㅅㄱ
{
	if (abs(wall_x - sphere_center_x) < radius)
	{
		return 1;
	}
	return 0;
}

// plate의 중점, plate의 크기, 구의 좌표값, 반지름
bool	plate_collide_1(float pl_x, float pl_y, float plsize_x, float plsize_y, float center_x, float center_y, float radius)	// [1] 발판과 충돌
{
	if (pl_y + plsize_y > center_y && pl_y - plsize_y < center_y && center_x < pl_x && pl_x - plsize_x < center_x + radius)	// [1] y 범위 조건 만족, x범위 조건 만족,  x값 충돌
	{
		return 1;
	}
	return 0;
}
bool	plate_collide_2(float pl_x, float pl_y, float plsize_x, float plsize_y, float center_x, float center_y, float radius)	// [2] 발판과 충돌
{
	if (pl_x + plsize_x > center_x && pl_x - plsize_x < center_x && center_y < pl_y && pl_y - plsize_y < center_y + radius)	// [2] x 범위 조건 만족, y값 충돌
	{
		return 1;
	}
	return 0;
}
bool	plate_collide_3(float pl_x, float pl_y, float plsize_x, float plsize_y, float center_x, float center_y, float radius)	// [3] 발판과 충돌
{
	if (pl_y + plsize_y > center_y && pl_y - plsize_y < center_y && center_x > pl_x && pl_x + plsize_x > center_x - radius)
	{
		return 1;
	}
	return 0;
}
bool	plate_collide_4(float pl_x, float pl_y, float plsize_x, float plsize_y, float center_x, float center_y, float radius)	// [4] 발판과 충돌
{
	if (pl_x + plsize_x > center_x && pl_x - plsize_x < center_x && center_y > pl_y && pl_y + plsize_y > center_y - radius)
	{
		return 1;
	}
	return 0;
}

// 실시간 구 좌표 값 변경 && 구 충돌 구현
int		sphere_t::collision(std::vector <rect_t>& floors, std::vector <rect_t>& walls, std::vector <plate_t>& plates, float t)
{
	float	floor_y = floors[0].center.y;	//y값
	int		is_collide = 0;
	vec3	save_pos_sphere = center;
	float	passed_time = (t - last_t) * game_speed;

	
	if (floor_collide(center.y, floor_y, radius))	//바닥과 충돌 시
	{
		y_speed *= -e_y * passed_time;
		x_speed *= e_x * passed_time;
		center.y = floor_y + radius;	//부르르방지
		is_collide = 2;
	}

	if (wall_collide(center.x, walls[1].center.x, radius))	//벽1과 충돌 시
	{
		x_speed *= -e_x * passed_time;
		if (center.x > walls[1].center.x)
		{
			center.x = walls[1].center.x + radius;
		}
		else {
			center.x = walls[1].center.x - radius;
		}
		is_collide = 3;
	}

	if (wall_collide(center.x, walls[2].center.x, radius))	//벽2과 충돌 시
	{
		x_speed *= -e_x * passed_time;
		if (center.x > walls[2].center.x)
		{
			center.x = walls[2].center.x + radius;
		}
		else {
			center.x = walls[2].center.x - radius;
		}
		is_collide = 3;
	}

	for (auto& plates : plates)
	{
		float pl_x = plates.center.x;
		float pl_y = plates.center.y;	//x,y좌표
		float plsize_x = plates.scale.x / 2;
		float plsize_y = plates.scale.y / 2;	//x,y크기의 1/2
		pl_y += plsize_y;	//정중앙이 아니었던거임~~

		if ((pl_x - center.x) * (pl_x - center.x) + (pl_y - center.y) * (pl_y - center.y) > (radius + plsize_x + plsize_y) * (radius + plsize_x + plsize_y))
		{
			continue;
		}
		if (plate_collide_1(pl_x, pl_y, plsize_x, plsize_y, center.x, center.y, radius))	// plate 안의 rect[1]와 충돌
		{
			x_speed *= -e_x * passed_time;
			center.x = pl_x - plsize_x - radius;
			is_collide = 4;
		}
		if (plate_collide_2(pl_x, pl_y, plsize_x, plsize_y, center.x, center.y, radius))	// plate 안의 rect[2]와 충돌
		{
			y_speed *= -e_y * passed_time;
			x_speed *= e_x * passed_time;
			center.y = pl_y - plsize_y - radius;
			is_collide = 4;
		}
		if (plate_collide_3(pl_x, pl_y, plsize_x, plsize_y, center.x, center.y, radius))	// plate 안의 rect[3]와 충돌
		{
			x_speed *= -e_x * passed_time;
			center.x = pl_x + plsize_x + radius;
			is_collide = 4;
		}
		if (plate_collide_4(pl_x, pl_y, plsize_x, plsize_y, center.x, center.y, radius))	// plate 안의 rect[4]와 충돌
		{
			y_speed *= -e_y * passed_time;
			x_speed *= e_x * passed_time;
			center.y = pl_y + plsize_y + radius;
			is_collide = 4;
		}
	}

	y_speed -= accel * gravity * passed_time;
	center += vec3(x_speed, y_speed, 0);


	last_t = t;
	if ((save_pos_sphere - center).length2() < 0.0002f) {
		if (stop_flag) {
			if (t - paused_time > 1.1f) {
				is_moving = false;
			}
		}
		else {
			paused_time = t;
			stop_flag = true;
		}
		return 0;
	}
	else {
		is_moving = true;
		stop_flag = false;
	}
	/*
	if (abs(x_speed) < 0.001f && abs(y_speed) < 0.001f && is_collide) {
		is_moving = false;
	}
	else {
		is_moving = true;
	}
	*/
	return is_collide;
}

#endif