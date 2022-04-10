#pragma once
#include "container/array.h"
#include "debug/debug.h"
#include "gfx/renderinfo.h"
#include "gfx/mesh.h"
#include "gfx/material.h"

#pragma pack(push, 1)
struct Particle_Vertex
{
	Vec3 position;
	Color color;

	float spawn_time;
	float kill_time;
};
#pragma pack(pop)

template<typename Particle_T>
class Emitter
{
	Mesh mesh;
	Material mat;

	Array<Particle_T> particles;
	Array<Particle_Vertex> particle_vertices;

public:
	Vec3 position;
	float age = 0.f;

	void init()
	{
		mesh.init();
		mesh.storage_mode = GL_STREAM_DRAW;
		mesh.add_buffer(0);
		mesh.bind_attribute(0, 0, 3, sizeof(Particle_Vertex), 0);
		mesh.bind_attribute(0, 1, 4, sizeof(Particle_Vertex), 3 * sizeof(float));
		mesh.bind_attribute(0, 2, 1, sizeof(Particle_Vertex), 7 * sizeof(float));
		mesh.bind_attribute(0, 3, 1, sizeof(Particle_Vertex), 8 * sizeof(float));

		mesh.draw_mode = GL_POINTS;
		mesh.draw_num = 0;
		mat.load_file("res/shader/geom_test.vert", "res/shader/geom_test.geom", "res/shader/geom_test.frag");
	}

	void emit_particle(const Vec3& position, const Color& color, float duration, const Particle_T& particle)
	{
		particles.add(particle);
		particle_vertices.add({ position, color, age, age + duration });

		mesh.buffer_data(0, particle_vertices.data_size(), particle_vertices.data());
		mesh.draw_num++;
	}

	void update()
	{
		age += time_delta();
		if (particles.count() == 0)
			return;

		// Kill particles
		for(u32 i = 0; i < particle_vertices.count(); ++i)
		{
			if (particle_vertices[i].kill_time < age)
			{
				particles.remove_at_swap(i);
				particle_vertices.remove_at_swap(i);
				mesh.draw_num--;
				i--;
			}
		}

		// Update particles
		for(u32 i = 0; i < particles.count(); ++i)
			particles[i].update(particle_vertices[i]);

		mesh.buffer_subdata(0, 0, particle_vertices.data_size(), particle_vertices.data());
	}

	void render(const Render_Info& render_info)
	{
		debug->print(String::printf("Particles: %d", particles.count()), 0.f);

		mat.use();
		mat.set("u_ViewProjection", render_info.view_projection);
		mat.set("u_Model", mat_translation(position));
		mat.set("u_EmitterAge", age);
		mesh.draw();
	}
};