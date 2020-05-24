#pragma once

#ifndef __PHYSICS_H__
#define __PHYSICS_H__
#include "object.h"

const float gravity = 12.3f;						// gravity
float		angle_const = 4.3f;
float		last_t;

// const
vec2		floor_elasticity = vec2(0.30f);
vec2		wall_elasticity = vec2(0.33f);
vec2		plate_elasticity = vec2(0.33f);
vec2		plate2_elasticity = vec2(0.03f, 0.33f);	//얼음
vec2		plate3_elasticity = vec2(0.9f, 0.9f);	//끈끈이
vec2		plate4_elasticity = vec2(0.33f, -0.6f);	//점프발판


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

int get_sign(float x) {
	return (x > 0) ? 1 : -1;
}


// 실시간 구 좌표 값 변경 && 구 충돌 구현
int		sphere_t::collision(std::vector <rect_t>& floors, std::vector <rect_t>& walls, std::vector <plate_t>& plates, float t, int fps)
{
	fps = 1;
	float	floor_y = floors[0].center.y;	//y값
	int		is_collide = 0;
	vec3	p0 = center;
	float	del_t = (t - last_t);
	float	maximum_friction = max(0.11f * del_t, 0.01f);
	y_speed -= gravity * del_t;

	vec3	pn = p0 + vec3(x_speed, y_speed, 0) * del_t;

	if (floor_collide(pn.y, floor_y, radius))	//바닥과 충돌 시
	{
		center.y = floor_y + radius;
		if (abs(p0.y - center.y) > maximum_friction) is_collide = 2;
		if (y_speed < 0) {
			y_speed = -y_speed;
		}
		x_speed -= x_speed * floor_elasticity.x;
		y_speed -= y_speed * floor_elasticity.y;
		if (abs(x_speed) < maximum_friction) {
			x_speed = 0;
		}
		if (abs(y_speed) < maximum_friction) {
			y_speed = 0;
		}

		angle_speed = -x_speed*angle_const;
	}

	if (wall_collide(pn.x, walls[1].center.x, radius))	//벽1과 충돌 시
	{
		// 왼쪽 벽
		center.x = walls[1].center.x + radius;
		if (abs(p0.x - center.x) > maximum_friction) is_collide = 3;
		if (x_speed < 0) {
			x_speed = -x_speed;
		}
		x_speed -= x_speed * wall_elasticity.x;
		y_speed -= y_speed * wall_elasticity.y;
		if (abs(x_speed) < maximum_friction) {
			x_speed = 0;
		}
		if (abs(y_speed) < maximum_friction) {
			y_speed = 0;
		}

		angle_speed = -y_speed * angle_const;
		
	}

	if (wall_collide(pn.x, walls[2].center.x, radius))	//벽2과 충돌 시
	{
		center.x = walls[2].center.x - radius;
		if (abs(p0.x - center.x) > maximum_friction) is_collide = 3;
		if (x_speed > 0) {
			x_speed = -x_speed;
		}
		x_speed -= x_speed * wall_elasticity.x;
		y_speed -= y_speed * wall_elasticity.y;
		if (abs(x_speed) < maximum_friction) {
			x_speed = 0;
		}
		if (abs(y_speed) < maximum_friction) {
			y_speed = 0;
		}

		angle_speed = -y_speed * angle_const;
		
	}

	for (auto& plates : plates)
	{
		float pl_x = plates.center.x;
		float pl_y = plates.center.y;	//x,y좌표
		float plsize_x = plates.scale.x / 2;
		float plsize_y = plates.scale.y / 2;	//x,y크기의 1/2
		pl_y += plsize_y;	//정중앙이 아니었던거임~~

		if ((pl_x - pn.x) * (pl_x - pn.x) + (pl_y - pn.y) * (pl_y - pn.y) > (radius + plsize_x + plsize_y) * (radius + plsize_x + plsize_y))
		{
			continue;
		}
		if (plate_collide_1(pl_x, pl_y, plsize_x, plsize_y, pn.x, pn.y, radius))	// plate 안의 rect[1]와 충돌
		{
			// 왼쪽
			center.x = pl_x - plsize_x - radius;
			if (abs(p0.x - center.x) > maximum_friction) is_collide = 4;
			if (x_speed > 0) {
				x_speed = -x_speed;
			}
			x_speed -= x_speed * plate_elasticity.x;
			y_speed -= y_speed * plate_elasticity.y;
			if (abs(x_speed) < maximum_friction) {
				x_speed = 0;
			}
			if (abs(y_speed) < maximum_friction) {
				y_speed = 0;
			}
			
			angle_speed = -y_speed * angle_const;
			//center.x = pl_x - plsize_x - radius;
			
		}
		if (plate_collide_2(pl_x, pl_y, plsize_x, plsize_y, pn.x, pn.y, radius))	// plate 안의 rect[2]와 충돌
		{
			// 아래
			center.y = pl_y - plsize_y - radius;
			if (abs(p0.y - center.y) > maximum_friction) is_collide = 4;
			if (y_speed > 0) {
				y_speed = -y_speed;
			}
			x_speed -= x_speed * plate_elasticity.x;
			y_speed -= y_speed * plate_elasticity.y;
			if (abs(x_speed) < maximum_friction) {
				x_speed = 0;
			}
			if (abs(y_speed) < maximum_friction) {
				y_speed = 0;
			}

			angle_speed = -x_speed * angle_const;
			//center.y = pl_y - plsize_y - radius;
			
		}
		if (plate_collide_3(pl_x, pl_y, plsize_x, plsize_y, pn.x, pn.y, radius))	// plate 안의 rect[3]와 충돌
		{
			// 오른쪽
			center.x = pl_x + plsize_x + radius;
			if (abs(p0.x - center.x) > maximum_friction) is_collide = 4;
			if (x_speed < 0) {
				x_speed = -x_speed;
			}
			x_speed -= x_speed * plate_elasticity.x;
			y_speed -= y_speed * plate_elasticity.y;
			if (abs(x_speed) < maximum_friction) {
				x_speed = 0;
			}
			if (abs(y_speed) < maximum_friction) {
				y_speed = 0;
			}
	
			angle_speed = -y_speed * angle_const;
			//center.x = pl_x + plsize_x + radius;
			
		}

		
		if (plate_collide_4(pl_x, pl_y, plsize_x, plsize_y, pn.x, pn.y, radius))	// plate 안의 rect[4]와 충돌
		{
			// 위쪽
			float new_ex;
			float new_ey;	// 발판 종류마다 마찰계수 다름
			switch (plates.type)
			{
			case 1:
				new_ex = plate_elasticity.x;
				new_ey = plate_elasticity.y;
				break;
			case 2:
				new_ex = plate2_elasticity.x;
				new_ey = plate2_elasticity.y;
				break;
			case 3:
				new_ex = plate3_elasticity.x;
				new_ey = plate3_elasticity.y;
				break;
			case 4:
				new_ex = plate4_elasticity.x;
				new_ey = plate4_elasticity.y;
				break;
			}
			center.y = pl_y + plsize_y + radius;
			if (abs(p0.y - center.y) > maximum_friction) is_collide = 4;
			if (y_speed < 0) {
				y_speed = -y_speed;
			}
			x_speed -= x_speed * new_ex;
			y_speed -= y_speed * new_ey;
			if (abs(x_speed) < maximum_friction) {
				x_speed = 0;
			}
			if (abs(y_speed) < maximum_friction) {
				y_speed = 0;
			}

			angle_speed = -x_speed * angle_const;
			//center.y = pl_y + plsize_y + radius;
			
		}
		
	}

	center += vec3(x_speed, y_speed, 0) * del_t;
	
	if ((p0 - center).length() < maximum_friction) {
		if (stop_flag) {
			if (t - paused_time > 0.5f) {
				is_moving = false;
			}
		}
		else {
			paused_time = t;
			stop_flag = true;
		}

		// return 0;
	}
	else {
		is_moving = true;
		stop_flag = false;
	}

	last_t = t;

	return is_collide;
}

#endif