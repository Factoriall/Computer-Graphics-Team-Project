#pragma once

#ifndef __OBJECT_H__
#define __OBJECT_H__

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "createTexture.h"

// constants
static const char* plate_image_path = "../bin/images/plate.jpg";
static const char* brick_image_path = "../bin/images/brick.jpg";
static const char* floor_image_path = "../bin/images/floor.jpg";
static const char* sphere_image_path = "../bin/images/earth.png";

// opengl variables
GLuint	rect_vertex_array = 0;					// ID holder for vertex array 
GLuint	sphere_vertex_array = 0;				// ID holder for vertex array object
GLuint	PlateTexture = 0;
GLuint	BrickTexture = 0;
GLuint	FloorTexture = 0;
GLuint	SphereTexture = 0;

// global variables
vec3	away_from_map = vec3(-0.5f, 0, -1.0f);	// adjust plate to map
bool	stop_simulation = false;
float	paused_time = 0.0f;
bool	b_index_buffer = true;					// index_buffering mode
uint	N = 72;									// logitute
uint	M = 72 / 2;								// latitute

// structure 
struct rect_t {
	vec3	center = vec3(0);
	vec2	scale = vec2(0);
	vec3	axle = vec3(0);
	float	angle = 0;
};
struct plate_t {
	vec3 center = vec3(0);				// position of center ( x, y, z )
	vec3 scale = vec3(1.0f);		// scale of ( x-size, y-size, z-size )
	float angle = 0;							// not use yet
	rect_t rect[6];								// cube uses 6 rectngles

	mat4 model_matrix;							// modeling transformation
	void make_cube(vec3 _center, vec3 _scale);
};
struct sphere_t {
	vec3	center = vec3(0);
	float	radius = 0.0f;
	vec2	angle = vec2(0);
	vec4	color = vec4(0.7f, 0.7f, 0.7f, 1.0f);
	mat4	model_matrix;

	void	update(float t);
	void	collide_with_wall() {};
	void	collide_with_cube() {};
};

// implement fuctions
void plate_t::make_cube(vec3 _center, vec3 _scale) {
	center = _center;
	scale = _scale;

	rect[0] = { _center - vec3(0, 0, _scale.z / 2.0f), vec2(_scale.x,_scale.y), vec3(0), 0 };
	rect[1] = { _center - vec3(_scale.x / 2.0f, 0, 0), vec2(_scale.z, _scale.y), vec3(0, 1, 0), PI / 2 };
	rect[2] = { _center - vec3(0, 0, _scale.z / 2.0f), vec2(_scale.x, _scale.z), vec3(1, 0, 0), PI / 2 };
	rect[3] = { _center + vec3(_scale.x / 2.0f, 0, 0), vec2(_scale.z, _scale.y), vec3(0, 1, 0),  PI / 2 };
	rect[4] = { _center - vec3(0, -_scale.y, _scale.z / 2.0f), vec2(_scale.x, _scale.z), vec3(1, 0, 0), PI / 2 };
	rect[5] = { _center + vec3(0, 0, _scale.z / 2.0f), vec2(_scale.x,_scale.y), vec3(0), 0 };
}
void sphere_t::update(float t) {
	model_matrix = mat4::translate(center) *
		mat4::rotate(vec3(1, 0, 0), angle.y) *
		mat4::rotate(vec3(0, 1, 0), angle.x) *
		mat4::scale(radius);
}

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

	// texture assign
	PlateTexture = create_texture(plate_image_path, true);
	BrickTexture = create_texture(brick_image_path, true);
	FloorTexture = create_texture(floor_image_path, true);
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
	
	SphereTexture = create_texture(sphere_image_path, true);
}

// render rectangle, no change
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
void render_cube(GLuint program, std::vector<plate_t> & cubes) {
	for (auto& c : cubes) {
		for (rect_t r : c.rect) {
			render_rect(program, r, PlateTexture);
		}
	}
}
void render_wall(GLuint program, std::vector<rect_t> & walls) {
	glBindVertexArray(rect_vertex_array);

	if (BrickTexture != 0) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, BrickTexture);
		glUniform1i(glGetUniformLocation(program, "TEX"), 0);
	}

	for (auto& w : walls) {//move the walls using the information inside the walls struct
		mat4 model_matrix = mat4::translate(w.center) *
			mat4::rotate(w.axle, w.angle) *
			mat4::scale(w.scale.x, w.scale.y, 0);

		glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, model_matrix);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);
	}
}
void render_floor(GLuint program, std::vector<rect_t> & floors) {
	glBindVertexArray(rect_vertex_array);

	if (FloorTexture != 0) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, FloorTexture);
		glUniform1i(glGetUniformLocation(program, "TEX"), 0);
	}

	for (auto& w : floors) {//move the walls using the information inside the walls struct
		mat4 model_matrix = mat4::translate(w.center) *
			mat4::rotate(w.axle, w.angle) *
			mat4::scale(w.scale.x, w.scale.y, 0);

		glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, model_matrix);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);
	}
}
void render_sphere(GLuint program, std::vector<sphere_t> & spheres, float t) {
	glBindVertexArray(sphere_vertex_array);

	if (SphereTexture != 0) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, SphereTexture);
		glUniform1i(glGetUniformLocation(program, "TEX"), 0);
	}

	for (auto& sphere : spheres) {//move the walls using the information inside the walls struct
		sphere.update(t);

		glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, sphere.model_matrix);
		if (b_index_buffer)	glDrawElements(GL_TRIANGLES, M * (N * 3 + 3) * 2, GL_UNSIGNED_INT, nullptr);
		else				glDrawArrays(GL_TRIANGLES, 0, M * (N * 3 + 3) * 2);
	}
}
// initialize cube structure with position of center coordinate and scale of cube, no change

// creating object functions
inline std::vector<plate_t> create_cubes() {
	std::vector<plate_t> cube_vector;
	plate_t cube;

	cube.make_cube(vec3(5, 5, 5), vec3(1, 1, 2));
	cube_vector.push_back(cube);
	cube.make_cube(vec3(5, 9, 7), vec3(0.2f, 3, 2));
	cube_vector.push_back(cube);
	cube.make_cube(vec3(6, 4, 7), vec3(1, 0.2f, 1));
	cube_vector.push_back(cube);
	// 뒷 벽을 기준으로 x 가 좌우, y가 높이(y_scale = 0.1f 고정), z가 앞뒤(z=2.0f, z_scale=2.0f) 고정하면 대충 맵에 맞음
	cube.make_cube(vec3(0.5f, 1, 2) + away_from_map, vec3(1.0f, 0.1f, 2.0f));
	cube_vector.push_back(cube);
	cube.make_cube(vec3(1.0f, 3, 2) + away_from_map, vec3(1.0f, 0.1f, 2.0f));
	cube_vector.push_back(cube);
	cube.make_cube(vec3(0.0f, 2, 2) + away_from_map, vec3(1.0f, 0.1f, 2.0f));
	cube_vector.push_back(cube);

	return cube_vector;
}
inline std::vector<rect_t> create_walls() //function to save the information about walls - 3 walls
{
	std::vector<rect_t> walls;
	rect_t w;

	w = { vec3(0), vec2(2.0f, 15.0f), vec3(0, 1.0f, 0), 0 };
	walls.emplace_back(w);
	w = { vec3(-1.0f, 0, 1.0f), vec2(2.0f, 15.0f), vec3(0, 1.0f, 0), PI / 2.0f };
	walls.emplace_back(w);
	w = { vec3(1.0f, 0, 1.0f), vec2(2.0f, 15.0f), vec3(0, 1.0f, 0), (PI * 3.0f) / 2.0f };
	walls.emplace_back(w);

	return walls;
}
inline std::vector<rect_t> create_floors() //function to save the information about walls - 3 walls
{
	std::vector<rect_t> floors;
	rect_t f;

	f = { vec3(0), vec2(2.0f, 2.0f), vec3(1.0f, 0, 0), PI / 2.0f };
	floors.emplace_back(f);


	return floors;
}
inline std::vector<sphere_t> create_spheres() {
	std::vector<sphere_t> sphere;
	sphere_t s = { vec3(0.0f, 0.3f, 1.0f), 0.3f, vec2(0, 0) };
	sphere.push_back(s);

	return sphere;
}

#endif
