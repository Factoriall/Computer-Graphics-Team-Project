#ifndef __PARTICLE_H__
#define __PARTICLE_H__
#pragma once

#include "cgmath.h"
#include "cgut.h"

GLuint	particle_vertex_array = 0;
GLuint	ParticleTexture = 0;
static const char* particle_image_path = "../bin/images/explosion.png";

inline float random_range(float min, float max) { return mix(min, max, rand() / float(RAND_MAX)); }

struct particle_t
{
	static constexpr int MAX_PARTICLES = 200;

	vec3 pos;
	vec4 color;
	vec2 velocity;
	float scale;
	float life;

	//optional
	float elapsed_time;
	float time_interval;

	particle_t() { reset(); }
	void reset();
	void update();
};

inline void particle_t::reset()
{
	pos = vec3(random_range(-5.0f, 5.0f), random_range(60.0f, 68.0f), 0.5f);
	color = vec4(random_range(0, 1.0f), random_range(0, 1.0f), random_range(0, 1.0f), 1);
	scale = random_range(0.5f, 1.5f);
	life = 1.0f;
	velocity = vec2(random_range(-1.0f, 1.0f), random_range(-1.0f, 1.0f)) * 0.003f;
	elapsed_time = 0.0f;
	time_interval = random_range(200.0f, 600.0f);
}

inline void particle_t::update()
{
	const float dwTime = (float)glfwGetTime();

	scale += 0.1f;

	constexpr float life_factor = 0.001f;
	life -= life_factor * dwTime;

	// disappear
	if (life < 0.0f)
	{
		reset();
	}

}


void render_particle(GLuint program, std::vector<particle_t>& particles) {
	glBindVertexArray(particle_vertex_array);

	
	if (ParticleTexture != 0) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ParticleTexture);
		glUniform1i(glGetUniformLocation(program, "TEX"), 0);
	}

	glEnable(GL_BLEND);
	for (auto& p : particles)
	{
		mat4 translate_matrix = mat4::translate(vec3(p.pos.x, p.pos.y, p.pos.z));
		mat4 scale_matrix = mat4::scale(p.scale);
		mat4 model_matrix = translate_matrix * scale_matrix;

		GLint uloc;
		uloc = glGetUniformLocation(program, "model_matrix");		if (uloc > -1) glUniformMatrix4fv(uloc, 1, GL_TRUE, model_matrix);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
	glDisable(GL_BLEND);
}

std::vector<vertex> create_particle_vertices() {
	return { {vec3(-1,-1,0),vec3(0,0,1),vec2(0,0)}, {vec3(1,-1,0),vec3(0,0,1),vec2(1,0)}, {vec3(-1,1,0),vec3(0,0,1),vec2(0,1)}, {vec3(1,1,0),vec3(0,0,1),vec2(1,1)} }; // strip ordering [0, 1, 3, 2]
}

void update_particle_vertex_buffer(const std::vector<vertex>& vertices) // function to update the wall_vertex_buffer
{
	GLuint vertex_buffer = 0;	// ID holder for vertex buffer

	if (vertices.empty()) { printf("[error] vertices is empty.\n"); return; }

	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * 4, &vertices[0], GL_STATIC_DRAW);

	// generate vertex array object, which is mandatory for OpenGL 3.3 and higher
	if (particle_vertex_array) glDeleteVertexArrays(1, &particle_vertex_array);
	particle_vertex_array = cg_create_vertex_array(vertex_buffer);
	if (!particle_vertex_array) { printf("%s(): failed to create vertex aray\n", __func__); return; }
}

std::vector<particle_t> particles;

#endif