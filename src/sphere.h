#ifndef __SPHERE_H__
#define __SPHERE_H__
#include "cgmath.h"

GLuint	sphere_vertex_array = 0;	// ID holder for vertex array object
bool stop_simulation = false;
float paused_time = 0.0f;
bool	b_index_buffer = true;		// index_buffering mode
// 위도랑 경도 세분화
uint N = 72;
uint M = 72 / 2;

struct sphere_t {
	vec3	center;
	float	radius;
	vec2	angle;
	vec4	color = vec4(0.7f, 0.7f, 0.7f, 1.0f);
	mat4	model_matrix;

	void	update(float t);
	void	collide_with_wall() {};
	void	collide_with_cube() {};
};

void sphere_t::update(float t) {
	model_matrix = mat4::translate(center) *
		mat4::rotate(vec3(1, 0, 0), angle.y) *
		mat4::rotate(vec3(0, 1, 0), angle.x) *
		mat4::scale(radius);
}


inline std::vector<sphere_t> create_spheres() {
	std::vector<sphere_t> sphere;
	sphere_t s = { vec3(0.0f, 0.3f, 1.0f), 0.3f, vec2(0, 0) };
	sphere.push_back(s);

	return sphere;
}

auto spheres = std::move(create_spheres());

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

void update_sphere_vertex_buffer(const std::vector<vertex>& vertices) // function to update the wall_vertex_buffer
{
	// make sphere
	static GLuint vertex_buffer = 0;	// ID holder for vertex buffer
	static GLuint index_buffer = 0;		// ID holder for index buffer

	// clear and create new buffers
	if (vertex_buffer)	glDeleteBuffers(1, &vertex_buffer);	vertex_buffer = 0;
	if (index_buffer)	glDeleteBuffers(1, &index_buffer);	index_buffer = 0;

	// check exceptions
	if (vertices.empty()) { printf("[error] vertices is empty.\n"); return ; }

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
	if (!sphere_vertex_array) { printf("%s(): failed to create vertex aray\n", __func__); return ; }
}

void render_sphere(GLuint program, float t) {
	glBindVertexArray(sphere_vertex_array);

	
	for (auto& sphere : spheres) {//move the walls using the information inside the walls struct
		sphere.update(t);
		
		glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, sphere.model_matrix);
		if (b_index_buffer)	glDrawElements(GL_TRIANGLES, M * (N * 3 + 3) * 2, GL_UNSIGNED_INT, nullptr);
		else				glDrawArrays(GL_TRIANGLES, 0, M * (N * 3 + 3) * 2);
	}
}

#endif // __PLANET_H__