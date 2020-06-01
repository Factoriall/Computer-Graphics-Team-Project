#pragma once

#ifndef __PHYSICS_H__
#define __PHYSICS_H__
#include "object.h"

float		gravity = 12.3f;			// �߷°� ���
float		angle_const = 4.3f;			// �� ȸ���ӵ� ���		

// const ��� �����
vec2		floor_elasticity = vec2(0.35f);			// �ٴ�
vec2		wall_elasticity = vec2(0.13f);			// ��
vec2		plate_elasticity = vec2(0.43f, 0.43f);	// �⺻ ����
vec2		plate2_elasticity = vec2(0.05f, 0.5f);	// ���� ����
vec2		plate3_elasticity = vec2(0.9f, 0.9f);	// ������ ����
vec2		plate4_elasticity = vec2(0.43f, 0.43f);	// ���� ����


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
int		sphere_t::collision(std::vector <rect_t>& floors, std::vector <rect_t>& walls, std::vector <plate_t>& plates, float del_t)
{
	/*
		is_collide �浹 ��� ���� ����
		0 - �浹 ����
		1 - ��ü�Ҹ��� �浹
		2 - �ٴڰ� �浹
		3 - ���� �浹
		4+ - ���ǰ� �浹
	*/
	int		is_collide = 0;						// �浹 ��� ����, return ��
	vec3	p0 = center;						// ���� �� ��ǥ ����
	float	maximum_friction =  0.55f * del_t;	// �̵��Ÿ��� ���� ���� ���� ���� 
	y_speed -= gravity * del_t;					// �߷� ����

	vec3	pn = p0 + vec3(x_speed, y_speed, 0) * del_t;	// ���� ���� ��ġ ���

	if (floor_collide(pn.y, floors[0].center.y, radius))	//�ٴڰ� �浹 ��
	{
		center.y = floors[0].center.y + radius;
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

	else if (wall_collide(pn.x, walls[1].center.x, radius))	//��1�� �浹 ��
	{
		// ���� ��
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

	else if (wall_collide(pn.x, walls[2].center.x, radius))	//��2�� �浹 ��
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
		float pl_x = plates.center.x, pl_y = plates.center.y;				// ������ x, y��ǥ
		float plsize_x = plates.scale.x / 2, plsize_y = plates.scale.y / 2;	// ������ x, yũ���� ����
		pl_y += plsize_y;	// ���� ���� ����

		if ((pl_x - pn.x) * (pl_x - pn.x) + (pl_y - pn.y) * (pl_y - pn.y) > (radius + plsize_x + plsize_y) * (radius + plsize_x + plsize_y))
		{	
			// ���ǰ��� �浹�� ����
			continue;
		}
		if (plate_collide_1(pl_x, pl_y, plsize_x, plsize_y, pn.x, pn.y, radius))
		{
			// ���ʸ�� �浹 ó��
			center.x = pl_x - plsize_x - radius;
			if (abs(p0.x - center.x) > maximum_friction) is_collide = 3 + plates.type;
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
		}
		if (plate_collide_2(pl_x, pl_y, plsize_x, plsize_y, pn.x, pn.y, radius))
		{
			// �Ʒ���� �浹 ó��
			center.y = pl_y - plsize_y - radius;
			if (abs(p0.y - center.y) > maximum_friction) is_collide = 3 + plates.type;
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
		}
		if (plate_collide_3(pl_x, pl_y, plsize_x, plsize_y, pn.x, pn.y, radius))
		{
			// ������ ��� �浹 ó��
			center.x = pl_x + plsize_x + radius;
			if (abs(p0.x - center.x) > maximum_friction) is_collide = 3 + plates.type;
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
		}
		if (plate_collide_4(pl_x, pl_y, plsize_x, plsize_y, pn.x, pn.y, radius))
		{
			// ���ʸ�� �浹 ó��
			float new_ex, new_ey;	// ���� �������� ������� �ٸ�
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
			if (abs(p0.y - center.y) > maximum_friction) is_collide = 3 + plates.type;
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
		}
	}

	center += vec3(x_speed, y_speed, 0) * del_t;	// ���� ��ġ ���
	
	if ((p0 - center).length() < maximum_friction) {
		// �ſ� ���� ������ �߻�
		if (stop_flag) {
			// ��� ������
			paused_time += del_t;
			if (paused_time > 0.5f) {
				// Ư�� �ð� �̻� ���� �� �������� ���� ���� Ȯ�� (������ ����)
				is_moving = false;
			}
		}
		else {
			// ���� ������ Ȯ��
			stop_flag = true;
			paused_time = del_t;
		}
	}
	else {
		// �����̰� ����
		is_moving = true;
		stop_flag = false;
	}
	
	return is_collide;
}

#endif