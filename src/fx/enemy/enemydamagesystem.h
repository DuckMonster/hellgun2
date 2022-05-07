#pragma once
#include "fx/particlesystem.h"

struct Mesh;
struct Materal;

class Enemy_Damage_System : public Particle_System
{
private:
	Mesh* mesh;
	Material* material;
	Mat4 transform;

public:
	void init(Mesh* mesh, const Mat4& transform);

	void system_update();
	void system_render(const Render_Info& info);
};