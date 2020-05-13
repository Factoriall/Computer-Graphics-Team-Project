#pragma once
#ifndef __WALL_H__
#define __WALL_H__
#include "camera.h"

GLuint	wall_vertex_array = 0;	// ID holder for vertex array object

struct wall_t
{
	vec3	center = vec3(0);	// 2D position for translation
	float	height = 15.0f;		// height of the wall
	float	width = 2.0f;		// width of the wall 
	float	angle = 0.0f;		// angle from the x axis 
	mat4	model_matrix;		// modeling transformation
};

inline std::vector<wall_t> create_walls()//function to save the information about walls - 3 walls
{
	std::vector<wall_t> walls;
	wall_t w;
	
	w = { vec3(0), 15.0f, 2.0f, 0.0f};
	walls.emplace_back(w);
	w = { vec3(-1.0f, 0, 1.0f), 15.0f, 2.0f, PI / 2.0f };
	walls.emplace_back(w);
	w = { vec3(1.0f, 0, 1.0f), 15.0f, 2.0f, (PI * 3.0f) / 2.0f };
	walls.emplace_back(w);

	return walls;
}

auto walls = std::move(create_walls());

std::vector<vertex> create_wall_vertices() // create vertices of the wall - rectangle
{
	std::vector<vertex> v = { { vec3(0), vec3(1), vec2(1) } };
	
	v.push_back({ vec3(-0.5f,0.0f, 0.0f), vec3(1), vec2(1) });
	v.push_back({ vec3(0.5f,0.0f, 0.0f), vec3(1), vec2(1) });
	v.push_back({ vec3(-0.5f,1.0f, 0.0f), vec3(1), vec2(1) });
	v.push_back({ vec3(0.5f,1.0f, 0.0f), vec3(1), vec2(1) });

	return v;
}

void update_wall_vertex_buffer(const std::vector<vertex>& vertices) // function to update the wall_vertex_buffer
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
	if (wall_vertex_array) glDeleteVertexArrays(1, &wall_vertex_array);
	wall_vertex_array = cg_create_vertex_array(vertex_buffer, index_buffer);
	if (!wall_vertex_array) { printf("%s(): failed to create vertex aray\n", __func__); return; }
}

void render_wall(GLuint program) {
	glBindVertexArray(wall_vertex_array);

	for (auto& w : walls) {//move the walls using the information inside the walls struct
		mat4 model_matrix = mat4::translate(w.center) *
							mat4::rotate(vec3(0, 1, 0), w.angle) *
							mat4::scale(w.width, w.height, 0);

		glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, model_matrix);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}
}

#endif
