#pragma once

#ifndef __OBJECT_H__
#define __OBJECT_H__

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "createTexture.h"

// const
const uint	N = 72;								// logitute
const uint	M = 72 / 2;							// latitute

// opengl variables
GLuint	rect_vertex_array = 0;					// ID holder for vertex array 
GLuint	sphere_vertex_array = 0;				// ID holder for vertex array object
GLuint	pointer_vertex_array = 0;				// ID holder for vertex array object
GLuint	circle_vertex_array = 0;				// ID holder for vertex array object

// global variables
bool	b_index_buffer = true;					// index_buffering mode

// structure 
struct rect_t {
	vec3	center = vec3(0);
	vec2	scale = vec2(0);
	vec3	axle = vec3(0);
	float	angle = 0;
};

struct plate_t {
	vec3 center = vec3(0);						// position of center ( x, y, z )
	vec3 scale = vec3(1.0f);					// scale of ( x-size, y-size, z-size )
	float angle = 0;							// not use yet
	rect_t rect[6];								// cube uses 6 rectngles
	int type;			// 1: 그냥 2: 얼음 3: 끈끈이 4: 점프발판
	float	time;

	mat4 model_matrix;							// modeling transformation
	void make_plate(vec3 _center, vec3 _scale, int _type);
	void update_plate();
};
struct sphere_t {
	vec3	center = vec3(0);
	float	radius = 0.0f;

	float	angle = 0.0f;				// z 축 회전
	float	angle_speed = 0.0f;
	vec4	color = vec4(0.7f, 0.7f, 0.7f, 1.0f);
	mat4	model_matrix;

	float	x_speed = 0.0f;
	float	y_speed = 0.0f;		// 구의 초기 x축, y축 속력
	float	paused_time = 0.0f;	// 공이 멈췄던 시간 저장
	bool	is_moving = true;
	bool	stop_flag = false;

	// public functions
	void	update(float del_t);
	int 	sphere_t::collision(std::vector <rect_t> & floors, std::vector <rect_t>& walls, std::vector <plate_t>& plates, float del_t);
	bool	set_sphere_condition_stop(float t) {};
	bool	set_sphere_condition_play() {};
};
struct circle_t {
	vec3	center = vec3(0);
	float	scale = 1.0f;
	float	angle = 0.0f;
	float	proceed = 0;
	float	angle_speed = 0.6f;
	
	bool	in_storm(vec3 sp_center, float sp_r);
	int		go_to_storm(sphere_t& sp, camera& cam, float del_t);
};

// create vertex vector
std::vector<vertex> create_rect_vertices() // create vertices of the wall - rectangle
{
	std::vector<vertex> v = { { vec3(0), vec3(1), vec2(1) } };

	v.push_back({ vec3(-0.5f,0.0f, 0.0f), vec3(1), vec2(0.0f, 0.0f) });
	v.push_back({ vec3(0.5f,0.0f, 0.0f), vec3(1), vec2(1.0f, 0.0f) });
	v.push_back({ vec3(-0.5f,1.0f, 0.0f), vec3(1), vec2(0.0f, 1.0f) });
	v.push_back({ vec3(0.5f,1.0f, 0.0f), vec3(1), vec2(1.0f, 1.0f) });

	return v;
}
std::vector<vertex> create_sphere_vertices() // create vertices of the wall - rectangle
{
	// sphere vertices
	std::vector<vertex> vertices = { { vec3(0), vec3(0,0,-1.0f), vec2(0.5f) } }; // origin
	for (uint k = 0; k <= M; k++)
	{
		// latitude
		float theta = PI * k / float(M), c_theta = cos(theta), s_theta = sin(theta);
		for (uint l = 0; l <= N; l++) {
			// longitude
			float pi = PI * 2.0f * l / float(N), c_pi = cos(pi), s_pi = sin(pi);
			vertices.push_back({ vec3(s_theta * c_pi, s_theta * s_pi, c_theta),
				vec3(s_theta * c_pi,s_theta * s_pi,c_theta),
				vec2(pi / (2.0f * PI), 1 - theta / PI) });
		}
	}
	return vertices;
}
std::vector<vertex> create_pointer_vertices()
{
	std::vector<vertex> v = { { vec3(0), vec3(0,0,-1.0f), vec2(0.5f) } }; // origin
	for (uint i = 0; i <= N/2; i++)
	{
		float t = PI * i / float(N), c = cos(t), s = sin(t);
		v.push_back({ vec3(s,c,0), vec3(0,0,-1.0f), vec2(s,c) * 0.5f + 0.5f });
	}
	v.push_back({ vec3(2.0f, 2.0f, 0), vec3(0,0,-1.0f), vec2(0.5f) });
	return v;
}
std::vector<vertex> create_circle_vertices()
{
	std::vector<vertex> v = { { vec3(0), vec3(0,0,-1.0f), vec2(0.5f) } }; // origin
	for (uint i = 0; i <= N; i++)
	{
		float t = 2 * PI * i / float(N), c = cos(t), s = sin(t);
		v.push_back({ vec3(s,c,0), vec3(0,0,-1.0f), vec2(s,c) * 0.5f + 0.5f });
	}
	return v;
}

// update rectangle vertices buffer, no change
void update_rect_vertex_buffer(const std::vector<vertex>& vertices) // function to update the wall_vertex_buffer
{
	static GLuint vertex_buffer = 0;	// ID holder for vertex buffer
	static GLuint index_buffer = 0;		// ID holder for index buffer

	// clear and create new buffers
	if (vertex_buffer)	glDeleteBuffers(1, &vertex_buffer);	vertex_buffer = 0;
	if (index_buffer)	glDeleteBuffers(1, &index_buffer);	index_buffer = 0;

	// check exceptions
	if (vertices.empty()) { printf("[error] vertices is empty.\n"); return; }

	// create buffers
	std::vector<uint> indices;

	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);

	indices.push_back(2);
	indices.push_back(4);
	indices.push_back(3);

	indices.push_back(1);
	indices.push_back(3);
	indices.push_back(2);

	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(4);

	// generation of vertex buffer: use vertices as it is
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// geneation of index buffer
	//GL_ELEMENT_ARRAY_BUFFER == INDEX_BUFFER
	glGenBuffers(1, &index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * indices.size(), &indices[0], GL_STATIC_DRAW);


	// generate vertex array object, which is mandatory for OpenGL 3.3 and higher
	if (rect_vertex_array) glDeleteVertexArrays(1, &rect_vertex_array);
	rect_vertex_array = cg_create_vertex_array(vertex_buffer, index_buffer);
	if (!rect_vertex_array) { printf("%s(): failed to create vertex aray\n", __func__); return; }

}
void update_sphere_vertex_buffer(const std::vector<vertex>& vertices) // function to update the wall_vertex_buffer
{
	// make sphere
	static GLuint vertex_buffer = 0;	// ID holder for vertex buffer
	static GLuint index_buffer = 0;		// ID holder for index buffer

	// clear and create new buffers
	if (vertex_buffer)	glDeleteBuffers(1, &vertex_buffer);	vertex_buffer = 0;
	if (index_buffer)	glDeleteBuffers(1, &index_buffer);	index_buffer = 0;

	// check exceptions
	if (vertices.empty()) { printf("[error] vertices is empty.\n"); return; }

	// with index buffer mode
	if (b_index_buffer) {
		// sphere indices
		std::vector<uint> indices;
		for (uint k = 0; k < M; k++)
		{
			for (uint l = 0; l < N; l++) {
				// routin a
				indices.push_back((N + 1) * k + l + 1);
				indices.push_back((N + 1) * (k + 1) + l + 1);
				indices.push_back((N + 1) * (k + 1) + l + 2);
				// routin b
				indices.push_back((N + 1) * k + l + 1);
				indices.push_back((N + 1) * (k + 1) + l + 2);
				indices.push_back((N + 1) * k + l + 2);
			}
			// routin a
			indices.push_back((N + 1) * k + N + 1);
			indices.push_back((N + 1) * (k + 1) + N + 1);
			indices.push_back((N + 1) * (k + 1) + 1);
			// routin b
			indices.push_back((N + 1) * k + N + 1);
			indices.push_back((N + 1) * (k + 1) + 1);
			indices.push_back((N + 1) * k + 1);
		}

		// generation of vertex buffer
		glGenBuffers(1, &vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

		// generation of index buffer
		glGenBuffers(1, &index_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * indices.size(), &indices[0], GL_STATIC_DRAW);
	}

	// with vertext buffer mode
	else {
		std::vector<vertex> v; // triangle vertices
		for (uint k = 0; k < M; k++)
		{
			for (uint l = 0; l < N; l++) {
				// routin a
				v.push_back(vertices[(N + 1) * k + l + 1]);
				v.push_back(vertices[(N + 1) * (k + 1) + l + 1]);
				v.push_back(vertices[(N + 1) * (k + 1) + l + 2]);
				// routin b
				v.push_back(vertices[(N + 1) * k + l + 1]);
				v.push_back(vertices[(N + 1) * (k + 1) + l + 2]);
				v.push_back(vertices[(N + 1) * k + l + 2]);
			}
			// routin a
			v.push_back(vertices[(N + 1) * k + N + 1]);
			v.push_back(vertices[(N + 1) * (k + 1) + N + 1]);
			v.push_back(vertices[(N + 1) * (k + 1) + 1]);
			// routin b
			v.push_back(vertices[(N + 1) * k + N + 1]);
			v.push_back(vertices[(N + 1) * (k + 1) + 1]);
			v.push_back(vertices[(N + 1) * k + 1]);
		}

		// generation of vertex buffer: use triangle_vertices instead of vertices
		glGenBuffers(1, &vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * v.size(), &v[0], GL_STATIC_DRAW);
	}

	// generate vertex array object, which is mandatory for OpenGL 3.3 and higher
	if (sphere_vertex_array) glDeleteVertexArrays(1, &sphere_vertex_array);
	sphere_vertex_array = cg_create_vertex_array(vertex_buffer, index_buffer);
	if (!sphere_vertex_array) { printf("%s(): failed to create vertex aray\n", __func__); return; }
}
void update_pointer_vertex_buffer(const std::vector<vertex>& vertices) // function to update the wall_vertex_buffer
{
	static GLuint vertex_buffer = 0;	// ID holder for vertex buffer
	static GLuint index_buffer = 0;		// ID holder for index buffer

	// clear and create new buffers
	if (vertex_buffer)	glDeleteBuffers(1, &vertex_buffer);	vertex_buffer = 0;
	if (index_buffer)	glDeleteBuffers(1, &index_buffer);	index_buffer = 0;

	// check exceptions
	if (vertices.empty()) { printf("[error] vertices is empty.\n"); return; }

	// create buffers
	std::vector<uint> indices;
	
	uint point_n = 38;
	for (uint i = 1; i < point_n-1; i++) {
		indices.push_back(i);
		indices.push_back(i+1);
		indices.push_back(point_n);

		indices.push_back(i);
		indices.push_back(point_n);
		indices.push_back(i + 1);
	}

	// generation of vertex buffer: use vertices as it is
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// geneation of index buffer
	//GL_ELEMENT_ARRAY_BUFFER == INDEX_BUFFER
	glGenBuffers(1, &index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * indices.size(), &indices[0], GL_STATIC_DRAW);


	// generate vertex array object, which is mandatory for OpenGL 3.3 and higher
	if (pointer_vertex_array) glDeleteVertexArrays(1, &pointer_vertex_array);
	pointer_vertex_array = cg_create_vertex_array(vertex_buffer, index_buffer);
	if (!pointer_vertex_array) { printf("%s(): failed to create vertex aray\n", __func__); return; }
}
void update_circle_vertex_buffer(const std::vector<vertex>& vertices) // function to update the wall_vertex_buffer
{
	static GLuint vertex_buffer = 0;	// ID holder for vertex buffer
	static GLuint index_buffer = 0;		// ID holder for index buffer

	// clear and create new buffers
	if (vertex_buffer)	glDeleteBuffers(1, &vertex_buffer);	vertex_buffer = 0;
	if (index_buffer)	glDeleteBuffers(1, &index_buffer);	index_buffer = 0;

	// check exceptions
	if (vertices.empty()) { printf("[error] vertices is empty.\n"); return; }

	// create buffers
	std::vector<uint> indices;

	for (uint i = 1; i <= N+1; i++) {
		indices.push_back(0);
		indices.push_back(i);
		indices.push_back(i+1);

		indices.push_back(0);
		indices.push_back(i+1);
		indices.push_back(i);
	}

	// generation of vertex buffer: use vertices as it is
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// geneation of index buffer
	//GL_ELEMENT_ARRAY_BUFFER == INDEX_BUFFER
	glGenBuffers(1, &index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * indices.size(), &indices[0], GL_STATIC_DRAW);


	// generate vertex array object, which is mandatory for OpenGL 3.3 and higher
	if (circle_vertex_array) glDeleteVertexArrays(1, &circle_vertex_array);
	circle_vertex_array = cg_create_vertex_array(vertex_buffer, index_buffer);
	if (!circle_vertex_array) { printf("%s(): failed to create vertex aray\n", __func__); return; }
}

// render function
void render_rect(GLuint program, rect_t& rect, GLuint texture) {

	glBindVertexArray(rect_vertex_array);

	if (texture != 0) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(glGetUniformLocation(program, "TEX"), 0);
	}

	mat4 model_matrix = mat4::translate(rect.center) *
		mat4::rotate(rect.axle, rect.angle) *
		mat4::scale(rect.scale.x, rect.scale.y, 0);

	glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, model_matrix);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);
}


#endif
