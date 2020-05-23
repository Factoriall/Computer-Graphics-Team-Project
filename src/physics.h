#pragma once

#ifndef __PHYSICS_H__
#define __PHYSICS_H__
#include "object.h"

const float gravity = 0.98f;						// gravity
const float e_x = 0.87f;							// elasticity_x  x ���� �������
const float e_y = 0.87f;							// elasticity_y  y ���� �������
float		angle_const = 4.3f;

// collide fuction
bool	floor_collide(float sphere_center_y, float floor_y, float radius)	//�ٴڰ� �浹 ����
{
	if (floor_y + radius > sphere_center_y)
	{
		return 1;
	}
	return 0;
}
bool	wall_collide(float sphere_center_x, float wall_x, float radius)	// �޺����� ����
{
	if (abs(wall_x - sphere_center_x) < radius)
	{
		return 1;
	}
	return 0;
}

// plate�� ����, plate�� ũ��, ���� ��ǥ��, ������
bool	plate_collide_1(float pl_x, float pl_y, float plsize_x, float plsize_y, float center_x, float center_y, float radius)	// [1] ���ǰ� �浹
{
	if (pl_y + plsize_y > center_y && pl_y - plsize_y < center_y && center_x < pl_x && pl_x - plsize_x < center_x + radius)	// [1] y ���� ���� ����, x���� ���� ����,  x�� �浹
	{
		return 1;
	}
	return 0;
}
bool	plate_collide_2(float pl_x, float pl_y, float plsize_x, float plsize_y, float center_x, float center_y, float radius)	// [2] ���ǰ� �浹
{
	if (pl_x + plsize_x > center_x && pl_x - plsize_x < center_x && center_y < pl_y && pl_y - plsize_y < center_y + radius)	// [2] x ���� ���� ����, y�� �浹
	{
		return 1;
	}
	return 0;
}
bool	plate_collide_3(float pl_x, float pl_y, float plsize_x, float plsize_y, float center_x, float center_y, float radius)	// [3] ���ǰ� �浹
{
	if (pl_y + plsize_y > center_y && pl_y - plsize_y < center_y && center_x > pl_x && pl_x + plsize_x > center_x - radius)
	{
		return 1;
	}
	return 0;
}
bool	plate_collide_4(float pl_x, float pl_y, float plsize_x, float plsize_y, float center_x, float center_y, float radius)	// [4] ���ǰ� �浹
{
	if (pl_x + plsize_x > center_x && pl_x - plsize_x < center_x && center_y > pl_y && pl_y + plsize_y > center_y - radius)
	{
		return 1;
	}
	return 0;
}

// �ǽð� �� ��ǥ �� ���� && �� �浹 ����
int		sphere_t::collision(std::vector <rect_t>& floors, std::vector <rect_t>& walls, std::vector <plate_t>& plates, float t)
{
	float	floor_y = floors[0].center.y;	//y��
	int		is_collide = 0;
	vec3	save_pos_sphere = center;
	
	if (floor_collide(center.y, floor_y, radius))	//�ٴڰ� �浹 ��
	{
		y_speed *= -e_y;
		x_speed *= e_x;
		angle_speed = -x_speed*angle_const;
		
		center.y = floor_y + radius;	//�θ�������
		is_collide = 2;
	}

	if (wall_collide(center.x, walls[1].center.x, radius))	//��1�� �浹 ��
	{
		x_speed *= -e_x;
		if (center.x > walls[1].center.x)
		{
			center.x = walls[1].center.x + radius;
		}
		else {
			center.x = walls[1].center.x - radius;
		}
		angle_speed = -y_speed * angle_const;
		is_collide = 3;
	}

	if (wall_collide(center.x, walls[2].center.x, radius))	//��2�� �浹 ��
	{
		x_speed *= -e_x;
		if (center.x > walls[2].center.x)
		{
			center.x = walls[2].center.x + radius;
		}
		else {
			center.x = walls[2].center.x - radius;
		}
		angle_speed = -y_speed * angle_const;
		is_collide = 3;
	}

	for (auto& plates : plates)
	{
		float pl_x = plates.center.x;
		float pl_y = plates.center.y;	//x,y��ǥ
		float plsize_x = plates.scale.x / 2;
		float plsize_y = plates.scale.y / 2;	//x,yũ���� 1/2
		pl_y += plsize_y;	//���߾��� �ƴϾ�������~~

		if ((pl_x - center.x) * (pl_x - center.x) + (pl_y - center.y) * (pl_y - center.y) > (radius + plsize_x + plsize_y) * (radius + plsize_x + plsize_y))
		{
			continue;
		}
		if (plate_collide_1(pl_x, pl_y, plsize_x, plsize_y, center.x, center.y, radius))	// plate ���� rect[1]�� �浹
		{
			x_speed *= -e_x;
			center.x = pl_x - plsize_x - radius;
			angle_speed = -y_speed * angle_const;
			is_collide = 4;
		}
		if (plate_collide_2(pl_x, pl_y, plsize_x, plsize_y, center.x, center.y, radius))	// plate ���� rect[2]�� �浹
		{
			y_speed *= -e_y;
			x_speed *= e_x;
			center.y = pl_y - plsize_y - radius;
			angle_speed = -x_speed * angle_const;
			is_collide = 4;
		}
		if (plate_collide_3(pl_x, pl_y, plsize_x, plsize_y, center.x, center.y, radius))	// plate ���� rect[3]�� �浹
		{
			x_speed *= -e_x;
			center.x = pl_x + plsize_x + radius;
			angle_speed = -y_speed * angle_const;
			is_collide = 4;
		}
		if (plate_collide_4(pl_x, pl_y, plsize_x, plsize_y, center.x, center.y, radius))	// plate ���� rect[4]�� �浹
		{
			y_speed *= -e_y;
			x_speed *= e_x;
			center.y = pl_y + plsize_y + radius;
			angle_speed = -x_speed * angle_const;
			is_collide = 4;
		}
	}

	y_speed -= accel * gravity;

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