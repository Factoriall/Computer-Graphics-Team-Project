#pragma once
#ifndef __FLOOR_H__
#define __FLOOR_H__

GLuint	floor_vertex_array = 0;	// ID holder for vertex array object
GLuint	FloorTexture = 0;

static const char* floor_image_path = "../bin/images/floor.jpg";

struct floor_t
{
	vec3	center = vec3(0);	// 2D position for translation
	float	height = 2.0f;		// height of the wall
	float	width = 2.0f;		// width of the wall 
	float	angle = 0.0f;		// angle from the x axis 
	mat4	model_matrix;		// modeling transformation
};

inline std::vector<floor_t> create_floors()//function to save the information about walls - 3 walls
{
	std::vector<floor_t> floors;
	floor_t f;

	f = { vec3(0), 2.0f, 2.0f, PI / 2.0f };
	floors.emplace_back(f);
	

	return floors;
}

auto floors = std::move(create_floors());

std::vector<vertex> create_floor_vertices() // create vertices of the wall - rectangle
{
	std::vector<vertex> v = { { vec3(0), vec3(1), vec2(1) } };

	v.push_back({ vec3(-0.5f,0.0f, 0.0f), vec3(1), vec2(0.0f, 0.0f) });
	v.push_back({ vec3(0.5f,0.0f, 0.0f), vec3(1), vec2(1.0f, 0.0f) });
	v.push_back({ vec3(-0.5f,1.0f, 0.0f), vec3(1), vec2(0.0f, 1.0f) });
	v.push_back({ vec3(0.5f,1.0f, 0.0f), vec3(1), vec2(1.0f, 1.0f) });

	return v;
}

void update_floor_vertex_buffer(const std::vector<vertex>& vertices) // function to update the wall_vertex_buffer
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
	indices.push_back(3);
	indices.push_back(2);

	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(4);

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
	if (floor_vertex_array) glDeleteVertexArrays(1, &floor_vertex_array);
	floor_vertex_array = cg_create_vertex_array(vertex_buffer, index_buffer);
	if (!floor_vertex_array) { printf("%s(): failed to create vertex aray\n", __func__); return; }

	FloorTexture = create_texture(floor_image_path, true);
}

void render_floor(GLuint program) {
	glBindVertexArray(floor_vertex_array);

	if (FloorTexture != 0) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, FloorTexture);
		glUniform1i(glGetUniformLocation(program, "TEX"), 0);
	}

	for (auto& f : floors) {//move the walls using the information inside the walls struct
		mat4 model_matrix = mat4::translate(f.center) *
			mat4::rotate(vec3(1, 0, 0), f.angle) *
			mat4::scale(f.width, f.height, 0);

		glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, model_matrix);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);
	}
}

#endif