#pragma once
#include "container/array.h"
#include "debug/debug.h"
#include "gfx/renderinfo.h"
#include "gfx/mesh.h"
#include "gfx/material.h"

class Particle_System;

#pragma pack(push, 1)
struct Particle_Vertex
{
	Vec3 position;
	Vec3 velocity;
	Color color;
	float scale;

	float spawn_time;
	float kill_time;
};
#pragma pack(pop)

class Emitter_Base
{
public:
	Particle_System* system;

	virtual u32 num_particles() { return 0; }
	virtual void update() {}
	virtual void render(const Render_Info& info) {}

	virtual Particle_Vertex* get_vertex(u32 idx) { return nullptr; }
};

struct Particle_Update_Data
{
	u32 index;
	Particle_System* system;
	Emitter_Base* emitter;
	Particle_Vertex* vertex;
};

template<typename Particle_T>
class Emitter : public Emitter_Base
{
	Mesh mesh;
	Material* material;

	Array<Particle_T> particles;
	Array<Particle_Vertex> vertices;

public:
	bool local_space = false;
	bool preserve_order = false;
	float spawn_time;

	u32 num_particles() override { return particles.count(); }
	void set_draw_mode(GLenum mode) { mesh.draw_mode = mode; }

	void init(Material* mat)
	{
		spawn_time = time_elapsed();
		material = mat;

		mesh.init();
		mesh.storage_mode = GL_STREAM_DRAW;
		mesh.add_buffer(0);
		mesh.bind_attribute(0, 0, 3, sizeof(Particle_Vertex), 0);
		mesh.bind_attribute(0, 1, 3, sizeof(Particle_Vertex), 3 * sizeof(float));
		mesh.bind_attribute(0, 2, 4, sizeof(Particle_Vertex), 6 * sizeof(float));
		mesh.bind_attribute(0, 3, 1, sizeof(Particle_Vertex), 10 * sizeof(float));
		mesh.bind_attribute(0, 4, 1, sizeof(Particle_Vertex), 11 * sizeof(float));
		mesh.bind_attribute(0, 5, 1, sizeof(Particle_Vertex), 12 * sizeof(float));

		mesh.draw_mode = GL_POINTS;
		mesh.draw_num = 0;
	}

	float get_age() { return time_since(spawn_time); }

	void emit_particle(const Vec3& position, const Color& color, float scale, float duration, const Particle_T& particle = Particle_T())
	{
		particles.add(particle);
		vertices.add({ position, Vec3::zero, color, scale, system->get_age(), system->get_age() + duration });

		mesh.buffer_data(0, vertices.data_size(), vertices.data());
		mesh.draw_num++;
	}

	void update() override
	{
		if (particles.count() == 0)
			return;

		// Kill particles
		Particle_Update_Data data;
		data.system = system;
		data.emitter = this;

		for(u32 i = 0; i < vertices.count(); ++i)
		{
			if (vertices[i].kill_time < system->get_age())
			{
				if (!preserve_order)
				{
					particles.remove_at_swap(i);
					vertices.remove_at_swap(i);
				}
				else
				{
					particles.remove_at(i);
					vertices.remove_at(i);
				}
				mesh.draw_num--;
				i--;
			}
			else
			{
				data.index = i;
				data.vertex = &vertices[i];

				particles[i].update(data);
			}
		}

		mesh.buffer_subdata(0, 0, vertices.data_size(), vertices.data());
	}

	void render(const Render_Info& render_info) override
	{
		debug->print(String::printf("Particles: %d", particles.count()), 0.f);

		material->use();
		material->set("u_ViewProjection", render_info.view_projection);
		if (local_space)
			material->set("u_Model", mat_translation(system->position) * system->rotation.matrix());
		else
			material->set("u_Model", Mat4::identity);

		material->set("u_SystemAge", system->get_age());
		mesh.draw();
	}
};

struct Simple_Particle
{
	Vec3 velocity;
	Vec3 force;
	float friction = 0.f;

	void update(const Particle_Update_Data& data)
	{
		velocity += force * time_delta();
		velocity -= velocity * friction * time_delta();
		data.vertex->position += velocity * time_delta();
	}
};

typedef Emitter<Simple_Particle> Simple_Emitter;